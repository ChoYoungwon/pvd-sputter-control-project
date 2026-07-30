#ifndef KUTLCHECK__H
#define KUTLCHECK__H

//==========================================================================================================================================================

int Get_Stable_Table( char *arg , int Type , int TypeSub , int TableID , double *StableTime , double *StableRange );

int Get_Level_Table( char *arg , int Type , int TypeSub , int TableID , int *SensorIO , double *LevelTime , double *LevelRange );

int Get_Alarm_Table( char *arg , int Type , int TypeSub , int TableID , double *DelayTime , double *AlarmTime , double *AlarmRange , double *WarnTime , double *WarnRange );

int Get_PID_Table( char *arg , int TableID , double SetData , double *seltemp , double *Band , double *Integ , double *Deriv );

int Check_Level_Data( int ASensor , double LevelTime , double LevelRange , int ResetCount );

int Check_Stable_Data( int ASensor , double Setdata , double StableTime , double StableRange , int ResetCount );

int Check_Alarm_Data( int ASensor , double Setdata , double DelayTime , double AlarmTime , double AlarmRange , int AlarmID_Up , int AlarmID_Down , double WarnTime , double WarnRange , int WarnID_Up , int WarnID_Down , int errorabort , int ResetCount );

//==========================================================================================================================================================

void ARGUE_to_REPLACE0_and_ALARM( int *alarmstart );
BOOL IO_CHECKING( int setio , int snsio , int data , double timesec , int alarm );
BOOL IO_CHECKING2( int msg , int Auto , int setio , int snsio , int data , double timesec , int alarm );

//==========================================================================================================================================================

void _iLOG_fPRINTF( LPSTR list , ... );
void _iLOG_PRINTF( LPSTR list , ... );
void _iLOG_USER_PRINTF( int display , LPSTR list , ... );
void _iLOG_USER_PRINTF_NA( int Display , LPSTR list , ... );


void _iLOG_MON_START( int depth );
void _iLOG_MON_END( int depth );

void _iLOG_ON();
void _iLOG_OFF();
int  _iLOG_RUN();

int  _iREAD_DIGITAL( int io , int *CommStatus );
void _iWRITE_DIGITAL( int io , int data , int *CommStatus );
double _iREAD_ANALOG( int io , int *CommStatus );
void _iWRITE_ANALOG( int io , double data , int *CommStatus );
void _iREAD_STRING( int io , char *data , int *CommStatus );
void _iWRITE_STRING( int io , char *data , int *CommStatus );

int  _iALARM_MANAGE( int AlarmID );
void _iALARM_POST( int AlarmID );

int  _iWAIT_SECONDS( double data );

void _iRUN_FUNCTION_ABORT( int io );
int  _iREAD_FUNCTION( int io );
int  _iRUN_FUNCTION( int io , char *data );
int  _iRUN_FUNCTION_FREE( int io , char *data );
int  _iRUN_SET_FUNCTION( int io , char *data );
void _iWRITE_FUNCTION_EVENT( int io , char *data );

void _iLOG_CHECK_ALARM( int mode , int realio , int targetio , char *targetstr , int timeoutio , int alarm );

//-------------------------------------
// Function Interlock Check Utility
//-------------------------------------
// 2010.03.15 - New
int IO_SETPOINT_BOTH_AND_CHECK( int io1, int Value1, int io2, int Value2, int Alarm );
int IO_SETPOINT_CHECK( int io, int Value, int Alarm );
int IO_SETPOINT_CHECK_FALSE( int io, int Value, int Alarm );





// 2010.02.28 - old
int IO_SETPOINT_TRUEVALUE_CHECK( int io , int Value , int Alarm );
int IO_SETPOINT_FALSEVALUE_CHECK( int io , int Value , int Alarm ); 

int IO_SETPOINT_TRUEVALUE_CHECK_ALWAYS( int io , int Value , int Alarm );
int IO_SETPOINT_FALSEVALUE_CHECK_ALWAYS( int io , int Value , int Alarm ); 
//-------------------------------------
//

int _COPYFILE_EC2CSV( char *srcfile , char *trgfile );

//==========================================================================================================================================================

#endif
