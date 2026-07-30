#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
#include <math.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
#include "C_Module.h"
#include "iodefine.h"
#include "Parameter.h"

extern int	gnALARM_START_INDEX;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void ALARM_MESSAGE_REMAPPING( int id, char *HWErrorNo ) 
{
	char Buffer[256] = "";
	char Buffer2[256] = "";
	
	ALARM_MESSAGE_GET( id , Buffer );
	sprintf( Buffer2 , "%s[%s]" , Buffer, HWErrorNo );
	ALARM_MESSAGE_SET( id , Buffer2 );
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status HW_AlarmClear( void ) 
{
	int	AlmResult, CommStatus;
	
	while( TRUE ) {
		WRITE_DIGITAL( AlarmReset, RUN, &CommStatus );
		if ( !CommStatus ) {
			AlmResult = ALARM_MANAGE( ALARM_PORT_ALARM_CLEAR_FAIL );
			if		( AlmResult == ALM_RETRY )	continue;
			else if ( AlmResult == ALM_IGNORE )	break;
			else return SYS_ABORTED;
		}
		else break;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int HW_Alarm_Notify( void ) 
{
	int AlmNo;
	int CommStatus;
	char ErrMsg[256] = "";
	char ErrNo[256] = "";
	
	
	READ_STRING( LastErrorNo, ErrNo , &CommStatus );
	
	if ( strlen( ErrNo ) > 0 ) {
		AlmNo = ALARM_HW_ERROR_NOTIFY;
		strcat( ErrNo, ":" );
		READ_STRING( LastErrorNo, ErrMsg , &CommStatus );
		strcat( ErrNo, ErrMsg );
		
		ALARM_MESSAGE_REMAPPING( AlmNo, ErrNo );
		if ( ALARM_STATUS( AlmNo ) != ALM_PAUSED ) ALARM_POST( AlmNo );
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int READ_Digital_Trigger( int IO, int *OldValue, int *ChangeFlag )
{
	int CommStatus;
	int CurrValue;
	
	CurrValue = READ_DIGITAL( IO, &CommStatus );
	
	if ( *OldValue == -1 || *OldValue != CurrValue ) {
		*ChangeFlag = TRUE;
	}
	else {
		*ChangeFlag = FALSE;
	}
	*OldValue = CurrValue;
	return CurrValue;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LoadUnloadLampControl( LPLAMPCON SetValue )
{
	int CommStatus;
	
	if ( SetValue == LOADON ) {
		WRITE_DIGITAL( LOAD_LAMP, LAMPON , &CommStatus );
		WRITE_DIGITAL( UNLOAD_LAMP, LAMPOFF , &CommStatus );
	}
	else {
		WRITE_DIGITAL( LOAD_LAMP, LAMPOFF , &CommStatus );
		WRITE_DIGITAL( UNLOAD_LAMP, LAMPON , &CommStatus );
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status ReadyStatusCheck( void )
{
	int AlmResult;
	int CommStatus;
	int Count = 0;
	
	while ( TRUE ) 	{	
		if ( READ_DIGITAL( Ready_Status , &CommStatus ) == RunREADY ) break;
		else { 
			if ( Count++ > 3 ) { 
				AlmResult = ALARM_MANAGE( ALARM_PORT_RUNNING );
				
				if      ( AlmResult == ALM_IGNORE ) break;
				else if	( AlmResult == ALM_RETRY  ) Count = 0; 
				else return SYS_ABORTED;
			}
		}
		
		if ( !WAIT_SECONDS( 1 ) ) return SYS_ABORTED; 
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status HomeStatusCheck( void )
{
	int HomeStatus;
	int CommStatus;
	
	HomeStatus = READ_DIGITAL( Home_Status, &CommStatus );
	
	if( HomeStatus == NHome ) {
		if ( ALARM_STATUS( ALARM_NOT_HOME ) != ALM_PAUSED ) {
			ALARM_POST( ALARM_NOT_HOME );
			return SYS_ABORTED;
		}
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Mapping_Data_Clear( void ) 
{
	int		i;
	int		CommStatus;


	for ( i = 0 ; i < MAX_WAFER_SLOT ; i++ ) {
		WRITE_DIGITAL( C01_Wafer + i , WUnknown, &CommStatus );
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int CheckFailureWaferInFOUP( void )
{
	int i;
	int Wafer_Sts;
	int CommStatus;
	int AlmResult;
	
	while( TRUE )
	{
		for( i = 0 ; i < MAX_WAFER_SLOT ; i++ ) {
			Wafer_Sts = READ_DIGITAL( C01_Wafer+i , &CommStatus );
			
			if ( Wafer_Sts == WFailure ) { 
				// Alarm Name 임시로 정의함. 추가 수정 필요함.
				AlmResult = ALARM_MANAGE( ALARM_PORT_MAPDATA_COMP_ERROR );
				if      ( AlmResult == ALM_IGNORE ) return FALSE;
				else if ( AlmResult == ALM_RETRY  ) continue;
				else return TRUE;
			}
		}
		if ( i == MAX_WAFER_SLOT ) return FALSE;		
	}
	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status WRITE_DIGITAL_TRUE( int io, int Data ) 
{
	int AlmResult, CommStatus;
	
	while( TRUE )
	{
		WRITE_DIGITAL( io, Data, &CommStatus );
		
		if ( CommStatus ) break;
		
		if ( HW_Alarm_Notify() ) return SYS_ABORTED; // HW Error가 있으면 HW Error Post하고 빠져 나간다.
		
		AlmResult = ALARM_MANAGE( ALARM_WRITE_COMM_ERROR );
		
		if      ( AlmResult == ALM_IGNORE ) break;
		else if ( AlmResult == ALM_RETRY )	return SYS_ABORTED;

		if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status READ_DIGITAL_TRUE( int io, int *Data ) 
{
	int AlmResult, CommStatus;
	
	while( TRUE )
	{
		*Data = READ_DIGITAL( io, &CommStatus );
		
		if ( CommStatus ) break;
		
		if ( HW_Alarm_Notify() ) return SYS_ABORTED; // HW Error가 있으면 HW Error Post하고 빠져 나간다.
		
		AlmResult = ALARM_MANAGE( ALARM_READ_COMM_ERROR );
		
		if      ( AlmResult == ALM_IGNORE ) break;
		else if ( AlmResult != ALM_RETRY )	return SYS_ABORTED;

		if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
