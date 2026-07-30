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
#include "../Library/kutlCheck.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
AMHSMODE AMHS = AMHS_Manual;
int gnAMHS_OLD = AMHS_Manual;
int gnLPResv_OLD;

int		gnALARM_START_INDEX	= 0;
int		gnCASSETTE_INDEX	= 0;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

char *gszEVENT_STRING_MOVINGIN			= "EVENT_ON FOUP_MOVING PORT%d|1";
char *gszEVENT_STRING_MOVINGOUT			= "EVENT_ON FOUP_MOVING PORT%d|2";

char *gszEVENT_STRING_DOCKING			= "EVENT_ON LL_DOCKING_COMPLETE PORT%d";
char *gszEVENT_STRING_UNDOCKING			= "EVENT_ON LL_UNDOCKING_COMPLETE PORT%d";

char *gszEVENT_STRING_DOOR_OPEN			= "EVENT_ON LL_OPEN_COMPLETE PORT%d";
char *gszEVENT_STRING_DOOR_CLOSE		= "EVENT_ON LL_CLOSE_COMPLETE PORT%d";

char *gszEVENT_UNLOAD_REQUEST			= "EVENT_ON UNLOAD_REQUEST_F PORT%d";

char *gszEVENT_STRING_MAPREQ_IN			= "EVENT_ON MAPPING_F PORT%d|0";				// After Door Opened, Mapping
char *gszEVENT_STRING_MAPCOMPLETE_IN	= "EVENT_ON MAPPING_COMPLETE_F PORT%d|0";		// After Door Opened, Mapping
char *gszEVENT_STRING_AT_SOURCE_IN		= "EVENT_ON STS_ATSOURCE_F PORT%d|0";
char *gszEVENT_STRING_AT_OCCUPY_IN		= "EVENT_ON STS_OCCUPIED_F PORT%d|0";

char *gszEVENT_STRING_MAPREQ_MAN		= "EVENT_ON MAPPING_F PORT%d|1";				// Before Process, Mapping
char *gszEVENT_STRING_MAPCOMPLETE_MAN	= "EVENT_ON MAPPING_COMPLETE_F PORT%d|1";		// Before Process, Mapping
char *gszEVENT_STRING_AT_SOURCE_MAN		= "EVENT_ON STS_ATSOURCE_F PORT%d|1";
char *gszEVENT_STRING_AT_OCCUPY_MAN		= "EVENT_ON STS_OCCUPIED_F PORT%d|1";

char *gszEVENT_STRING_MAPREQ_OUT		= "EVENT_ON MAPPING_F PORT%d|2";				// After Process, Mapping
char *gszEVENT_STRING_MAPCOMPLETE_OUT	= "EVENT_ON MAPPING_COMPLETE_F PORT%d|2";		// After Process, Mapping

char *gszEVENT_STRING_RFID_NOUSE		= "EVENT_ON RF_NOT_USE_MODE PORT%d";
char *gszEVENT_STRING_RFATREAD_COMPT	= "EVENT_ON MID_AUTO_READ_COMPLETE PORT%d";
char *gszEVENT_STRING_RFATREAD_FAIL		= "EVENT_ON MID_AUTO_READ_FAIL PORT%d";
char *gszEVENT_STRING_RFMIDREAD_COMPT	= "EVENT_ON MID_READ_COMPLETE PORT%d";
char *gszEVENT_STRING_RFMIDREAD_FAIL	= "EVENT_ON MID_READ_FAIL PORT%d";
char *gszEVENT_STRING_RFDATAREAD_COMPT	= "EVENT_ON RFDATA_READ_COMPLETE PORT%d";
char *gszEVENT_STRING_RFDATAREAD_FAIL	= "EVENT_ON RFDATA_READ_FAIL PORT%d";
char *gszEVENT_STRING_RFMIDWRITE_COMPT	= "EVENT_ON MID_WRITE_COMPLETE PORT%d";
char *gszEVENT_STRING_RFMIDWRITE_FAIL	= "EVENT_ON MID_WRITE_FAIL PORT%d";
char *gszEVENT_STRING_RFDATAWRITE_COMPT	= "EVENT_ON RFDATA_WRITE_COMPLETE PORT%d";
char *gszEVENT_STRING_RFDATAWRITE_FAIL	= "EVENT_ON RFDATA_WRITE_FAIL PORT%d";
char *gszEVENT_CARR_ASSOCIATED			= "EVENT_ON CARR_ASSOCIATED_F PORT%d";
char *gszEVENT_TRANSFER_STATE			= "EVENT_ON PORT_TRANSFER_STATE CM%d|%d";
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = 
{
	{ "IO_Driver_Loding"				, _K_D_IO	,	IO_Driver_Loding				,	0	} ,

	{ "e:SCHEDULER"						, _K_S_IO	,	SCHEDULER						,	0	} ,
	
	{ "CTC.FA_STATUS_AGV_$0"			, _K_D_IO	,	AMHS_STATUS						,	0	} ,   // Manual, Auto
	{ "CTC.MAIN_CONTROL$1"				, _K_D_IO	,	MAIN_CONTROL					,	0	} ,
	{ "CTC.MAIN_STATUS$1"				, _K_D_IO	,	MAIN_STATUS						,	0	} ,
	{ "GEM.$0_LPReserveStatus"			, _K_D_IO	,	RESERVE_STATUS					,	0	} ,
	{ "$0.PORT_SERVICE"					, _K_D_IO	,	PORT_SERVICE					,	0	} ,	
	
	{ "siLP_$0_MappingData"				, _K_S_IO	,	MappingData						,	0	} ,
	{ "siLP_$0_LastErrorMessage"		, _K_S_IO	,	LastErrorMessage				,	0	} ,	
	{ "siLP_$0_LastErrorNo"				, _K_S_IO	,	LastErrorNo						,	0	} ,	

	{ "diLP_$0_Comm_Status"				, _K_D_IO	,	Comm_Status						,	0	} ,
	{ "diLP_$0_Ready_Status"			, _K_D_IO	,	Ready_Status					,	0	} ,
	{ "diLP_$0_Home_Status"				, _K_D_IO	,	Home_Status						,	0	} ,
	{ "diLP_$0_Shuttle_Status"			, _K_D_IO	,	Shuttle_Status					,	0	} ,
	{ "diLP_$0_Present"					, _K_D_IO	,	Present							,	0	} ,
	{ "diLP_$0_Placed"					, _K_D_IO	,	Placed							,	0	} ,	
	{ "diLP_$0_Door_Status"				, _K_D_IO	,	Door_Status						,	0	} ,	
	{ "diLP_$0_Clamp_Status"			, _K_D_IO	,	Clamp_Status					,	0	} ,
	{ "diLP_$0_Run_Status"				, _K_D_IO	,	Run_Status						,	0	} ,

	{ "diLP_$0_Wfr01"					, _K_D_IO	,	WFR_INF01						,	0	} ,
	{ "diLP_$0_Wfr02"					, _K_D_IO	,	WFR_INF02						,	0	} ,
	{ "diLP_$0_Wfr03"					, _K_D_IO	,	WFR_INF03						,	0	} ,
	{ "diLP_$0_Wfr04"					, _K_D_IO	,	WFR_INF04						,	0	} ,
	{ "diLP_$0_Wfr05"					, _K_D_IO	,	WFR_INF05						,	0	} ,
	{ "diLP_$0_Wfr06"					, _K_D_IO	,	WFR_INF06						,	0	} ,
	{ "diLP_$0_Wfr07"					, _K_D_IO	,	WFR_INF07						,	0	} ,
	{ "diLP_$0_Wfr08"					, _K_D_IO	,	WFR_INF08						,	0	} ,
	{ "diLP_$0_Wfr09"					, _K_D_IO	,	WFR_INF09						,	0	} ,
	{ "diLP_$0_Wfr10"					, _K_D_IO	,	WFR_INF10						,	0	} ,
	{ "diLP_$0_Wfr11"					, _K_D_IO	,	WFR_INF11						,	0	} ,
	{ "diLP_$0_Wfr12"					, _K_D_IO	,	WFR_INF12						,	0	} ,
	{ "diLP_$0_Wfr13"					, _K_D_IO	,	WFR_INF13						,	0	} ,
	{ "diLP_$0_Wfr14"					, _K_D_IO	,	WFR_INF14						,	0	} ,
	{ "diLP_$0_Wfr15"					, _K_D_IO	,	WFR_INF15						,	0	} ,
	{ "diLP_$0_Wfr16"					, _K_D_IO	,	WFR_INF16						,	0	} ,
	{ "diLP_$0_Wfr17"					, _K_D_IO	,	WFR_INF17						,	0	} ,
	{ "diLP_$0_Wfr18"					, _K_D_IO	,	WFR_INF18						,	0	} ,
	{ "diLP_$0_Wfr19"					, _K_D_IO	,	WFR_INF19						,	0	} ,
	{ "diLP_$0_Wfr20"					, _K_D_IO	,	WFR_INF20						,	0	} ,
	{ "diLP_$0_Wfr21"					, _K_D_IO	,	WFR_INF21						,	0	} ,
	{ "diLP_$0_Wfr22"					, _K_D_IO	,	WFR_INF22						,	0	} ,
	{ "diLP_$0_Wfr23"					, _K_D_IO	,	WFR_INF23						,	0	} ,
	{ "diLP_$0_Wfr24"					, _K_D_IO	,	WFR_INF24						,	0	} ,
	{ "diLP_$0_Wfr25"					, _K_D_IO	,	WFR_INF25						,	0	} ,
	{ "diLP_$0_Wfr26"					, _K_D_IO	,	WFR_INF26						,	0	} ,
	
	{ "doLP_$0_Docking"					, _K_D_IO	,	Docking							,	0	} ,
	{ "doLP_$0_AlarmReset"				, _K_D_IO	,	AlarmReset						,	0	} ,
	{ "doLP_$0_Homing"					, _K_D_IO	,	Homing							,	0	} ,
	{ "doLP_$0_Load"					, _K_D_IO	,	Load							,	0	} ,
	{ "doLP_$0_Unload"					, _K_D_IO	,	Unload							,	0	} ,
	{ "doLP_$0_Clamp"					, _K_D_IO	,	Clamp							,	0	} ,
	{ "doLP_$0_UnClamp"					, _K_D_IO	,	UnClamp							,	0	} ,
	{ "doLP_$0_Door_Ctrl"				, _K_D_IO	,	Door_Ctrl						,	0	} ,
	
	{ "doLP_$0_Scan_Down"				, _K_D_IO	,	Scan_Down						,	0	} , 
	{ "doLP_$0_TOPEN"					, _K_D_IO	,	TOPEN							,	0	} ,
	{ "doLP_$0_TCLOSE"					, _K_D_IO	,	TCLOSE							,	0	} ,
	{ "doLP_$0_UPMappingEnable"			, _K_D_IO	,	UPMappingEnable					,	0	} ,
	{ "doLP_$0_Auto_Lamp"				, _K_D_IO	,	AUTO_LAMP						,	0	} ,
	{ "doLP_$0_Manual_Lamp"				, _K_D_IO	,	MANUAL_LAMP						,	0	} ,
	{ "doLP_$0_Load_Lamp"				, _K_D_IO	,	LOAD_LAMP						,	0	} ,
	{ "doLP_$0_Unload_Lamp"				, _K_D_IO	,	UNLOAD_LAMP						,	0	} ,
	{ "doLP_$0_Reserve_Lamp"			, _K_D_IO	,	RESERVE_LAMP					,	0	} ,
	{ "dioLP_$0_OpMode"					, _K_D_IO	,	OpMode							,	0	} ,
	{ "doLP_$0_LastAlarmFind"			, _K_D_IO	,	LastAlarmFind					,	0	} ,	

	{ "$0.C01_Wafer"					, _K_D_IO	,	C01_Wafer						,	0	} ,
	{ "$0.C02_Wafer"					, _K_D_IO	,	C02_Wafer						,	0	} ,
	{ "$0.C03_Wafer"					, _K_D_IO	,	C03_Wafer						,	0	} ,
	{ "$0.C04_Wafer"					, _K_D_IO	,	C04_Wafer						,	0	} ,
	{ "$0.C05_Wafer"					, _K_D_IO	,	C05_Wafer						,	0	} ,
	{ "$0.C06_Wafer"					, _K_D_IO	,	C06_Wafer						,	0	} ,
	{ "$0.C07_Wafer"					, _K_D_IO	,	C07_Wafer						,	0	} ,
	{ "$0.C08_Wafer"					, _K_D_IO	,	C08_Wafer						,	0	} ,
	{ "$0.C09_Wafer"					, _K_D_IO	,	C09_Wafer						,	0	} ,
	{ "$0.C10_Wafer"					, _K_D_IO	,	C10_Wafer						,	0	} ,
	{ "$0.C11_Wafer"					, _K_D_IO	,	C11_Wafer						,	0	} ,
	{ "$0.C12_Wafer"					, _K_D_IO	,	C12_Wafer						,	0	} ,
	{ "$0.C13_Wafer"					, _K_D_IO	,	C13_Wafer						,	0	} ,
	{ "$0.C14_Wafer"					, _K_D_IO	,	C14_Wafer						,	0	} ,
	{ "$0.C15_Wafer"					, _K_D_IO	,	C15_Wafer						,	0	} ,
	{ "$0.C16_Wafer"					, _K_D_IO	,	C16_Wafer						,	0	} ,
	{ "$0.C17_Wafer"					, _K_D_IO	,	C17_Wafer						,	0	} ,
	{ "$0.C18_Wafer"					, _K_D_IO	,	C18_Wafer						,	0	} ,
	{ "$0.C19_Wafer"					, _K_D_IO	,	C19_Wafer						,	0	} ,
	{ "$0.C20_Wafer"					, _K_D_IO	,	C20_Wafer						,	0	} ,
	{ "$0.C21_Wafer"					, _K_D_IO	,	C21_Wafer						,	0	} ,
	{ "$0.C22_Wafer"					, _K_D_IO	,	C22_Wafer						,	0	} ,
	{ "$0.C23_Wafer"					, _K_D_IO	,	C23_Wafer						,	0	} ,
	{ "$0.C24_Wafer"					, _K_D_IO	,	C24_Wafer						,	0	} ,
	{ "$0.C25_Wafer"					, _K_D_IO	,	C25_Wafer						,	0	} ,
	{ "$0.C26_Wafer"					, _K_D_IO	,	C26_Wafer						,	0	} ,
	{ "$0.MAPWAFER_COUNT"				, _K_D_IO	,	MAPWAFER_COUNT					,	0	} ,
	{ "$0.MAPWAFER_INFO"				, _K_S_IO	,	MAPWAFER_INFO					,	0	} ,

	{ "dpLP_$0_Control"					, _K_D_IO	,	LP_Control						,	0	} ,
	{ "dpLP_$0_Unload_Clamp_Mode"		, _K_D_IO	,	Unload_Clamp_Mode				,	0	} ,	// Unclamp, Clamp 
	{ "dpLP_$0_Unload_UpMapping"		, _K_D_IO	,	Unload_Upmapping				,	0	} ,	
	{ "dpLP_$0_Move_Timeout"			, _K_D_IO	,	Move_Timeout					,	0	} ,

	""
};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LPRUNRESULT WaitRunComplete( int AlarmNo, int EventFlag, CONTROLMODE Mode, int AutoMode )
{
	int nCommStatus;
	int nLPReadyStatus;
	char szBuffer[256] = "";
	double dbTimeout;
	AlarmStyle AlmResult;

	dbTimeout = (double)READ_DIGITAL( Move_Timeout , &nCommStatus );
	
	S_TIMER_READY();
	
	while( TRUE ) {
		if ( !WAIT_SECONDS( 0.5 ) ) return SYS_ABORTED; 
		
		if ( READ_DIGITAL_TRUE( Ready_Status , &nLPReadyStatus ) == SYS_ABORTED ) return SYS_ABORTED;
				
		if ( nLPReadyStatus == RunREADY ) {
			if ( !WAIT_SECONDS( 0.5 ) ) return SYS_ABORTED; // Driver 내에서 IO Status Refresh Time 확보
			break;
		}
		else if ( nLPReadyStatus == RunERROR ) {
			HW_Alarm_Notify();  
			return LPRUN_ABORT;	 
		}
			
		if ( S_TIMER_CHECK( 1, dbTimeout ) ) 
		{
			AlmResult = ALARM_MANAGE( AlarmNo );
			if ( AlmResult == ALM_RETRY  ) return LPRUN_RETRY;
			else return LPRUN_ABORT;
		}
	}

	if ( EventFlag )
	{
		if ( Mode == LoadMode )
		{
			if ( READ_DIGITAL( Shuttle_Status , &nCommStatus ) == DOCKED ) {
				sprintf( szBuffer, gszEVENT_STRING_DOCKING, gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}	
			
			if ( READ_DIGITAL( Door_Status , &nCommStatus ) == OPENED ) {
				sprintf( szBuffer, gszEVENT_STRING_DOOR_OPEN, gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}

			if ( AutoMode == 0 ) {
				sprintf( szBuffer, gszEVENT_STRING_MAPREQ_IN, gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}
			else {
				sprintf( szBuffer, gszEVENT_STRING_MAPREQ_IN, gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}
		}
		else
		{
			if ( READ_DIGITAL( Door_Status , &nCommStatus ) == CLOSED ) {
				sprintf( szBuffer, gszEVENT_STRING_DOOR_CLOSE, gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}

			if ( READ_DIGITAL( Shuttle_Status , &nCommStatus ) == UNDOCKED ) {
				sprintf( szBuffer , gszEVENT_STRING_UNDOCKING , gnCASSETTE_INDEX );
				WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
			}
		}
	}

	return LPRUN_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status RemappingControl( void )
{
	LPRUNRESULT LPResult;
	
	while( TRUE ) {
		if ( WRITE_DIGITAL_TRUE( Scan_Down, RUN ) == SYS_ABORTED ) return SYS_ABORTED; 
		
		LPResult = WaitRunComplete ( ALARM_PORT_MAPPING_TIMEOUT, FALSE, InitMode, 0 );
		if		( LPResult == LPRUN_RETRY ) continue;
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;
		break;
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status GetMappingData( int Mode ) 
{
	int	i;
	int	nData;
	int	nCommStatus;

	int	nCTCMappingIO	=	C01_Wafer;
	int AlmResult;

	char szBuffer[256];

	int	nCount = 0;
	int	nD_Wfr = 0;
	int nC_Wfr = 0;
	int nU_Wfr = 0;
	int nSim_Wfr = 0;
	

	while( TRUE )
	{
		nCount = 0;
		nD_Wfr = 0;
		nC_Wfr = 0;
		nU_Wfr = 0;
		sprintf( szBuffer, "" );
		
		for ( i = 0 ; i < MAX_WAFER_SLOT ; i++ ) 
		{
			nData = READ_DIGITAL( WFR_INF01+i, &nCommStatus );

			if ( nData > MAPAbsent && nData < MAPUnknown ) nCount++;
			
			switch( nData ) 
			{
			case MAPAbsent  :
				strcat( szBuffer , "1" );
				WRITE_DIGITAL( nCTCMappingIO + i, WAbsent,  &nCommStatus );
				break;
			case MAPPresent : 
				strcat( szBuffer , "3" );
				WRITE_DIGITAL( nCTCMappingIO + i, WPresent, &nCommStatus );	
				break;
			case MAPDouble : 
				nD_Wfr++;
				strcat( szBuffer , "4" );
				WRITE_DIGITAL( nCTCMappingIO + i, WDOUBLE,  &nCommStatus ); 
				break;
			case MAPCross   :
				nC_Wfr++;
				strcat( szBuffer , "5" );
				WRITE_DIGITAL( nCTCMappingIO + i, WCROSS,   &nCommStatus ); 
				break;
			default  :			
				nU_Wfr++;
				strcat( szBuffer , "0" );
				WRITE_DIGITAL( nCTCMappingIO + i, WUnknown, &nCommStatus ); 
				break;
			}
			
			WRITE_DIGITAL( MAPWAFER_COUNT , nCount , &nCommStatus );
			WRITE_STRING( MAPWAFER_INFO , szBuffer , &nCommStatus );
		}

		//------------- Abnormal Wafer Check	
		if ( nC_Wfr > 0 ) { 
			AlmResult = ALARM_MANAGE( ALARM_CROSS_WAFER );
			if ( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult == ALM_ABORT ) return SYS_ABORTED;
			else if ( AlmResult == ALM_RETRY ) {
				if ( RemappingControl() == SYS_ABORTED ) return SYS_ABORTED; 
				continue;
			}
		}
		
		if ( nD_Wfr > 0 ) { 
			AlmResult = ALARM_MANAGE( ALARM_DOUBLE_WAFER );
			if ( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult == ALM_ABORT ) return SYS_ABORTED;
			else if ( AlmResult == ALM_RETRY ) {
				if (  RemappingControl() == SYS_ABORTED ) return SYS_ABORTED; 
				continue;
			}
		}
		
		if ( nU_Wfr > 0 ) { 
			AlmResult = ALARM_MANAGE( ALARM_UNKNOWN_WAFER );
			if ( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult == ALM_ABORT ) return SYS_ABORTED;
			else if ( AlmResult == ALM_RETRY ) {
				if (  RemappingControl() == SYS_ABORTED ) return SYS_ABORTED; 
				continue;
			}
		}
		break;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL FOUP_ExistanceCheck_Only( BOOL* CassExist ) 
{
	int nData1, nData2;
	int nCount = 0;
	
	*CassExist = FALSE;

	while( TRUE ) {
		if ( READ_DIGITAL_TRUE( Present, &nData1 ) == SYS_ABORTED ) return SYS_ABORTED;
		if ( READ_DIGITAL_TRUE( Placed , &nData2 ) == SYS_ABORTED ) return SYS_ABORTED;

		if ( nData1 == ON && nData2 == ON ) {
			*CassExist = TRUE;
			break;
		}
		if  ( nCount++ > 3 ) {
			*CassExist = FALSE;
			break;
		}
		
		if ( !WAIT_SECONDS( 0.5 ) ) return SYS_ABORTED; 
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Initialize( void )
{
	int nCommStatus;
	BOOL bEventFlag;
	LPRUNRESULT LPResult;
	

	if ( FOUP_ExistanceCheck_Only ( &bEventFlag ) != SYS_SUCCESS ) return SYS_ABORTED;

	while( TRUE ) {
		if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;
		
		WRITE_DIGITAL( Homing, Home, &nCommStatus );	// Loadport Init Command Send
		LPResult = WaitRunComplete ( ALARM_PORT_INIT_TIMEOUT, FALSE, InitMode, 0 );
		if ( LPResult == LPRUN_RETRY ) continue;
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;
		break;
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status LP_READY() 
{
	if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status FOUP_ExistanceCheck( void ) 
{
	int nData1, nData2;
	int nCount = 0;
	AlarmStyle AlmResult;

	while( TRUE ) {
		if ( READ_DIGITAL_TRUE( Present, &nData1 ) == SYS_ABORTED ) return SYS_ABORTED;
		if ( READ_DIGITAL_TRUE( Placed , &nData2 ) == SYS_ABORTED ) return SYS_ABORTED;

		if ( nData1 == ON && nData2 == ON ) break;
		if  ( nCount++ > 3 ) {
			AlmResult = ALARM_MANAGE( ALARM_PORT_FOUP_ABSENT );
			if      ( AlmResult == ALM_IGNORE ) break;
			else if ( AlmResult != ALM_RETRY  ) return SYS_ABORTED;
		}
		
		if ( !WAIT_SECONDS( 0.5 ) ) return SYS_ABORTED; 
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status PrepCheckPort( void ) 
{
	if ( HomeStatusCheck() == SYS_ABORTED ) return SYS_ABORTED;
	if ( ReadyStatusCheck() == SYS_ABORTED ) return SYS_ABORTED;
	if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Mapping_Control( char *MapInfo ) 
{
	int nCommStatus;
	char szSlot[32] , szOption[32] , szBuffer[256];
	
	if ( GetMappingData( eLoad_1 ) == SYS_ABORTED ) return SYS_ABORTED;

	STR_SEPERATE( MapInfo	, szSlot	, szOption	, 31 );

	if ( STRCMP( szSlot , "IN"  ) ) {
		if ( strlen( szOption ) > 0 ) { // after run
			sprintf( szBuffer , gszEVENT_STRING_MAPCOMPLETE_MAN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

			sprintf( szBuffer , gszEVENT_STRING_AT_SOURCE_MAN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

			sprintf( szBuffer , gszEVENT_STRING_AT_OCCUPY_MAN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
		}
		else { // before run
			sprintf( szBuffer , gszEVENT_STRING_MAPCOMPLETE_IN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

			sprintf( szBuffer , gszEVENT_STRING_AT_SOURCE_IN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

			sprintf( szBuffer , gszEVENT_STRING_AT_OCCUPY_IN	, gnCASSETTE_INDEX );
			WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
		}
	}
	else if ( STRCMP( szSlot , "OUT"  ) ) {	 // before end
		sprintf( szBuffer , gszEVENT_STRING_MAPCOMPLETE_OUT , gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Clamp_Control( CONTROLMODE dMode, int ConditionCheckFlag ) 
{
	int	nS_IO, nR_IO, nTarget, nS_IO2;
	int	nTimeoutAlarm;
	int nFailAlarm;
	int	nCommStatus;
	int	nClampStatus;
	AlarmStyle	AlmResult;
	LPRUNRESULT LPResult;
	
	if ( dMode == ClampMode ) {
		nS_IO			= Clamp;   
		nR_IO			= Clamp_Status; 
		nTarget			= CLAMP; 
		nTimeoutAlarm   = ALARM_PORT_CLAMP_TIMEOUT; 
		nFailAlarm		= ALARM_PORT_CLAMP_FAIL; 
		nS_IO2			= UnClamp;
	}
	else if ( dMode == UnclampMode ) {
		nS_IO			= UnClamp; 
		nR_IO			= Clamp_Status; 
		nTarget			= UNCLAMP; 
		nTimeoutAlarm	= ALARM_PORT_UNCLAMP_TIMEOUT; 
		nFailAlarm		= ALARM_PORT_UNCLAMP_FAIL; 
		nS_IO2			= Clamp;
	}


	while( TRUE ) {
		if ( ConditionCheckFlag ) {
			if ( PrepCheckPort() == SYS_ABORTED ) return SYS_ABORTED;
			if ( dMode == ClampMode ) {
				if ( FOUP_ExistanceCheck() == SYS_ABORTED ) return SYS_ABORTED;
			}
		}

		if ( WRITE_DIGITAL_TRUE( nS_IO2, eOFF_0 ) == SYS_ABORTED ) return SYS_ABORTED; 
		if ( WRITE_DIGITAL_TRUE( nS_IO, eON_1 ) == SYS_ABORTED ) return SYS_ABORTED; 

		if ( !WAIT_SECONDS( 0.25 ) ) return SYS_ABORTED; 
	
		LPResult = WaitRunComplete( nTimeoutAlarm, FALSE, dMode, 0 );
		if ( LPResult == LPRUN_RETRY ) {
			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
			continue;
		}
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;
		

		nClampStatus = READ_DIGITAL( nR_IO, &nCommStatus );

		if ( nClampStatus != nTarget ) {
			AlmResult = ALARM_MANAGE( nFailAlarm );
			
			if		( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult != ALM_RETRY  )	return SYS_ABORTED;

			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
		}
		else break;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Door_Control( CONTROLMODE dMode ) 
{
	int	nS_IO, nR_IO, nTarget;
	int	nTimeoutAlarm;
	int nFailAlarm;
	int	nCommStatus;
	DOORSTS	DoorStatus;
	AlarmStyle	AlmResult;
	LPRUNRESULT LPResult;

	if ( dMode == TOpenMode ) {
		nS_IO			= TOPEN;   
		nR_IO			= Door_Status; 
		nTarget			= OPENED;
		nTimeoutAlarm   = ALARM_PORT_OPEN_TIMEOUT; 
		nFailAlarm		= ALARM_PORT_OPEN_FAIL; 
	}
	else if ( dMode == TCloseMode ) {
		nS_IO			= TCLOSE; 
		nR_IO			= Door_Status; 
		nTarget			= CLOSED;
		nTimeoutAlarm	= ALARM_PORT_CLOSE_TIMEOUT; 
		nFailAlarm		= ALARM_PORT_CLOSE_FAIL; 
	}
	else {
		return SYS_ABORTED;
	}
	
	
	while( TRUE ) {
		if ( PrepCheckPort() == SYS_ABORTED ) return SYS_ABORTED;
		if ( FOUP_ExistanceCheck() == SYS_ABORTED ) return SYS_ABORTED;
		
		if ( WRITE_DIGITAL_TRUE( nS_IO, eON_1 ) == SYS_ABORTED ) return SYS_ABORTED; 
		
		if ( !WAIT_SECONDS( 0.25 ) ) return SYS_ABORTED; 
		
		LPResult = WaitRunComplete( nTimeoutAlarm, FALSE, dMode, 0 );
		if ( LPResult == LPRUN_RETRY ) {
			if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;
			continue;
		}
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;
		
	
		DoorStatus = READ_DIGITAL( nR_IO, &nCommStatus );
		
		if ( DoorStatus != nTarget ) {
			AlmResult = ALARM_MANAGE( nFailAlarm );
			
			if		( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult != ALM_RETRY  )	return SYS_ABORTED;

			if ( HW_AlarmClear() == SYS_ABORTED ) return SYS_ABORTED;
		}
		else break;
	}

	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status MonitoringForTOpen( void )
{
	int nCommStatus;
	CTCMAINTSTATUS MainStatus;
	CTCMAINCONTROL MainControl;
	
	while ( TRUE ) 
	{
		MainStatus = READ_DIGITAL( MAIN_STATUS, &nCommStatus );
		MainControl = READ_DIGITAL( MAIN_CONTROL, &nCommStatus );
		
		if ( MainStatus == sRunning	&& MainControl == cRunning ) {
			if ( Door_Control( TOpenMode ) == SYS_ABORTED ) return SYS_ABORTED;
			break;
		}
		
		if ( !WAIT_SECONDS (0.1) ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status LOAD_Control( int automode ) 
{
	int nCommStatus;
	char szBuffer[256] = "";
	DOORSTS DoorStatus;
	LPRUNRESULT LPResult;
	SHUTTLEPOS ShuttleStatus;
	AlarmStyle AlmResult;
	
	
	if ( AMHS != AMHS_Auto ) LoadUnloadLampControl( LOADON );
	
	Mapping_Data_Clear();
	
	
	sprintf( szBuffer , gszEVENT_STRING_MOVINGIN , gnCASSETTE_INDEX );
	WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
	
	while ( TRUE ) {
		if ( PrepCheckPort() == SYS_ABORTED ) return SYS_ABORTED;
		if ( FOUP_ExistanceCheck() == SYS_ABORTED ) return SYS_ABORTED;
		if ( Clamp_Control( ClampMode, FALSE ) == SYS_ABORTED ) return SYS_ABORTED;

		if ( WRITE_DIGITAL_TRUE( Load, eON_1 ) == SYS_ABORTED ) return SYS_ABORTED; 
	

		LPResult = WaitRunComplete( ALARM_PORT_LOAD_TIMEOUT, TRUE, LoadMode, automode );
		if ( LPResult == LPRUN_RETRY ) {
			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
			continue;
		}
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;


		DoorStatus  = READ_DIGITAL( Door_Status , &nCommStatus );
		ShuttleStatus = READ_DIGITAL( Shuttle_Status , &nCommStatus );
		
		if ( DoorStatus != OPENED || ShuttleStatus != DOCKED ) {
			AlmResult = ALARM_MANAGE( ALARM_PORT_LOAD_FAIL );
			
			if		( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult != ALM_RETRY  )	return SYS_ABORTED;

			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
		}
		else break;
	}

	if ( GetMappingData( eLoad_1 ) == SYS_ABORTED )	return SYS_ABORTED;

	if ( automode ) {		// Auto/Manual 기준이 없음.
		
		sprintf( szBuffer , gszEVENT_STRING_MAPCOMPLETE_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

		sprintf( szBuffer , gszEVENT_STRING_AT_SOURCE_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

		sprintf( szBuffer , gszEVENT_STRING_AT_OCCUPY_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
	}
	else
	{
		sprintf( szBuffer , gszEVENT_STRING_MAPCOMPLETE_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

		sprintf( szBuffer , gszEVENT_STRING_AT_SOURCE_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

		sprintf( szBuffer , gszEVENT_STRING_AT_OCCUPY_IN	, gnCASSETTE_INDEX );
		WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status UNLOAD_Control( int automode ) 
{
	int nCommStatus;
	char szBuffer[256] = "";
	AlarmStyle AlmResult;
	LPRUNRESULT LPResult;
	DOORSTS DoorStatus;
	SHUTTLEPOS ShuttleStatus;
	
	if ( AMHS != AMHS_Auto ) LoadUnloadLampControl( UNLOADON );


	sprintf( szBuffer , gszEVENT_STRING_MOVINGOUT , gnCASSETTE_INDEX );
	WRITE_STRING( SCHEDULER , szBuffer , &nCommStatus );

	// Fail Wafer 존재 시 Not Unload
	//CheckFailureWaferInFOUP();

	if ( READ_DIGITAL( Unload_Upmapping, &nCommStatus ) == eENABLE_1 ) {
		WRITE_DIGITAL( UPMappingEnable, eON_1, &nCommStatus );
	}
	else {
		WRITE_DIGITAL( UPMappingEnable, eOFF_0, &nCommStatus );
	}

	while ( TRUE ) 
	{
		if ( PrepCheckPort() == SYS_ABORTED ) return SYS_ABORTED;
		
		if ( WRITE_DIGITAL_TRUE( Unload, eON_1 ) == SYS_ABORTED ) return SYS_ABORTED; 


		LPResult = WaitRunComplete( ALARM_PORT_UNLOAD_TIMEOUT, TRUE, UnloadMode, automode );
		if ( LPResult == LPRUN_RETRY ) {
			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
			continue;
		}
		else if ( LPResult == LPRUN_ABORT ) return SYS_ABORTED;


		DoorStatus  = READ_DIGITAL( Door_Status , &nCommStatus );
		ShuttleStatus = READ_DIGITAL( Shuttle_Status , &nCommStatus );

		if ( DoorStatus != CLOSED || ShuttleStatus != UNDOCKED ) {
			AlmResult = ALARM_MANAGE( ALARM_PORT_UNLOAD_FAIL );
			
			if		( AlmResult == ALM_IGNORE ) return SYS_SUCCESS;
			else if ( AlmResult != ALM_RETRY  )	return SYS_ABORTED;

			if ( LP_READY() == SYS_ABORTED ) return SYS_ABORTED;
		}
		else break;
	}

	if ( READ_DIGITAL( Unload_Upmapping, &nCommStatus ) == ON ) {
		if ( GetMappingData( eUnload_0 ) == SYS_ABORTED )	return SYS_ABORTED;
	}

	if ( READ_DIGITAL( Unload_Clamp_Mode , &nCommStatus ) == CLAMP ) {
		if ( Clamp_Control( ClampMode, TRUE ) == SYS_ABORTED ) return SYS_ABORTED;
	}
	else {
		if ( Clamp_Control( UnclampMode, TRUE ) == SYS_ABORTED ) return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Port_Lamp_Control() 
{
	int nCommStatus;
	int nChangeFlag;
	int nLPResv;
	static int nStartFlag = FALSE;
	
	
	while ( TRUE ) {
		Sleep( 1000 );
		
		nLPResv = READ_Digital_Trigger( RESERVE_STATUS, &gnLPResv_OLD, &nChangeFlag );
		
		if ( !nStartFlag || nChangeFlag ) {
			if ( nLPResv == LPNot_Reserved )	WRITE_DIGITAL( RESERVE_LAMP , LAMPOFF  , &nCommStatus );					
			else							WRITE_DIGITAL( RESERVE_LAMP , LAMPON   , &nCommStatus );					
		}
		
		AMHS = READ_Digital_Trigger( AMHS_STATUS, &gnAMHS_OLD, &nChangeFlag );
		
		if ( !nStartFlag || nChangeFlag ) {
			// Loadport Control Mode를 설정하는 IO이나 Mode 변경하여도 사용상에 차이점이 변화가 없고
			// Auto/Manual Lamp Control IO로는 Lamp를 Control할 수 없어 이 IO로 대체 가능하다고 
			// Cymechs사와 협의하여 이 IO를 사용하는 것으로 함.
			if ( AMHS == AMHS_Manual ) {
				WRITE_DIGITAL( OpMode		, LP_Manual , &nCommStatus ); 
			}
			else {
				WRITE_DIGITAL( OpMode		, LP_Auto , &nCommStatus ); 
				WRITE_DIGITAL( LOAD_LAMP	, LAMPOFF , &nCommStatus );
				WRITE_DIGITAL( UNLOAD_LAMP	, LAMPOFF , &nCommStatus );
			}
		}
		if ( !nStartFlag ) nStartFlag = TRUE;
	}
	_endthread();
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() 
{
	int nCommStatus, nAUTO_MODE;
	char szRunMsg[32]={0,}, szParam[32]={0,};	// PARAMETER
	Module_Status MDLStatus=SYS_SUCCESS;
	int i,j;

	STR_SEPERATE( PROGRAM_PARAMETER_READ() , szRunMsg , szParam , 31 );


	if ( strlen( szParam ) > 0 ) nAUTO_MODE = 1;
	else                         nAUTO_MODE = 0;


	if ( READ_DIGITAL( LP_Control, &nCommStatus) )	 return SYS_SUCCESS;

	if		( STRCMP_L( szRunMsg , "READY"			) )		MDLStatus =  LP_READY();
	else if	( STRCMP_L( szRunMsg , "HOME"			) )		MDLStatus =  Initialize();
	else if	( STRCMP_L( szRunMsg , "ALARM_CLEAR"	) )		MDLStatus =  HW_AlarmClear();
	else if	( STRCMP_L( szRunMsg , "CLAMP"			) )		MDLStatus =  Clamp_Control( ClampMode, TRUE );
	else if ( STRCMP_L( szRunMsg , "UNCLAMP"		) )		MDLStatus =  Clamp_Control( UnclampMode, TRUE );
	else if ( STRCMP_L( szRunMsg , "LOAD"			) )		MDLStatus =  LOAD_Control( nAUTO_MODE );
	else if ( STRCMP_L( szRunMsg , "UNLOAD"			) )		MDLStatus =  UNLOAD_Control( nAUTO_MODE );
	else if ( STRCMP_L( szRunMsg , "UNLOAD_OPTION"	) )		MDLStatus =  UNLOAD_Control( nAUTO_MODE );
	else if ( STRCMP_L( szRunMsg , "MAPPING"		) )		MDLStatus =  Mapping_Control( szParam );
	else if ( STRCMP_L( szRunMsg , "MAPDATACLEAR"	) )		MDLStatus =  Mapping_Data_Clear();
	else if ( STRCMP_L( szRunMsg , "TOPEN"			) )		
	{
		//	2017.02.14	Run 대기 후 Door Open Option 적용.
		if ( READ_DIGITAL( Door_Status, &nCommStatus ) != OPENED ) {
			MDLStatus =  Door_Control( TOpenMode );
		}
	}
	else if ( STRCMP_L( szRunMsg , "TCLOSE"			) )	{
		MDLStatus =  Door_Control( TCloseMode );
	}
	else if ( STRCMP_L( szRunMsg , "LOOPTEST"			) ) {
		i=0; j=0;

		while ( TRUE ) {

			i++;
		
			if ( LOAD_Control( nAUTO_MODE ) == SYS_ABORTED ) return SYS_ABORTED; 
			
			if ( !WAIT_SECONDS( 0.5 ) ) return SYS_ABORTED; 

			j++;
			
			if ( UNLOAD_Control( nAUTO_MODE ) == SYS_ABORTED ) return SYS_ABORTED;
			
			// 모터  과열 방지 delay
			if ( !WAIT_SECONDS( 5 ) ) return SYS_ABORTED; 

			if ( ( i > 9999 ) || ( j > 9999 ) ) {
				return SYS_SUCCESS;
			}
		}
	}
	else {
		printf( "[%s] Not Supported Parameter [%s]\n", PROGRAM_FUNCTION_READ() , szRunMsg );
		return SYS_ABORTED;
	}	
	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main()
{
	Module_Status MDLStatus;

	//==============================================================================================================
	_iLOG_PRINTF( "  =>> START [%s]\n" , PROGRAM_PARAMETER_READ() );
	//==============================================================================================================
	
	MDLStatus = Program_Main_Sub();
	
	//==============================================================================================================
	switch( MDLStatus ) {
	case SYS_SUCCESS	:	_iLOG_PRINTF( "  <<= END [%s] is SUCCESS\n" , PROGRAM_PARAMETER_READ() );			break;
	case SYS_ABORTED	:	_iLOG_PRINTF( "  <<= END [%s] is ABORTED\n" , PROGRAM_PARAMETER_READ() );			break;
	case SYS_ERROR		:	_iLOG_PRINTF( "  <<= END [%s] is ERROR\n"   , PROGRAM_PARAMETER_READ() );			break;
	default				:	_iLOG_PRINTF( "  <<= END [%s] is [%d]\n"    , PROGRAM_PARAMETER_READ() , MDLStatus );break;
	}
	//==============================================================================================================

	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Program_Enter_Code() 
{
	char szModule[64]={0,}, szAlarm[64]={0,}, szArgument[64]={0,};

	STR_SEPERATE(PROGRAM_ARGUMENT_READ(), szModule, szArgument, 63);
	STR_SEPERATE(szArgument, szAlarm, szArgument, 63);

	REPLACE_CHAR_SET(0, szModule);
	REPLACE_CHAR_SET(1, szArgument);

	gnCASSETTE_INDEX = szModule[2] - '0';
	gnALARM_START_INDEX = atoi(szAlarm);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code() 
{
	int nCommStatus;
	char szVersion[64]={0,};
		
	_beginthread( (void *)Port_Lamp_Control , 0 , NULL );

	sprintf( szVersion, "Ver.1.0[%s]", __DATE__ );
	REGIST_FILE_INFO( szVersion );
	
	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Event_Message_Received() 
{
	//
}
