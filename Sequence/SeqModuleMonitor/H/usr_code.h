#ifndef __USERCODE__
#define __USERCODE__

BOOL Check_Carrier_Placed(int* CommStatus , int i);
BOOL Check_Push_LoadButton(int* CommStatus , int CMNo);
BOOL Check_Push_UnloadButton(int* CommStatus , int CMNo);
void Lamp_Status_Control(int iMode, int iLampSts, int index );
void SignalTowerControl( int  AlarmLamp , int GreenLamp , int EndLamp , int YellowLamp , int BlueLamp );

int	Get_Max_PM_Number( void );
int Get_Max_FM_Number( void );
int Get_Max_TM_Number( void );
int	Get_Max_BM_Number( void );
int	Get_Max_CM_Number( void );

BOOL Get_TM_Use( int i );
BOOL Get_PM_Use( int i );
BOOL Get_FM_Use( int i );
BOOL Get_BM_Use( int i );

#endif