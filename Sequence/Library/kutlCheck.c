#include <stdio.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <direct.h>
#include <shlobj.h>
#include <SYS/STAT.H>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/locking.h>

#include <windows.h>

#include <CimSeqnc.h>

#include <kutlfile.h>
#include <kutlanal.h>
#include <kutlstr.h>

#include <kutlcheck.h>
#define  HANDLE_RETRY_COUNT	10000


void _GetRealIOName( int Index , char *_GetRealIOString );
int _iLOG_READ_CHECK();
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
void _File_Get_Name( char *filename , char *arg , int style , int style2 ) {
	if      ( style == 0 ) {
		sprintf( filename , "Parameter\\%s_Alarm_Temp.cfg" , arg );
	}
	else if ( style == 1 ) {
		sprintf( filename , "Parameter\\%s_Alarm_MFC%02d.cfg" , arg , style2 + 1 );
	}
	else if ( style == 2 ) {
		sprintf( filename , "Parameter\\%s_Alarm_Press.cfg" , arg );
	}
	else if ( style == 3 ) {
		sprintf( filename , "Parameter\\%s_PID_Table%02d.cfg" , arg , style2 + 1 );
	}
	else {
		sprintf( filename , "" );
	}
}
//-----------------------------------------------------------------------------------
BOOL _File_Read( char *arg , int style , int style2 , int mode , int Index , double setdata , double *r1 , double *r2 , double *r3 , double *r4 , double *r5 ) {
	HFILE hFile;
	int retindexs[256] , retindexsz[256];
	char FileName[ 256 + 1 ];
	char Buffer[ 1024 + 1 ];
	int  ReadCnt , Line;
	int s3mode;
	double s3read , s3lastseltemp;
	BOOL FileEnd = TRUE;

	char chr_return[ 256 + 1 ];

	//
	*r1 = 0;
	*r2 = 0;
	*r3 = 0;
	*r4 = 0;
	*r5 = 0;
	//
	if ( Index < 0 ) return FALSE;
	//
	if      ( style == 0 ) {}
	else if ( style == 1 ) {}
	else if ( style == 2 ) {}
	else if ( style == 3 ) {}
	else return FALSE;
	//
	if ( ( style == 1 ) && ( ( style2 < 0 ) || ( style2 >= 10 ) ) ) return FALSE;
	if ( ( style == 3 ) && ( ( style2 < 0 ) || ( style2 >=  5 ) ) ) return FALSE;
	//
	_File_Get_Name( FileName , arg , style , style2 );
	//
	hFile = _lopen( FileName , OF_READ );
	//
	if ( hFile == -1 ) return FALSE;
	//
	if ( mode == 3 ) s3mode = 0;
	//
	for ( Line = 1 ; FileEnd ; Line++ ) {
		FileEnd = H_Get_Line_String_From_File2_Include_Index( hFile , Buffer , 1024 , &ReadCnt , retindexs , retindexsz );
		if ( ReadCnt > 0 ) {
			//============================================================================================
			if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[0] , retindexsz[0] ) ) {
				_lclose( hFile );
				return FALSE;
			}
			//============================================================================================
			if ( mode == 0 ) { // alarm
				//
				if ( ReadCnt <= 6 ) continue;
				//
				if ( STRCMP_L( chr_return , "$ALARM" ) ) {
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[1] , retindexsz[1] ) ) {
						continue;
					}
					//
					if ( Index == ( atoi( chr_return ) - 1 ) ) {
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[2] , retindexsz[2] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r1 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[3] , retindexsz[3] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r2 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[4] , retindexsz[4] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r3 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[5] , retindexsz[5] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r4 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[6] , retindexsz[6] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r5 = atof( chr_return );
						//
						_lclose( hFile );
						return TRUE;
					}
				}
			}
			else if ( mode == 1 ) { // Stable
				//
				if ( ReadCnt <= 3 ) continue;
				//
				if ( STRCMP_L( chr_return , "$STABLE" ) ) {
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[1] , retindexsz[1] ) ) {
						continue;
					}
					//
					if ( Index == ( atoi( chr_return ) - 1 ) ) {
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[2] , retindexsz[2] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r1 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[3] , retindexsz[3] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r2 = atof( chr_return );
						//
						_lclose( hFile );
						return TRUE;
					}
				}
			}
			else if ( mode == 2 ) { // Level
				//
				if ( ReadCnt <= 4 ) continue;
				//
				if ( STRCMP_L( chr_return , "$LEVEL" ) ) {
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[1] , retindexsz[1] ) ) {
						continue;
					}
					//
					if ( Index == ( atoi( chr_return ) - 1 ) ) {
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[2] , retindexsz[2] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r1 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[3] , retindexsz[3] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r2 = atof( chr_return );
						//
						if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[4] , retindexsz[4] ) ) {
							_lclose( hFile );
							return FALSE;
						}
						*r3 = atof( chr_return );
						//
						_lclose( hFile );
						return TRUE;
					}
				}
			}
			else if ( mode == 3 ) { // pid
				//
				if ( ReadCnt <= 5 ) continue;
				//
				if ( STRCMP_L( chr_return , "$PID" ) ) {
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[1] , retindexsz[1] ) ) {
						continue;
					}
					//
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[2] , retindexsz[2] ) ) {
						_lclose( hFile );
						return FALSE;
					}
					//
					s3read = atof( chr_return );
					//
					if ( setdata == s3read ) {
						s3mode = 2;
					}
					else {
						if ( s3mode == 0 ) {
							s3mode = 1;
						}
						else {
							if ( s3lastseltemp > setdata ) {
								if ( s3read < setdata ) continue;
								if ( s3read > s3lastseltemp ) continue;
							}
							else {
								if ( s3read < setdata ) {
									if ( s3read < s3lastseltemp ) continue;
								}
							}
						}
					}
					//
					*r1 = s3read;
					//
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[3] , retindexsz[3] ) ) {
						_lclose( hFile );
						return FALSE;
					}
					*r2 = atof( chr_return );
					//
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[4] , retindexsz[4] ) ) {
						_lclose( hFile );
						return FALSE;
					}
					*r3 = atof( chr_return );
					//
					if ( !Get_Data_From_String_with_index( Buffer , chr_return , retindexs[5] , retindexsz[5] ) ) {
						_lclose( hFile );
						return FALSE;
					}
					*r4 = atof( chr_return );
					//
					s3lastseltemp = s3read;
					//
					if ( s3mode == 2 ) break;
					//

				}
			}
		}
	}
	_lclose( hFile );
	if ( mode == 3 ) return ( s3mode != 0 );
	return FALSE;
}



int Get_Stable_Table( char *arg , int Type , int TypeSub , int TableID , double *StableTime , double *StableRange ) {
	double r3 , r4 , r5;
	if ( !_File_Read( arg , Type , TypeSub , 1 , TableID , 0 , StableTime , StableRange , &r3 , &r4 , &r5 ) ) {
		return FALSE;
	}
	return TRUE;
}

int Get_Level_Table( char *arg , int Type , int TypeSub , int TableID , int *SensorIO , double *LevelTime , double *LevelRange ) {
	double r1 , r4 , r5;
	if ( !_File_Read( arg , Type , TypeSub , 2 , TableID , 0 , &r1 , LevelTime , LevelRange , &r4 , &r5 ) ) {
		return FALSE;
	}
	*SensorIO = (int) r1;
	return TRUE;
}

int Get_Alarm_Table( char *arg , int Type , int TypeSub , int TableID , double *DelayTime , double *AlarmTime , double *AlarmRange , double *WarnTime , double *WarnRange ) {
	if ( !_File_Read( arg , Type , TypeSub , 0 , TableID , 0 , DelayTime , AlarmTime , AlarmRange , WarnTime , WarnRange ) ) {
		return FALSE;
	}
	return TRUE;
}


int Get_PID_Table( char *arg , int TableID , double SetData , double *seltemp , double *Band , double *Integ , double *Deriv ) {
	double r5;
	if ( !_File_Read( arg , 3 , TableID , 3 , 0 , SetData , seltemp , Band , Integ , Deriv , &r5 ) ) {
		return FALSE;
	}
	return TRUE;
}





int Check_Level_Data( int ASensor , double LevelTime , double LevelRange , int ResetCount ) {
	int CommStatus;
	int Timer;
	int Count;
	double currdata;
	//
	if ( ( LevelRange <= 0 ) || ( LevelTime <= 0 ) ) return SYS_SUCCESS;
	//
	Timer = GetTickCount();
	Count = 0;
	//
	_iLOG_USER_PRINTF( FALSE , "Level Check Start(Range=%.1f)(Time=%.1f)\n" , LevelRange , LevelTime );
	//
	_iLOG_CHECK_ALARM( 25 , ASensor , (int) LevelRange , "$DATA$" , (int) LevelTime , -1 );
	//
	_iLOG_MON_START( 0 );
	//
	while( TRUE ) {
		//
		currdata = _iREAD_ANALOG( ASensor , &CommStatus );
		//
		if ( currdata <= LevelRange ) {
			Count = 0;
			if ( ( GetTickCount() - Timer ) >= (unsigned int) ( LevelTime * 1000 ) ) {
				//
				_iLOG_MON_END( 0 );
				//
				return SYS_SUCCESS;
			}
		}
		else {
			Count++;
			if ( Count >= ResetCount ) {
				Count = 0;
				Timer = GetTickCount();
			}
		}
		//
		if ( !_iWAIT_SECONDS( 0.1 ) ) {
			//
			_iLOG_MON_END( 0 );
			//
			return SYS_ABORTED;
		}
	}
	//
	_iLOG_MON_END( 0 );
	//
	return SYS_SUCCESS;
}


int Check_Stable_Data( int ASensor , double Setdata , double StableTime , double StableRange , int ResetCount ) {
	int CommStatus;
	int Timer;
	int Count;
	double currdata;
	double up , down;
	//
	if ( ( StableRange <= 0 ) || ( StableTime <= 0 ) ) return SYS_SUCCESS;
	//
	up = Setdata + StableRange;
	down = Setdata - StableRange;
	//
	Timer = GetTickCount();
	Count = 0;
	//
	_iLOG_USER_PRINTF( FALSE , "Stable Check Start(Set=%.1f)(Range=%.1f,%.1f)\n" , Setdata , down , up );
	//
	_iLOG_CHECK_ALARM( 25 , ASensor , (int) up   , "$DATA$" , (int) StableTime , -1 );
	_iLOG_CHECK_ALARM( 23 , ASensor , (int) down , "$DATA$" , (int) StableTime , -1 );
	//
	_iLOG_MON_START( 0 );
	//
	while( TRUE ) {
		//
		currdata = _iREAD_ANALOG( ASensor , &CommStatus );
		//
		if ( ( currdata <= up ) && ( currdata >= down ) ) {
			Count = 0;
			if ( ( GetTickCount() - Timer ) >= (unsigned int) ( StableTime * 1000 ) ) {
				//
				_iLOG_MON_END( 0 );
				//
				return SYS_SUCCESS;
			}
		}
		else {
			Count++;
			if ( Count >= ResetCount ) {
				Count = 0;
				Timer = GetTickCount();
			}
		}
		//
		if ( !_iWAIT_SECONDS( 0.1 ) ) {
			//
			_iLOG_MON_END( 0 );
			//
			return SYS_ABORTED;
		}
	}
	//
	_iLOG_MON_END( 0 );
	//
	return SYS_SUCCESS;
}


int Check_Alarm_Data( int ASensor , double Setdata , double DelayTime , double AlarmTime , double AlarmRange , int AlarmID_Up , int AlarmID_Down , double WarnTime , double WarnRange , int WarnID_Up , int WarnID_Down , int errorabort , int ResetCount ) {
	int CommStatus;
	int Timer_W , Timer_A;
	int Count_W , Count_A;
	double currdata;
	double Warn_up , Warn_down , Warn_Time;
	double Alarm_up , Alarm_down , Alarm_Time;
	int Warn_AID_Up , Alarm_AID_Up;
	int Warn_AID_Down , Alarm_AID_Down;
	BOOL Alarm_Check , Warn_Check;
	//
	if ( AlarmRange > WarnRange ) { // Normal
		Alarm_up = Setdata + AlarmRange;
		Alarm_down = Setdata - AlarmRange;
		Alarm_Time = AlarmTime;
		Alarm_AID_Up = AlarmID_Up;
		Alarm_AID_Down = AlarmID_Down;
		//
		Alarm_Check = ( ( AlarmRange > 0 ) && ( AlarmTime > 0 ) );
		//
		Warn_up = Setdata + WarnRange;
		Warn_down = Setdata - WarnRange;
		Warn_Time = WarnTime;
		Warn_AID_Up = WarnID_Up;
		Warn_AID_Down = WarnID_Down;
		//
		Warn_Check = ( ( WarnRange > 0 ) && ( WarnTime > 0 ) );
		//
	}
	else {
		Alarm_up = Setdata + WarnRange;
		Alarm_down = Setdata - WarnRange;
		Alarm_Time = WarnTime;
		Alarm_AID_Up = WarnID_Up;
		Alarm_AID_Down = WarnID_Down;
		//
		Alarm_Check = ( ( WarnRange >= 0 ) && ( WarnTime >= 0 ) );
		//
		Warn_up = Setdata + AlarmRange;
		Warn_down = Setdata - AlarmRange;
		Warn_Time = AlarmTime;
		Warn_AID_Up = AlarmID_Up;
		Warn_AID_Down = AlarmID_Down;
		//
		Warn_Check = ( ( AlarmRange >= 0 ) && ( AlarmTime >= 0 ) );
		//
	}
	//
	if ( !Alarm_Check && !Warn_Check ) return SYS_SUCCESS;
	//
	if ( DelayTime > 0 ) {
		Timer_W = GetTickCount();
		//
		while( TRUE ) {
			if ( ( GetTickCount() - Timer_W ) >= (unsigned int) ( DelayTime * 1000 ) ) {
				break;
			}
			//
			if ( !_iWAIT_SECONDS( 0.1 ) ) return SYS_ABORTED;
		}
	}
	if ( Warn_Check ) {
		//---------------------------------------------------------------------------------
		_iLOG_CHECK_ALARM( 25 , ASensor , (int) Warn_up   , "$DATA$" , (int) Warn_Time , Warn_AID_Up );
		//---------------------------------------------------------------------------------
		_iLOG_CHECK_ALARM( 22 , ASensor , (int) Warn_down , "$DATA$" , (int) Warn_Time , Warn_AID_Down );
		//---------------------------------------------------------------------------------
	}
	//
	if ( Alarm_Check ) {
		//---------------------------------------------------------------------------------
		_iLOG_CHECK_ALARM( 25 , ASensor , (int) Alarm_up   , "$DATA$" , (int) Alarm_Time , Alarm_AID_Up );
		//---------------------------------------------------------------------------------
		_iLOG_CHECK_ALARM( 22 , ASensor , (int) Alarm_down , "$DATA$" , (int) Alarm_Time , Alarm_AID_Down );
		//---------------------------------------------------------------------------------
	}
	//
	Timer_W = GetTickCount();
	Timer_A = GetTickCount();
	Count_W = 0;
	Count_A = 0;
	//
	while( TRUE ) {
		//
		currdata = _iREAD_ANALOG( ASensor , &CommStatus );
		//
		if ( Warn_Check ) {
			if ( ( currdata >= Warn_up ) || ( currdata <= Warn_down ) ) {
				Count_W = 0;
				if ( ( GetTickCount() - Timer_W ) >= (unsigned int) ( Warn_Time * 1000 ) ) {
					//
					if      ( currdata >= Warn_up ) {
						if ( Warn_AID_Up > 0 ) {
							if ( ALARM_STATUS( Warn_AID_Up ) != ALM_PAUSED ) _iALARM_POST( Warn_AID_Up );
						}
					}
					else if ( currdata <= Warn_down ) {
						if ( Warn_AID_Down > 0 ) {
							if ( ALARM_STATUS( Warn_AID_Down ) != ALM_PAUSED ) _iALARM_POST( Warn_AID_Down );
						}
					}
					//
					Count_W = 0;
					Timer_W = GetTickCount();
					if ( errorabort > 0 ) return SYS_ABORTED;
				}
			}
			else {
				Count_W++;
				if ( Count_W >= ResetCount ) {
					Count_W = 0;
					Timer_W = GetTickCount();
				}
			}
		}
		//
		if ( Alarm_Check ) {
			if ( ( currdata > Alarm_up ) || ( currdata < Alarm_down ) ) {
				Count_A = 0;
				if ( ( GetTickCount() - Timer_A ) >= (unsigned int) ( Alarm_Time * 1000 ) ) {
					//
					if      ( currdata >= Alarm_up ) {
						if ( Alarm_AID_Up > 0 ) {
							if ( ALARM_STATUS( Alarm_AID_Up ) != ALM_PAUSED ) _iALARM_POST( Alarm_AID_Up );
						}
					}
					else if ( currdata <= Alarm_down ) {
						if ( Alarm_AID_Down > 0 ) {
							if ( ALARM_STATUS( Alarm_AID_Down ) != ALM_PAUSED ) _iALARM_POST( Alarm_AID_Down );
						}
					}
					//
					Count_A = 0;
					Timer_A = GetTickCount();
					if ( errorabort > 1 ) return SYS_ABORTED;
				}
			}
			else {
				Count_A++;
				if ( Count_A >= ResetCount ) {
					Count_A = 0;
					Timer_A = GetTickCount();
				}
			}
		}
		//
		if ( !_iWAIT_SECONDS( 0.1 ) ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}

//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================

void ARGUE_to_REPLACE0_and_ALARM( int *alarmstart ) {
	char buffer[256];
	char buffer2[256];
	STR_SEPERATE_CHAR( PROGRAM_ARGUMENT_READ() , '|' , buffer , buffer2 , 255 );
	REPLACE_CHAR_SET( 0 , buffer );
	*alarmstart = atoi( buffer2 );
}

BOOL IO_CHECKING2( int msg , int Auto , int setio , int snsio , int data , double timesec , int alarm ) {
	int retvalue , Res , CommStatus , set , retalm , almon , readdata , oldread , read1ok , oldcomm;
	int timeclock;
	char _GetRealIOString[256];
	//
	if ( alarm > 0 ) {
		if ( snsio >= 0 ) {
			if ( msg ) {
				_iLOG_CHECK_ALARM( 21 , snsio , data , "$DATA$" , ( timesec <= 0 ) ? -1 : (int) timesec , alarm );
			}
		}
	}
	//
	if ( ( snsio >= 0 ) && ( timesec > 0 ) && ( alarm > 0 ) ) _iLOG_MON_START( 0 );
	//
	timeclock = GetTickCount();
	//
	set = 0;
	retalm = 0;
	almon = FALSE;
	//
	read1ok = 0;
	//
	retvalue = FALSE;
	//
	while( TRUE ) {
		//
		if ( snsio >= 0 ) {
			//
			readdata = READ_DIGITAL( snsio , &CommStatus );
			//
			if ( ( read1ok == 0 ) || ( oldread != readdata ) || ( oldcomm != CommStatus ) ) {
				switch ( _iLOG_READ_CHECK() ) {  // 2010.06.18
				case 1 :
				case 2 :
					_GetRealIOName(snsio, _GetRealIOString);
					if ( CommStatus )	_iLOG_PRINTF( "\t - [IO] READ_DIGITAL %30s\t\t<= [%d]\n" , _GetRealIOString , readdata );
					else				_iLOG_PRINTF( "\t - [IO] READ_DIGITAL %30s\t\t<= [%d][FALSE]\n" , _GetRealIOString , readdata );
					break;
				}
				//
				read1ok = 1;
				oldread = readdata;
				oldcomm = CommStatus;
				//
			}
			//
			if ( readdata == data ) {
				if ( alarm > 0 ) {
					if ( almon ) {
						if ( ALARM_STATUS( alarm ) == ALM_PAUSED ) ALARM_CLEAR( alarm );
					}
				}
				retvalue = TRUE;
				break;
			}
		}
		//
		if ( setio >= 0 ) {
			if ( set == 0 ) {
				set = 1;
				_iWRITE_DIGITAL( setio , data , &CommStatus );
				//
				if ( !CommStatus ) {
					//
					if ( _IO_CONSOLE_STANDALONE_STATUS() ) {
						_iLOG_PRINTF( "[IO] SETTING FAILED %s to %d\n" , IO_STR_Map_Table[setio].IO_Name , data );
					}
					else {
						printf( "=======================================\n" );
						printf( "IO SETTING FAILED %s to %d\n" , IO_STR_Map_Table[setio].IO_Name , data );
						printf( "=======================================\n" );
					}
					//
					retvalue = FALSE;
					break;
				}
				//
			}
		}
		//
		if ( snsio >= 0 ) {
			if ( ( setio >= 0 ) && ( timesec <= 0 ) && ( alarm == -99 ) ) {
				retvalue = TRUE;
				break;
			}
		}
		else {
			if ( alarm <= 0 ) {
				retvalue = TRUE;
				break;
			}
		}
		//
		if ( Auto && almon ) {
			Res = ALARM_STATUS( alarm );
			if ( Res != ALM_PAUSED ) {
				//
				if ( Res == ALM_ABORT ) {
					retvalue = FALSE;
					break;
				}
				if ( Res == ALM_IGNORE ) {
					retvalue = TRUE;
					break;
				}
				if ( Res == ALM_CLEAR ) {
					if ( retalm >= 2 ) {
						retvalue = FALSE;
						break;
					}
				}
				//
				almon = FALSE;
				//
				timeclock = GetTickCount();
				set = 0;
				//
				retalm++;
				//
				read1ok = 0;
			}
		}
		else {
			if ( ( timesec <= 0 ) || ( ( GetTickCount() - timeclock ) >= (unsigned int) ( timesec * 1000 ) ) ) {
				//
				if ( alarm > 0 ) {
					//
					if ( Auto ) {
						_iALARM_POST( alarm );
						almon = TRUE;
					}
					else {
						Res = _iALARM_MANAGE( alarm );
						//
						if      ( Res == ALM_ABORT )	{
							retvalue = FALSE;
							break;
						}
						else if ( Res == ALM_IGNORE ) {
							retvalue = TRUE;
							break;
						}
						else if ( Res == ALM_CLEAR )	{
							if ( retalm >= 2 ) {
								retvalue = FALSE;
								break;
							}
						}
						//
						timeclock = GetTickCount();
						set = 0;
						//
						read1ok = 0;
						//
						retalm++;
					}
				}
				else {
					//
					if ( _IO_CONSOLE_STANDALONE_STATUS() ) {
						_iLOG_PRINTF( "[IO] SETTING FAILED %s to %d\n" , IO_STR_Map_Table[snsio].IO_Name , data );
					}
					else {
						printf( "=======================================\n" );
						printf( "IO SETTING FAILED %s to %d\n" , IO_STR_Map_Table[snsio].IO_Name , data );
						printf( "=======================================\n" );
					}
					//
					retvalue = FALSE;
					break;
				}
			}
		}
		//
		if ( read1ok == 1 ) {
			if ( !_iWAIT_SECONDS( 0.1 ) ) {
				retvalue = FALSE;
				break;
			}
			read1ok = 2;
		}
		else {
			if ( !WAIT_SECONDS( 0.1 ) ) {
				retvalue = FALSE;
				break;
			}
		}
		//
	}
	//
	if ( ( snsio >= 0 ) && ( timesec > 0 ) && ( alarm > 0 ) ) _iLOG_MON_END( 0 );
	//
	return retvalue;
}


BOOL IO_CHECKING( int setio , int snsio , int data , double timesec , int alarm ) {
	return IO_CHECKING2( TRUE , FALSE , setio , snsio , data , timesec , alarm );
}


//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//
#define  MAX_FUNCTION_LOG_SUB	4
enum { F_FM, F_BM1, F_BM2, F_TMC, F_PM1, F_PM2, F_PM3, F_PM4 };
int Event_To_Tag		= -1;

int _iLOG_ADDRESSF_FM  = -2;
int _iLOG_ADDRESSF_BM1 = -2;
int _iLOG_ADDRESSF_BM2 = -2;
int _iLOG_ADDRESSF_TMC = -2;
int _iLOG_ADDRESSF_PM1 = -2;
int _iLOG_ADDRESSF_PM2 = -2;
int _iLOG_ADDRESSF_PM3 = -2;
int _iLOG_ADDRESSF_PM4 = -2;
int _iLOG_ADDRESSF_TARGET = -2;
//

extern char *_SpecialCharData[ 36 ];
//
int _iLOG_ADDRESS = -2;
int _iLOG_ADDRESSF = -2;
int _iLOG_ADDRESST = -2;
int _iLOG_CONTROL = 0;
//char _GetRealIOString[ 256 ];
//
void Target_Find() {
	if		( Event_To_Tag == F_FM	   ) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_FM;
	else if ( Event_To_Tag == F_BM1	   ) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_BM1; 
	else if ( Event_To_Tag == F_BM2	   ) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_BM2; 
	else if ( Event_To_Tag == F_TMC	   ) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_TMC; 
	else if ( Event_To_Tag == F_PM1	   ) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_PM1; 
	else if ( Event_To_Tag == F_PM2		) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_PM2; 
	else if ( Event_To_Tag == F_PM3		) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_PM3; 
	else if ( Event_To_Tag == F_PM4		) _iLOG_ADDRESSF_TARGET = _iLOG_ADDRESSF_PM4; 
}
//
int _iLOG_GLOBAL_RUN_CHECK() {
	int c;
	if ( _iLOG_ADDRESS == -2 ) {
		_iLOG_ADDRESS = _FIND_FROM_STRING( 0 , "FUNCTION_LOG_CONTROL" );
	}
	//
	if ( _iLOG_ADDRESS == -1 ) return 1;
	//
	switch( _dREAD_DIGITAL( _iLOG_ADDRESS , &c ) ) {
	case 1 : return 1;
	case 2 :
		//
		if ( _iLOG_ADDRESSF == -2 ) {			
			//
			_iLOG_ADDRESSF	   = 1;
			//
			_iLOG_ADDRESSF_FM  = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_FM"  );
			_iLOG_ADDRESSF_BM1 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_BM1" );
			_iLOG_ADDRESSF_BM2 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_BM2" );
			_iLOG_ADDRESSF_TMC = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_TMC" );
			_iLOG_ADDRESSF_PM1 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_PM1" );
			_iLOG_ADDRESSF_PM2 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_PM2" );
			_iLOG_ADDRESSF_PM3 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_PM3" );
			_iLOG_ADDRESSF_PM4 = _FIND_FROM_STRING( 4 , "FUNCTION_LOG_SERVER_PM4" );
		}
		//
		if ( _iLOG_ADDRESST == -2 ) _iLOG_ADDRESST = _FIND_FROM_STRING( 1 , "FUNCTION_LOG_WAIT" );
		return 2;
	case 3 :
		return 3;
	}
	return 0;
}
//
void _GetRealIOName( int Index , char *_GetRealIOString ) {
	int PosNum , Index2 , Index3 , Index4 , Len , Len2 , id;
	//
	Len = strlen( IO_STR_Map_Table[ Index ].IO_Name );
	//
	_GetRealIOString[0] = 0;
	//
	for ( PosNum = 0 , Index2 = 0 , Index3 = 0 ; Index3 < Len ; Index3++ ) {
		if ( PosNum == 0 ) {
			if ( IO_STR_Map_Table[ Index ].IO_Name[Index3] == '$' ) {
				PosNum = 1;
			}
			else {
				_GetRealIOString[Index2] = IO_STR_Map_Table[ Index ].IO_Name[Index3];
				Index2++;
			}
		}
		else if ( PosNum == 1 ) {
			if ( ( IO_STR_Map_Table[ Index ].IO_Name[Index3] >= '0' ) && ( IO_STR_Map_Table[ Index ].IO_Name[Index3] <= '9' ) ) {
				id = (int) IO_STR_Map_Table[ Index ].IO_Name[Index3] - '0';
				Len2 = strlen( _SpecialCharData[ id ] );
				for ( Index4 = 0 ; Index4 < Len2 ; Index4++ ) {
					_GetRealIOString[Index2] = _SpecialCharData[ id ][Index4];
					Index2++;
				}
			}
			else if ( ( IO_STR_Map_Table[ Index ].IO_Name[Index3] >= 'A' ) && ( IO_STR_Map_Table[ Index ].IO_Name[Index3] <= 'Z' ) ) { // 2002.07.19
				id = (int) IO_STR_Map_Table[ Index ].IO_Name[Index3] - 'A' + 10;
				Len2 = strlen( _SpecialCharData[ id ] );
				for ( Index4 = 0 ; Index4 < Len2 ; Index4++ ) {
					_GetRealIOString[Index2] = _SpecialCharData[ id ][Index4];
					Index2++;
				}
			}
			else if ( ( IO_STR_Map_Table[ Index ].IO_Name[Index3] >= 'a' ) && ( IO_STR_Map_Table[ Index ].IO_Name[Index3] <= 'z' ) ) { // 2002.07.19
				id = (int) IO_STR_Map_Table[ Index ].IO_Name[Index3] - 'a' + 10;
				Len2 = strlen( _SpecialCharData[ id ] );
				for ( Index4 = 0 ; Index4 < Len2 ; Index4++ ) {
					_GetRealIOString[Index2] = _SpecialCharData[ id ][Index4];
					Index2++;
				}
			}
			else {
				_GetRealIOString[Index2] = IO_STR_Map_Table[ Index ].IO_Name[Index3];
				Index2++;
			}
			PosNum = 0;
		}
	}
	_GetRealIOString[Index2] = 0x00;
	// 2010.05.21
	//return _GetRealIOString;
}

//
void _iLOG_GetFileName( char *FileName , int wYear , int wMonth , int wDay , int wHour ) {
	if      ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "PM1." , 4 ) ) {
		Event_To_Tag = F_PM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "PM2." , 4 ) ) {
		Event_To_Tag = F_PM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "PM3." , 4 ) ) {
		Event_To_Tag = F_PM3;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM3.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "PM4." , 4 ) ) {
		Event_To_Tag = F_PM4;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM4.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "PM1" , 3 ) ) {
		Event_To_Tag = F_PM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "PM2" , 3 ) ) {
		Event_To_Tag = F_PM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "PM3" , 3 ) ) {
		Event_To_Tag = F_PM3;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM3.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "PM4" , 3 ) ) {
		Event_To_Tag = F_PM4;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-PM4.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_MOTOR_L1" , 12 ) ) {
		Event_To_Tag = F_BM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_MOTOR_L2" , 12 ) ) {
		Event_To_Tag = F_BM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_LLA" , 7 ) ) {
		Event_To_Tag = F_BM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_LLB" , 7 ) ) {
		Event_To_Tag = F_BM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_BM1" , 7 ) ) {
		Event_To_Tag = F_BM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_BM2" , 7 ) ) {
		Event_To_Tag = F_BM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "BM1" , 3 ) ) {
		Event_To_Tag = F_BM1;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM1.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "BM2" , 3 ) ) {
		Event_To_Tag = F_BM2;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-BM2.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "TMC_FM" , 6 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_FUNCTION_READ() , "FM" , 2 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "CM1" , 3 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "CM2" , 3 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "CM3" , 3 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else if ( STRNCMP_L( PROGRAM_ARGUMENT_READ() , "CM4" , 3 ) ) {
		Event_To_Tag = F_FM;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-FM.log" , wYear , wMonth , wDay , wHour );
	}
	else {
		Event_To_Tag = F_TMC;
		sprintf( FileName , "HISTORY\\FUNC_%04d%02d%02d%02d-TMC.log" , wYear , wMonth , wDay , wHour );
	}
}

void _iLOG_PRINTF( LPSTR list , ... ) {
	HFILE hFile;
	char DataBuffer[1024];
	char FileName[64];
	SYSTEMTIME		SysTime;
	va_list va;
	char SendData[1024];
	char SendData2[128];
	int lm, ls, ws;
//	long c;
	//
	GetLocalTime( &SysTime );
	//
	if ( _iLOG_CONTROL == 0 ) {
		if ( _IO_CONSOLE_STANDALONE_STATUS() ) {
			va_start( va , list );
			vprintf( list , (LPSTR) va );
			va_end( va );
		}
	}
	else {
		sprintf( FileName , "HISTORY\\FN%04d%02d%02d%02d-%s.log" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , PROGRAM_FUNCTION_READ() );
		//
		va_start( va , list );
		//
		hFile = _open( FileName , _O_BINARY | _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
		//
		if ( hFile != -1 ) {
			//
			_vsnprintf( SendData , 1000 , list , (LPSTR) va );
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData );
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData );   // 2011.02.18 jiahn ( Issue No 495 )
			//
			_write( hFile , DataBuffer , strlen( DataBuffer ) );
			//
			_close( hFile );	
		}
		//
		if ( _IO_CONSOLE_STANDALONE_STATUS() ) vprintf( list , (LPSTR) va );
		//
		va_end( va );
	}
	//
	lm = _iLOG_GLOBAL_RUN_CHECK();
	if ( lm > 0 ) {
		//
		_iLOG_GetFileName( FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour );
		//
		va_start( va , list );
		//
		_vsnprintf( SendData , 1000 , list , (LPSTR) va );
		//
		if ( _iLOG_ADDRESSF >= 0 ) {
			//_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11   // 2011.02.18 jiahn ( Issue No 495 )
			//		
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , DataBuffer );	
		}
		else {
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11   // 2011.02.18 jiahn ( Issue No 495 )
			//
			ls = strlen( DataBuffer );
			//
			hFile = _open( FileName , _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
			//
			if ( hFile != -1 ) {
				ws = _write( hFile , DataBuffer , ls );
				_close( hFile );
				//
				if ( ls != ws ) {
					_IO_CIM_PRINTF( "[ERROR1] File Write Fail for [%s] [%d/%d]\n" , FileName , ls , ws );
				}
			}
			else {
				_IO_CIM_PRINTF( "[ERROR2] File Open Fail for [%s]\n" , FileName );
			}
		}
		//
		if ( ( lm == 2 ) && ( _iLOG_ADDRESSF >= 0 ) ) {
			_vsnprintf( SendData2 , 127 , list , (LPSTR) va );
			_snprintf( SendData , 155 , "$F$%s|%s" , PROGRAM_FUNCTION_READ() , SendData2 );
			//
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , SendData );  		
		}
		//
		va_end( va );
	}
}

void _iLOG_fPRINTF( LPSTR list , ... ) {
	HFILE hFile;
	char DataBuffer[1024];
	char FileName[64];
	SYSTEMTIME		SysTime;
	char SendData[1024];
	va_list va;
	int lm, ls, ws;
	//
	GetLocalTime( &SysTime );
	//
	if ( _iLOG_CONTROL == 0 ) {
		if ( _IO_CONSOLE_STANDALONE_STATUS() ) {
			va_start( va , list );
			vprintf( list , (LPSTR) va );
			va_end( va );
		}
	}
	else {
		sprintf( FileName , "HISTORY\\FN%04d%02d%02d%02d-%s.log" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , PROGRAM_FUNCTION_READ() );
		//
		va_start( va , list );
		//
		hFile = _open( FileName , _O_BINARY | _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
		//
		if ( hFile != -1 ) {
			//
			_vsnprintf( SendData , 1000 , list , (LPSTR) va );
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%:%d:%d\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData );
			//
			_write( hFile , DataBuffer , strlen( DataBuffer ) );
			//
			_close( hFile );	
		}
		//
		if ( _IO_CONSOLE_STANDALONE_STATUS() ) vprintf( list , (LPSTR) va );
		//
		va_end( va );
	}
	//
	lm = _iLOG_GLOBAL_RUN_CHECK();
	if ( lm > 0 ) {
		//
		_iLOG_GetFileName( FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour );
		//
		va_start( va , list );
		//
		_vsnprintf( SendData , 1000 , list , (LPSTR) va );
		//
		if ( _iLOG_ADDRESSF >= 0 ) {
			//_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//		
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , DataBuffer );  		
		}
		else {
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//
			ls = strlen( DataBuffer );
			//
			hFile = _open( FileName , _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
			//
			if ( hFile != -1 ) {
				ws = _write( hFile , DataBuffer , ls );
				_close( hFile );	
				//
				if ( ls != ws ) {
					_IO_CIM_PRINTF( "[ERROR3] File Write Fail for [%s] [%d/%d]\n" , FileName , ls , ws );
				}
			}
			else {
				_IO_CIM_PRINTF( "[ERROR4] File Open Fail for [%s]\n" , FileName );
			}
		}
		//
		va_end( va );
		//
	}
}


void _iLOG_USER_PRINTF( int Display , LPSTR list , ... ) {
	HFILE hFile;
	char DataBuffer[1024];
	char FileName[64];
	SYSTEMTIME		SysTime;
	va_list va;
	char SendData[1024];
	char SendData2[128];
	int lm, ls, ws;
	//
	//
	GetLocalTime( &SysTime );
	//
	if ( _iLOG_CONTROL == 0 ) {
		if ( _IO_CONSOLE_STANDALONE_STATUS() || ( Display == 1 ) ) {
			va_start( va , list );
			vprintf( list , (LPSTR) va );
			va_end( va );
		}
	}
	else {
		sprintf( FileName , "HISTORY\\FN%04d%02d%02d%02d-%s.log" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , PROGRAM_FUNCTION_READ() );
		//
		va_start( va , list );
		//
		hFile = _open( FileName , _O_BINARY | _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
		//
		if ( hFile != -1 ) {
			//
			_vsnprintf( SendData , 1000 , list , (LPSTR) va );
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t   * %s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t   * %s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//
			_write( hFile , DataBuffer , strlen( DataBuffer ) );
			//
			_close( hFile );	
		}
		//
		if ( _IO_CONSOLE_STANDALONE_STATUS() || ( Display == 1 ) ) vprintf( list , (LPSTR) va );
		//
		va_end( va );
	}
	//
	lm = _iLOG_GLOBAL_RUN_CHECK();
	if ( lm > 0 ) {
		//
		_iLOG_GetFileName( FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour );
		//
		va_start( va , list );
		//
		_vsnprintf( SendData , 1000 , list , (LPSTR) va );
		//
		if ( _iLOG_ADDRESSF >= 0 ) {
			//_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//		
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , DataBuffer );  	
		}
		else {
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//
			ls = strlen( DataBuffer );
			//
			hFile = _open( FileName , _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
			//
			if ( hFile != -1 ) {
				ws = _write( hFile , DataBuffer , ls );
				_close( hFile );
				//
				if ( ls != ws ) {
					_IO_CIM_PRINTF( "[ERROR5] File Write Fail for [%s] [%d/%d]\n" , FileName , ls , ws );
				}
			}
			else {
				_IO_CIM_PRINTF( "[ERROR6] File Open Fail for [%s]\n" , FileName );
			}
		}
		//
		if ( Display != -1 ) {
			if ( ( lm == 2 ) && ( _iLOG_ADDRESSF >= 0 ) ) {
				//
				_vsnprintf( SendData2 , 127 , list , (LPSTR) va );
				_snprintf( SendData , 155 , "$f$%s|   * %s" , PROGRAM_FUNCTION_READ() , SendData2 );
				//	
				Target_Find();
				_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , SendData );  	
			}
		}
		//
		va_end( va );
		//
	}
}
//
void _iLOG_USER_PRINTF_NA( int Display , LPSTR list , ... ) {
	HFILE hFile;
	char DataBuffer[1024];
	char FileName[64];
	SYSTEMTIME		SysTime;
	va_list va;
	char SendData[1024];
	char SendData2[128];
	int lm, ls, ws;
	//
	//
	GetLocalTime( &SysTime );
	//
	if ( _iLOG_CONTROL == 0 ) {
		if ( _IO_CONSOLE_STANDALONE_STATUS() || ( Display == 1 ) ) {
			va_start( va , list );
			vprintf( list , (LPSTR) va );
			va_end( va );
		}
	}
	else {
		sprintf( FileName , "HISTORY\\FN%04d%02d%02d%02d-%s.log" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , PROGRAM_FUNCTION_READ() );
		//
		va_start( va , list );
		//
		hFile = _open( FileName , _O_BINARY | _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
		//
		if ( hFile != -1 ) {
			//
			_vsnprintf( SendData , 1000 , list , (LPSTR) va );
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t   * %s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t   * %s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//
			_write( hFile , DataBuffer , strlen( DataBuffer ) );
			//
			_close( hFile );	
		}
		//
		if ( _IO_CONSOLE_STANDALONE_STATUS() || ( Display == 1 ) ) vprintf( list , (LPSTR) va );
		//
		va_end( va );
	}
	//
	lm = _iLOG_GLOBAL_RUN_CHECK();
	if ( lm >= 0 ) {
		//
		_iLOG_GetFileName( FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour );
		//
		va_start( va , list );
		//
		_vsnprintf( SendData , 1000 , list , (LPSTR) va );
		//
		if ( _iLOG_ADDRESSF >= 0 ) {
			//_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "$S$%s|%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , FileName , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//		
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , DataBuffer );  
		}
		else {
			//_snprintf( DataBuffer , 1023 , "%d/%d/%d %d:%d:%d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11
			_snprintf( DataBuffer , 1023 , "%d/%d/%d %02d:%02d:%02d.%03d\t%-30s\t%s" , SysTime.wYear , SysTime.wMonth , SysTime.wDay , SysTime.wHour , SysTime.wMinute , SysTime.wSecond , SysTime.wMilliseconds , PROGRAM_FUNCTION_READ() , SendData ); // 2010.05.11  // 2011.02.18 jiahn ( Issue No 495 )
			//
			ls = strlen( DataBuffer );
			//
			hFile = _open( FileName , _O_RDWR | _O_CREAT | _O_APPEND , _S_IREAD | _S_IWRITE );
			//
			if ( hFile != -1 ) {
				ws = _write( hFile , DataBuffer , ls );
				_close( hFile );
				//
				if ( ls != ws ) {
					_IO_CIM_PRINTF( "[ERROR5] File Write Fail for [%s] [%d/%d]\n" , FileName , ls , ws );
				}
			}
			else {
				_IO_CIM_PRINTF( "[ERROR6] File Open Fail for [%s]\n" , FileName );
			}
		}
		//
		//
		if ( Display != -1 ) {
			if ( ( lm == 2 ) && ( _iLOG_ADDRESSF >= 0 ) ) {
				//
				_vsnprintf( SendData2 , 127 , list , (LPSTR) va );
				_snprintf( SendData , 155 , "$f$%s|   * %s" , PROGRAM_FUNCTION_READ() , SendData2 );
				//	
				Target_Find();
				_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , SendData );  		
			}
		}
		//
		va_end( va );
		//
	}
}
//

void _iLOG_MON_START( int depth ) {
	switch( depth ) {
	case 0 :	_iLOG_PRINTF( "\t - >>>>\n" );	break;
	case 1 :	_iLOG_PRINTF( "\t - -->>>>\n" );	break;
	case 2 :	_iLOG_PRINTF( "\t - ---->>>>\n" );	break;
	case 3 :	_iLOG_PRINTF( "\t - ------>>>>\n" );	break;
	case 4 :	_iLOG_PRINTF( "\t - -------->>>>\n" );	break;
	}
}

void _iLOG_MON_END( int depth ) {
	switch( depth ) {
	case 0 :	_iLOG_PRINTF( "\t - <<<<\n" );	break;
	case 1 :	_iLOG_PRINTF( "\t - --<<<<\n" );	break;
	case 2 :	_iLOG_PRINTF( "\t - ----<<<<\n" );	break;
	case 3 :	_iLOG_PRINTF( "\t - ------<<<<\n" );	break;
	case 4 :	_iLOG_PRINTF( "\t - --------<<<<\n" );	break;
	}
}
//-------------------------------------------------------------------------------------
// 	FUNCTION_LOG_READ_IO	OnOff     # READ_IO Log On/Off
// 	FUNCTION_LOG_WRITE_IO	OnOff	  # WRITE_IO Log On/Off
// 	FUNCTION_LOG_FUNC	OnOff	  # FUNCTION Log On/Off
// 
// 	FUNCTION_LOG_MERGE	OnOff	  # FUNCTION_EVENT 1/5 call
// 
//
int _iLOG_READ_CHECK() {
	int add, c, data;

	add  = _FIND_FROM_STRING( _K_D_IO , "FUNCTION_LOG_READ_IO" );
	data = _dREAD_DIGITAL( add , &c );

	if ( data ==  0 ) return 2;
	else			  return 0;
}
//
//
int _iLOG_WRITE_CHECK() {
	int add, c, data;

	add  = _FIND_FROM_STRING( _K_D_IO , "FUNCTION_LOG_WRITE_IO" );
	data = _dREAD_DIGITAL( add , &c );

	if ( data ==  0 ) return 1;
	else			  return 0;
}
//
//
int _iLOG_FUNC_CHECK() {
	int add, c, data;

	add  = _FIND_FROM_STRING( _K_D_IO , "FUNCTION_LOG_FUNC" );
	data = _dREAD_DIGITAL( add , &c );

	if ( data ==  0 ) return 2;
	else			  return 0;
}
//
//-------------------------------------------------------------------------------------
//
void _iLOG_ON() {
	_iLOG_CONTROL = 1;
}

void _iLOG_OFF() {
	_iLOG_CONTROL = 0;
}

int _iLOG_RUN() {
	switch( _iLOG_GLOBAL_RUN_CHECK() ) {
	case 1 : return 1;
	case 2 : return 2;
	case 3 : return 3;
	default :
		if ( _iLOG_CONTROL == 1 ) return 1;
		if ( _IO_CONSOLE_STANDALONE_STATUS() ) return 1;
	}
	return 0;
}

int _iREAD_DIGITAL( int io , int *CommStatus ) {
	int data;
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] READ_DIGITAL %30s\n" , _GetRealIOName(io, _GetRealIOString) );
	data = READ_DIGITAL( io , CommStatus );
	switch ( _iLOG_READ_CHECK() ) {
	case 1 :
	case 2 :
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] READ_DIGITAL %30s\t\t<= [%d]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] READ_DIGITAL %30s\t\t<= [%d][FALSE]\n" , _GetRealIOString , data );
		break;
	}
	return data;
}

void _iWRITE_DIGITAL( int io , int data , int *CommStatus ) {
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] WRITE_DIGITAL %30s [%d]\n" , _GetRealIOName(io, _GetRealIOString) , data );
	WRITE_DIGITAL( io , data , CommStatus );
	if ( _iLOG_WRITE_CHECK() > 0 ) {
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] WRITE_DIGITAL %30s [%d]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] WRITE_DIGITAL %30s [%d]\t=> [FALSE]\n" , _GetRealIOString , data );
	}
}

double _iREAD_ANALOG( int io , int *CommStatus ) {
	double data;
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] READ_ANALOG %30s\n" , _GetRealIOName(io, _GetRealIOString) );
	data = READ_ANALOG( io , CommStatus );
	switch ( _iLOG_READ_CHECK() ) {
	case 1 :
	case 2 :
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] READ_ANALOG  %30s\t\t<= [%f]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] READ_ANALOG  %30s\t\t<= [%f][FALSE]\n" , _GetRealIOString , data );
		break;
	}
	return data;
}

void _iWRITE_ANALOG( int io , double data , int *CommStatus ) {
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] WRITE_ANALOG %30s [%f]\n" , _GetRealIOName(io, _GetRealIOString) , data );
	WRITE_ANALOG( io , data , CommStatus );
	if ( _iLOG_WRITE_CHECK() > 0 ) {
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] WRITE_ANALOG %30s [%f]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] WRITE_ANALOG %30s [%f]\t=> [FALSE]\n" , _GetRealIOString , data );
	}
}

void _iREAD_STRING( int io , char *data , int *CommStatus ) {
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] READ_STRING  %30s\n" , _GetRealIOName(io, _GetRealIOString) );
	READ_STRING( io , data , CommStatus );
	switch ( _iLOG_READ_CHECK() ) {
	case 1 :
	case 2 :
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] READ_STRING  %30s\t\t<= [%s]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] READ_STRING  %30s\t\t<= [%s][FALSE]\n" , _GetRealIOString , data );
		break;
	}
}

void _iWRITE_STRING( int io , char *data , int *CommStatus ) {
	char _GetRealIOString[256];
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [IO] WRITE_STRING %30s [%s]\n" , _GetRealIOName(io, _GetRealIOString) , data );
	WRITE_STRING( io , data , CommStatus );
	if ( _iLOG_WRITE_CHECK() > 0 ) {
		_GetRealIOName(io, _GetRealIOString);
		if ( *CommStatus )	_iLOG_PRINTF( "\t - [IO] WRITE_STRING %30s [%s]\n" , _GetRealIOString , data );
		else				_iLOG_PRINTF( "\t - [IO] WRITE_STRING %30s [%s]\t=> [FALSE]\n" , _GetRealIOString , data );
	}
}

int _iALARM_MANAGE( int AlarmID ) {
	int Res;
	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\n" , AlarmID );
	Res = ALARM_MANAGE( AlarmID );
	if ( _iLOG_RUN() > 0 ) {
		switch( Res ) {
		case ALM_CLEAR	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [CLEAR]\n" , AlarmID );	break;
		case ALM_RETRY	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [RETRY]\n" , AlarmID );	break;
		case ALM_WAIT	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [WAIT]\n" , AlarmID );	break;
		case ALM_IGNORE	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [IGNORE]\n" , AlarmID );	break;
		case ALM_DISABLE:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [DISABLE]\n" , AlarmID );	break;
		case ALM_ENABLE	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [ENABLE]\n" , AlarmID );	break;
		case ALM_STOP	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [STOP]\n" , AlarmID );	break;
		case ALM_ABORT	:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [ABORT]\n" , AlarmID );	break;
		default			:	_iLOG_PRINTF( "\t - [ALRM] ALARM_MANAGE %30d\t\t=> [ALM=%d]\n" , AlarmID , Res );	break;
		}
	}
	return Res;
}

void _iALARM_POST( int AlarmID ) {
	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [ALRM] ALARM_POST   %30d\n" , AlarmID );
	ALARM_POST( AlarmID );
}

int _iWAIT_SECONDS( double data ) {
	int Res;
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "v- [WS] WAIT_SECONDS %f\n" , data );
	Res = WAIT_SECONDS( data );
	switch ( _iLOG_FUNC_CHECK() ) {
	case 1 :
	case 2 :
		if ( Res )			_iLOG_PRINTF( "\t - [WS] WAIT_SECONDS %30.1f\n" , data );
		else				_iLOG_PRINTF( "\t - [WS] WAIT_SECONDS %30.1f\t\t=> [FALSE]\n" , data );
		break;
	}
	return Res;
}



void _iRUN_FUNCTION_ABORT( int io ) {
	char _GetRealIOString[256];
	switch ( _iLOG_FUNC_CHECK() ) {
	case 1 :
	case 2 :
		_GetRealIOName(io, _GetRealIOString);
		_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_ABORT   %30s\n" , _GetRealIOString );
		break;
	}
	RUN_FUNCTION_ABORT( io );
}

int _iREAD_FUNCTION( int io ) {
	int Res;
	char _GetRealIOString[256];
//	_GetRealIOName(io, _GetRealIOString)
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTION %s\n" , _GetRealIOString );
	Res = READ_FUNCTION( io );
	switch ( _iLOG_FUNC_CHECK() ) {
	case 1 :
	case 2 :
		_GetRealIOName(io, _GetRealIOString);
		switch( Res ) {
		case SYS_RUNNING:	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTIN %30s\t\t<= [RUNNING]\n" ,  _GetRealIOString );	break;
		case SYS_ABORTED:	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTIN %30s\t\t<= [ABORTED]\n" ,_GetRealIOString );	break;
		case SYS_SUCCESS:	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTIN %30s\t\t<= [SUCCESS]\n" , _GetRealIOString );	break;
		case SYS_ERROR	:	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTIN %30s\t\t<= [ERROR]\n" , _GetRealIOString );	break;
		default			:	_iLOG_PRINTF( "\t - [FUNC] READ_FUNCTIN %30s\t\t<= [FN=%d]\n" , _GetRealIOString , Res );	break;
		}
		break;
	}
	return Res;
}


int _iRUN_FUNCTION( int io , char *data ) {
	int Res;
	char _GetRealIOString[256];

	//
	_GetRealIOName(io, _GetRealIOString);
	//
	if ( _iLOG_FUNC_CHECK() > 0 )	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %s [%s]\n" , _GetRealIOString , data );
	Res = RUN_FUNCTION( io , data );
	if ( _iLOG_FUNC_CHECK() > 0 ) {
		switch( Res ) {
		case SYS_RUNNING:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %30s [%s]\t<= [RUNNING]\n" , _GetRealIOString , data );	break;
		case SYS_ABORTED:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %30s [%s]\t<= [ABORTED]\n" , _GetRealIOString , data );	break;
		case SYS_SUCCESS:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %30s [%s]\t<= [SUCCESS]\n" , _GetRealIOString , data );	break;
		case SYS_ERROR	:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %30s [%s]\t<= [ERROR]\n" , _GetRealIOString , data );	break;
		default			:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION %30s [%s]\t<= [FN=%d]\n" , _GetRealIOString , data , Res );	break;
		}
	}
	return Res;
}


int _iRUN_FUNCTION_FREE( int io , char *data ) {
	int Res;
	char _GetRealIOString[256];

	//
	_GetRealIOName(io, _GetRealIOString);
	//
	if ( _iLOG_FUNC_CHECK() > 0 )	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %s [%s]\n" , _GetRealIOString , data );
	Res = RUN_FUNCTION_FREE( io , data );
	if ( _iLOG_FUNC_CHECK() > 0 ) {
		switch( Res ) {
		case SYS_RUNNING:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %30s [%s]\t<= [RUNNING]\n" , _GetRealIOString , data );	break;
		case SYS_ABORTED:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %30s [%s]\t<= [ABORTED]\n" , _GetRealIOString , data );	break;
		case SYS_SUCCESS:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %30s [%s]\t<= [SUCCESS]\n" , _GetRealIOString , data );	break;
		case SYS_ERROR	:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %30s [%s]\t<= [ERROR]\n" , _GetRealIOString , data );	break;
		default			:	_iLOG_PRINTF( "\t - [FUNC] RUN_FUNCTION_FREE %30s [%s]\t<= [FN=%d]\n" , _GetRealIOString , data , Res );	break;
		}
	}
	return Res;
}


int _iRUN_SET_FUNCTION( int io , char *data ) {
	int Res;
	char _GetRealIOString[256];


	_GetRealIOName(io, _GetRealIOString);
//	if ( _iLOG_RUN() > 0 )	_iLOG_PRINTF( "\t - [FUNC] RUN_SET_FUNCTION %s [%s]\n" , _GetRealIOString , data );
	Res = RUN_SET_FUNCTION( io , data );
	if ( _iLOG_FUNC_CHECK() > 0 ) {
		if ( Res ) {
							_iLOG_PRINTF( "\t - [FUNC] RUN_SET_FUNCTION %30s [%s]\n" , _GetRealIOString , data );
		}
		else {
							_iLOG_PRINTF( "\t - [FUNC] RUN_SET_FUNCTION %30s [%s] => [FALSE]\n" , _GetRealIOString , data );
		}
	}
	return Res;
}

void _iWRITE_FUNCTION_EVENT( int io , char *data ) {
	char _GetRealIOString[256];
	
	_GetRealIOName(io, _GetRealIOString);
	if ( _iLOG_FUNC_CHECK() > 0 )	_iLOG_PRINTF( "\t - [FUNC] WRITE_FUNCTION_EVENT  %30s [%s]\n" , _GetRealIOString , data );
	WRITE_FUNCTION_EVENT( io , data );
}



void _iLOG_CHECK_ALARM( int mode , int realio , int targetio , char *data2 , int timeoutio , int alarm ) {
	int c;
	double d;
	int lm , iom , ioa , timer;
	char ioname1[64];
	char ioname2[64];
	char iodata2[64];
	char ioname3[64];
	char sbuf[256];
	char _GetRealIOString[256];

	lm = _iLOG_RUN();
	//
	if ( lm > 0 ) {
		//
		if ( realio < 0 ) return;
		//
		if      ( ( mode / 100 ) == 8 ) { // D
			_dDIGITAL_NAME( realio , ioname1 );
			iom = _K_D_IO;
			ioa = realio;
			//
			if ( STRCMP_L( data2 , "$DATA$" ) ) {
				sprintf( iodata2 , "%d" , targetio );
				sprintf( ioname2 , "%d" , targetio );
			}
			else if ( STRCMP_L( data2 , "$USER$" ) ) {
				strcpy( iodata2 , "$USER$" );
				strcpy( ioname2 , "$USER$" );
			}
			else {
				if ( targetio >= 0 ) {
					_dDIGITAL_NAME( targetio , sbuf );
					sprintf( iodata2 , "%d" , _dREAD_DIGITAL( targetio , &c ) );
					sprintf( ioname2 , "%s(%s)" , sbuf , iodata2 );
					sprintf( iodata2 , "D:%d" , targetio );
				}
				else {
					strcpy( iodata2 , "$INVALID$" );
					strcpy( ioname2 , data2 );
				}
			}
			//
			if ( timeoutio >= 0 ) {
				if      ( ( ( mode % 100 ) / 10 ) == 2 ) {
					strcpy( ioname3 , "Over" );
					d = timeoutio;
				}
				else if ( ( ( mode % 100 ) / 10 ) == 1 ) {
					//
					_dDIGITAL_NAME( timeoutio , sbuf );
					sprintf( ioname3 , "%s Over" , sbuf );
					d = (double) _dREAD_DIGITAL(timeoutio,&c);
					//
				}
				else {
					//
					_dANALOG_NAME( timeoutio , sbuf );
					sprintf( ioname3 , "%s Over" , sbuf );
					d = _dREAD_ANALOG(timeoutio,&c);
					//
				}
			}
			else {
				d = 0;
			}
		}
		else if ( ( mode / 100 ) == 9 ) { // A
			_dANALOG_NAME( realio , ioname1 );
			iom = _K_A_IO;
			ioa = realio;
			//
			if ( STRCMP_L( data2 , "$DATA$" ) ) {
				sprintf( iodata2 , "%d" , targetio );
				sprintf( ioname2 , "%d" , targetio );
			}
			else if ( STRCMP_L( data2 , "$USER$" ) ) {
				strcpy( iodata2 , "$USER$" );
				strcpy( ioname2 , "$USER$" );
			}
			else {
				if ( targetio >= 0 ) {
					_dANALOG_NAME( targetio , sbuf );
					sprintf( iodata2 , "%.3f" , _dREAD_ANALOG( targetio , &c ) );
					sprintf( ioname2 , "%s(%s)" , sbuf , iodata2 );
					sprintf( iodata2 , "A:%d" , targetio );
				}
				else {
					strcpy( iodata2 , "$INVALID$" );
					strcpy( ioname2 , data2 );
				}
			}
			//
			if ( timeoutio >= 0 ) {
				if      ( ( ( mode % 100 ) / 10 ) == 2 ) {
					strcpy( ioname3 , "Over" );
					d = timeoutio;
				}
				else if ( ( ( mode % 100 ) / 10 ) == 1 ) {
					//
					_dDIGITAL_NAME( timeoutio , sbuf );
					sprintf( ioname3 , "%s Over" , sbuf );
					d = (double) _dREAD_DIGITAL(timeoutio,&c);
					//
				}
				else {
					//
					_dANALOG_NAME( timeoutio , sbuf );
					sprintf( ioname3 , "%s Over" , sbuf );
					d = _dREAD_ANALOG(timeoutio,&c);
					//
				}
			}
			else {
				d = 0;
			}
		}
		else {
			_GetRealIOName(realio, _GetRealIOString);
			strcpy( ioname1 , _GetRealIOString );
			iom = IO_STR_Map_Table[ realio ].IO_Mode;
			ioa = IO_STR_Map_Table[ realio ].IO_Match_Point;
			//
			if ( STRCMP_L( data2 , "$DATA$" ) ) {
				sprintf( iodata2 , "%d" , targetio );
				sprintf( ioname2 , "%d" , targetio );
			}
			else if ( STRCMP_L( data2 , "$USER$" ) ) {
				strcpy( iodata2 , "$USER$" );
				strcpy( ioname2 , "$USER$" );
			}
			else {
				if ( targetio >= 0 ) {
					switch( IO_STR_Map_Table[ targetio ].IO_Mode ) {
					case _K_D_IO :
						sprintf( iodata2 , "%d" , READ_DIGITAL( targetio , &c ) );
						break;
					case _K_A_IO :
						sprintf( iodata2 , "%.3f" , READ_ANALOG( targetio , &c ) );
						break;
					default :
						strcpy( iodata2 , "$INVALID$" );
						break;
					}
					//
					_GetRealIOName(targetio, _GetRealIOString);
					sprintf( ioname2 , "%s(%s)" , _GetRealIOString , iodata2 );
					//
					switch( IO_STR_Map_Table[ targetio ].IO_Mode ) {
					case _K_D_IO :
						sprintf( iodata2 , "D:%d" , IO_STR_Map_Table[ targetio ].IO_Match_Point );
						break;
					case _K_A_IO :
						sprintf( iodata2 , "A:%d" , IO_STR_Map_Table[ targetio ].IO_Match_Point );
						break;
					default :
						strcpy( iodata2 , "$INVALID$" );
						break;
					}
				}
				else {
					strcpy( iodata2 , "$INVALID$" );
					strcpy( ioname2 , data2 );
				}
			}
			//
			if ( timeoutio >= 0 ) {
				if      ( ( ( mode % 100 ) / 10 ) == 2 ) {
					strcpy( ioname3 , "Over" );
					d = timeoutio;
				}
				else if ( ( ( mode % 100 ) / 10 ) == 1 ) {
					//
					_GetRealIOName(timeoutio, _GetRealIOString);
					sprintf( ioname3 , "%s Over" , _GetRealIOString );
					d = (double) READ_DIGITAL(timeoutio,&c);
					//
				}
				else {
					//
					_GetRealIOName(timeoutio, _GetRealIOString);
					sprintf( ioname3 , "%s Over" , _GetRealIOString );
					d = READ_ANALOG(timeoutio,&c);
					//
				}
			}
			else {
				d = 0;
			}
		}
		//
		if ( timeoutio >= 0 ) {
			if      ( ( mode % 10 ) == 0 ) { // ==
				_iLOG_fPRINTF( "\t - [AC] if [%s == %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 1 ) { // !=
				_iLOG_fPRINTF( "\t - [AC] if [%s != %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 2 ) { // <
				_iLOG_fPRINTF( "\t - [AC] if [%s < %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 3 ) { // <=
				_iLOG_fPRINTF( "\t - [AC] if [%s <= %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 4 ) { // >
				_iLOG_fPRINTF( "\t - [AC] if [%s > %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 5 ) { // >=
				_iLOG_fPRINTF( "\t - [AC] if [%s >= %s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 8 ) { //
				_iLOG_fPRINTF( "\t - [AC] if [%s,%s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
			else if ( ( mode % 10 ) == 9 ) { //
				_iLOG_fPRINTF( "\t - [AC] if [%s%s] and [%s %.0f sec] then\tAlarm [%d]\n" , ioname1 , ioname2 , ioname3 , d , alarm );
			}
		}
		else {
			if      ( ( mode % 10 ) == 0 ) { // ==
				_iLOG_fPRINTF( "\t - [AC] if [%s == %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 1 ) { // !=
				_iLOG_fPRINTF( "\t - [AC] if [%s != %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 2 ) { // <
				_iLOG_fPRINTF( "\t - [AC] if [%s < %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 3 ) { // <=
				_iLOG_fPRINTF( "\t - [AC] if [%s <= %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 4 ) { // >
				_iLOG_fPRINTF( "\t - [AC] if [%s > %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 5 ) { // >=
				_iLOG_fPRINTF( "\t - [AC] if [%s >= %s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 8 ) { //
				_iLOG_fPRINTF( "\t - [AC] if [%s,%s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
			else if ( ( mode % 10 ) == 9 ) { //
				_iLOG_fPRINTF( "\t - [AC] if [%s%s] then\tAlarm [%d]\n" , ioname1 , ioname2 , alarm );
			}
		}
		//
		if ( ( lm == 2 ) && ( _iLOG_ADDRESSF >= 0 ) ) {
			//               function style cond iomode ioaddr ioname iotrg timeout alarm
			sprintf( sbuf , "$A$%s %d %d %d %d %s %s %.1f %d" , PROGRAM_FUNCTION_READ() , ( mode / 100 ) , ( mode % 10 ) , iom , ioa , ioname1 , iodata2 , d , alarm );
			//	
			Target_Find();
			_dWRITE_FUNCTION_EVENT( _iLOG_ADDRESSF_TARGET , sbuf );  
			//
			if ( ( mode / 100 ) == 0 ) {
				//
				if ( _iLOG_ADDRESST >= 0 ) {
					//
					d = _dREAD_ANALOG( _iLOG_ADDRESST , &lm );
					//
					if ( d > 0 ) {
						//
						timer = GetTickCount();
						//
						while( TRUE ) {
							//
							_sleep(1);
							//
							d = _dREAD_ANALOG( _iLOG_ADDRESST , &lm );
							if ( d > 30 ) d = 30;
							if ( d <= 0 ) break;
							//
							if ( ( GetTickCount() - timer ) >= (unsigned int) ( d * 1000 ) ) break;
						}
						//
					}
				}
				//
			}
			//
		}
		//
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
// Function Interlock Check Utility
//
int IO_SETPOINT_BOTH_AND_CHECK( int io1, int Value1, int io2, int Value2, int Alarm ) {   // 2010.03.15
	
	int CommStatus , Res;
	//------
	_iLOG_USER_PRINTF( FALSE , "===> IO_SETPOINT_BOTH_AND_CHECK - ALARM[%d]\n" , Alarm );
	//------
	while( TRUE ) {
		_iLOG_CHECK_ALARM( 1,  io1 , Value1 , "$DATA$" , -1 , Alarm );
		_iLOG_CHECK_ALARM( 1,  io2 , Value2 , "$DATA$" , -1 , Alarm );
		if ( _iREAD_DIGITAL( io1 , &CommStatus ) == Value1 && _iREAD_DIGITAL( io2 , &CommStatus ) == Value2 ) break;	
			Res = _iALARM_MANAGE( Alarm );
		if  ( Res == ALM_IGNORE ) break;
		if  ( Res == ALM_ABORT ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
int IO_SETPOINT_CHECK( int io, int Value, int Alarm ) {   // 2010.03.15
	
	int CommStatus , Res;
	//------
	_iLOG_USER_PRINTF( FALSE , "===> IO_SETPOINT_CHECK - ALARM[%d]\n" , Alarm );
	//------
	while( TRUE ) {
		_iLOG_CHECK_ALARM( 1,  io , Value , "$DATA$" , -1 , Alarm );
		if ( _iREAD_DIGITAL( io , &CommStatus ) == Value ) break;	
			Res = _iALARM_MANAGE( Alarm );
		if  ( Res == ALM_IGNORE ) break;
		if  ( Res == ALM_ABORT ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
int IO_SETPOINT_CHECK_FALSE( int io, int Value, int Alarm ) {   // 2010.03.15
	
	int CommStatus , Res;
	//------
	_iLOG_USER_PRINTF( FALSE , "===> IO_SETPOINT_CHECK_FALSE - ALARM[%d]\n" , Alarm );
	//------
	while( TRUE ) {
		_iLOG_CHECK_ALARM( 10,  io , Value , "$DATA$" , -1 , Alarm );
		if ( _iREAD_DIGITAL( io , &CommStatus ) != Value ) break;	
			Res = _iALARM_MANAGE( Alarm );
		if  ( Res == ALM_IGNORE ) break;
		if  ( Res == ALM_ABORT ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
// 2010.02.28
int IO_SETPOINT_TRUEVALUE_CHECK( int io , int Value , int Alarm ) {
	int CommStatus , Res;
	int MainSts1, MainSts2, MainSts3, MainSts4;
	BOOL bAUTORUN_FLAG1 = FALSE, bAUTORUN_FLAG2 = FALSE, bAUTORUN_FLAG3 = FALSE, bAUTORUN_FLAG4 = FALSE;

	MainSts1 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS" );
	MainSts2 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS2" );
	MainSts3 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS3" );
	MainSts4 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS4" );
	//
	if ( MainSts1 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS" );
	if ( MainSts2 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS2" );
	if ( MainSts3 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS3" );
	if ( MainSts4 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS4" );
	
	// AutoRun Flag Set
	if ( _dREAD_DIGITAL( MainSts1 , &CommStatus ) > 0 )	bAUTORUN_FLAG1 = TRUE;
	if ( _dREAD_DIGITAL( MainSts2 , &CommStatus ) > 0 )	bAUTORUN_FLAG2 = TRUE;
	if ( _dREAD_DIGITAL( MainSts3 , &CommStatus ) > 0 )	bAUTORUN_FLAG3 = TRUE;
	if ( _dREAD_DIGITAL( MainSts4 , &CommStatus ) > 0 )	bAUTORUN_FLAG4 = TRUE;
	//
	
	// AutoRun Only Check
	if ( bAUTORUN_FLAG1 || bAUTORUN_FLAG2 || bAUTORUN_FLAG3 || bAUTORUN_FLAG4 ) { 
		//------
		_iLOG_USER_PRINTF( FALSE , "===> IO_SETPOINT_TRUEVALUE_CHECK - ALARM[%d]\n" , Alarm );
		//------
		while( TRUE ) {
			_iLOG_CHECK_ALARM( 1 , io , Value , "$DATA$" , -1 , Alarm );
			if ( _iREAD_DIGITAL( io , &CommStatus ) == Value ) break;	

			Res = _iALARM_MANAGE( Alarm );
			if  ( Res == ALM_IGNORE ) break;
			if  ( Res == ALM_ABORT ) return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
// 2010.02.28
int IO_SETPOINT_FALSEVALUE_CHECK( int io , int Value , int Alarm ) {
	int CommStatus , Res;
	int MainSts1, MainSts2, MainSts3, MainSts4;
	BOOL bAUTORUN_FLAG1 = FALSE, bAUTORUN_FLAG2 = FALSE, bAUTORUN_FLAG3 = FALSE, bAUTORUN_FLAG4 = FALSE;

	MainSts1 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS" );
	MainSts2 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS2" );
	MainSts3 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS3" );
	MainSts4 = _FIND_FROM_STRING( _K_D_IO , "CTC.MAIN_STATUS4" );
	//
	if ( MainSts1 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS" );
	if ( MainSts2 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS2" );
	if ( MainSts3 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS3" );
	if ( MainSts4 < 0 ) printf( "=====>> IO_SETPOINT_TRUEVALUE_CHECK IO Name [ %s ] Search Error\n" , "CTC.MAIN_STATUS4" );
	
	// AutoRun Flag Set
	if ( _dREAD_DIGITAL( MainSts1 , &CommStatus ) > 0 )	bAUTORUN_FLAG1 = TRUE;
	if ( _dREAD_DIGITAL( MainSts2 , &CommStatus ) > 0 )	bAUTORUN_FLAG2 = TRUE;
	if ( _dREAD_DIGITAL( MainSts3 , &CommStatus ) > 0 )	bAUTORUN_FLAG3 = TRUE;
	if ( _dREAD_DIGITAL( MainSts4 , &CommStatus ) > 0 )	bAUTORUN_FLAG4 = TRUE;
	//
	
	// AutoRun Only Check
	if ( bAUTORUN_FLAG1 || bAUTORUN_FLAG2 || bAUTORUN_FLAG3 || bAUTORUN_FLAG4 ) { 
		//------
		_iLOG_USER_PRINTF( FALSE , "===> IO_SETPOINT_FALSEVALUE_CHECK - ALARM[%d]\n" , Alarm );
		//------
		while( TRUE ) {
			_iLOG_CHECK_ALARM( 1 , io , Value , "$DATA$" , -1 , Alarm );
			if ( _iREAD_DIGITAL( io , &CommStatus ) != Value ) break;	

			Res = _iALARM_MANAGE( Alarm );
			if  ( Res == ALM_IGNORE ) break;
			if  ( Res == ALM_ABORT ) return SYS_ABORTED;
		}
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
BOOL read_form_NAME_FORMAT( HFILE hFilePointer , char *str_return ) {
	char  cc;
	int   ii;
	char  Reader[1025];
	int   wbyte;
	
	wbyte = _lread( hFilePointer , Reader , strlen(BEGIN_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , BEGIN_DELEMETER ) != 0 ) return( FALSE );
	wbyte = _lread( hFilePointer , &ii , sizeof(int) );
	if ( wbyte <= 0 ) return FALSE;
	wbyte = _lread( hFilePointer , Reader , ii );          Reader[wbyte] = 0x00;
	strcpy( str_return , Reader );
	wbyte = _lread( hFilePointer , &cc , sizeof(char) );
	if ( wbyte <= 0 ) return FALSE;
	if ( NAME_FORMAT != cc ) return ( FALSE );
	wbyte = _lread( hFilePointer , Reader , strlen(END_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , END_DELEMETER ) != 0 ) return( FALSE );
	return( TRUE );
}
//

BOOL read_return_form_MaxLimit( HFILE hFilePointer , char *ReturnNameIndicator , char *type_return , char *cc_return , int *ii_return , float *ff_return , double *dd_return , char *str_return ) {
	char  cc;
	int   ii;
	char  Reader[ 256 ];
	int   wbyte;
	
	wbyte = _lread( hFilePointer , Reader , strlen(BEGIN_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , BEGIN_DELEMETER ) != 0 ) return( FALSE );

	wbyte = _lread( hFilePointer , &ii , sizeof(int) );
	if ( wbyte <= 0 ) return FALSE;
	wbyte = _lread( hFilePointer , Reader , ii );          Reader[wbyte] = 0x00;
	strcpy( ReturnNameIndicator , Reader );

	wbyte = _lread( hFilePointer , Reader , sizeof(char) );
	if ( wbyte <= 0 ) return FALSE;
	memcpy( &cc , Reader , sizeof(char) );
	if ( cc < NAME_FORMAT || cc > DOUBLE_X_FORMAT ) return ( FALSE );
	*type_return = cc;

	switch ( cc ) {
		case NAME_FORMAT   :
		case NAME_X_FORMAT   :
								break;
		case CHAR_FORMAT   :
		case CHAR_X_FORMAT   :
								wbyte = _lread( hFilePointer , cc_return , sizeof(char) );
								if ( wbyte <= 0 ) return FALSE;
								break;
		case INT_FORMAT    :
		case INT_X_FORMAT    :
								wbyte = _lread( hFilePointer , ii_return , sizeof(int) );
								if ( wbyte <= 0 ) return FALSE;
								break;
		case FLOAT_FORMAT  :
		case FLOAT_X_FORMAT  :
								wbyte = _lread( hFilePointer , ff_return , sizeof(float) );
								if ( wbyte <= 0 ) return FALSE;
								break;
		case DOUBLE_FORMAT  :
		case DOUBLE_X_FORMAT  :
								wbyte = _lread( hFilePointer , dd_return , sizeof(double) );
								if ( wbyte <= 0 ) return FALSE;
								break;
		case STRING_FORMAT :
		case FILENAME_FORMAT :
		case STRING_X_FORMAT :
		case FILENAME_X_FORMAT :
								wbyte = _lread( hFilePointer , &ii , sizeof(int) );
								if ( wbyte <= 0 ) return FALSE;
								wbyte = _lread( hFilePointer , str_return , ii );
								str_return[wbyte] = 0x00;
								break;
	}
	wbyte = _lread( hFilePointer , Reader , strlen(END_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , END_DELEMETER ) != 0 ) return( FALSE );
	return( TRUE );
}

BOOL _iGet_Line_String_From_File_Include_Index( HFILE hFilePointer , char *ReadBuffer , int MaxCount , int *ReturnDelimiterCount , int *retindexs , int *retindexsz ) { // # is Comment & Tab(9) Read with 4 SPACE char // 2003.07.31
	char ch;
	BOOL Comment = FALSE , FindChr = FALSE , StrMode = FALSE , OldStr = FALSE;
	int wbyte;
	int ocnt = 0 , cnt = 0;

	*ReturnDelimiterCount = 0;
	for ( wbyte = _lread( hFilePointer , &ch , sizeof(char) ) ; wbyte > 0 ; wbyte = _lread( hFilePointer , &ch , sizeof(char) ) ) {
		//_sleep(200);
		switch ( ch ) {
			//case '#' :
			//			if ( Comment ) break;
			//			if ( StrMode ) {
			//				OldStr = FALSE;
			//				if ( ( cnt + 1 ) < MaxCount ) {
			//					*ReadBuffer++ = ch;
			//					cnt++;
			//				}
			//				FindChr = TRUE;
			//				break;
			//			}
			//			//
			//			if ( !Comment && FindChr ) {
			//				//-----------------------
			//				retindexs[*ReturnDelimiterCount] = ocnt;
			//				retindexsz[*ReturnDelimiterCount] = cnt - ocnt;
			//				//-----------------------
			//				FindChr = FALSE;
			//				//-----------------------
			//				ocnt = cnt;
			//				//-----------------------
			//				(*ReturnDelimiterCount)++;
			//			}
			//			Comment = TRUE;
			//			StrMode = FALSE;
			//			OldStr  = FALSE;
			//			break;

			case   9 :
//			case ' ' :	if ( Comment ) break;
						if ( StrMode ) {
							if ( ch == 9 ) {
								if ( ( cnt + 1 ) < MaxCount ) {
									*ReadBuffer++ = ' ';
									cnt++;
								}
								if ( ( cnt + 1 ) < MaxCount ) {
									*ReadBuffer++ = ' ';
									cnt++;
								}
								if ( ( cnt + 1 ) < MaxCount ) {
									*ReadBuffer++ = ' ';
									cnt++;
								}
							}
							if ( ( cnt + 1 ) < MaxCount ) {
								*ReadBuffer++ = ' ';
								cnt++;
							}
							FindChr = TRUE;
							OldStr  = FALSE;
						}
						else {
							if ( FindChr ) {
								//-----------------------
								retindexs[*ReturnDelimiterCount] = ocnt;
								retindexsz[*ReturnDelimiterCount] = cnt - ocnt;
								//-----------------------
								FindChr = FALSE;
								//-----------------------
								ocnt = cnt;
								//-----------------------
								(*ReturnDelimiterCount)++;
								OldStr  = FALSE;
								StrMode = FALSE;
							}
						}
						break;

			case  13 :	break;
			case  10 :	if ( !Comment && FindChr ) {
							//-----------------------
							retindexs[*ReturnDelimiterCount] = ocnt;
							retindexsz[*ReturnDelimiterCount] = cnt - ocnt;
							//-----------------------
							ocnt = cnt;
							//-----------------------
							(*ReturnDelimiterCount)++;
						}
						*ReadBuffer = 0;
						return TRUE;
						break;

			default  :	if ( Comment ) break;
						if ( ch == '"' ) {
							if      ( StrMode              ) {
								StrMode = FALSE;
								OldStr  = TRUE;
								if ( ( cnt + 1 ) < MaxCount ) {
									*ReadBuffer++ = ch;
									cnt++;
								}
								FindChr = TRUE;
							}
							else if ( !StrMode && !FindChr ) {
								StrMode = TRUE;
								OldStr  = FALSE;
								if ( ( cnt + 1 ) < MaxCount ) {
									*ReadBuffer++ = ch;
									cnt++;
								}
								FindChr = TRUE;
							}
							else if ( !StrMode && OldStr   ) {
								StrMode = TRUE;
								OldStr  = FALSE;
							}
						}
						else {
							OldStr = FALSE;
							if ( ( cnt + 1 ) < MaxCount ) {
								*ReadBuffer++ = ch;
								cnt++;
							}
							FindChr = TRUE;
						}
						break;
		}
	}
	if ( !Comment && FindChr ) {
		//-----------------------
		retindexs[*ReturnDelimiterCount] = ocnt;
		retindexsz[*ReturnDelimiterCount] = cnt - ocnt;
		//-----------------------
		(*ReturnDelimiterCount)++;
	}
	*ReadBuffer = 0;
	return FALSE;
}
//------------------------------------------------------------------------------------------
int _COPYFILE_EC2CSV( char *srcfile , char *trgfile ) {
	FILE *tfpt;
	FILE *tfpt2;
	HFILE shFile;

	int retindexs[256] , retindexsz[256];

	int mode;
	int  i , Step_Count = 0;
	BOOL StepEndFind;
	int  kill;
	int  write_byte;
	
	char  cc_return;
	int   ii_return;
	float ff_return;
	double dd_return;
	char  str_return[4096];
	char  name_return[4096];
	char  type_return;
	char  buf1[256];
	char  buf2[256];
	int  ferror_Sts = 0; // 2011.04.05 hhkim
	
	if ( srcfile == NULL ) return 1;
	if ( trgfile == NULL ) return 2;
	//
	shFile = _lopen( srcfile , OF_READ );
	if ( shFile == -1 ) return 3;
	//-------------------------------------------------
	// INDICATOR
	//-------------------------------------------------
	if ( !read_form_NAME_FORMAT( shFile , name_return ) ) {
		_lclose( shFile );
		//
		shFile = _lopen( srcfile , OF_READ );
		if ( shFile == -1 ) return 4;
		//
		tfpt = fopen( trgfile , "w" );
		//
		if ( tfpt == NULL ) {
			_lclose( shFile );
			return 5;
		}
		//
		StepEndFind = TRUE;
		//
		for ( ii_return = 1 ; StepEndFind ; ii_return++ ) {
			StepEndFind = _iGet_Line_String_From_File_Include_Index( shFile , str_return , 4095 , &Step_Count , retindexs , retindexsz );
//			printf("\n\nstr_return = [%s], StepEndFind = [%d], Step_Count = [%d]\n",str_return, StepEndFind, Step_Count );
//			printf("retindexs = [%d], retindexsz [%d]\n",retindexs[0]  , retindexsz[0] );
//			printf("retindexs = [%d], retindexsz [%d]\n",retindexs[1]  , retindexsz[1] );
//			printf("retindexs = [%d], retindexsz [%d]\n",retindexs[2]  , retindexsz[2] );
			//
			for ( i = 0 ; i < Step_Count ; i++ ) {
				if ( !Get_Data_From_String_with_index( str_return , name_return , retindexs[i] , retindexsz[i] ) ) {
					fprintf( tfpt , "FiILE COPY ERROR 1 in Line %d\n" , ii_return );

					fclose( tfpt );
					_lclose( shFile );
					return 6;
				}
				//
//				printf("		str_return = [%s], name_return = [%s], retindexs = [%d], retindexsz [%d]\n",str_return, name_return, retindexs[i] , retindexsz[i]);
				if ( i != 0 ) {
					fprintf( tfpt , "," );
//					printf(",");
				}
				else {
					STR_SEPERATE( name_return , buf1 , buf2 , 255 );
//					printf("		name_return = [%s], buf1 = [%s], buf2 = [%s]\n",name_return, buf1, buf2);
					//
					if ( buf2[0] != 0 ) {
						fprintf( tfpt , "%c%s%c,%c%s%c" , '"' , buf1 , '"' , '"' , buf2 , '"' );
//						printf("			f[%c%s%c,%c%s%c]" , '"' , buf1 , '"' , '"' , buf2 , '"');
						continue;
					}
					//
				}
				//
				kill = strlen( name_return );
				if ( ( kill >= 2 ) && ( name_return[0] == '"' ) && ( name_return[kill-1] == '"' ) ) {
					fprintf( tfpt , "%s" , name_return );
//					printf("		f[%s]" , name_return);
				}
				else {
					fprintf( tfpt , "%c%s%c" , '"' , name_return , '"' );
//					printf("		f[%c%s%c]\n" , '"' , name_return , '"');
				}
			}
			//
			fprintf( tfpt , "\n" );
		}
		if ( fseek(tfpt, 0, SEEK_END) != 0 ) ferror_Sts = 32;
		if ( ftell(tfpt) < 1 ) 	ferror_Sts = 32;
		//
		if ( ferror(tfpt) != 0 ) ferror_Sts = 32;
		fclose( tfpt );
		_lclose( shFile );
		if ( ferror_Sts != 0 ) return 32;
		//
		return 0;
	}
	//
	if ( !read_form_NAME_FORMAT( shFile , name_return ) ) {
		_lclose( shFile );
		return 12;
	}
	//-------------------------------------------------
	if ( !read_form_NAME_FORMAT( shFile , name_return ) ) {
		_lclose( shFile );
		return 13;
	}
	if      ( STRCMP_L( name_return , "__&&LOT_RECIPE_BEGIN&&__" ) ) {
		mode = 1;
	}
	else if ( STRCMP_L( name_return , "__&&CLUSTER_RECIPE_BEGIN&&__" ) ) {
		mode = 2;
	}
	else if ( STRCMP_L( name_return , "__&&HEADER_BEGIN&&__" ) ) {
		mode = 0;
	}
	//====================================================================
	else if ( STRCMP_L( name_return , "__&&LOT_HEADER_BEGIN&&__" ) ) {
		mode = 1;
		if ( !read_form_name_until( shFile , "__&&LOT_RECIPE_BEGIN&&__" ) ) {
			_lclose( shFile );
			return 14;
		}
	}
	else if ( STRCMP_L( name_return , "__&&CLUSTER_HEADER_BEGIN&&__" ) ) {
		mode = 2;
		if ( !read_form_name_until( shFile , "__&&CLUSTER_RECIPE_BEGIN&&__" ) ) {
			_lclose( shFile );
			return 15;
		}
	}
	//====================================================================
	else {
		_lclose( shFile );
		return 16;
	}
	//-------------------------------------------------
	//
	tfpt = fopen( trgfile , "w" );
	//
	if ( tfpt == NULL ) {
		_lclose( shFile );
		return 5;
	}
	//
	//-------------------------------------------------
	if ( mode != 0 ) {
		if ( mode == 1 ) {
			fprintf( tfpt , "%c<LOT RECIPE>%c\n" , '"' , '"' );
		}
		else {
			fprintf( tfpt , "%c<CLUSTER RECIPE>%c\n" , '"' , '"' );
		}
		//
		StepEndFind = FALSE;
		//
		kill = 0;
		//
		while( !StepEndFind ) {
			if ( !read_return_form_MaxLimit( shFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
				_lclose( shFile );
				fclose( tfpt );
				return 21;
			}
			switch ( type_return ) {
				case NAME_FORMAT :
									if ( mode == 1 ) {
										if ( STRCMP_L( name_return , "__&&LOT_RECIPE_END&&__" ) ) {
											StepEndFind = TRUE;
										}
										else {
											fprintf( tfpt , "%c%c,%cDATA(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
										}
									}
									else {
										if ( STRCMP_L( name_return , "__&&CLUSTER_RECIPE_END&&__" ) ) {
											StepEndFind = TRUE;
										}
										else {
											fprintf( tfpt , "%c%c,%cDATA(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
										}
									}
									break;
				case INT_FORMAT  :	
									fprintf( tfpt , "%c%c,%cDATA(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
									break;

				case FLOAT_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
									break;

				case DOUBLE_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
									break;

				case STRING_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
									break;

				default :			kill++;
									break;
			}
			if ( kill >= 4096 )	{
				_lclose( shFile );
				fclose( tfpt );
				return 99;
			}
		}
	}
	else {
		fprintf( tfpt , "%c<PROCESS RECIPE>%c\n" , '"' , '"' );
		//
		if ( !read_form( shFile , "NumberOfSteps" , INT_FORMAT , &cc_return , &Step_Count , &ff_return , str_return ) ) {
			_lclose( shFile );
			fclose( tfpt );
			return 15;
		}
		//
		fprintf( tfpt , "%cHEADER_DATA%c,%cNumberOfSteps%c,%c%d%c\n" , '"' , '"' , '"' , '"' , '"' , Step_Count , '"' );
		//
		kill = 0;
		//
		while ( TRUE ) {
			//
			if ( !read_return_form_MaxLimit( shFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
				_lclose( shFile );
				fclose( tfpt );
				return 15;
			}
			//
			if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&HEADER_END&&__" ) == 0 ) ) break;
			//
			switch (type_return) {
			case NAME_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
								break;
			case INT_FORMAT  :	
								fprintf( tfpt , "%c%c,%cDATA(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
								break;
			case FLOAT_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
								break;
			case DOUBLE_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
								break;
			case STRING_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;
			case FILENAME_FORMAT:
								fprintf( tfpt , "%c%c,%ctFILE%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;
			default :			kill++;
								break;
			}
		}
		//
		fprintf( tfpt , "%cHEADER_END_ID%c\n" , '"' , '"' );
		//
		if ( !read_form( shFile , "__&&STEP0_BEGIN&&__" , NAME_FORMAT , &cc_return , &ii_return , &ff_return , str_return ) ) {
			_lclose( shFile );
			fclose( tfpt );
			return 17;
		}
		//
		fprintf( tfpt , "%cSTEP0_BEGIN_ID%c,%c__&&STEP0_BEGIN&&__%c\n" , '"' , '"' , '"' , '"' );
		//
		kill = 0;
		//
		StepEndFind = FALSE;
		while( !StepEndFind ) {
			if ( !read_return_form_MaxLimit( shFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
				_lclose( shFile );
				fclose( tfpt );
				return 18;
			}
			switch (type_return) {
			case NAME_FORMAT :	if ( strcmp( name_return , "__&&STEP0_END&&__" ) == 0 ) {
									fprintf( tfpt , "STEP0_END_ID = __&&STEP0_END&&__\n" );
									StepEndFind = TRUE;
								}
								else {
									fprintf( tfpt , "%c%c,%cDATA(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
								}
								break;
			case NAME_X_FORMAT :	
								fprintf( tfpt , "%c%c,%cDATA-X(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
								break;
			case INT_FORMAT  :	
								fprintf( tfpt , "%c%c,%cDATA(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
								break;
			case INT_X_FORMAT  :	
								fprintf( tfpt , "%c%c,%cDATA-X(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
								break;
			case FLOAT_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
								break;
			case FLOAT_X_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA-X(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
								break;
			case DOUBLE_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
								break;
			case DOUBLE_X_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA-X(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
								break;
			case STRING_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;
			case STRING_X_FORMAT :
								fprintf( tfpt , "%c%c,%cDATA-X(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;
			case FILENAME_FORMAT:
								fprintf( tfpt , "%c%c,%cFILE(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;
			case FILENAME_X_FORMAT:
								fprintf( tfpt , "%c%c,%cFILE-X(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
								break;

			default :			kill++;
								break;
			}
			if ( kill >= 4096 )	{
				_lclose( shFile );
				fclose( tfpt );
				return 99;
			}
		}

		for ( i = 0 ; ( i < Step_Count ) && ( i < 999 ) ; i++ ) {
			if ( !read_form( shFile , "__&&STEP_BEGIN&&__"   , NAME_FORMAT   , &cc_return , &ii_return , &ff_return , str_return        ) ) {
				_lclose( shFile );
				fclose( tfpt );
				return 19;
			}
			fprintf( tfpt , "%cSTEP_BEGIN_ID%c,%c__&&STEP_BEGIN&&__%c,%c%d%c\n" , '"' , '"' , '"' , '"' , '"' , i + 1 , '"' );
			//
			StepEndFind = FALSE;
			while( !StepEndFind ) {
				if ( !read_return_form_MaxLimit( shFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) { // 2008.05.30
					_lclose( shFile );
					fclose( tfpt );
					return 20;
				}
				switch (type_return) {
				case NAME_FORMAT :	if ( strcmp( name_return , "__&&STEP_END&&__" ) == 0 ) {
										fprintf( tfpt , "STEP[%d]_END_ID = __&&STEP_END&&__\n" , i + 1 );
										StepEndFind = TRUE;
									}
									else {
										fprintf( tfpt , "%c%c,%cDATA(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
									}
									break;
				case NAME_X_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA-X(n)%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' );
									break;
				case INT_FORMAT  :	
									fprintf( tfpt , "%c%c,%cDATA(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
									break;
				case INT_X_FORMAT  :	
									fprintf( tfpt , "%c%c,%cDATA-X(i)%c,%c%s%c,%c%d%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ii_return , '"' );
									break;
				case FLOAT_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
									break;
				case FLOAT_X_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA-X(f)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , ff_return , '"' );
									break;
				case DOUBLE_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
									break;
				case DOUBLE_X_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA-X(d)%c,%c%s%c,%c%f%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , dd_return , '"' );
									break;
				case STRING_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
									break;
				case STRING_X_FORMAT :
									fprintf( tfpt , "%c%c,%cDATA-X(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
									break;
				case FILENAME_FORMAT:
									fprintf( tfpt , "%c%c,%cFILE(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
									break;
				case FILENAME_X_FORMAT:
									fprintf( tfpt , "%c%c,%cFILE-X(s)%c,%c%s%c,%c%s%c\n"  , '"' , '"' , '"' , '"' , '"' , name_return , '"' , '"' , str_return , '"' );
									break;

				default :			kill++;
									break;
				}
				if ( kill >= 4096 )	{
					_lclose( shFile );
					fclose( tfpt );
					return 100;
				}
			}
		}
	}
	fprintf( tfpt , "\n" );
	//
	if ( fseek(tfpt, 0, SEEK_END) != 0 ) ferror_Sts = 32;
	if ( ftell(tfpt) < 1 ) 	ferror_Sts = 32;
	//
	if ( ferror(tfpt) != 0 ) ferror_Sts = 32;
	_lclose( shFile );
	fclose( tfpt );
	if ( ferror_Sts != 0 ) return 32;
	//
	return 0;
}
//
//

