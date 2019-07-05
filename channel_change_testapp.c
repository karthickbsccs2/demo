#define CDI_IOCTL_DEFINITIONS "ioctl_magic.h"

#include "lnb.h"
#include "tuner.h"
#include "poll.h"
#include "errno.h"
#include "fcntl.h"
#include "demuxcommon.h"
#include "demuxelementaryfilter.h"
#include "demuxdescrambler.h"
#include "demuxtpidfilter.h"
#include "clocksync.h"
#include "demuxclockfilter.h"
#include "decoderstreamformat.h"
#include "videodecoder.h"
#include "surface.h"
#include "viewport.h"
#include "blitter.h"
#include "blender.h"
#include "denc.h"
#include "audiodecoder.h"
#include "audiooutput.h"
#include "audiomixer.h"
#include "audiopostprocessor.h"
//#include "dvihdmi.h"
#include "spdifformatter.h"
#include "scart.h"
#include "hdmi.h"
#include "avmixer.h"
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#define MPEG2_13181_1 (0x02)
#define MPEG4_13181_1 (0x1B)

#define TUNER_PATH "/dev/nds/tuner0"
#define LNB_PATH "/dev/nds/lnb0"
#define DEMUX_PATH "/dev/nds/demux0"
#define TPID_PATH "/dev/nds/demux0/tpidfilter0"
#define FILTER_PATH "/dev/nds/demux0/elementaryfilter"
#define TSCHANNEL_PATH "/dev/nds/demux0/tschannel0"
#define TSCHANNEL_PATH3 "/dev/nds/demux0/tschannel3"
#define CLOCK_SYNC_PATH "/dev/nds/clocksync0"
#define CLOCK_FILTER_PATH "/dev/nds/demux0/clockfilter0"
#define DESCRAMBLER_PATH "/dev/nds/demux0/icam0/descrambler"
#define TPID_PATH1 "/dev/nds/demux0/tpidfilter1"
#define TPID_PATHA "/dev/nds/demux0/tpidfilter2"
#define SURFACE_PATH "/dev/nds/surface1"
#define HD_GR_SURFACE_PATH "/dev/nds/surface2"
#define SD_GR_SURFACE_PATH "/dev/nds/surface4"
#define VIEWPORT_PATH "/dev/nds/viewport2"
#define HD_VIEWPORT_PATH "/dev/nds/viewport0"
#define HDGR_VIEWPORT_PATH "/dev/nds/viewport1"
#define SDGR_VIEWPORT_PATH "/dev/nds/viewport3"
#define VIDEO_PATH "/dev/nds/videodecoder0"
#define AUDIO_DECODER_PATH "/dev/nds/audiodecoder0"
#define AUDIO_OUTPUT_PATH "/dev/nds/avoutput0/audiooutput0"
#define BLENDER_PATH "/dev/nds/blender1"
#define HD_BLENDER_PATH "/dev/nds/blender0"
#define CVBS_PATH "/dev/nds/avoutput0/cvbscinch0"
#define DENC_PATH "/dev/nds/avoutput0/denc0"
#define AUDIO_PP_PATH "/dev/nds/audiopostprocessor0"
#define STEREO_PATH "/dev/nds/avoutput0/stereoaudiocinch0"
#define HDMI_PATH "/dev/nds/digitaloutput0/hdmi"
#define SPDIF_PATH "/dev/nds/digitaloutput0/spdifformatter0"
#define YUV_PATH "/dev/nds/avoutput0/componentcinch0"
#define SVIDEO_PATH "/dev/nds/avoutput0/ushiden0"
#define SCART_PATH "/dev/nds/avoutput0/scart0"
#define AVMIXER_PATH "/dev/nds/avoutput0/avmixer0"



#define STILL_SURFACE  (0) 
#define VIDEO_SURFACE  (1)
#define HD_SURFACE_GR  2
#define SD_SURFACE_GR  4
//#define SURFACE_NUM 1

#define SD_BLENDER 1
#define HD_BLENDER 0

#define VIDEO_VIEWPORT_HD 0
#define IFRAMEVIEWPORT 1
#define HDVIEWPORT_GR 2
#define SDVIEWPORT_GR 3
#define VIDEO_VIEWPORT_SD 2

//Assuming HD_TYPE are always MPEG4
#define HD_TYPE 0
#define SD_TYPE 1

#if 1
#define VIDEO_PID 539
#define PCR_PID 8190
#define AUDIO_PID 639

#define CHANNEL_STREAM_TYPE     SD_TYPE

#define CHANNEL_FREQ_AS2        1166000
#define CHANNEL_SR_AS2          27500000
#define CHANNEL_MODE_AS2        TUNER_MODE_SAT_DVB_S

#else  
#define CHANNEL_FREQ_AS2        1200000
#define CHANNEL_SR_AS2          27500000

#define VIDEO_PID (2311)
#define PCR_PID (2311)
#define AUDIO_PID (2312)
#endif 

//Channel Zap
#define SAME_TRANSPONDER_SDHD_ZAPPING_COUNT 1000
#define DIFFERENT_TRANSPONDER_SDHD_ZAPPING_COUNT 1000
#define DIFFERENT_TRANSPONDER_SDSD_ZAPPING_COUNT 1000
#define TOTAL_CHANNEL_ZAPS  1000

#define NO_COLUMNS 8
//Ordered by transponder no
// transponder no, freq, sym rate, video pid, audio pid, pcr pid, type, mode


int zap_data[][NO_COLUMNS] = { {1, CHANNEL_FREQ_AS2, CHANNEL_SR_AS2, VIDEO_PID, AUDIO_PID, PCR_PID, CHANNEL_STREAM_TYPE, CHANNEL_MODE_AS2},
              {2, 1000000, CHANNEL_SR_AS2, 517, 697, 517, HD_TYPE, TUNER_MODE_SAT_DVB_S2},
              {2, 1000000, CHANNEL_SR_AS2, 531, 631, 531, SD_TYPE, TUNER_MODE_SAT_DVB_S2}
            };


static int clocksyncFD = -1;

static int clockfilterFD = -1;

static int tunerFD = -1;

static int lnbFD = -1;

static int demuxFD;

static int DescramblerFD;

static int tpidFD = -1, tpidFD1 = -1, tpidA = -1;

static int filterFD = -1;

static int audioFilterFD = -1;

static int tschannelFD = -1;

static int surfaceFD, HDgfxFD, SDgfxFD;

static int ViewPortFD, HDViewPortFD, HDGRViewPortFD, SDGRViewPortFD;

static int VideoFD = -1;

static int AudioFD = -1;

static int AudioOutFD;

static int blenderFD, HDBlenderFD;

static int DencFD;

static int AudioMixerFD;

static int audioPostProcessorFD;

static int stereoFD;

static int hdmiFD;

static int spdifFD;

static int cvbsFD;

static int yuvFD;

static int svideoFD;

static int scartFD;

static int avmixerFD;

static int hw_blitter_fd;


static void setupTuner (uint32_t freq, uint32_t symbolRate, uint32_t mode);

static void setupDemux (uint32_t video_pid, uint32_t audio_pid, uint32_t streamType);

static void setupClockSyncDevice (uint32_t pcr_pid);

static void setupSurface (void);

static void setupViewPort (void);

static void setupAudiodecoder (void);

static void setupVideodecoder (uint32_t streamType);

static void setupBlender (void);

static void setupDenc (void);

static void setupDescrambler (void);

static void setupDigitalDisplay (void);

static void setupBlitter (void);

static void TestDraw ();

static void createTask(void *fnPtr, pthread_t *tid);

static void videoTask();

static void channelZap();

static unsigned long getTime();

pthread_t tid_vt;
pthread_t tid_cz;
int checkBit;
int firstFramePresented = 0;
unsigned long framePresentedTime;



main ()
{  
  AVMixerCaps caps;

  setupTuner (CHANNEL_FREQ_AS2, CHANNEL_SR_AS2, CHANNEL_MODE_AS2);

  VideoFD = open (VIDEO_PATH, O_RDWR);

  if (VideoFD > 0)
  {
    printf ("VIDEO OPEN SUCCESSFULL\n");
  }
  else
  {
    printf ("VIDEO OPEN FAILED %d!!!\n",errno);
    return;
  }


  setupDemux (VIDEO_PID, AUDIO_PID, CHANNEL_STREAM_TYPE);
  /*setupDescrambler(); */
  setupClockSyncDevice (PCR_PID);

  setupVideodecoder (CHANNEL_STREAM_TYPE);

  setupAudiodecoder ();

  setupSurface ();

  setupViewPort ();
  
  setupDenc ();
  
  setupDigitalDisplay();


  setupBlender ();

  setupBlitter ();


      
  if (ioctl (VideoFD, VIDEO_DECODER_FLUSH_RATE_BUFFER) == -1)
    {
      printf ("VIDEO_DECODER_FLUSH_RATE_BUFFER ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }

  else
    printf ("VIDEO_DECODER_FLUSH_RATE_BUFFER ioctl SUCCESS\n");


  if (ioctl (AudioFD, AUDIO_DECODER_FLUSH_RATE_BUFFER) == -1)
    {
      printf ("AUDIO_DECODER_FLUSH_RATE_BUFFER ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }

  else
    printf ("AUDIO_DECODER_FLUSH_RATE_BUFFER ioctl SUCCESS\n");



  if (ioctl (filterFD, DEMUX_START) == -1)
    {
      printf ("DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }

  else
    {
      printf ("DEMUX_START SUCCESS\n");
    }


  if (ioctl (audioFilterFD, DEMUX_START) == -1)
    {
      printf ("AUDIO_DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }

  else
    {
      printf ("AUDIO_DEMUX_START SUCCESS\n");
    }
    
    
  //Create a task for video decoder events now

  createTask(videoTask, &tid_vt);
  
  //createTask(channelZap, &tid_cz);
    
  checkBit = VIDEO_DECODER_EVENT_FRAME_DECODED | VIDEO_DECODER_EVENT_FRAME_PRESENTED |VIDEO_DECODER_EVENT_NO_FRAME_TO_PRESENT | VIDEO_DECODER_EVENT_PARAMETER_CHANGE;

  if (ioctl (VideoFD, VIDEO_DECODER_START) == -1)
    {
      printf ("VIDEO_DECODER_START ioctl failed, errno = %d\n", errno);
      exit (0);
    }

  else
      printf ("VIDEO_DECODER_START ioctl SUCCESS %ld\n", getTime()); 
  


  if (ioctl (AudioFD, AUDIO_DECODER_START) == -1)
    {
      printf ("AUDIO_DECODER_START ioctl failed, errno = %d\n", errno);
      exit (0);
    }

  else

    printf ("AUDIO_DECODER_START ioctl SUCCESS\n");


  avmixerFD = open (AVMIXER_PATH, O_RDWR);

  if (avmixerFD)
    {
      printf ("Success in opening AVMixer\n\n");
    }
  else
    {
      printf ("Error in Opening avmixer\n");
      exit (0);
    }

  if (ioctl (avmixerFD, AVMIXER_GET_CAPS, &caps) == -1)
    {
      printf ("Error in Getting caps\n");
      exit (0);
    }


  if (ioctl
      (VideoFD, VIDEO_DECODER_SUBSCRIBE_EVENTS,
       VIDEO_DECODER_EVENT_FRAME_DECODED
       | VIDEO_DECODER_EVENT_NO_FRAME_TO_PRESENT
       | VIDEO_DECODER_EVENT_PARAMETER_CHANGE
       | VIDEO_DECODER_EVENT_FRAME_PRESENTED) == -1)
    {
      printf ("VIDEO_DECODER_SUBSCRIBE_EVENTS ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }

  else
    printf ("VIDEO_DECODER_SUBSCRIBE_EVENTS ioctl SUCCESS\n");


  ViewportWindows window = {
    {
     SURFACE_WINDOW_COORDINATE_TYPE_SCALAR,
     {
      0, 0}
     ,
     {
      32767, 32767}
     }

    ,
    {
     SURFACE_WINDOW_COORDINATE_TYPE_SCALAR,
     //SURFACE_WINDOW_COORDINATE_TYPE_PIXEL,
     {
      0, 0}
     ,
     {
      32767, 32767}
      //720,576}
     }

  };

  if (ioctl (ViewPortFD, VIEWPORT_SET_WINDOWS, &window) == -1)
    {
      printf ("VIEWPORT_SET_WINDOWS failed, errno = %d\n", errno);
      exit (0);
    }

  else
    {
      printf ("VIEWPORT_SET_WINDOWS SUCCESS\n");
    }


  ViewportWindows windowHD = {
    {
     SURFACE_WINDOW_COORDINATE_TYPE_SCALAR,
     {
      0, 0}
     ,
     {
      32767, 32767}
     }

    ,
    {
     SURFACE_WINDOW_COORDINATE_TYPE_SCALAR,
     //SURFACE_WINDOW_COORDINATE_TYPE_PIXEL,
     {
      0, 0}
     ,
     {
      32767, 32767}
//      1920,1080}
     }

  };

  if (ioctl (HDViewPortFD, VIEWPORT_SET_WINDOWS, &windowHD) == -1)
    {
      printf ("VIEWPORT_SET_WINDOWS HD failed, errno = %d\n", errno);
      exit (0);
    }

  else
    {
      printf ("VIEWPORT_SET_WINDOWS HD SUCCESS\n");
    }


ViewportWindows windowHDGR = {
    {
     SURFACE_WINDOW_COORDINATE_TYPE_SCALAR,
     {
      0, 0}
     ,
     {
      32767, 32767}
     }

    ,
    {
     SURFACE_WINDOW_COORDINATE_TYPE_PIXEL,
     {
      0, 0}
     ,
     {
      32767, 32767}
     }

  };

  if (ioctl (HDGRViewPortFD, VIEWPORT_SET_WINDOWS, &windowHDGR) == -1)
    {
      printf ("VIEWPORT_SET_WINDOWS HD failed, errno = %d\n", errno);
      exit (0);
    }

  else
    {
      printf ("VIEWPORT_SET_WINDOWS HD SUCCESS\n");
    }

  //TestDraw ();
  
    pthread_join( tid_vt, NULL);
    pthread_join( tid_cz, NULL);
    return 0;
}

static void createTask(void *fnPtr, pthread_t *tid)
{   
    pthread_attr_t attr;
    struct sched_param schedParam;
    unsigned short stack_size;
    int fSchedPolicy;
    int fPrioMin;
    int fPrioMax;
    int result;
    
    fSchedPolicy = SCHED_RR;
    
    fPrioMin = sched_get_priority_min(fSchedPolicy);
    fPrioMax = sched_get_priority_max(fSchedPolicy);
    
    result = sched_getparam(0, &schedParam);
    schedParam.sched_priority = fPrioMin;
    result = sched_setscheduler(0, fSchedPolicy, &schedParam);
    printf("The new scheduler is %d prioMin %d prioMax %d\n", sched_getscheduler(0),fPrioMin, fPrioMax);
    
    
    
    result = pthread_attr_init(&attr);
    result = pthread_attr_getschedparam(&attr, &schedParam);
    
    pthread_attr_setschedpolicy(&attr, fSchedPolicy);
    
    schedParam.sched_priority = 35;
    result = pthread_attr_setschedparam(&attr, &schedParam);
    stack_size = PTHREAD_STACK_MIN;
    result = pthread_attr_setstacksize(&attr, stack_size);
    result = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(result)
    {
	perror("vt");
    }
    
    if (pthread_create(tid, &attr, fnPtr, 0) == 0)
    {
	//printf("Video task created\n\n");
    }
    
    pthread_attr_destroy(&attr);
}

static void videoTask()
{
    VideoDecoderRateBufferState bufferState;
    
    while (1)
	
    {
	
	struct pollfd fds[1];
	
	int rc = 0;
	
	int event;
	
	fds[0].fd = VideoFD;
	
	fds[0].events = POLLPRI;
	
#if 0
      if (ioctl (VideoFD, VIDEO_DECODER_GET_RATE_BUFFER_STATE, &bufferState)
	  == -1)
	  
      {
	  
	  printf
	  ("VIDEO_DECODER_GET_RATE_BUFFER_STATE ioctl failed, errno = %d\n",
	    errno);
	    exit (0);
      }
      
      else
      {
	  if (bufferState.currentLevel <= 0)
	      printf
	      ("VIDEO_DECODER_GET_RATE_BUFFER_STATE ioctl SUCCESS, bufferLevel = %d\n",
		bufferState.currentLevel);
		
      }
#endif
      rc = poll (fds, 1, 2000 /* 2 seconds timeout */ );
      
      if ((fds[0].revents & POLLPRI) == POLLPRI)
	  
      {	    
	  
	  rc = ioctl (VideoFD, VIDEO_DECODER_GET_EVENTS, &event);
	  //printf("--- Some events have come ... %d\n", event);     
      
	  if ((event & VIDEO_DECODER_EVENT_FRAME_DECODED) & checkBit)
	      
	  {	      
	      printf("-----decoded Frame OK\n");
	      checkBit =  ~(VIDEO_DECODER_EVENT_FRAME_DECODED) & checkBit;
	  }
	  
	  if ((event & VIDEO_DECODER_EVENT_NO_FRAME_TO_PRESENT) & checkBit)
	      
	  {
	      
	      printf
	      ("-----VIDEO_DECODER_EVENT_NO_FRAME_TO_PRESENT occured\n");
	      
	  }
	  
	  if ((event & VIDEO_DECODER_EVENT_PARAMETER_CHANGE) & checkBit)
	      
	  {
	      
	      printf ("-----VIDEO_DECODER_EVENT_PARAMETER_CHANGE occured\n");
	      
	  }
	  
	  
	  if ((event & VIDEO_DECODER_EVENT_FRAME_PRESENTED) & checkBit)
	      
	  {
	      framePresentedTime = getTime();
	      printf ("Frame Presented Time  : %ld\n", framePresentedTime);
	      firstFramePresented = 1;
	      checkBit =  ~(VIDEO_DECODER_EVENT_FRAME_PRESENTED) & checkBit;
	  }
      }
      
      else
	  
      {
	  
	  //printf("IFRAME POLL FAILED, errno = %d rc = %d\n", errno, rc);
      }
      
    }
}

static void stopDevices()
{
  uint32_t mode = DEMUX_TS_MODE_MPEG;
  
  if(VideoFD != -1)
  {
    if (ioctl (VideoFD, VIDEO_DECODER_STOP) == -1)
    {

      printf ("VIDEO_DECODER_STOP ioctl failed, errno = %d\n", errno);
      exit (0);
    }
  }
  

  if(AudioFD != -1)
  {
    if (ioctl (AudioFD, AUDIO_DECODER_STOP) == -1)
    {
      printf ("AUDIO_DECODER_STOP ioctl failed, errno = %d\n", errno);
      exit (0);
    }
  }
  
   /* Close all the channel fds is already open */
  /* tschannelFD, tpidFD, tpidA, filterFD, audioFilterFD*/
  /* Assmued that the decoders are stopped before hand */
  if(audioFilterFD != -1)
  {
    if (ioctl (audioFilterFD, DEMUX_STOP) == -1)
    {
      printf ("AUDIO_DEMUX_STOP failed with error=%d\n", errno);
      exit (0);
    }
    close(audioFilterFD);
    audioFilterFD = -1;
  }
  if(filterFD != -1)
  {
    if (ioctl (filterFD, DEMUX_STOP) == -1)
    {
      printf ("VIDEO_DEMUX_STOP failed with error=%d\n", errno);
      exit (0);
    }
    close(filterFD);
    filterFD = -1;
  }  
  if(clockfilterFD != -1)
  {
    if (ioctl (clockfilterFD, DEMUX_STOP) == -1)
    {
      printf ("DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }
    
    close(clockfilterFD);
    clockfilterFD = -1;
  }  
  if(tpidA != -1)
  {
    close(tpidA);
    tpidA = -1;
  }
  if(tpidFD != -1)
  {
    close(tpidFD);
    tpidFD = -1;
  }
  if(tpidFD1 != -1)
  {
    close(tpidFD1);
    tpidFD1 = -1;
  }  
  if(clocksyncFD != -1)
  {
    close(clocksyncFD);
    clocksyncFD = -1;
  }

  
  if(tschannelFD != -1)
  {
    if (ioctl (tschannelFD, DEMUX_SET_TS_MODE, &mode) == -1)
    {
      printf ("DEMUX_SET_TS_MODE failed with error=%d\n", errno);
      exit (0);
    }
    close(tschannelFD);
    tschannelFD = -1;
  }

}

static void startDevices()
{
  if(filterFD != -1)
  {
    if (ioctl (filterFD, DEMUX_START) == -1)
    {
      printf ("DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }
  }

  if(audioFilterFD != -1)
  {
    if (ioctl (audioFilterFD, DEMUX_START) == -1)
    {
      printf ("AUDIO_DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }
  }

  if(VideoFD != -1)
  {
    if (ioctl (VideoFD, VIDEO_DECODER_FLUSH_RATE_BUFFER) == -1)
    {
      printf ("VIDEO_DECODER_FLUSH_RATE_BUFFER ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }
    
    if (ioctl (VideoFD, VIDEO_DECODER_START) == -1)
    {
      printf ("VIDEO_DECODER_START ioctl failed, errno = %d\n", errno);
      exit (0);
    }
  }
  
  if(AudioFD != -1)
  {
    if (ioctl (AudioFD, AUDIO_DECODER_FLUSH_RATE_BUFFER) == -1)
    {
      printf ("AUDIO_DECODER_FLUSH_RATE_BUFFER ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }
    
    if (ioctl (AudioFD, AUDIO_DECODER_START) == -1)
    {
      printf ("AUDIO_DECODER_START ioctl failed, errno = %d\n", errno);
      exit (0);
    }
  }
}

//Wait for first frame presented event to come and then start zapping ...

//Channel Zap
/*
#define SAME_TRANSPONDER_SDHD_ZAPPING_COUNT 10
#define DIFFERENT_TRANSPONDER_SDHD_ZAPPING_COUNT 10
#define DIFFERENT_TRANSPONDER_SDSD_ZAPPING_COUNT 10
#define TOTAL_CHANNEL_ZAPS  1000
*/
static void channelZap()
{
    int sameTransponderSDHDIndexes[2] = { 1, 2};
    int diffTransponderSDHDIndexes[2] = { 0, 1};
    int diffTransponderSDSDIndexes[2] = { 0, 2};
    
    int startChannelZapTime = 0;
    
    int i = 0;
    int j = 0;
    int k = 0;
    int noOfEntries = sizeof(zap_data)/(sizeof(int) * NO_COLUMNS);
    if(noOfEntries < 2)
    {
	printf("Insufficient data in \"zap_data\" to zap\n");
	return;
    }
    
 
    
    while(!firstFramePresented)
    {
      usleep(1000000);
    }
    
    usleep(3000000);
   
}

static unsigned long getTime()
{
    struct timeval time;
    int rval = 0;
    unsigned long timeStamp = 0;
    
    rval = gettimeofday(&time, 0);
    if((!time.tv_sec) && (!time.tv_usec))
    {
	printf("SCHED: timeval is ZERO\n");
	return -1;
    }
    if(rval)
    {
	printf("SCHED: gettimeofday FAILED!!, errno = %d\n", errno);
	return -1;
    }
    timeStamp = (time.tv_sec * 1000 + time.tv_usec/1000);
    
    return timeStamp;
}


static void
setupTuner (uint32_t freq, uint32_t symbolRate, uint32_t mode)
{

  int lnbID = 0;
  uint32_t tuner_mode = 0;
  TunerTune tune;
  TunerCaps caps;
  LNBConfiguration lnbConf;
  
  if(lnbFD == -1 && tunerFD == -1)
  {
    lnbFD = open (LNB_PATH, O_RDWR);
    if (lnbFD)
    {
      printf ("LNB OPEN SUCCESSFULL\n");
    }
    else
    {
      printf ("LNB OPEN FAILED!!!, errno = %d\n", errno);
      exit (0);
    }

    tunerFD = open (TUNER_PATH, O_RDWR);
    if (tunerFD)
    {
      printf ("TUNER OPEN SUCCESSFULL\n");
    }
    else
    {
      printf ("TUNER OPEN FAILED!!!, errno = %d\n", errno);
      exit (0);
    }

    if (ioctl(tunerFD, TUNER_GET_MODES, &tuner_mode))
    {
      printf ("TUNER_GET_MODES failed with error=%d\n", errno);
      exit (0);
    } 
    else
    {
            printf("\nSupported Mode = %x", tuner_mode);
    }

    if (ioctl (tunerFD, TUNER_ATTACH_LNB, 0) == -1)
    {
      printf ("TUNER_ATTACH_LNB failed with error=%d\n", errno);
    }
    else
    {
      printf ("TUNER_ATTACH_LNB SUCCESS\n");
    }

    lnbConf.polarisation = LNB_POLARISATION_VERTICAL;
    lnbConf.band = LNB_BAND_LOW;

    if (ioctl (lnbFD, LNB_CONFIGURE, &lnbConf) == -1)
    {
      printf ("LNB CONFIGURE failed with error=%d\n", errno);
      exit (0);
    }
    else
    {
      printf ("LNB CONFIGURE SUCCESS\n");
    }

    if (ioctl (lnbFD, LNB_SET_SAT_POSITION, LNB_POSITION_A) == -1)
    {
      printf ("LNB SAT POSITION failed with error=%d\n", errno);
      exit (0);
    }
    else
    {
      printf ("LNB SAT POSITION SUCCESS\n");
    }
   }

  if(mode == TUNER_MODE_SAT_DVB_S)
  {
    tune.mode = TUNER_MODE_SAT_DVB_S;
    tune.u.DVBS.frequency = freq;
    tune.u.DVBS.fec = TUNER_FEC_3_4;
    tune.u.DVBS.inversion = TUNER_INVERSION_OFF;
    tune.u.DVBS.symbolRate = symbolRate;
  }
  else if( mode == TUNER_MODE_SAT_DVB_S2)
  {
    tune.mode = TUNER_MODE_SAT_DVB_S2;
    tune.u.DVBS2.frequency = freq;
    tune.u.DVBS2.fec = TUNER_FEC_2_3;
    tune.u.DVBS2.inversion = TUNER_INVERSION_AUTO;
    tune.u.DVBS2.rollOff = TUNER_ROLL_OFF_AUTO    ;
    tune.u.DVBS2.modulation.psk = TUNER_MODULATION_8PSK;
    tune.u.DVBS2.symbolRate = symbolRate;
  }  

  if (ioctl (tunerFD, TUNER_TUNE, &tune) == -1)
  {
    printf ("TUNER_TUNE failed with error=%d\n", errno);
    exit (0);
  }
  else
  {
    //printf ("TUNER_TUNE SUCCESS\n");
  }


  if (ioctl
      (tunerFD, TUNER_SUBSCRIBE_EVENTS,
       TUNER_EVENT_CARRIER_LOCKED |
       TUNER_EVENT_SYNC_LOCKED | TUNER_EVENT_DEMOD_LOCKED) == -1)
  {
    printf ("TUNER_SUBSCRIBE_EVENTS failed with error=%d\n", errno);
    exit (0);
  }
  else
  {
    //printf ("TUNER_SUBSCRIBE_EVENTS SUCCESS\n");
  }


  {
    struct pollfd fds[1];
    int rc = 0;
    int event;
    fds[0].fd = tunerFD;
    fds[0].events = POLLPRI;

    rc = poll (fds, 1, 2000 /* 2 seconds timeout */ );

    if ((rc == 1) && ((fds[0].revents & POLLPRI) == POLLPRI))
    {
      rc = ioctl (tunerFD, TUNER_GET_EVENTS, &event);
      if ((event & TUNER_EVENT_CARRIER_LOCKED))
      {
        //printf ("-----TUNER_EVENT_CARRIER_LOCKED\n");
      }

      if ((event & TUNER_EVENT_SYNC_LOCKED))
      {
        //printf ("-----TUNER_EVENT_SYNC_LOCKED\n");
      }

      if ((event & TUNER_EVENT_DEMOD_LOCKED))
      {
        //printf ("-----TUNER_EVENT_DEMOD_LOCKED\n");
      }
    }
    else
    {
      printf("TUNER_POLL timedout or resulted error, errno = %d rc = %d\n",
          errno, rc);
      return;
      //exit(0);
    }
  }
}




static void
setupDemux (uint32_t video_pid, uint32_t audio_pid, uint32_t streamType)
{

  int tschannelID = 0;

  int tpidID = 0;

  int tpidIDA = 2;

  char source[] = TUNER_PATH;

  DeviceName deviceName;

  uint32_t mode = DEMUX_TS_MODE_MPEG;

  size_t buffer_size = 32 * 1024;

  DemuxElementaryfilter filterC;

  uint32_t pid = video_pid;  
  uint32_t Audiopid = audio_pid;
 
  if(demuxFD == -1)
  {
  demuxFD = open (DEMUX_PATH, O_RDWR);

  if (demuxFD)

    {

      //printf ("DEMUX OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("DEMUX OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }
  }

  tschannelFD = open (TSCHANNEL_PATH, O_RDWR);

  if (tschannelFD)

    {

     // printf ("TSCHANNEL OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("TSCHANNEL OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  tpidFD = open (TPID_PATH, O_RDWR);

  if (tpidFD)

    {

      //printf ("TPID OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("TPID OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  tpidA = open (TPID_PATHA, O_RDWR);

  if (tpidA)

    {

      //printf ("TPIDA OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("TPIDA OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  filterFD = open (FILTER_PATH, O_RDWR);

  if (filterFD)

    {

      //printf ("FILTER OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("FILTER OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  audioFilterFD = open (FILTER_PATH, O_RDWR);

  if (audioFilterFD)

    {

      //printf ("AUDIO FILTER OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("AUDIO FILTER OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  if (ioctl (tschannelFD, DEMUX_SET_TS_MODE, &mode) == -1)

    {

      printf ("DEMUX_SET_TS_MODE failed with error=%d\n", errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_SET_TS_MODE SUCCESS\n");

    }



  deviceName.name = source;

  deviceName.length = strlen (source) + 1;

  if (ioctl (tschannelFD, DEMUX_ATTACH_SOURCE, &deviceName) == -1)

    {

      printf ("DEMUX_ATTACH_SOURCE failed with error=%d\n", errno);

    }

  else

    {

      //printf ("DEMUX_ATTACH_SOURCE SUCCESS\n");

    }


  if (ioctl (tpidFD, DEMUX_ATTACH_TSCHANNEL, &tschannelID) == -1)

    {

      printf ("%s : DEMUX_ATTACH_TSCHANNEL failed with error=%d\n", __func__, errno);

    }

  else

    {

      //printf ("DEMUX_ATTACH_TSCHANNEL SUCCESS\n");

    }


  if (ioctl (tpidA, DEMUX_ATTACH_TSCHANNEL, &tschannelID) == -1)

    {

      printf ("%s : DEMUX_ATTACH_TSCHANNEL failed with error=%d\n", __func__, errno);

    }

  else

    {

      //printf ("DEMUX_ATTACH_TSCHANNEL SUCCESS\n");

    }



  if (ioctl (filterFD, DEMUX_ATTACH_TPIDFILTER, &tpidID) == -1)

    {

      printf ("DEMUX_ATTACH_TPIDFILTER failed with error=%d\n", errno);

    }

  else

    {

      //printf ("DEMUX_ATTACH_TPIDFILTER SUCCESS\n");

    }


  if (ioctl (audioFilterFD, DEMUX_ATTACH_TPIDFILTER, &tpidIDA) == -1)

    {

      printf ("DEMUX_ATTACH_TPIDFILTER failed with error=%d\n", errno);

    }

  else

    {

      //printf ("DEMUX_ATTACH_TPIDFILTER SUCCESS\n");

    }

  if(streamType = HD_TYPE)
  {
    filterC.filterMode = DEMUX_ELEMENTARYFILTER_MODE_PES;
    filterC.outputDevice.length = strlen (VIDEO_PATH) + 1;
    filterC.outputDevice.name = VIDEO_PATH;
    filterC.streamType = MPEG4_13181_1;
  }
  else
  {
    filterC.filterMode = DEMUX_ELEMENTARYFILTER_MODE_PES;
    filterC.outputDevice.length = strlen (VIDEO_PATH) + 1;
    filterC.outputDevice.name = VIDEO_PATH;
    filterC.streamType = MPEG2_13181_1;
  }

  if (ioctl (filterFD, DEMUX_ELEMENTARYFILTER_SET_FILTER, &filterC) == -1)

    {

      printf ("DEMUX_ELEMENTARYFILTER_SET_FILTER failed with error=%d\n",
              errno);

    }

  else

    {

      //printf ("DEMUX_ELEMENTARYFILTER_SET_FILTER SUCCESS\n");

    }


  filterC.filterMode = DEMUX_ELEMENTARYFILTER_MODE_PES;

  filterC.outputDevice.length = strlen (AUDIO_DECODER_PATH) + 1;

  filterC.outputDevice.name = AUDIO_DECODER_PATH;

  filterC.streamType = 0x04;

  if (ioctl (audioFilterFD, DEMUX_ELEMENTARYFILTER_SET_FILTER, &filterC) ==
      -1)

    {

      printf ("DEMUX_ELEMENTARYFILTER_SET_FILTER failed with error=%d\n",
              errno);

    }

  else

    {

      //printf ("DEMUX_ELEMENTARYFILTER_SET_FILTER SUCCESS\n");

    }



  if (ioctl (tpidFD, DEMUX_TPIDFILTER_SET_TPID, &pid) == -1)

    {

      printf ("DEMUX_TPIDFILTER_SET_TPID failed with error=%d\n", errno);

    }

  else

    {

      //printf ("DEMUX_TPIDFILTER_SET_TPID SUCCESS\n");

    }


  if (ioctl (tpidA, DEMUX_TPIDFILTER_SET_TPID, &Audiopid) == -1)

    {

      printf ("DEMUX_TPIDFILTER_SET_TPID failed with error=%d\n", errno);

    }

  else

    {

     // printf ("DEMUX_TPIDFILTER_SET_TPID SUCCESS\n");

    }


}


static void
setupDescrambler (void)
{

  uint8_t tschannelID = 0;

  uint32_t VPid = VIDEO_PID;

  DemuxDescramblerControlWord cw;


  DescramblerFD = open (DESCRAMBLER_PATH, O_RDWR);

  if (DescramblerFD)

    {

      printf ("DESCRAMBLER OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("DESCRAMBLER OPEN FAILED!!!, errno = %d\n", errno);

      return;

    }


  if (ioctl (DescramblerFD, DEMUX_ATTACH_TSCHANNEL, &tschannelID) == -1)

    {

      printf ("%s : DEMUX_ATTACH_TSCHANNEL failed with error=%d\n", __func__, errno);

    }

  else

    {

      printf ("DEMUX_ATTACH_TSCHANNEL SUCCESS\n");

    }


  if (ioctl (DescramblerFD, DEMUX_DESCRAMBLER_ADD_TPID, &VPid) == -1)

    {

      printf ("DEMUX_DESCRAMBLER_ADD_TPID failed with error=%d\n", errno);

    }

  else

    {

      printf ("DEMUX_DESCRAMBLER_ADD_TPID SUCCESS\n");

    }


  if (ioctl (DescramblerFD, DEMUX_DESCRAMBLER_SET_CONTROL_WORD, &cw) == -1)

    {

      printf ("DEMUX_DESCRAMBLER_SET_CONTROL_WORD failed with error=%d\n",
              errno);

    }

  else

    {

      printf ("DEMUX_DESCRAMBLER_SET_CONTROL_WORD SUCCESS\n");

    }


  if (ioctl (DescramblerFD, DEMUX_START) == -1)

    {

      printf ("DESCRAMBLER_DEMUX_START failed with error=%d\n", errno);

    }

  else

    {

      printf ("DESCRAMBLER_DEMUX_START SUCCESS\n");

    }



}


static void
setupClockSyncDevice (uint32_t pcr_pid)
{

  DeviceName deviceName;

  int clocksyncID = 0;

  int tpidfilterid = 1;

  int tschannelID = 0;

  int pid = pcr_pid;            //0x210;
 
  clocksyncFD = open (CLOCK_SYNC_PATH, O_RDWR);

  if (clocksyncFD)

    {

      //printf ("clocksyncFD OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("clocksyncFD OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  tpidFD1 = open (TPID_PATH1, O_RDWR);

  if (tpidFD1)

    {

      //printf ("TPID OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("TPID OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  if (ioctl (tpidFD1, DEMUX_ATTACH_TSCHANNEL, &tschannelID) == -1)

    {

      printf ("%s : DEMUX_ATTACH_TSCHANNEL failed with error=%d\n", __func__, errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_ATTACH_TSCHANNEL SUCCESS\n");

    }


  if (ioctl (tpidFD1, DEMUX_TPIDFILTER_SET_TPID, (uint32_t *) & pid) == -1)

    {

      printf ("%s : DEMUX_ATTACH_TSCHANNEL failed with error=%d\n", __func__, errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_ATTACH_TSCHANNEL SUCCESS\n");

    }



  clockfilterFD = open (CLOCK_FILTER_PATH, O_RDWR);

  if (clockfilterFD)

    {

      //printf ("clockfilterFD OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("clockfilterFD OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }


  if (ioctl
      (clockfilterFD, DEMUX_CLOCKFILTER_ATTACH_CLOCK_SYNC,
       &clocksyncID) == -1)

    {

      printf ("DEMUX_CLOCKFILTER_ATTACH_CLOCK_SYNC failed with error=%d\n",
              errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_CLOCKFILTER_ATTACH_CLOCK_SYNC SUCCESS\n");

    }


  if (ioctl (clockfilterFD, DEMUX_ATTACH_TPIDFILTER, &tpidfilterid) == -1)

    {

      printf ("DEMUX_ATTACH_TPIDFILTER failed with error=%d\n", errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_ATTACH_TPIDFILTER SUCCESS\n");

    }


  if (ioctl (clockfilterFD, DEMUX_START) == -1)

    {

      printf ("DEMUX_START failed with error=%d\n", errno);
      exit (0);
    }

  else

    {

      //printf ("DEMUX_START SUCCESS\n");

    }




  deviceName.name = CLOCK_FILTER_PATH;

  deviceName.length = strlen (CLOCK_FILTER_PATH) + 1;

  if (ioctl (clocksyncFD, CLOCK_SYNC_SET_SOURCE, &deviceName) == -1)

    {

      printf ("CLOCK_SYNC_SET_SOURCE failed with error=%d\n", errno);
      exit (0);
    }
}



static void
setupVideodecoder (uint32_t streamType)
{

  int result;

  int i, rval, j;

  uint32_t version;

  //VideoDecoderCaps caps;

  VideoDecoderConfigurationEx Config;
  
  VideoFD = open(VIDEO_PATH, O_RDWR);
     if(VideoFD)
     {
     printf("VIDEO OPEN SUCCESSFULL\n");
     }
     else
     {
     printf("VIDEO OPEN FAILED!!!\n");
     } 
 
  if (ioctl (VideoFD, VIDEO_DECODER_GET_CAPS, &caps) == -1)

    {

      printf ("VIDEO_DECODER_GET_CAPS FAILED!!!\n");
      exit (0);
    }

  else

    printf ("VIDEO_DECODER_GET_CAPS SUCCESS\n");


  if(streamType == HD_TYPE)
  {
    Config.encodedFormat = VIDEO_DECODER_ENCODED_FORMAT_MPEG4_AVC; // VIDEO_DECODER_ENCODED_FORMAT_MPEG2;
  }
  else
  {
    Config.encodedFormat = VIDEO_DECODER_ENCODED_FORMAT_MPEG2;
  }

  Config.source = VIDEO_DECODER_INPUT_DEMUX;

  Config.streamFormat = STREAM_FORMAT_MPEG_PES;

  Config.streamId = VIDEO_DECODER_IGNORE_ID;

  if (ioctl (VideoFD, VIDEO_DECODER_CONFIGURE_EX, &Config) == -1)
    {
      printf ("VIDEO_DECODER_CONFIGURE_EX ioctl failed, errno = %d\n", errno);
      exit (0);
    }
  
  if (ioctl (VideoFD, VIDEO_DECODER_ATTACH_CLOCK_SYNC, 0) == -1)
    {
      printf ("VIDEO_DECODER_ATTACH_CLOCK_SYNC ioctl failed, errno = %d\n",
              errno);
      exit (0);
    }

}



static void
setupAudiodecoder (void)
{

  AudioDecoderConfigurationEx2 config;

  DeviceName deviceName;

  AudioMixerAttachment Mixerattach;

  AudioMixerPortAttenuation pAttenuation;

  ConnectionName sourcedev;

  char device[64];


  AudioFD = open (AUDIO_DECODER_PATH, O_RDWR);

  if (AudioFD)

    {

      printf ("AUDIO OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("AUDIO OPEN FAILED!!!\n");
      exit (0);
    }


  config.encodedFormat = AUDIO_DECODER_ENCODED_MPEG;

  //config.pcmFlags = 0;

  //config.pcmSampleRate = 0;

  //config.pcmSampleSize = 0;

  config.source = AUDIO_DECODER_INPUT_DEMUX;

  config.streamFormat = STREAM_FORMAT_MPEG_PES;

  config.streamId = AUDIO_DECODER_IGNORE_ID;

  //printf(" %d %d %d %d \n", AUDIO_DECODER_ENCODED_MPEG, AUDIO_DECODER_INPUT_DEMUX, STREAM_FORMAT_MPEG_PES, AUDIO_DECODER_IGNORE_ID);
  if (ioctl (AudioFD, AUDIO_DECODER_CONFIGURE_EX, &config) == -1)

    {

      printf ("AUDIO_DECODER_CONFIGURE_EX FAILED!!! err = %d\n",errno);
      exit (0);
    }

  else

    printf ("AUDIO_DECODER_CONFIGURE_EX SUCCESS\n");


  AudioOutFD = open (AUDIO_OUTPUT_PATH, O_RDWR);

  if (AudioOutFD)

    {

      printf ("AUDIO_OUT OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("AUDIO_OUT OPEN FAILED!!!\n");
      exit (0);
    }


  sourcedev.device.name = device;

  sourcedev.device.length = 64U;

  sourcedev.connectionIndex = 0;

  if (ioctl (AudioOutFD, AUDIO_OUTPUT_GET_CONNECTION_NAME, &sourcedev) == -1)

    {

      printf ("AUDIO_OUTPUT_GET_CONNECTION_NAME FAILED!!!, errno = %d\n",
              errno);
      exit (0);
    }

  else

    printf
      ("AUDIO_OUTPUT_GET_CONNECTION_NAME SUCCESS, connection name = %s\n",
       sourcedev.device.name);


  if (ioctl (AudioOutFD, AUDIO_OUTPUT_ATTACH_SOURCE, &sourcedev.device) == -1)

    {

      printf ("AUDIO_OUTPUT_ATTACH_SOURCE FAILED!!!, errno = %d\n", errno);
      exit (0);
    }

  else

    printf ("AUDIO_OUTPUT_ATTACH_SOURCE SUCCESS\n");


  stereoFD = open (STEREO_PATH, O_RDWR);

  if (stereoFD)

    {

      printf ("STEREO OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("STEREO OPEN FAILED!!!\n");
      exit (0);
    }


  if (ioctl (AudioFD, AUDIO_DECODER_ATTACH_CLOCK_SYNC, 0) == -1)

    {

      printf ("AUDIO_DECODER_ATTACH_CLOCK_SYNC FAILED!!!\n");
      exit (0);
    }

  else

    printf ("AUDIO_DECODER_ATTACH_CLOCK_SYNC SUCCESS\n");


}



void
setupSurface (void)
{

  int retval;

  int rv;

  int i;

  uint8_t numVideoSurfaces = 0;

  uint8_t numIframeSurfaces = 0;

  uint8_t numOSDSurfaces = 0;

  uint32_t version;

  SurfaceCapsEx caps;

  uint32_t functionalityMask;

  DeviceName deviceName;

  SurfaceConfiguration Config;


  printf ("Entering SetupSurface");


  surfaceFD = open (SURFACE_PATH, O_RDWR);

  if (surfaceFD > 0)

    {

      printf ("SURFACE OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SURFACE OPEN FAILED!!!\n");

    }


  if (ioctl (surfaceFD, SURFACE_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("SURFACE GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("SURFACE GET_CAPS_EX_VER SUCCESS\n");


  if (version & SURFACE_CAP_V1)

    {

      caps.version = SURFACE_CAP_V1;

      caps.capabilitySize = sizeof (SurfaceCapsV1);

      caps.capability = malloc (sizeof (SurfaceCapsV1));


      if (ioctl (surfaceFD, SURFACE_GET_CAPS_EX, &caps) == -1)

        {

          printf ("SURFACE GET_CAPS_EX FAILED!!!\n");

        }

      else

        printf ("SURFACE GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((SurfaceCapsV1 *) (caps.capability))->functionalityMask;

      if (functionalityMask & SURFACE_FEATURE_VIDEO)

        printf ("THE SURFACE OPENED SUPPORTS VIDEO\n");

      else
        {
          printf ("THE SURFACE DOESNT SUPPORT VIDEO\n");
          printf ("functionalityMask : %X\n\n", functionalityMask);
          exit (0);
        }
    }


  deviceName.length = strlen ("/dev/nds/videodecoder0") + 1;

  deviceName.name = "/dev/nds/videodecoder0";

  if (ioctl (surfaceFD, SURFACE_ATTACH_DECODER, &deviceName) == -1)

    {

      printf ("ioctl SURFACE_ATTACH_DECODER FAILED!! errno = %d\n", errno);

    }

  else

    {

      printf ("ioctl SURFACE_ATTACH_DECODER SUCCESS\n");

    }

  Config.clutFormat = 0;

  Config.pixelFormat = 2;//PIXEL_FORMAT_YUV420_MB;

  Config.dimension.height = 576;//720;

  Config.dimension.width = 720;//1280;

  rv = ioctl (surfaceFD, SURFACE_CONFIGURE, &Config);

  if (rv < 0)

    {

      printf (" SURFACE_CONFIGURE ioctl failed, rv = %d, errno = %d\n", rv,
              errno);
      exit (0);
    }

  else

    {

      printf ("VoSurfaceSetConfig: Set the surface config successfully\n");

    }




/* Graphics Surface */

/* HD GR Surface */
  HDgfxFD = open (HD_GR_SURFACE_PATH, O_RDWR);

  if (HDgfxFD > 0)

    {

      printf ("SURFACE OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SURFACE OPEN FAILED!!!\n");

    }


  if (ioctl (HDgfxFD, SURFACE_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("SURFACE GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("SURFACE GET_CAPS_EX_VER SUCCESS\n");


  if (version & SURFACE_CAP_V1)

    {

      caps.version = SURFACE_CAP_V1;

      caps.capabilitySize = sizeof (SurfaceCapsV1);

      caps.capability = malloc (sizeof (SurfaceCapsV1));


      if (ioctl (HDgfxFD, SURFACE_GET_CAPS_EX, &caps) == -1)

        {

          printf ("SURFACE GET_CAPS_EX FAILED!!!\n");

        }

      else

        printf ("SURFACE GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((SurfaceCapsV1 *) (caps.capability))->functionalityMask;

      if (functionalityMask & SURFACE_FEATURE_GFX)

        printf ("THE SURFACE OPENED SUPPORTS GFX\n");

      else
        {
          printf ("THE SURFACE DOESNT SUPPORT GFX\n");
          printf ("functionalityMask : %X\n\n", functionalityMask);
          exit (0);
        }
    }

  Config.clutFormat = 0;

  Config.pixelFormat = PIXEL_FORMAT_ARGB_8888_PACKED;

  Config.dimension.height = 576;

  Config.dimension.width = 720;

  rv = ioctl (HDgfxFD, SURFACE_CONFIGURE, &Config);

  if (rv < 0)

    {

      printf (" HD GFX SURFACE_CONFIGURE ioctl failed, rv = %d, errno = %d\n",
              rv, errno);
      exit (0);
    }

  else

    {

      printf
        ("VoSurfaceSetConfig: Set the surface config successfully for HD GFX\n");

    }


#if 0
/* SD GR Surface */
  SDgfxFD = open (SD_GR_SURFACE_PATH, O_RDWR);

  if (SDgfxFD > 0)

    {

      printf ("SURFACE OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SURFACE OPEN FAILED!!!\n");

    }


  if (ioctl (SDgfxFD, SURFACE_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("SURFACE GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("SURFACE GET_CAPS_EX_VER SUCCESS\n");


  if (version & SURFACE_CAP_V1)

    {

      caps.version = SURFACE_CAP_V1;

      caps.capabilitySize = sizeof (SurfaceCapsV1);

      caps.capability = malloc (sizeof (SurfaceCapsV1));


      if (ioctl (SDgfxFD, SURFACE_GET_CAPS_EX, &caps) == -1)

        {

          printf ("SURFACE GET_CAPS_EX FAILED!!!\n");

        }

      else

        printf ("SURFACE GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((SurfaceCapsV1 *) (caps.capability))->functionalityMask;

      if (functionalityMask & SURFACE_FEATURE_GFX)

        printf ("THE SURFACE OPENED SUPPORTS GFX\n");

      else
        {
          printf ("THE SURFACE DOESNT SUPPORT GFX\n");
          printf ("functionalityMask : %X\n\n", functionalityMask);
          exit (0);
        }
    }

  Config.clutFormat = 0;

  Config.pixelFormat = PIXEL_FORMAT_ARGB_8888_PACKED;

  Config.dimension.height = 576;

  Config.dimension.width = 720;

  rv = ioctl (SDgfxFD, SURFACE_CONFIGURE, &Config);

  if (rv < 0)

    {

      printf (" SD GFX SURFACE_CONFIGURE ioctl failed, rv = %d, errno = %d\n",
              rv, errno);
      exit (0);
    }

  else

    {

      printf
        ("VoSurfaceSetConfig: Set the surface config successfully for SD GFX\n");

    }
#endif

/* IFrame Surface */
#if 0
  surfaceFD = open (SURFACE_PATH, O_RDWR);

  if (surfaceFD > 0)

    {

      printf ("SURFACE OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SURFACE OPEN FAILED!!!\n");

    }


  if (ioctl (surfaceFD, SURFACE_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("SURFACE GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("SURFACE GET_CAPS_EX_VER SUCCESS\n");


  if (version & SURFACE_CAP_V1)

    {

      caps.version = SURFACE_CAP_V1;

      caps.capabilitySize = sizeof (SurfaceCapsV1);

      caps.capability = malloc (sizeof (SurfaceCapsV1));


      if (ioctl (surfaceFD, SURFACE_GET_CAPS_EX, &caps) == -1)

        {

          printf ("SURFACE GET_CAPS_EX FAILED!!!\n");

        }

      else

        printf ("SURFACE GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((SurfaceCapsV1 *) (caps.capability))->functionalityMask;

      if (functionalityMask & SURFACE_FEATURE_VIDEO)

        printf ("THE SURFACE OPENED SUPPORTS VIDEO\n");

      else
        {
          printf ("THE SURFACE DOESNT SUPPORT VIDEO\n");
          printf ("functionalityMask : %X\n\n", functionalityMask);
          exit (0);
        }
    }


  deviceName.length = strlen ("/dev/nds/videodecoder0") + 1;

  deviceName.name = "/dev/nds/videodecoder0";

  if (ioctl (surfaceFD, SURFACE_ATTACH_DECODER, &deviceName) == -1)

    {

      printf ("ioctl SURFACE_ATTACH_DECODER FAILED!! errno = %d\n", errno);

    }

  else

    {

      printf ("ioctl SURFACE_ATTACH_DECODER SUCCESS\n");

    }


  Config.clutFormat = 0;

  Config.pixelFormat = PIXEL_FORMAT_YUV420_MB;

  Config.dimension.height = 720;

  Config.dimension.width = 1280;

  rv = ioctl (surfaceFD, SURFACE_CONFIGURE, &Config);

  if (rv < 0)

    {

      printf (" SURFACE_CONFIGURE ioctl failed, rv = %d, errno = %d\n", rv,
              errno);
      exit (0);
    }

  else

    {

      printf ("VoSurfaceSetConfig: Set the surface config successfully\n");

    }
#endif
}




void
setupViewPort (void)
{

  int retval;

  int i;

  int numStillViewport = 0;

  uint32_t version;

  ViewportCapsEx caps;

  uint32_t functionalityMask;

  ViewportAttach viewport_attach;

  int surfaceIDs[5];


  printf ("Entering setupViewports\n");


  ViewPortFD = open (VIEWPORT_PATH, O_RDWR);

  if (ViewPortFD)

    {

      printf ("VIEWPORT OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("VIEWPORT OPEN FAILED!!!\n");

    }


  if (ioctl (ViewPortFD, VIEWPORT_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("VIEWPORT GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("VIEWPORT GET_CAPS_EX_VER SUCCESS\n");



  if (version & VIEWPORT_CAP_V1)

    {

      caps.version = VIEWPORT_CAP_V1;

      caps.capabilitySize = sizeof (ViewportCapsV1);

      caps.capability = malloc (sizeof (ViewportCapsV1));


      if (ioctl (ViewPortFD, VIEWPORT_GET_CAPS_EX, &caps) == -1)

        {

          printf ("VIEWPORT GET_CAPS_EX FAILED!!!\n");
          exit (0);
        }

      else

        printf ("VIEWPORT GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((ViewportCapsV0 *) (caps.capability))->functionalityMask;

      if (functionalityMask & VIEWPORT_FEATURE_VIDEO)

        printf ("THE VIEWPORT OPENED SUPPORTS VIDEO\n");

      else
        {
          printf ("THE VIEWPORT DOESNT SUPPORT VIDEO\n");

          printf ("functionalityMask %x\n\n", functionalityMask);
          exit (0);
        }
    }
  else
    {
      printf ("Wrong Version \n\n");
    }


  viewport_attach.flipFlag = VIEWPORT_FLIP_NONE;

  viewport_attach.surfaceId = VIDEO_SURFACE;

  if (ioctl (ViewPortFD, VIEWPORT_ATTACH_SURFACE, &viewport_attach) == -1)

    {

      printf ("VIEWPORT VIEWPORT_ATTACH_SURFACE FAILED!!!, errno = %d\n",
              errno);

    }

  else

    printf ("VIEWPORT VIEWPORT_ATTACH_SURFACE SUCCESS\n");




/* HD View Port */
  HDViewPortFD = open (HD_VIEWPORT_PATH, O_RDWR);

  if (HDViewPortFD)

    {

      printf ("HD VIEWPORT OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("HD VIEWPORT OPEN FAILED!!!\n");

    }


  if (ioctl (HDViewPortFD, VIEWPORT_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("HD VIEWPORT GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("HD VIEWPORT GET_CAPS_EX_VER SUCCESS\n");



  if (version & VIEWPORT_CAP_V1)

    {

      caps.version = VIEWPORT_CAP_V1;

      caps.capabilitySize = sizeof (ViewportCapsV1);

      caps.capability = malloc (sizeof (ViewportCapsV1));


      if (ioctl (HDViewPortFD, VIEWPORT_GET_CAPS_EX, &caps) == -1)

        {

          printf ("HD VIEWPORT GET_CAPS_EX FAILED!!!\n");
          exit (0);
        }

      else

        printf ("HD VIEWPORT GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((ViewportCapsV0 *) (caps.capability))->functionalityMask;

      if (functionalityMask & VIEWPORT_FEATURE_VIDEO)

        printf ("THE HD VIEWPORT OPENED SUPPORTS VIDEO\n");

      else
        {
          printf ("THE HD VIEWPORT DOESNT SUPPORT VIDEO\n");

          printf ("functionalityMask %x\n\n", functionalityMask);
          exit (0);
        }
    }
  else
    {
      printf ("Wrong Version for HD \n\n");
    }


  viewport_attach.flipFlag = VIEWPORT_FLIP_NONE;

  viewport_attach.surfaceId = VIDEO_SURFACE;

  if (ioctl (HDViewPortFD, VIEWPORT_ATTACH_SURFACE, &viewport_attach) == -1)

    {

      printf ("HD VIEWPORT VIEWPORT_ATTACH_SURFACE FAILED!!!, errno = %d\n",
              errno);

    }

  else
    {
      printf ("HD VIEWPORT VIEWPORT_ATTACH_SURFACE SUCCESS\n");
    }

/* HD GR View Port */
  HDGRViewPortFD = open (HDGR_VIEWPORT_PATH, O_RDWR);

  if (HDGRViewPortFD)

    {

      printf ("HD VIEWPORT OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("HD VIEWPORT OPEN FAILED!!!\n");

    }


  if (ioctl (HDGRViewPortFD, VIEWPORT_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("HD VIEWPORT GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("HD VIEWPORT GET_CAPS_EX_VER SUCCESS\n");



  if (version & VIEWPORT_CAP_V0)

    {

      caps.version = VIEWPORT_CAP_V0;

      caps.capabilitySize = sizeof (ViewportCapsV0);

      caps.capability = malloc (sizeof (ViewportCapsV0));


      if (ioctl (HDGRViewPortFD, VIEWPORT_GET_CAPS_EX, &caps) == -1)

        {

          printf ("HD VIEWPORT GET_CAPS_EX FAILED!!!\n");
          exit (0);
        }

      else

        printf ("HD VIEWPORT GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((ViewportCapsV0 *) (caps.capability))->functionalityMask;

      if (functionalityMask & VIEWPORT_FEATURE_GFX)

        printf ("THE HD VIEWPORT OPENED SUPPORTS VIDEO\n");

      else
        {
          printf ("THE HD VIEWPORT DOESNT SUPPORT VIDEO\n");

          printf ("functionalityMask %x\n\n", functionalityMask);
          exit (0);
        }
    }
  else
    {
      printf ("Wrong Version for HD \n\n");
    }


  viewport_attach.flipFlag = VIEWPORT_FLIP_NONE;

  viewport_attach.surfaceId = HD_SURFACE_GR;

  if (ioctl (HDGRViewPortFD, VIEWPORT_ATTACH_SURFACE, &viewport_attach) == -1)

    {

      printf ("HD VIEWPORT VIEWPORT_ATTACH_SURFACE FAILED!!!, errno = %d\n",
              errno);

    }

  else
    {
      printf ("HD VIEWPORT VIEWPORT_ATTACH_SURFACE SUCCESS\n");
    }

#if 0
/* SD GR View Port */
  SDGRViewPortFD = open (SDGR_VIEWPORT_PATH, O_RDWR);

  if (SDGRViewPortFD)

    {

      printf ("SD GFX VIEWPORT OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SD GFX VIEWPORT OPEN FAILED!!!\n");

    }


  if (ioctl (SDGRViewPortFD, VIEWPORT_GET_CAPS_EX_VERSIONS, &version) == -1)

    {

      printf ("SD GFX VIEWPORT GET_CAPS_EX_VER FAILED!!!\n");

    }

  else

    printf ("SD GFX VIEWPORT GET_CAPS_EX_VER SUCCESS\n");



  if (version & VIEWPORT_CAP_V0)

    {

      caps.version = VIEWPORT_CAP_V0;

      caps.capabilitySize = sizeof (ViewportCapsV0);

      caps.capability = malloc (sizeof (ViewportCapsV0));


      if (ioctl (SDGRViewPortFD, VIEWPORT_GET_CAPS_EX, &caps) == -1)

        {

          printf ("SD GFX VIEWPORT GET_CAPS_EX FAILED!!!\n");
          exit (0);
        }

      else

        printf ("SD GFX VIEWPORT GET_CAPS_EX SUCCESS\n");


      functionalityMask =
        ((ViewportCapsV0 *) (caps.capability))->functionalityMask;

      if (functionalityMask & VIEWPORT_FEATURE_GFX)

        printf ("THE SD VIEWPORT OPENED SUPPORTS GFX\n");

      else
        {
          printf ("THE SD VIEWPORT DOESNT SUPPORT GFX\n");

          printf ("functionalityMask %x\n\n", functionalityMask);
          exit (0);
        }
    }
  else
    {
      printf ("Wrong Version for SD GFX \n\n");
    }


  viewport_attach.flipFlag = VIEWPORT_FLIP_NONE;

  viewport_attach.surfaceId = SD_SURFACE_GR;

  if (ioctl (SDGRViewPortFD, VIEWPORT_ATTACH_SURFACE, &viewport_attach) == -1)

    {

      printf
        ("SD GFX VIEWPORT VIEWPORT_ATTACH_SURFACE FAILED!!!, errno = %d\n",
         errno);

    }

  else
    {
      printf ("SD GFX VIEWPORT VIEWPORT_ATTACH_SURFACE SUCCESS\n");
    }
#endif

}



void
setupBlender (void)
{

  uint8_t zlist[32];

  Dimension pDimension;

//SPDIFAttachments spdifAttach;

//SPDIFAudioConfiguration spdifAudioMode;


  blenderFD = open (BLENDER_PATH, O_RDWR);

  if (blenderFD)

    {

      printf ("BLENDER OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("BLENDER OPEN FAILED!!!, errno = %d\n", errno);

    }


  pDimension.height = 576;

  pDimension.width = 720;

  if (ioctl (blenderFD, BLENDER_SET_CANVAS_DIMENSIONS, &pDimension) == -1)

    {

      printf
        ("VoBlenderSetCanvasDimensions ioctl BLENDER_SET_CANVAS_DIMENSIONS failed err %d",
         errno);

    }

  else
    {
      printf
        ("VoBlenderSetCanvasDimensions ioctl BLENDER_SET_CANVAS_DIMENSIONS SUCCESS\n");
    }

  memset (zlist, (uint8_t) BLENDER_ZLIST_ENTRY_NULL, sizeof (zlist));

zlist[0] = SDVIEWPORT_GR;  
  zlist[0] = VIDEO_VIEWPORT_SD;

  printf ("BLENDER_SET_ZLIST( Video )\n");


  if (ioctl (blenderFD, BLENDER_SET_ZLIST_EX, zlist) < 0)

    {

      printf (" ioctl(BLENDER_SET_ZLIST) failed: %d\n", errno);
      exit (0);
    }

  else
    {
      printf (" ioctl(BLENDER_SET_ZLIST) SUCCESS\n");
    }
/* HD Blender */

  HDBlenderFD = open (HD_BLENDER_PATH, O_RDWR);

  if (HDBlenderFD)

    {

      printf ("HD BLENDER OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("HD BLENDER OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }

/*
spdifFD = open (SPDIF_PATH, O_RDWR);
  
if (spdifFD)
    
    {
      
printf ("SPDIF OPEN SUCCESSFULL\n");
    
}
  
  else
    
    {
      
printf ("SPDIF OPEN FAILED!!!, errno = %d\n", errno);
      
return;
    
}
  

spdifAttach.audioDecoderId = 0;
  
spdifAttach.audioMixerId = 0;
  
if (ioctl (spdifFD, SPDIF_FORMATTER_ATTACH, &spdifAttach) < 0)
    
    {
      
printf ("SPDIF_FORMATTER_ATTACH failed: %d\n", errno);
    
}
  
  else
    
printf ("SPDIF_FORMATTER_ATTACH SUCCESS\n");
  

spdifAudioMode.audioMode = DIGITAL_AUDIO_MODE_IEC60958;
  
spdifAudioMode.sourceDevice = 0;
  
if (ioctl (spdifFD, SPDIF_FORMATTER_SET_AUDIO_MODE, &spdifAudioMode) < 0)
    
    {
      
printf ("SPDIF_FORMATTER_SET_AUDIO_MODE failed: %d\n", errno);
    
}
  
  else
    
printf ("SPDIF_FORMATTER_SET_AUDIO_MODE SUCCESS\n");
  */

  pDimension.height = 720;

  pDimension.width = 1280;

  if (ioctl (HDBlenderFD, BLENDER_SET_CANVAS_DIMENSIONS, &pDimension) == -1)

    {

      printf ("HD ioctl BLENDER_SET_CANVAS_DIMENSIONS failed err %d", errno);
      exit (0);
    }

  else

    printf ("HD ioctl BLENDER_SET_CANVAS_DIMENSIONS SUCCESS\n");


  memset (zlist, (uint8_t) BLENDER_ZLIST_ENTRY_NULL, sizeof (zlist));

  //zlist[0] = HDVIEWPORT_GR;
  //zlist[1] = VIDEO_VIEWPORT_HD;
  zlist[1] = HDVIEWPORT_GR;
  zlist[0] = VIDEO_VIEWPORT_HD;

//zlist[1] = 0;
//zlist[0] = 2;

  printf ("BLENDER_SET_ZLIST( Video )\n");


  if (ioctl (HDBlenderFD, BLENDER_SET_ZLIST_EX, zlist) < 0)

    {

      printf ("HD ioctl(BLENDER_SET_ZLIST) failed: %d\n", errno);
      exit (0);
    }

  else
    {
      printf ("HD ioctl(BLENDER_SET_ZLIST) SUCCESS\n");
    }

}



void
setupDenc (void)
{

  int retval;

  int rv, i, temp;

  uint8_t dacs_cfg_index, blenderID = 0xff;

  DencCaps caps;

  DACsConfiguration DacsConfig;

  DencAttachment attach;

  DencStandardParams standard;

  uint32_t CapsVersion = 0;

  DencCapsEx CapsEx;

  DencCapsV3 V3Caps;

  DencInputConfiguration InputConfig;

  DencInputCaps InputCaps;

  DencInputCaps InputCapsIndex0;

  uint32_t scartVideoFormat = 0;


  DencFD = open (DENC_PATH, O_RDWR);

  if (DencFD)

    {

      printf ("DENC OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("DENC OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);;

    }


  if (ioctl (DencFD, DENC_GET_CAPS_EX_VERSIONS, &CapsVersion) == -1)

    {

      printf ("DENC_GET_CAPS_EX_VERSIONS failed with error=%d", errno);
      exit (0);
    }

  else

    {

      printf ("DENC_GET_CAPS_EX_VERSIONS SUCCESS, capsVersion = %d\n",
              CapsVersion);

    }


if (CapsVersion & DENC_CAP_V3)

    printf ("V3 caps SUPORTED\n");

  else

    {

      printf ("V3 caps not SUPPORTED\n");

      exit (0);;

    }


  CapsEx.version = DENC_CAP_V3;

  CapsEx.capability = &V3Caps;

  CapsEx.capabilitySize = sizeof (DencCapsV3);

  if (ioctl (DencFD, DENC_GET_CAPS_EX, &CapsEx) == -1)

    {

      printf ("DENC_GET_CAPS_EX failed with error=%d", errno);
      exit (0);
    }

  else

    {

      printf ("DENC_GET_CAPS_EX SUCCESS\n");

      printf ("\n******** SUPER DENC *******\n");

      printf
        (" Features = %d\n supportedFormats = %d\n numInputs = %d\n",
         V3Caps.features, V3Caps.supportedOutputFormats, V3Caps.numInputs);

      printf
        (" NumDACS = %d\n NumDACSConfiguration = %d\n NumBlenderConn = %d\n",
         V3Caps.numDACs, V3Caps.numDACsConfigurations,
         V3Caps.numBlenderConnections);

      printf ("**************************\n");

    }

  if (V3Caps.numDACsConfigurations > 0)

    {

      DacsConfig.dacs = malloc (V3Caps.numDACs * sizeof (DACConfiguration));

      if (DacsConfig.dacs != 0)

        {

          for (i = 0; i < V3Caps.numDACsConfigurations; i++)

            {

              DacsConfig.configurationIndex = i;

              rv = ioctl (DencFD, DENC_GET_DACS_CONFIGURATION, &DacsConfig);

              if (rv < 0)

                {

                  printf
                    (" DENC_GET_DACS_CONFIGURATION ioctl failed, rv = %d, errno = %d\n",
                     rv, errno);
                  exit (0);
                }

              else

                {

                  int j = 0;

                  printf
                    (" ---------------- %d configuration ------------------\n",
                     i);

                  printf (" DENC_GET_DACS_CONFIGURATION ioctl SUCCESS\n");

                  for (j = 0; j < V3Caps.numDACs; j++)

                    {

                      printf ("\n********* %d DAC *********\n", j);

                      printf
                        (" VideoFormat = 0x%x\n inputIndex = %d\n AVMixerID = %d\n",
                         DacsConfig.dacs[j].videoFormat,
                         DacsConfig.dacs[j].inputIndex,
                         DacsConfig.dacs[j].avMixerId);

                      printf ("*************************************\n");

                    }

                }

            }

        }

    }

  memset (&InputCaps, 0, sizeof (DencInputCaps));

  for (i = 0; i < V3Caps.numInputs; i++)

    {

      InputCaps.inputIndex = i;

      rv = ioctl (DencFD, DENC_GET_INPUT_CAPS, &InputCaps);

      if (rv < 0)

        {

          printf
            (" DENC_GET_INPUT_CAPS ioctl failed, rv = %d, errno = %d input_index = %d\n",
             rv, errno, i);
          exit (0);
        }

      else

        {

          printf (" DENC_GET_INPUT_CAPS ioctl SUCCESS\n");

          printf ("\n********** %d InputIndex ***********\n", i);

          printf
            (" Features = 0x%x\n VideoFormats = 0x%x\n cvbsStdrds = 0x%x\n VideoTimings = 0x%x\n colorSpace = 0x%x\n",
             InputCaps.features, InputCaps.videoFormats,
             InputCaps.cvbsStandards,
             InputCaps.videoTimings, InputCaps.colourSpaces);

          printf ("***********************************\n");

        }

      //break;
    }


  attach.blenderId = HD_BLENDER;

  attach.inputIndex = 0;

  rv = ioctl (DencFD, DENC_ATTACH_BLENDER, &attach);

  if (rv < 0)

    {

      printf
        (" DENC_ATTACH_BLENDER for HD ioctl failed, rv = %d, errno = %d\n",
         rv, errno);
      exit (0);
    }

  else

    {

      printf (" DENC_ATTACH_BLENDER for HD ioctl SUCCESS\n");

    }


  attach.blenderId = SD_BLENDER;

  attach.inputIndex = 1;

  rv = ioctl (DencFD, DENC_ATTACH_BLENDER, &attach);

  if (rv < 0)

    {

      printf (" DENC_ATTACH_BLENDER ioctl failed, rv = %d, errno = %d\n", rv,
              errno);
      exit (0);
    }

  else

    {

      printf (" DENC_ATTACH_BLENDER ioctl SUCCESS\n");

    }


  InputConfig.colourSpace = 0;

  InputConfig.cvbsStandard = DENC_STANDARD_PAL_BDGHI;

  InputConfig.embeddedSynchro = 0;

  InputConfig.inputIndex = 1;

  InputConfig.videoFormat = DENC_VIDEO_FORMAT_SD;

  InputConfig.videoTiming = DENC_VT_720x576_I_50;

  rv = ioctl (DencFD, DENC_CONFIGURE_INPUT, &InputConfig);

  if (rv < 0)

    {

      printf (" DENC_CONFIGURE_INPUT ioctl failed, rv = %d, errno = %d\n",
              rv, errno);
      exit (0);
    }

  else

    {

      printf (" DENC_CONFIGURE_INPUT ioctl SUCCESS\n");

    }



  InputConfig.colourSpace = DENC_COLOUR_SPACE_SMPTE_240M;

  InputConfig.cvbsStandard = DENC_STANDARD_PAL_BDGHI;

  InputConfig.embeddedSynchro = 1;

  InputConfig.inputIndex = 0;

  InputConfig.videoFormat = DENC_VIDEO_FORMAT_HD;

  InputConfig.videoTiming = DENC_VT_1280x720_P_5994;

  rv = ioctl (DencFD, DENC_CONFIGURE_INPUT, &InputConfig);

  if (rv < 0)

    {

      printf
        (" DENC_CONFIGURE_INPUT for HD ioctl failed, rv = %d, errno = %d\n",
         rv, errno);
      exit (0);
    }

  else

    {

      printf (" DENC_CONFIGURE_INPUT for HD ioctl SUCCESS\n");

    }


  dacs_cfg_index = 0;

  rv = ioctl (DencFD, DENC_SET_DACS_CONFIGURATION, &dacs_cfg_index);

  if (rv < 0)

    {

      printf
        (" DENC_SET_DACS_CONFIGURATION ioctl failed, rv = %d, errno = %d\n",
         rv, errno);
      exit (0);
    }

  else

    printf (" DENC_SET_DACS_CONFIGURATION ioctl SUCCESS\n");

  cvbsFD = open (CVBS_PATH, O_RDWR);

  if (cvbsFD != -1)

    {

      printf ("CVBS OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("CVBS OPEN FAILED!!!, errno = %d\n", errno);
      exit (0);
      //return;
    }


  yuvFD = open (YUV_PATH, O_RDWR);

  if (yuvFD != -1)

    {

      printf ("YUV OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("YUV OPEN FAILED!!!, errno = %d\n", errno);
      exit (0);
      //return;
    }

//SVideo not supported as of now in ST
#if 0
  svideoFD = open (SVIDEO_PATH, O_RDWR);

  if (svideoFD != -1)

    {

      printf ("SVIDEO OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("SVIDEO OPEN FAILED!!!, errno = %d\n", errno);

      exit (0);

    }
#endif
/*
scartFD = open (SCART_PATH, O_RDWR);
  
if (scartFD != -1)
    
    {
      
printf ("SCART OPEN SUCCESSFULL\n");
    
}
  
  else
    
    {
      
printf ("SCART OPEN FAILED!!!, errno = %d\n", errno);    

    }
  

scartVideoFormat = SCART_VIDEO_FORMAT_RGB_CVBS;
  
rv = ioctl (scartFD, SCART_CONFIGURE, &scartVideoFormat);
  
if (rv < 0)
    
    {
      
printf (" SCART_CONFIGURE ioctl failed, rv = %d, errno = %d\n", rv,
               errno);
    
}
  
  else
    
printf (" SCART_CONFIGURE ioctl SUCCESS\n");
*/

}


static void
setupDigitalDisplay (void)
{

  uint8_t zlist[32];

  Dimension pDimension;

  //DVIHDMIAttachments hdmiattach;
  HDMIAttachSource hdmiattach;

  HDMIColorimetry colorimetry1;
  uint8_t blenderId = 0;

  HDMIConnectionStatus presence;


  uint8_t pixelFormat = 1;

  int i = 0;
HDMIAspectRatio hdmi_aspectRatio =
{
        .pictureAspectRatio = HDMI_SOURCE_ASPECT_RATIO_16_9,
        .activeFormatDescriptor = HDMI_AFD_16_9_CENTER,
        .topBarEndsLine = 0,
        .bottomBarStartsLine = 0,
        .leftBarEndsPixel = 0,
        .rightBarStartsPixel = 0,
        .barinfoValidity = HDMI_BARINFO_INVALID,
        .afdValidity = HDMI_AFDINFO_VALID,
        .scanInfo = HDMI_SCANINFO_OVERSCANNED,
        .scalingInfo = HDMI_SCALING_NO_SCALING
};



  hdmiFD = open (HDMI_PATH, O_RDWR);

  if (hdmiFD != -1)

    {

      printf ("HDMI OPEN SUCCESSFULL\n");

    }

  else

    {

      printf ("HDMI OPEN FAILED!!!, errno = %d\n", errno);

      exit(0);

    }


#if 1
  hdmiattach.blenderId = blenderId;

  hdmiattach.spdifFormatterId = 0;

  if (ioctl (hdmiFD, HDMI_ATTACH_SOURCE, &hdmiattach) < 0)

    {

      printf ("HDMI_ATTACH_SOURCE failed: %d\n", errno);
	exit(0);

    }

  else

    printf ("HDMI_ATTACH_SOURCE SUCCESS\n");


  if (ioctl (hdmiFD, HDMI_GET_CONNECTION_STATUS, &presence) < 0)

    {

      
exit(0);
    }

  else

    printf ("HDMI_GET_CONNECTION_STATUS SUCCESS, powered = %d\n",
            presence.powered);

	    i = 4;         
	    
	    if (ioctl (hdmiFD, HDMI_SET_VIDEO_CODE, &i) < 0)
		
	    {
		
		printf ("HDMI_SET_VIDEO_CODE failed: %d\n", errno);
		exit(0);
	    }
	    
	    else
		
		printf ("HDMI_SET_VIDEO_CODE SUCCESS\n");
	    
	    
  pixelFormat = HDMI_PIXEL_FORMAT_RGB;//HDMI_PIXEL_FORMAT_YCbCr422;

  if (ioctl (hdmiFD, HDMI_SET_PIXEL_FORMAT, &pixelFormat) < 0)

    {

      printf ("HDMI_SET_PIXEL_FORMAT failed: %d\n", errno);
exit(0);
    }

  else

    printf ("HDMI_SET_PIXEL_FORMAT SUCCESS\n");

  

if(ioctl( hdmiFD, HDMI_SET_ASPECT_RATIO, &hdmi_aspectRatio ) <0)
{
	printf("HDMI_SET_ASPECT_RATIO failed: %d\n",errno);
exit(0);
}
else
{
                        printf("HDMI_SET_ASPECT_RATIO\n");
}


#if 1
  colorimetry1.colorimetry = HDMI_COLORIMETRY_SMPTE_170M; //HDMI_COLORIMETRY_ITU_709;

  colorimetry1.extendedColorimetry = HDMI_EXTENDED_COLORIMETRY_XVCC709;
  if (ioctl (hdmiFD, HDMI_SET_COLORIMETRY, &colorimetry1) < 0)

    {

      printf ("HDMI_SET_COLORIMETRY failed: %d\n", errno);
exit(0);
    }

  else

    printf ("HDMI_SET_COLORIMETRY SUCCESS\n");

#endif

  if (ioctl (hdmiFD, HDMI_START, HDMI_PROTOCOL_HDMI) < 0)

    {

      printf ("HDMI_START failed: %d\n", errno);
exit(0);
    }

  else

    printf ("HDMI_START SUCCESS\n");


  if (ioctl (hdmiFD, HDMI_SUBSCRIBE_EVENTS, HDMI_EVENT_HOTPLUGGED) < 0)

    {

      printf ("HDMI_SUBSCRIBE_EVENTS failed: %d\n", errno);
exit(0);
    }

  else

    printf ("HDMI_SUBSCRIBE_EVENTS SUCCESS\n");


  /*if( ioctl(hdmiFD, HDMI_SET_AVMUTE) < 0 ) 
     {
     printf( "HDMI_SET_AVMUTE failed: %d\n", errno);
     }
     else
     printf( "HDMI_SET_AVMUTE SUCCESS\n");

     if( ioctl(hdmiFD, HDMI_CLEAR_AVMUTE) < 0 ) 
     {
     printf( "HDMI_CLEAR_AVMUTE failed: %d\n", errno);
     }
     else
     printf( "HDMI_CLEAR_AVMUTE SUCCESS\n");
   */
#endif /* 
        */
}







static void
setupBlitter (void)
{
  int32_t rv = 0;
  char blitterrname[64];
  uint32_t versions;
  uint32_t features;
  uint32_t srcFormats;
  uint32_t clutPixelFormats;
  uint32_t blendingRules;
  uint32_t operations;
  uint32_t operationListMax;
  BlitterCaps blittercapsEx;
  BlitterPixelFormatConstraints blitter_format_constraints;
  //BlitterOperationCaps                                                    blitterOperationCaps;
  BlitterFillAreaCaps blitterFillAreaOperationCaps = { 0 };
  BlitterCopyAreaCaps blitterCopyAreaOperationCaps = { 0 };
  BlitterResizeAreaCaps blitterResizeAreaOperationCaps = { 0 };
  BlitterShadeAreaCaps blitterShadeAreaOperationCaps = { 0 };
  BlitterBlendAreaCaps blitterBlendAreaOperationCaps = { 0 };
  //BlitterResizeAndBlendAreaCaps                  blitterResizeAndBlendAreaOperationCaps = {0};
  //Blitter2SrcResizeAndBlendAreaCaps              blitter2SrcResizeAndBlendAreaOperationCaps = {0};       
  //Blitter2SrcShadeResizeAndBlendAreaCaps blitter2SrcShadeResizeAndBlendAreaOperationCaps = {0};

  hw_blitter_fd = -1;           /* File not Open */
  memset (blitterrname, 0, NDS_MAX_DEV_LENGTH);

  sprintf (blitterrname, "%s", "/dev/nds/blitter");
  printf ("\nEnter setupBlitter\n");

  hw_blitter_fd = open (blitterrname, O_RDWR);

  if (hw_blitter_fd == -1)
    {
      printf ("OSDInit(): Can't open %s Device, errno = %u\n", blitterrname,
              errno);
      exit (0);
    }
  else
    {
      printf ("OSDInit(): Blitter opened as fd:%d\n", hw_blitter_fd);
/*
                rv = ioctl(hw_blitter_fd,BLITTER_GET_CAPS_EX_VERSIONS,&versions);
                if(rv != 0)
                {
                        printf("*E*, BLITTER_GET_CAPS_EX_VERSIONS ioctl failed, rv = %d, errno = %d\n", rv, errno);
                        exit(0);
                }       
                else
                {
                        printf("OSDInit(): Blitter Caps Ex : version(s) = 0x%X, \n",
                                                                                 versions);
                }
*/
    }

  /*if(versions == BLITTER_CAP_V1)
     {
     blittercapsEx.version                   = BLITTER_CAP_V1;
     blittercapsEx.capabilitySize            = sizeof(BlitterCapsV1);
     blittercapsEx.capability                        = (BlitterCapsV1*) malloc(sizeof(BlitterCapsV1)); */

  rv = ioctl (hw_blitter_fd, BLITTER_GET_CAPS, &(blittercapsEx));

  if (rv < 0)
    {
      printf ("*E*, BLITTER_GET_CAPS_EX ioctl failed, rv = %d, errno = %d\n",
              rv, errno);
      exit (0);
    }
  else
    {
      printf ("OSDInit(): Blitter Device Capabilities for fd %d\n",
              hw_blitter_fd);

      /*features                        = ((BlitterCapsV1*)(blittercapsEx.capability))->features;
         srcFormats              = ((BlitterCapsV1*)(blittercapsEx.capability))->srcFormats;
         clutPixelFormats        = ((BlitterCapsV1*)(blittercapsEx.capability))->clutPixelFormats;
         blendingRules           = ((BlitterCapsV1*)(blittercapsEx.capability))->blendingRules;
         operations              = ((BlitterCapsV1*)(blittercapsEx.capability))->operations;
         operationListMax        = ((BlitterCapsV1*)(blittercapsEx.capability))->operationListMax; */

      //features                = blittercapsEx.features;
      srcFormats = blittercapsEx.srcFormats;
      //clutPixelFormats        = blittercapsEx.clutPixelFormats;
      //blendingRules           = blittercapsEx.blendingRules;
      operations = blittercapsEx.operations;

      //printf("OSDInit(): Features = 0x%X\n",features);
      printf ("OSDInit(): SrcFormats = 0x%X \n", srcFormats);
      printf ("OSDInit(): operations =0x%X \n", operations);
    }

  /*}
     else
     {
     printf("OSDInit(): Please use Drivers >= M4.9.4\n");
     exit(0);
     } */


  if (srcFormats & PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT)
    {
      blitter_format_constraints.srcFormat =
        PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
               &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT: BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_ARGB_8888_PACKED)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_ARGB_8888_PACKED;

      rv = ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
                  &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_8888_PACKED BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_ARGB_4444_PACKED)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_ARGB_4444_PACKED;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
               &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_4444_PACKED BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_CLUT_8)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_CLUT_8;

      rv = ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
                  &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_CLUT8 BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else
    {
      printf
        ("OSDInit(): Blitter does not support ARGB_8888 / ARGB_8888_PREMULT / 4444/ CLUT8\n");
      exit (0);
    }



  if (operations & BLITTER_OPERATION_FILL_AREA)
    {

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_FILL_AREA_CAPS,
               &(blitterFillAreaOperationCaps));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS==> BLITTER_OPERATION_FILL_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterFillAreaOperationCaps - dest Format(s) 0x%X\n",
             blitterFillAreaOperationCaps.dstFormats);
        }
    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_FILL_AREA not supported\n");
      //exit(0);
    }



  if (operations & BLITTER_OPERATION_COPY_AREA)
    {
      blitterCopyAreaOperationCaps.srcFormat = PIXEL_FORMAT_ARGB_8888_PACKED;


      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_COPY_AREA_CAPS,
               &(blitterCopyAreaOperationCaps));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_COPY_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          /*printf("OSDInit(): BlitterCopyAreaOperationCaps - Src Fmt(0x%X) srcClutFmt (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X)\n",
             blitterCopyAreaOperationCaps.srcFormat,
             blitterCopyAreaOperationCaps.srcClutFormat,
             blitterCopyAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterCopyAreaOperationCaps.dstFormatsGlobalBlending); */
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_COPY_AREA not supported\n");
    }


#if 0
  if (operations & BLITTER_OPERATION_RESIZE_AREA)
    {

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_RESIZE_AREA_CAPS,
               &(blitterResizeAreaOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_RESIZE_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          /*printf("OSDInit(): BlitterResizeAreaOperationCaps - Src Fmt(0x%X) SrcClutFmt (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X), H-dwnMax %d, H-UpMax %d, V-dwnMax %d, V-upMax %d\n",
             blitterResizeAreaOperationCaps.srcFormat,
             blitterResizeAreaOperationCaps.srcClutFormat,
             blitterResizeAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterResizeAreaOperationCaps.dstFormatsGlobalBlending,
             blitterResizeAreaOperationCaps.scalingCaps.horizontalDownScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.horizontalUpScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.verticalDownScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.verticalUpScaleFactorMax); */
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_RESIZE_AREA not supported\n");
    }

#endif

  if (operations & BLITTER_OPERATION_SHADE_AREA)
    {

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_SHADE_AREA_CAPS,
               &(blitterShadeAreaOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_SHADE_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);;
        }
      else
        {
          /*printf("OSDInit(): BlitterShadeAreaOperationCaps - dest Format(s) 0x%X, BlendingRules 0x%X\n",
             blitterShadeAreaOperationCaps.dstFormats,
             blitterShadeAreaOperationCaps.blendingRule); */
        }
    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_SHADE_AREA not supported\n");
    }

#if 0
  if (operations & BLITTER_OPERATION_BLEND_AREA)
    {

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_BLEND_AREA_CAPS,
               &(blitterBlendAreaOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_BLEND_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          /*printf("OSDInit(): BlitterBlendAreaOperationCaps - Src Fmt(0x%X) srcClutFmt (0x%X) BlendingRule (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X)\n",
             blitterBlendAreaOperationCaps.srcFormat,
             blitterBlendAreaOperationCaps.srcClutFormat,
             blitterBlendAreaOperationCaps.blendingRule,
             blitterBlendAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterBlendAreaOperationCaps.dstFormatsGlobalBlending); */
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_BLEND_AREA not supported\n");
    }
#endif
  printf ("\nExit setupBlitter\n\n");

}

static void
TestDraw ()
{
  int32_t retval;
  //OperationArea   opArea = {0};
  //BlitterOperation        blitterOp = {0};
  //uint32_t                        num_of_operations;
  //BlitterBufferEx         *ptrbuff = NULL;
  //BlitterFillAreaOperation  *fillAreaOp;
  //BlitterOperationList            blitterOpList;
  //BlitterBufferEx                 dst;
  BlitterFillArea fillArea;


  printf ("TestDraw Called\n");


  fillArea.dst.type = BLITTER_BUFFER_TYPE_SURFACE;
  fillArea.dst.buffer.surface.surfaceId = HD_SURFACE_GR;

  //fillAreaOp = (BlitterFillAreaOperation*) malloc(sizeof(BlitterCopyAreaOperation));
  fillArea.area.dimension.width = 700;
  fillArea.area.dimension.height = 500;
  fillArea.area.position.x = 0;
  fillArea.area.position.y = 0;

  fillArea.solidColourPixel = 0XFFFF0000;

  //blitterOp.operation                     = BLITTER_OPERATION_FILL_AREA;
  //blitterOp.parameters            = fillAreaOp;

  //blitterOpList.operationList             = &blitterOp;
  //blitterOpList.operationListLength       = num_of_operations;

  if ((retval = ioctl (hw_blitter_fd, BLITTER_FILL_AREA, &fillArea)) != 0)
    {
      printf
        ("Error in HD BLITTER_EXECUTE_OPERATION_LIST retval = %d, ErrNo = %d\n",
         retval, errno);
      exit (0);
    }
#if 0
  fillArea.dst.type = BLITTER_BUFFER_TYPE_SURFACE;
  fillArea.dst.buffer.surface.surfaceId = SD_SURFACE_GR;
  fillArea.area.dimension.width = 720;
  fillArea.area.dimension.height = 576;
  fillArea.area.position.x = 0;
  fillArea.area.position.y = 0;

  fillArea.solidColourPixel = 0XFFFF0000;

  if ((retval = ioctl (hw_blitter_fd, BLITTER_FILL_AREA, &fillArea)) != 0)
    {
      printf
        ("Error in SD BLITTER_EXECUTE_OPERATION_LIST retval = %d, ErrNo = %d\n",
         retval, errno);
      exit (0);
    }
#endif
}

#if 0
void
TestIframe ()
{
  int32_t retval;
  OperationArea opArea = { 0 };
  BlitterOperation blitterOp = { 0 };
  uint32_t num_of_operations;
  BlitterBufferEx *ptrbuff = NULL;
  BlitterFillAreaOperation *fillAreaOp;
  BlitterCopyAreaOperation *copyAreaOp;
  BlitterOperationList blitterOpList;
  BlitterBufferEx dst;
  BlitterBufferEx src;


  printf ("TestIframe Called\n");
  opArea.dstArea.dimension.width = 1280;
  opArea.dstArea.dimension.height = 720;

  opArea.src1Area.position.x = 0;
  opArea.src1Area.position.y = 0;
  opArea.src1Area.dimension.width = 1280;
  opArea.src1Area.dimension.height = 720;
  opArea.dstArea.position.x = 0;
  opArea.dstArea.position.y = 0;
  num_of_operations = 1;

  dst.type = BLITTER_BUFFER_TYPE_EX_SURFACE;
  dst.buffer.surface.surfaceId = HD_GR_SURFACE;

  src.type = BLITTER_BUFFER_TYPE_EX_SURFACE;
  src.buffer.surface.surfaceId = STILL_SURFACE;

  copyAreaOp =
    (BlitterCopyAreaOperation *) malloc (sizeof (BlitterCopyAreaOperation));

  if (copyAreaOp != NULL)
    {
      copyAreaOp->src = src;
      copyAreaOp->dst = dst;
      copyAreaOp->srcArea = opArea.src1Area;
      copyAreaOp->dstPosition = opArea.dstArea.position;

      copyAreaOp->globalBlendingFactor = 255;

      blitterOp.operation = BLITTER_OPERATION_COPY_AREA;
      blitterOp.parameters = copyAreaOp;
    }

  blitterOpList.operationList = &blitterOp;
  blitterOpList.operationListLength = num_of_operations;

  if ((retval =
       ioctl (hw_blitter_fd, BLITTER_EXECUTE_OPERATION_LIST,
              &blitterOpList)) != 0)
    {
      printf
        ("Error in BLITTER_EXECUTE_OPERATION_LIST retval = %d, ErrNo = %d\n",
         retval, errno);
      retval = -1;
    }
}

static void
setupIframedecoder (void)
{
  int result;
  int i, rval, j;
  uint32_t version;
  IframeDecoderCaps caps;
  IframeDecoderConfiguration Config;
  printf ("Enter SetupIframeDecoder \n");
  IframeFD = open (IFRAME_PATH, O_RDWR);
  if (IframeFD)
    {
      printf ("SURFACE OPEN SUCCESSFULL\n");
    }
  else
    {
      printf ("SURFACE OPEN FAILED!!!\n");
    }

  if (ioctl (IframeFD, IFRAME_DECODER_GET_CAPS, &caps) == -1)
    {
      printf ("IFRAME_DECODER_GET_CAPS FAILED!!!\n");
    }
  else
    printf ("IFRAME_DECODER_GET_CAPS SUCCESS\n");

  Config.streamFormat = STREAM_FORMAT_MPEG_ES;
  if (ioctl (IframeFD, IFRAME_DECODER_CONFIGURE, &Config) == -1)
    {
      printf
        ("CdiConfigureIframeDecoder:: IFRAME_DECODER_CONFIGURE ioctl failed, errno = %d\n",
         errno);
    }
  else
    printf
      ("CdiConfigureIframeDecoder:: IFRAME_DECODER_CONFIGURE ioctl SUCCESS\n");

}




static void
setupBlitter (void)
{
  int32_t rv = 0;
  char blitterrname[64];
  uint32_t versions;
  uint32_t features;
  uint32_t srcFormats;
  uint32_t clutPixelFormats;
  uint32_t blendingRules;
  uint32_t operations;
  uint32_t operationListMax;
  BlitterCaps blittercapsEx;
  BlitterPixelFormatConstraints blitter_format_constraints;
  //BlitterOperationCaps                                                    blitterOperationCaps;
  BlitterFillAreaCaps blitterFillAreaOperationCaps = { 0 };
  BlitterCopyAreaCaps blitterCopyAreaOperationCaps = { 0 };
  BlitterResizeAreaCaps blitterResizeAreaOperationCaps = { 0 };
  BlitterShadeAreaCaps blitterShadeAreaOperationCaps = { 0 };
  BlitterBlendAreaCaps blitterBlendAreaOperationCaps = { 0 };
  //BlitterResizeAndBlendAreaCaps                  blitterResizeAndBlendAreaOperationCaps = {0};
  //Blitter2SrcResizeAndBlendAreaCaps              blitter2SrcResizeAndBlendAreaOperationCaps = {0};       
  //Blitter2SrcShadeResizeAndBlendAreaCaps blitter2SrcShadeResizeAndBlendAreaOperationCaps = {0};

  hw_blitter_fd = -1;           /* File not Open */
  memset (blitterrname, 0, NDS_MAX_DEV_LENGTH);

  sprintf (blitterrname, "%s", "/dev/nds/blitter");
  printf ("\nEnter setupBlitter\n");

  hw_blitter_fd = open (blitterrname, O_RDWR);

  if (hw_blitter_fd == -1)
    {
      printf ("OSDInit(): Can't open %s Device, errno = %u\n", blitterrname,
              errno);
      exit (0);
    }
  else
    {
      printf ("OSDInit(): Blitter opened as fd:%d\n", hw_blitter_fd);
/*
                rv = ioctl(hw_blitter_fd,BLITTER_GET_CAPS_EX_VERSIONS,&versions);
                if(rv != 0)
                {
                        printf("*E*, BLITTER_GET_CAPS_EX_VERSIONS ioctl failed, rv = %d, errno = %d\n", rv, errno);
                        exit(0);
                }       
                else
                {
                        printf("OSDInit(): Blitter Caps Ex : version(s) = 0x%X, \n",
                                                                                 versions);
                }
*/
    }

  /*if(versions == BLITTER_CAP_V1)
     {
     blittercapsEx.version                   = BLITTER_CAP_V1;
     blittercapsEx.capabilitySize            = sizeof(BlitterCapsV1);
     blittercapsEx.capability                        = (BlitterCapsV1*) malloc(sizeof(BlitterCapsV1)); */

  rv = ioctl (hw_blitter_fd, BLITTER_GET_CAPS_EX, &(blittercapsEx));

  if (rv < 0)
    {
      printf ("*E*, BLITTER_GET_CAPS_EX ioctl failed, rv = %d, errno = %d\n",
              rv, errno);
      exit (0);
    }
  else
    {
      printf ("OSDInit(): Blitter Device Capabilities for fd %d\n",
              hw_blitter_fd);

      /*features                        = ((BlitterCapsV1*)(blittercapsEx.capability))->features;
         srcFormats              = ((BlitterCapsV1*)(blittercapsEx.capability))->srcFormats;
         clutPixelFormats        = ((BlitterCapsV1*)(blittercapsEx.capability))->clutPixelFormats;
         blendingRules           = ((BlitterCapsV1*)(blittercapsEx.capability))->blendingRules;
         operations              = ((BlitterCapsV1*)(blittercapsEx.capability))->operations;
         operationListMax        = ((BlitterCapsV1*)(blittercapsEx.capability))->operationListMax; */

      //features                = blittercapsEx.features;
      srcFormats = blittercapsEx.srcFormats;
      //clutPixelFormats        = blittercapsEx.clutPixelFormats;
      //blendingRules           = blittercapsEx.blendingRules;
      operations = blittercapsEx.operations;

      printf ("OSDInit(): Features = 0x%X\n", features);
      printf ("OSDInit(): SrcFormats = 0x%X clutPixelFormats = %d\n",
              srcFormats, clutPixelFormats);
      printf
        ("OSDInit(): BlendingRules = 0x%X operations =0x%X operationListMax = 0x%X\n",
         blendingRules, operations, operationListMax);
    }
  if (blittercapsEx.capability != NULL)
    free (blittercapsEx.capability);
  /*}
     else
     {
     printf("OSDInit(): Please use Drivers >= M4.9.4\n");
     exit(0);
     } */


  if (srcFormats & PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT)
    {
      blitter_format_constraints.srcFormat =
        PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
               &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_8888_PACKED_PREMULT: BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_ARGB_8888_PACKED)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_ARGB_8888_PACKED;

      rv = ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
                  &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_8888_PACKED BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_ARGB_4444_PACKED)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_ARGB_4444_PACKED;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
               &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_ARGB_4444_PACKED BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else if (srcFormats & PIXEL_FORMAT_CLUT_8)
    {
      blitter_format_constraints.srcFormat = PIXEL_FORMAT_CLUT_8;

      rv = ioctl (hw_blitter_fd, BLITTER_GET_FORMAT_CONSTRAINTS,
                  &(blitter_format_constraints));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_FORMAT_CONSTRAINTS ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): PIXEL_FORMAT_CLUT8 BlitterPixelFormatConstraints, srcFmt 0x%X, xAlign %d, yAlign %d, baseAlign %d, BPLAlign %d\n",
             blitter_format_constraints.srcFormat,
             blitter_format_constraints.xAlignment,
             blitter_format_constraints.yAlignment,
             blitter_format_constraints.baseAlignment,
             blitter_format_constraints.numBytesPerLineAlignment);
        }
    }
  else
    {
      printf
        ("OSDInit(): Blitter does not support ARGB_8888 / ARGB_8888_PREMULT / 4444/ CLUT8\n");
      exit (0);
    }



  if (operations & BLITTER_OPERATION_FILL_AREA)
    {
      blitterOperationCaps.operation = BLITTER_OPERATION_FILL_AREA;
      blitterOperationCaps.capability = &blitterFillAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS==> BLITTER_OPERATION_FILL_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterFillAreaOperationCaps - dest Format(s) 0x%X\n",
             blitterFillAreaOperationCaps.dstFormats);
        }
    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_FILL_AREA not supported\n");
      //exit(0);
    }



  if (operations & BLITTER_OPERATION_COPY_AREA)
    {
      blitterCopyAreaOperationCaps.srcFormat = PIXEL_FORMAT_ARGB_8888_PACKED;

      blitterOperationCaps.operation = BLITTER_OPERATION_COPY_AREA;
      blitterOperationCaps.capability = &blitterCopyAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));
      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_COPY_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterCopyAreaOperationCaps - Src Fmt(0x%X) srcClutFmt (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X)\n",
             blitterCopyAreaOperationCaps.srcFormat,
             blitterCopyAreaOperationCaps.srcClutFormat,
             blitterCopyAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterCopyAreaOperationCaps.dstFormatsGlobalBlending);
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_COPY_AREA not supported\n");
    }


  if (operations & BLITTER_OPERATION_RESIZE_AREA)
    {
      blitterOperationCaps.operation = BLITTER_OPERATION_RESIZE_AREA;
      blitterOperationCaps.capability = &blitterResizeAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_RESIZE_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterResizeAreaOperationCaps - Src Fmt(0x%X) SrcClutFmt (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X), H-dwnMax %d, H-UpMax %d, V-dwnMax %d, V-upMax %d\n",
             blitterResizeAreaOperationCaps.srcFormat,
             blitterResizeAreaOperationCaps.srcClutFormat,
             blitterResizeAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterResizeAreaOperationCaps.dstFormatsGlobalBlending,
             blitterResizeAreaOperationCaps.scalingCaps.
             horizontalDownScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.
             horizontalUpScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.
             verticalDownScaleFactorMax,
             blitterResizeAreaOperationCaps.scalingCaps.
             verticalUpScaleFactorMax);
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_RESIZE_AREA not supported\n");
    }



  if (operations & BLITTER_OPERATION_SHADE_AREA)
    {
      blitterOperationCaps.operation = BLITTER_OPERATION_SHADE_AREA;
      blitterOperationCaps.capability = &blitterShadeAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_SHADE_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);;
        }
      else
        {
          printf
            ("OSDInit(): BlitterShadeAreaOperationCaps - dest Format(s) 0x%X, BlendingRules 0x%X\n",
             blitterShadeAreaOperationCaps.dstFormats,
             blitterShadeAreaOperationCaps.blendingRule);
        }
    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_SHADE_AREA not supported\n");
    }


  if (operations & BLITTER_OPERATION_BLEND_AREA)
    {
      blitterOperationCaps.operation = BLITTER_OPERATION_BLEND_AREA;
      blitterOperationCaps.capability = &blitterBlendAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_BLEND_AREA ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterBlendAreaOperationCaps - Src Fmt(0x%X) srcClutFmt (0x%X) BlendingRule (0x%X) dstFmtsNoGlobalBlending (0x%X) dstFrmtsGlobalBlending (0x%X)\n",
             blitterBlendAreaOperationCaps.srcFormat,
             blitterBlendAreaOperationCaps.srcClutFormat,
             blitterBlendAreaOperationCaps.blendingRule,
             blitterBlendAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitterBlendAreaOperationCaps.dstFormatsGlobalBlending);
        }

    }
  else
    {
      printf ("OSDInit(): BLITTER_OPERATION_BLEND_AREA not supported\n");
    }

  if (operations & BLITTER_OPERATION_RESIZE_AND_BLEND_AREA)
    {
      blitterOperationCaps.operation =
        BLITTER_OPERATION_RESIZE_AND_BLEND_AREA;
      blitterOperationCaps.capability =
        &blitterResizeAndBlendAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_RESIZE_AND_BLEND_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          printf
            ("OSDInit(): BlitterResizeAndBlendAreaOperationCaps - Src Fmt(0x%X) Src ClutFmt (0x%X) BlendRule (0x%X) dstNoGlobalBlend (0x%X) dstGlobalBlend (0x%X), H-dwnMax %d, H-UpMax %d, V-dwnMax %d, V-upMax %d\n",
             blitterResizeAndBlendAreaOperationCaps.srcFormat,
             blitterResizeAndBlendAreaOperationCaps.srcClutFormat,
             blitterResizeAndBlendAreaOperationCaps.blendingRule,
             blitterResizeAndBlendAreaOperationCaps.
             dstFormatsNoGlobalBlending,
             blitterResizeAndBlendAreaOperationCaps.dstFormatsGlobalBlending,
             blitterResizeAndBlendAreaOperationCaps.scalingCaps.
             horizontalDownScaleFactorMax,
             blitterResizeAndBlendAreaOperationCaps.scalingCaps.
             horizontalUpScaleFactorMax,
             blitterResizeAndBlendAreaOperationCaps.scalingCaps.
             verticalDownScaleFactorMax,
             blitterResizeAndBlendAreaOperationCaps.scalingCaps.
             verticalUpScaleFactorMax);
        }
    }
  else
    {
      printf
        ("OSDInit(): BLITTER_OPERATION_RESIZE_AND_BLEND_AREA not supported\n");
    }


  if (operations & BLITTER_OPERATION_2SRC_RESIZE_AND_BLEND_AREA)
    {
      blitterOperationCaps.operation =
        BLITTER_OPERATION_2SRC_RESIZE_AND_BLEND_AREA;
      blitterOperationCaps.capability =
        &blitter2SrcResizeAndBlendAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_2SRC_RESIZE_AND_BLEND_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          /*printf("OSDInit(): Blitter2SrcResizeAndBlendAreaOperationCaps - Src1 Fmt(0x%X) Src1 ClutFmt (0x%X) Src2 Fmt(0x%X) Src2 ClutFmt (0x%X) BlendRule (0x%X) dstNoGlobalBlend (0x%X) dstGlobalBlend (0x%X), H-dwnMax %d, H-UpMax %d, V-dwnMax %d, V-upMax %d\n",
             blitter2SrcResizeAndBlendAreaOperationCaps.src1Format,
             blitter2SrcResizeAndBlendAreaOperationCaps.src1ClutFormat,
             blitter2SrcResizeAndBlendAreaOperationCaps.src2Format,
             blitter2SrcResizeAndBlendAreaOperationCaps.src2ClutFormat,
             blitter2SrcResizeAndBlendAreaOperationCaps.blendingRule,
             blitter2SrcResizeAndBlendAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitter2SrcResizeAndBlendAreaOperationCaps.dstFormatsGlobalBlending,
             blitter2SrcResizeAndBlendAreaOperationCaps.scalingCaps.horizontalDownScaleFactorMax,
             blitter2SrcResizeAndBlendAreaOperationCaps.scalingCaps.horizontalUpScaleFactorMax,
             blitter2SrcResizeAndBlendAreaOperationCaps.scalingCaps.verticalDownScaleFactorMax,
             blitter2SrcResizeAndBlendAreaOperationCaps.scalingCaps.verticalUpScaleFactorMax); */
        }
    }
  else
    {
      printf
        ("OSDInit(): BLITTER_OPERATION_2SRC_RESIZE_AND_BLEND_AREA not supported\n");
    }


  if (operations & BLITTER_OPERATION_2SRC_SHADE_RESIZE_AND_BLEND_AREA)
    {
      blitterOperationCaps.operation =
        BLITTER_OPERATION_2SRC_SHADE_RESIZE_AND_BLEND_AREA;
      blitterOperationCaps.capability =
        &blitter2SrcShadeResizeAndBlendAreaOperationCaps;

      rv =
        ioctl (hw_blitter_fd, BLITTER_GET_OPERATION_CAPS,
               &(blitterOperationCaps));

      if (rv < 0)
        {
          printf
            ("*E*, BLITTER_GET_OPERATION_CAPS ==> BLITTER_OPERATION_2SRC_SHADE_RESIZE_AND_BLEND_AREA: ioctl failed, rv = %d, errno = %d\n",
             rv, errno);
          exit (0);
        }
      else
        {
          /*printf("OSDInit(): Blitter2SrcShadeResizeAndBlendAreaOperationCaps - Src1 Fmt(0x%X) Src1 ClutFmt (0x%X) Src2 Fmt(0x%X) Src2 ClutFmt (0x%X) BlendRule (0x%X) dstNoGlobalBlend (0x%X) dstGlobalBlend (0x%X), H-dwnMax %d, H-UpMax %d, V-dwnMax %d, V-upMax %d\n",
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.src1Format,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.src1ClutFormat,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.src2Format,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.src2ClutFormat,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.blendingRule,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.dstFormatsNoGlobalBlending,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.dstFormatsGlobalBlending,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.scalingCaps.horizontalDownScaleFactorMax,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.scalingCaps.horizontalUpScaleFactorMax,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.scalingCaps.verticalDownScaleFactorMax,
             blitter2SrcShadeResizeAndBlendAreaOperationCaps.scalingCaps.verticalUpScaleFactorMax); */
        }
    }
  else
    {
      printf
        ("OSDInit(): BLITTER_OPERATION_2SRC_SHADE_RESIZE_AND_BLEND_AREA not supported\n");
    }



  printf ("\nExit setupBlitter\n\n");

}

#endif
