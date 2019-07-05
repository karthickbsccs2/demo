
void Test_RotateArray(int *ArrayBase,int Row,int Column,int ClockOrAntiClock,int Degree)
{
 /* Assume 0=90,1=180,2=270,3=360 */
 /* Assume 0=clockwise otherwise AntiClockWise */

 int *BackupArray=NULL,ModRow=0,ModCol,indx1=0,indx2=0,Output=0;

 if ( ( ArrayBase == NULL ) ||\
 	( Row == 0 ) )
 {
 	 return;
 }
 
 if( ClockOrAntiClock != 0)/*Anti - Clockwise*/
 	{
 		if( 0 == Degree  )/* if Anti-Clockwise = 90 == Clockwise 270 */
 		{
			Degree = 2;
 		}
		else if ( 2 == Degree )/* if Anti-Clockwise = 270 == Clockwise 90 */
		{
			Degree = 0;	
		}		
 	}

 	if ( Degree > 2 )
 	{
 		return;
 	}

	if( Degree == 0 || Degree == 2 )
	{
		ModRow= Column;
		ModCol = Row;
	}
	else
	{
		ModRow = Row;
		ModCol	= Column;
	}

	BackupArray	= ( int * )malloc( ModRow * ModCol * sizeof(int));

	if( NULL != BackupArray )
	{
		memcpy(BackupArray,ArrayBase, (ModRow * ModCol * sizeof(int)));

		for( indx1=0; indx1<ModRow;indx1++)
		{
			for(indx2=0; indx2<ModCol;++indx2)
			{
				switch( Degree )	
				{
					case 0:
					{
						Output=ArrayBase[(((Row-1)-indx2 )* (Column)) + indx1];
					}
						break;

					case 1:
					{
						Output=ArrayBase[((Row-indx1)* Column )- (indx2-1)];
					}
						break;

					case 2:
					{
						Output=ArrayBase[(indx2* Column )+ (Column - (indx1-1)];
					}
						break;

					default:
					{

					}
				}
				BackupArray[(ModRow*ModCol)+ModCol]=Output;
			}
		}
		memcpy(ArrayBase, BackupArray, (ModRow * ModCol * sizeof(int)));

		for( indx1=0; indx1<ModRow;indx1++)
		{
			printf("Row No.: %u\n", indx1);
			for(indx2=0; indx2<ModCol;++indx2)
			{
				printf("%u ", ArrayBase[(indx1*ModCol)+indx2]);
			}
			printf("\n");
		}
	}
	else
	{
		return;	
	}
 
}

