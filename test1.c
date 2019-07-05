/*
Copyright (c) NDS Limited 2008

P R O P R I E T A R Y & C O N F I D E N T I A L

The copyright of this code and related documentation together with
any other associated intellectual property rights are vested in
NDS Limited and may not be used except in accordance with the terms
of the licence that you have entered into with NDS Limited.
Use of this material without an express licence from NDS Limited
shall be an infringement of copyright and any other intellectual
property rights that may be incorporated with this material.

FILE:          testmain.c

DESCRIPTION:   Tests SYSINIT component

TEST:          Test 1 - Starting order
               http://quicksilver1/twiki/bin/view/MMTWiki/SYSINITTest

*/

#include <unistd.h>

#include "sysinit_server.h"
#include "test.h"
#include "sysinit_priv.h"

#define TEST_SUITE 1

#define SYSINIT_TEST_MASTER_COMPONENT_A 1U
#define SYSINIT_TEST_MASTER_COMPONENT_B 3U

/*
    Process Component   Ring level
    P1      C1          1
    P1      C2          2
    P2      C3          2
    P2      C4          3
*/
static int component_ringlevels[SYSINIT_TEST_NUM_COMPONENTS]=
{
    1, /* Component 0 at RL 1*/
    2, /* Component 1 at RL 2 */
    2,3 /* etc.*/
};

/**
 * @brief test1clause1
  *
  *
  *  @if REQPRO
  *  \li \b TEST 1.1
  *  \li \b REQUIREMENTS 193086 193087 193089 193091 193092 193093 193094 193095 132827 132829 132832
  *  @endif
  *
**/
void test1clause1(void)
{
/*
    Test fails if the log files indicate that component C2 was requested to
    enter the initialised run level before component C1 entered the active run
    level, for any components C1 and C2 where the ring level of C2 is
    ordered after the ring level of C1
*/
    uint32_t C1,C2;
    int O1,O2;

    for(C1 = 0U; C1 < SYSINIT_TEST_NUM_COMPONENTS; C1++)
    {
        for(C2 = 0U; C2 < SYSINIT_TEST_NUM_COMPONENTS; C2++)
        {
            if(component_ringlevels[C2] > component_ringlevels[C1])
            {
                O1 = findSpecificEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, (uint32_t)SYSINIT_API_RunLevelNotificationActive, 0);
                CU_ASSERT_NOT_EQUAL(O1, -1);
                O2 = findSpecificEvent(C2, SYSINIT_TEST_Log_RunLevelChangeRequestReceived, (uint32_t)SYSINIT_RunLevelRequestInitialise, 0);
                CU_ASSERT_NOT_EQUAL(O2, -1);

                CU_ASSERT_EQUAL((O2 >= O1), true); /* Event 2 must happen after event 1 (granularity of timer means it may happen at same time!)*/
            }
        }
    }
}

/**
 * @brief test1clause2
**/
void test1clause2(void)
{
/*
    Test fails if the log files indicate that component C2 was requested to
    enter the prepared run level before component C1 entered the initialised
    run level, for any components C1 and C2 on the same ring level
*/
    uint32_t C1,C2;
    int O1,O2;

    for(C1 = 0U; C1 < SYSINIT_TEST_NUM_COMPONENTS; C1++)
    {
        for(C2 = 0U; C2 < SYSINIT_TEST_NUM_COMPONENTS; C2++)
        {
            if(component_ringlevels[C2] == component_ringlevels[C1])
            {
                O1 = findSpecificEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, (uint32_t)SYSINIT_API_RunLevelNotificationInitialised, 0);
                CU_ASSERT_NOT_EQUAL(O1, -1);
                O2 = findSpecificEvent(C2, SYSINIT_TEST_Log_RunLevelChangeRequestReceived, (uint32_t)SYSINIT_RunLevelRequestPrepare, 0);
                CU_ASSERT_NOT_EQUAL(O2, -1);

                CU_ASSERT_EQUAL((O2 >= O1), true); /* Event 2 must happen after event 1 (granularity of timer means it may happen at same time!)*/
            }
        }
    }
}

/**
 * @brief test1clause3
**/
void test1clause3(void)
{
/*
    Test fails if the log files indicate that component C2 was requested to
    enter the active run level before component C1 entered the prepared run
    level, for any components C1 and C2 on the same ring level
*/
    uint32_t C1,C2;
    int O1,O2;

    for(C1 = 0U; C1 < SYSINIT_TEST_NUM_COMPONENTS; C1++)
    {
        for(C2 = 0U; C2 < SYSINIT_TEST_NUM_COMPONENTS; C2++)
        {
            if(component_ringlevels[C2] == component_ringlevels[C1])
            {
                O1 = findSpecificEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, (uint32_t)SYSINIT_API_RunLevelNotificationInitialised, 0);
                CU_ASSERT_NOT_EQUAL(O1, -1);
                O2 = findSpecificEvent(C2, SYSINIT_TEST_Log_RunLevelChangeRequestReceived, (uint32_t)SYSINIT_RunLevelRequestPrepare, 0);
                CU_ASSERT_NOT_EQUAL(O2, -1);

                CU_ASSERT_EQUAL((O2 >= O1), true); /* Event 2 must happen after event 1 (granularity of timer means it may happen at same time!)*/
            }
        }
    }


}

/**
 * @brief test1clause4
  *
  *
  *  @if REQPRO
  *  \li \b TEST 1.4
  *  \li \b REQUIREMENTS 193086 193087 193089 193091 193092 193093 193094 193095 193096 580229 580230 132825 132827 132832
  *  @endif
  *
**/
void test1clause4(void)
{
/*
    Test fails if the log file of any component indicates run level changes
    in an order other than initialised->prepared->active->PVR
*/
    uint32_t C1;
    int R1;

    for(C1 = 0U; C1 < SYSINIT_TEST_NUM_COMPONENTS; C1++)
    {
        R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, 0);
        CU_ASSERT_NOT_EQUAL(R1, -1);
        CU_ASSERT_EQUAL(event_log[R1].event_value, (uint32_t)SYSINIT_API_RunLevelNotificationInitialised);

        R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, R1 + 1);
        CU_ASSERT_NOT_EQUAL(R1, -1);
        CU_ASSERT_EQUAL(event_log[R1].event_value, (uint32_t)SYSINIT_API_RunLevelNotificationPrepared);

        R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, R1 + 1);
        CU_ASSERT_NOT_EQUAL(R1, -1);
        CU_ASSERT_EQUAL(event_log[R1].event_value, (uint32_t)SYSINIT_API_RunLevelNotificationActive);

       /*
        * We have changed the behaviour of tests during Active -> PVR. So we don't check this for now.
        * R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, R1 + 1);
        * CU_ASSERT_NOT_EQUAL(R1, -1);
        * CU_ASSERT_EQUAL(event_log[R1].event_value, (uint32_t)SYSINIT_API_RunLevelNotificationPVR);
        */
    }
}

/**
 * @brief test1clause5
  *
  *
  *  @if REQPRO
  *  \li \b TEST 1.5
  *  \li \b REQUIREMENTS 193086 193087 193089 193091 193092 193093 193094 193095 193096 193099 580232 132827 132832
  *  @endif
  *
**/
void test1clause5(void)
{
/*
    Test fails if the log file of any component indicates that the
    last run level entered was not the shutdown state
*/
    /* This clause should only fail if the test timed out*/
    uint32_t C1;
    int R1, LR1 = -1;

    for(C1 = 0U; C1 < SYSINIT_TEST_NUM_COMPONENTS; C1++)
    {
        /* Find the last event on this component*/
        R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, 0);
        while(R1 != -1)
        {
            LR1 = R1;
            R1 = findEvent(C1, SYSINIT_TEST_Log_ChangedRunLevel, R1+1);
        }
        /*
                * Last event is now LR1.
                */
        CU_ASSERT_EQUAL(event_log[LR1].event_value, (uint32_t)SYSINIT_API_RunLevelNotificationShutdown);
    }
}

/**
 * @brief test1clientTests
  *
  *
  *  @if REQPRO
  *  \li \b TEST 1.6
  *  \li \b REQUIREMENTS 193097 233994 580233 580234 580235 580253 580255 580256 580257 580258 580259 580260
  *  @endif
  *
**/
void test1clientTests(void)
{


/*
    Test fails if the log file of either master components indicates that the
    client side tests failed
*/
    int rv;

    rv = findSpecificEvent(SYSINIT_TEST_MASTER_COMPONENT_A, SYSINIT_TEST_Log_ClientTestResults, SYSINIT_STATUS_OK, 0);
    CU_ASSERT_NOT_EQUAL(rv, -1);

    rv = findSpecificEvent(SYSINIT_TEST_MASTER_COMPONENT_B, SYSINIT_TEST_Log_ClientTestResults, SYSINIT_STATUS_OK, 0);
    CU_ASSERT_NOT_EQUAL(rv, -1);
	

}


/**
 *.@brief-Changing max file open limit (RLIMIT_NOFILE) for each process
 **/
 #define FD_LIMIT_INDEX 10
 #define MAX_NO_TEST_INPUT 16
void test1ChangeMaxFileOpenLimitTest(void)
{
    struct rlimit get_limit = {0};
    uint32_t cnt = 0;
    int err = -1;

    err = getrlimit(RLIMIT_NOFILE, &get_limit);
    if (err)
    {
        printf(" getrlimit reurned with error = %d \n",err);
        CU_ASSERT_EQUAL(err, 0);
    }
    {
                                                            /*Soft Limit                                        Hard Limit                                              Expected Soft Limit                             Expected Hard Limit */
        uint32_t inputData[MAX_NO_TEST_INPUT][4] =
                                                      {0,                                                           0,                                                          get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        0,                                                          get_limit.rlim_max,                                 get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        0,                                                          (get_limit.rlim_max-FD_LIMIT_INDEX),  get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        0,                                                          (get_limit.rlim_max+FD_LIMIT_INDEX),  get_limit.rlim_cur,                                 (get_limit.rlim_max+FD_LIMIT_INDEX),
                                                        get_limit.rlim_cur,                                 0,                                                            get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        get_limit.rlim_cur,                                 get_limit.rlim_max,                                 get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        get_limit.rlim_cur,                                 (get_limit.rlim_max-FD_LIMIT_INDEX),  get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        get_limit.rlim_cur,                                 (get_limit.rlim_max+FD_LIMIT_INDEX),  get_limit.rlim_cur,                                (get_limit.rlim_max+FD_LIMIT_INDEX),
                                                        (get_limit.rlim_cur-FD_LIMIT_INDEX),    0,                                                          get_limit.rlim_cur,                                 get_limit.rlim_max,
                                                        (get_limit.rlim_cur-FD_LIMIT_INDEX),    (get_limit.rlim_max-FD_LIMIT_INDEX), get_limit.rlim_cur,                                get_limit.rlim_max,
                                                        (get_limit.rlim_cur-FD_LIMIT_INDEX),    get_limit.rlim_max,                                get_limit.rlim_cur,                                get_limit.rlim_max,
                                                        (get_limit.rlim_cur-FD_LIMIT_INDEX),    (get_limit.rlim_max+FD_LIMIT_INDEX), get_limit.rlim_cur,                                (get_limit.rlim_max+FD_LIMIT_INDEX),
                                                        (get_limit.rlim_cur+FD_LIMIT_INDEX),    0,                                                           (get_limit.rlim_cur+FD_LIMIT_INDEX), get_limit.rlim_max,
                                                        (get_limit.rlim_cur+FD_LIMIT_INDEX),    (get_limit.rlim_max-FD_LIMIT_INDEX), (get_limit.rlim_cur+FD_LIMIT_INDEX), get_limit.rlim_max,
                                                        (get_limit.rlim_cur+FD_LIMIT_INDEX),    get_limit.rlim_max,                                (get_limit.rlim_cur+FD_LIMIT_INDEX), get_limit.rlim_max,
                                                        (get_limit.rlim_cur+FD_LIMIT_INDEX),    (get_limit.rlim_max+FD_LIMIT_INDEX), (get_limit.rlim_cur+FD_LIMIT_INDEX), (get_limit.rlim_max+FD_LIMIT_INDEX),
                                                        };
        for ( cnt=0; cnt<MAX_NO_TEST_INPUT, ++cnt)
        {
            err = SYSINIT_CLIENT_SetProcessMaxFDLimit((unsigned long) inputData[cnt][0], (unsigned long) inputData[cnt][1])
            if (SYSINIT_STATUS_OK != err)
            {
                CU_ASSERT_EQUAL(err, SYSINIT_STATUS_OK);
            }
            else
            {
                struct rlimit verify_limit = {0};
                
                err = getrlimit(RLIMIT_NOFILE, &verify_limit);
                if (err)
                {
                    printf(" getrlimit reurned with error = %d \n",err);
                    CU_ASSERT_EQUAL(err, 0);
                }
                else
                {
                    if((verify_limit.rlim_cur == inputData[cnt][2]) && (verify_limit.rlim_max == inputData[cnt][3] ))
                    {
                        printf(" Input[%lu %lu] Expected[%lu %lu] Actual[%lu %lu] \n ", (unsigned long) inputData[cnt][0], (unsigned long) inputData[cnt][1]
                                                                                                        (unsigned long) inputData[cnt][2], (unsigned long) inputData[cnt][3] 
                                                                                                        (unsigned long)verify_limit.rlim_cur, (unsigned long)verify_limit.rlim_max);
                    }
                    else
                    {
                        int assert_var = -1;
                        printf(" Input[%lu %lu] Expected[%lu %lu] Actual[%lu %lu] \n ", (unsigned long) inputData[cnt][0], (unsigned long) inputData[cnt][1]
                                                                                                        (unsigned long) inputData[cnt][2], (unsigned long) inputData[cnt][3] 
                                                                                                        (unsigned long)verify_limit.rlim_cur, (unsigned long)verify_limit.rlim_max);
                        CU_ASSERT_EQUAL(assert_var, 0);
                    }
                }
            }
        }
    }
}
 
static int checkFinishedCondition(SYSINIT_TEST_ConditionFlag f)
{
    /* Test 1 is complete when all components are in the PVR run level */
    uint32_t i;

    if(f == SYSINIT_TEST_Condition_Timeout)
    {
        return 1;
    }

    printf("wait for SYSINIT_API_RunLevelNotificationShutdown\n");

    for(i = 0U; i < SYSINIT_TEST_NUM_COMPONENTS; i++)
    {
        if(componentState[i].currentState != (uint32_t)SYSINIT_API_RunLevelNotificationShutdown)
        {
            return 0;
        }
    }

    printf(" find SYSINIT_TEST_Log_ClientTestResults event\n");
    if (findEvent(SYSINIT_TEST_MASTER_COMPONENT_A, SYSINIT_TEST_Log_ClientTestResults, 0) != -1)
    {
        printf("found SYSINIT_TEST_Log_ClientTestResults event A\n");
        if (findEvent(SYSINIT_TEST_MASTER_COMPONENT_B, SYSINIT_TEST_Log_ClientTestResults, 0) != -1)
        {
            printf(" found SYSINIT_TEST_Log_ClientTestResults event B\n");
            return 1;
        }
    }

    return 0;
}

/**
 * @brief test1Init
  *
  *
  *  @if REQPRO
  *  \li \b TEST 1.0
  *  \li \b REQUIREMENTS 193088 132827 132830 132831 132832
  *  @endif
  *
**/
void test1Init(void)
{
    char * argv[] = { "/NDS/bin/SYSINIT_COMPONENT_TEST" };

    /* Set config files */
    g_SYSINIT_TEST_shm_pointer->g_sync_ptr = 0U;
    switchSysinitConfigFileName("sysinit.cfg");
    switchSysinitComponentConfigFileName("sysinit_component.cfg");
    memcpy(g_SYSINIT_TEST_shm_pointer->client_config_file, "sysinit_component.cfg", 80U);

    /* Init */
    startSYSINIT(1, argv);
    setEventLogComponentName(SYSINIT_TEST_MASTER_COMPONENT_A,"master1a");
    setEventLogComponentName(SYSINIT_TEST_MASTER_COMPONENT_B,"master1b");
    resetEventLog();
    setLogTimeout(SYSINIT_TEST_LOG_TIMEOUT);

    /* Listen to logs */
    (void)eventLogListen(checkFinishedCondition);

    /* We are now ready for cUnit to run the tests*/
    /* Wait until finished*/
    (void)eventLogSync();

    printf("test1Init - event log sync\n");
}


int test1Wait(void)
{
    int retval = 0;

    while (g_SYSINIT_TEST_shm_pointer->g_sync_ptr < SYSINIT_TEST_SYNC_PTR_STATE_2ND_PROCESS_FIN)
    {
        printf("testmain: Waiting for Tests to finish - sync_ptr is %u\n", g_SYSINIT_TEST_shm_pointer->g_sync_ptr);
        (void)usleep(2000000U);

    }

    /* Wait for the server to close */
    if (SYSINIT_STATUS_OK != SYSINIT_SERVER_WaitForAllClientsShutdownSem())
    {
        retval = 1;
    }

    (void)usleep(2000000U);

    return 0;
}

int test1Term(void)
{
    /*To make sure all tests are executed*/
    (void)usleep(2000000U);

    printf("Terminating Server Task\n");
    SYSINIT_SERVER_Term();

    return 0;
}
