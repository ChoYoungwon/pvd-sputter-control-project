#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutltime.h>
#include <Kutlgui.h>

#include "cimseqnc.h"
#include "iodefine.h"
#include "../Library/kutlCheck.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define	ALARM_VALID_TIMEOUT			gnALARM_START_INDEX + 0
#define	ALARM_TRREQ_TIMEOUT			gnALARM_START_INDEX + 1
#define	ALARM_BUSY_TIMEOUT			gnALARM_START_INDEX + 2
#define	ALARM_CARRECV_TIMEOUT		gnALARM_START_INDEX + 3
#define	ALARM_COMPLETE_TIMEOUT		gnALARM_START_INDEX + 4
#define	ALARM_CSOFF_TIMEOUT			gnALARM_START_INDEX + 5
#define	ALARM_CARREMOVE_TIMEOUT		gnALARM_START_INDEX + 6
#define	ALARM_CLAMP_TIMEOUT			gnALARM_START_INDEX + 7
#define	ALARM_UNCLAMP_TIMEOUT		gnALARM_START_INDEX + 8

#define	ALARM_FOUP_PRESENT			gnALARM_START_INDEX + 10
#define	ALARM_FOUP_ABSENT			gnALARM_START_INDEX + 11
#define	ALARM_AHMS_MANUAL			gnALARM_START_INDEX + 12
#define	ALARM_VALID_OFF				gnALARM_START_INDEX + 13
#define	ALARM_TM_Light_Curtain_ON	gnALARM_START_INDEX + 14

#define	DEFAULT_TIME_OUT			20		// 20sec
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int		gnCassette_Index		= 1;
int		gnControl_Refresh		= FALSE;
int		gnMonitor_View			= FALSE;
int		gnMonitor_Count			= 0;

BOOL	gbControl_AutoMode		= FALSE;
int		gnALARM_START_INDEX		= 0;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
char *gszEVENT_STRING_LOAD_COMPLETE			= "EVENT_ON LOAD_COMPLETE_F CM";
char *gszEVENT_STRING_CLAMP_COMPLETE		= "EVENT_ON LL_CLAMP_COMPLETE CM";
char *gszEVENT_STRING_UNLOAD_COMPLETE		= "EVENT_ON UNLOAD_COMPLETE_F CM";
char *gszEVENT_STRING_UNCLAMP_COMPLETE		= "EVENT_ON LL_UNCLAMP_COMPLETE CM";

char *gszEVENT_STRING_STS_UNOCCUPIED		= "EVENT_ON STS_UNOCCUPIED_F CM";
char *gszEVENT_STRING_CARR_DELETED			= "EVENT_ON CARR_DELETED_F CM";
char *gszEVENT_STRING_STS_DELETED			= "EVENT_ON STS_DELETED_F CM";
char *gszEVENT_STRING_CARR_NOTASSOCIATED	= "EVENT_ON CARR_NOTASSOCIATED_F CM";

char *gszEVENT_STRING_TR_LOAD_RECV			= "EVENT_ON LOAD_TB_F CM";
char *gszEVENT_STRING_TR_LOAD_RECV2			= "EVENT_ON LOAD_TB2_F CM";
char *gszEVENT_STRING_TR_UNLOAD_RECV		= "EVENT_ON UNLOAD_TB_F CM";

char *gszEVENT_LOAD_REQUEST					= "EVENT_ON LOAD_REQUEST_F CM";
char *gszEVENT_UNLOAD_REQUEST				= "EVENT_ON UNLOAD_REQUEST_F CM";

char *gszEVENT_STRING_UNDOCKING				= "EVENT_ON LL_UNDOCKING_COMPLETE CM";

char gszStrBuffer[256];
char gszStrCarrierID[256];
char gszStrPPID[256];
char gszStrJobID[256];
char gszStrLotID[256];
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum {
	eMON_LOAD_WAIT_CARRIER_0			, eMON_LOAD_WAIT_CLAMP_1			,
	eMON_UNLOAD_WAIT_UNCLAMP_2			, eMON_UNLOAD_WAIT_CARRIER_3		,
	eMON_LOAD_AGV_WAIT_VALID_4			, eMON_LOAD_AGV_WAIT_TRREQ_5		,
	eMON_LOAD_AGV_WAIT_BUSY_6			, eMON_LOAD_AGV_WAIT_CARRIER_7		,
	eMON_LOAD_AGV_WAIT_COMPLETE_8		, eMON_LOAD_AGV_WAIT_COMPLETE2_9	,
	eMON_LOAD_AGV_WAIT_CSOFF_10			, eMON_LOAD_AGV_WAIT_FINISH_11		, 
	eMON_UNLOAD_AGV_WAIT_UNCLAMPED_12	, eMON_UNLOAD_AGV_WAIT_VALID_13		, 
	eMON_UNLOAD_AGV_WAIT_TRREQ_14		, eMON_UNLOAD_AGV_WAIT_BUSY_15		,
	eMON_UNLOAD_AGV_WAIT_CARRIER_16		, eMON_UNLOAD_AGV_WAIT_COMPLETE_17	, 
	eMON_UNLOAD_AGV_WAIT_COMPLETE2_18	, eMON_UNLOAD_AGV_WAIT_CSOFF_19
}; 
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eSIM_0			, eREAL_1		};
enum { eLOAD_0			, eUNLOAD_1		};
enum { eABSENT_0		, ePRESENT_1	};
enum { eOFF_0			, eON_1			, eBLINK_2	};
enum { eNONE_0			, eABSENT2_1	, ePRESENT2_2		};
enum { eSTYLE_FOUP_0	, eSTYLE_CASS_1	, eSTYLE_SMIF_2		};
enum { eCLAMP_0			, eUNCLAMP_1	};
enum { eUNCLAMP_0		, eCLAMP_1		};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {

	{ "IO_Driver_Loding"				, _K_D_IO	,	IO_DRIVER_LOADING				,	0	} ,

	{ "e:SCHEDULER"						, _K_S_IO	,	SCHEDULER						,	0	} ,

	{ "SEQ_CM$1_MID_CONTROL"				, _K_F_IO	,	CM_MID_CONTROL					,	0	} ,

	{ "dvLOAD_$0_Signal"				, _K_D_IO	,	dvLOAD_CM_Signal				,	0	} ,
	{ "dvULOAD_$0_Signal"				, _K_D_IO	,	dvULOAD_CM_Signal				,	0	} ,
	{ "$0.AMHS_MOVING"					, _K_D_IO	,	dvAMHS_CM_Moving				,	0	} ,
	{ "CM$1.PROCESS_END"				, _K_D_IO	,	dvPRCS_CM_End					,	0	} ,
	{ "CM$1.OPERATOR_CANCEL"			, _K_D_IO	,	dvOPER_CM_Cancel				,	0	} ,
	{ "CTC.FA_STATUS_MDL_CM$1"			, _K_D_IO	,	CTC_FA_STATUS_MDL_CM			,	0	} ,
	{ "dpLP_$0_Unload_Clamp_Mode"		, _K_D_IO	,	Unload_Clamp_Mode				,	0	} , // Clamp , Unclamp

	{ "dvAMHS_$0_HostAvailable"			, _K_D_IO	,	dvAMHS_CM_HostAvailable			,	0	} ,
	{ "dvAMHS_$0_EStop"					, _K_D_IO	,	dvAMHS_CM_EStop					,	0	} ,
	{ "dvAMHS_$0_L_Request"				, _K_D_IO	,	dvAMHS_CM_L_Request				,	0	} ,
	{ "dvAMHS_$0_U_Request"				, _K_D_IO	,	dvAMHS_CM_U_Request				,	0	} ,
	{ "dvAMHS_$0_Ready"					, _K_D_IO	,	dvAMHS_CM_Ready					,	0	} ,
	
	{ "dvAMHS_$0_CS0"					, _K_D_IO	,	dvAMHS_CM_CS0					,	0	} ,
	{ "dvAMHS_$0_CS1"					, _K_D_IO	,	dvAMHS_CM_CS1					,	0	} ,
	{ "dvAMHS_$0_Valid"					, _K_D_IO	,	dvAMHS_CM_Valid					,	0	} ,
	{ "dvAMHS_$0_TR_Request"			, _K_D_IO	,	dvAMHS_CM_TR_Request			,	0	} ,
	{ "dvAMHS_$0_Busy"					, _K_D_IO	,	dvAMHS_CM_Busy					,	0	} ,
	{ "dvAMHS_$0_Complete"				, _K_D_IO	,	dvAMHS_CM_Complete				,	0	} ,
	{ "dvAMHS_$0_Cont"					, _K_D_IO	,	dvAMHS_CM_Cont					,	0	} ,
	
	{ "CTC.AMHS_TP1"					, _K_D_IO	,	CTC_AMHS_TP1					,	0	} ,
	{ "CTC.AMHS_TP2"					, _K_D_IO	,	CTC_AMHS_TP2					,	0	} ,
	{ "CTC.AMHS_TP3"					, _K_D_IO	,	CTC_AMHS_TP3					,	0	} ,
	{ "CTC.AMHS_TP4"					, _K_D_IO	,	CTC_AMHS_TP4					,	0	} ,
	{ "CTC.AMHS_TP5"					, _K_D_IO	,	CTC_AMHS_TP5					,	0	} ,
	{ "CTC.AMHS_TP6"					, _K_D_IO	,	CTC_AMHS_TP6					,	0	} ,
	//
	{ "diLP_$0_Present"					, _K_D_IO	,	diLP_CM_Present					,	0	} , // present
	{ "diLP_$0_Placed"					, _K_D_IO	,	diLP_CM_Placed					,	0	} , // Placement
	//
	{ "doLP_$0_UnClamp"					, _K_D_IO	,	doLP_CM_UnClamp					,	0	} ,
	{ "doLP_$0_Clamp"					, _K_D_IO	,	doLP_CM_Clamp					,	0	} ,

	{ "diLP_$0_Clamp_Status"			, _K_D_IO	,	diLP_Clamp_Status				,	0	} ,
//	{ "diLP_$0_UnClamp_Status"			, _K_D_IO	,	UnClamp_Status					,	0	} ,
	//
	{ "dvLP_$0_Load_Button"				, _K_D_IO	,	Load_Button						,	0	} ,
	{ "dvLP_$0_Unload_Button"			, _K_D_IO	,	Unload_Button					,	0	} ,
	
	{ "doLP_$0_Load_Lamp"				, _K_D_IO	,	doLP_Load_Lamp					,	0	} , // Load Button Lamp
	{ "doLP_$0_Unload_Lamp"				, _K_D_IO	,	doLP_Unload_Lamp				,	0	} , // Unload Button Lamp
	
	{ "CM$1.EVENT_SERVICE"				, _K_D_IO	,	CM_EVENT_SERVICE				,	0	} ,
	
	{ "CTC.MID_NAME"					, _K_S_IO	,	CARRIER_ID						,	0	} ,
	{ "CTC.MID_NAME2"					, _K_S_IO	,	CARRIER_ID2						,	0	} ,
	{ "CTC.MID_NAME3"					, _K_S_IO	,	CARRIER_ID3						,	0	} ,
	{ "CTC.MID_NAME4"					, _K_S_IO	,	CARRIER_ID4						,	0	} ,

	{ "CTC.MID_READ"					, _K_S_IO	,	CARREAD_ID						,	0	} ,
	{ "CTC.MID_READ2"					, _K_S_IO	,	CARREAD_ID2						,	0	} ,
	{ "CTC.MID_READ3"					, _K_S_IO	,	CARREAD_ID3						,	0	} ,
	{ "CTC.MID_READ4"					, _K_S_IO	,	CARREAD_ID4						,	0	} ,

	{ "CTC.JOB_NAME"					, _K_S_IO	,	JOBNAME_ID						,	0	} ,
	{ "CTC.JOB_NAME2"					, _K_S_IO	,	JOBNAME_ID2						,	0	} ,
	{ "CTC.JOB_NAME3"					, _K_S_IO	,	JOBNAME_ID3						,	0	} ,
	{ "CTC.JOB_NAME4"					, _K_S_IO	,	JOBNAME_ID4						,	0	} ,

	{ "CTC.JID_READ"					, _K_S_IO	,	JOBREAD_ID						,	0	} ,
	{ "CTC.JID_READ2"					, _K_S_IO	,	JOBREAD_ID2						,	0	} ,
	{ "CTC.JID_READ3"					, _K_S_IO	,	JOBREAD_ID3						,	0	} ,
	{ "CTC.JID_READ4"					, _K_S_IO	,	JOBREAD_ID4						,	0	} ,

	{ "CTC.RECIPE_FILE"					, _K_S_IO	,	RECIPE_ID						,	0	} ,
	{ "CTC.RECIPE_FILE2"				, _K_S_IO	,	RECIPE_ID2						,	0	} ,
	{ "CTC.RECIPE_FILE3"				, _K_S_IO	,	RECIPE_ID3						,	0	} ,
	{ "CTC.RECIPE_FILE4"				, _K_S_IO	,	RECIPE_ID4						,	0	} ,

	{ "GEM.Port1_LotID"					, _K_S_IO	,	LOTNAME_ID						,	0	} ,
	{ "GEM.Port2_LotID"					, _K_S_IO	,	LOTNAME_ID2						,	0	} ,
	{ "GEM.Port3_LotID"					, _K_S_IO	,	LOTNAME_ID3						,	0	} ,
	{ "GEM.Port4_LotID"					, _K_S_IO	,	LOTNAME_ID4						,	0	} ,

	//{ "TM.Light_Curtain"				, _K_D_IO	,	TM_Light_Curtain				,	0	} ,

	""
};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Monitor( int nMode ) {

	if ( !gnMonitor_View ) {
		gnMonitor_Count = 0;
		return;
	}

	gnMonitor_Count++;
	if ( gnMonitor_Count > 3 ) gnMonitor_View = FALSE;

	switch( nMode ) {
	case eMON_LOAD_WAIT_CARRIER_0 :
		printf( "[%s] CHECKING CM%d LOAD_WAIT_CARRIER\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_WAIT_CLAMP_1 :
		printf( "[%s] CHECKING CM%d LOAD_WAIT_CLAMP\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	//====
	case eMON_UNLOAD_WAIT_UNCLAMP_2 :
		printf( "[%s] CHECKING CM%d UNLOAD_WAIT_UNCLAMP\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_WAIT_CARRIER_3 :
		printf( "[%s] CHECKING CM%d UNLOAD_WAIT_CARRIER\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	//====
	case eMON_LOAD_AGV_WAIT_VALID_4 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_VALID\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_TRREQ_5 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_TRREQ\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_BUSY_6 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_BUSY\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_CARRIER_7 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_CARRIER\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_COMPLETE_8 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_COMPLETE\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_COMPLETE2_9 :
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_COMPLETE2\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_CSOFF_10 : 
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_CSOFF\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_LOAD_AGV_WAIT_FINISH_11 : 
		printf( "[%s] CHECKING CM%d LOAD_AGV_WAIT_VALID\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	//====
	case eMON_UNLOAD_AGV_WAIT_UNCLAMPED_12 :
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_UNCLAMPED\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_VALID_13 : 
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_VALID\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_TRREQ_14 :
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_TRREQ\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_BUSY_15 :
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_BUSY\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_CARRIER_16 : 
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_CARRIER\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_COMPLETE_17 : 
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_COMPLETE\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_COMPLETE2_18 : 
		printf( "[%s] CHECKING CM%d UNLOAD_AGV_WAIT_COMPLETE2\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	case eMON_UNLOAD_AGV_WAIT_CSOFF_19 : 
		printf( "[%s] CHECKING CM%d eMON_UNLOAD_AGV_WAIT_CSOFF_19\n" , PROGRAM_FUNCTION_READ(), gnCassette_Index );
		break;
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int Read_Digital_True(int nAMHS_CM, int *nCommStatus) {

	int nData;

	nData = READ_DIGITAL(nAMHS_CM, nCommStatus);

	if (*nCommStatus) return nData;

	Sleep(500);
	nData = READ_DIGITAL(nAMHS_CM, nCommStatus);

	if (*nCommStatus) return nData;

	Sleep(1000);
	nData = READ_DIGITAL(nAMHS_CM, nCommStatus);

	if (*nCommStatus) return nData;

	return nData;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int Read_Digital_Double(int nUnClampStatus, int nClampStatus, int *nCommStatus) {

	int i;
	int nData = 0;

	for ( i = 0 ; i < 2 ; i++ ) {

		nData = (READ_DIGITAL(nUnClampStatus, nCommStatus) * 2);
		nData = nData + READ_DIGITAL(nClampStatus, nCommStatus);

		if ( *nCommStatus ) return nData;
	}

	return nData;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int ALARM_MANAGE_for_CarrHandling( int nAlarmID ) {

	int nAlmResult;

	if ( gnControl_Refresh ) return SYS_ABORTED;

	ALARM_POST(nAlarmID);

	while ( TRUE ) {

		Sleep(2);
		nAlmResult = ALARM_STATUS(nAlarmID);

		if ( nAlmResult != ALM_PAUSED ) break;
		if ( gnControl_Refresh ) {
			ALARM_CLEAR(nAlarmID);
			return SYS_ABORTED;
		}
	}

	return nAlmResult;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status RUN_FUNCTION_for_CarrHandling( int nInd , char *szParam ) {

	Module_Status nMS;

	while ( TRUE ) {

		if ( gnControl_Refresh ) return SYS_ABORTED;

		if (READ_FUNCTION(nInd) == SYS_RUNNING) {
			do {
				if ( gnControl_Refresh ) return SYS_ABORTED;
				Sleep(10);
			}
			while (READ_FUNCTION(nInd) == SYS_RUNNING);
			Sleep(500);
		}

		nMS = WRITE_FUNCTION(nInd, szParam);

		if ( nMS == 2 ) Sleep( 500 );
		else           break;
	}

	while ( TRUE ) {
		nMS = READ_FUNCTION(nInd);

		if ( nMS != SYS_RUNNING ) break;

		if ( gnControl_Refresh ) {

			RUN_FUNCTION_ABORT(nInd);

			while ( TRUE ) {

				nMS = READ_FUNCTION(nInd);

				if ( nMS != SYS_RUNNING ) return nMS;

				Sleep(10);
			}
		}
		Sleep(1);
	}

	if ( nMS == SYS_AVAILABLE ) nMS = SYS_ABORTED;

	return( nMS );
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void AGV_INIT_SET_IO( int nMode ) {

	int nCommStatus;

	switch( nMode ) {
	case -1 : // AGV Possible
		WRITE_DIGITAL(dvAMHS_CM_HostAvailable	, eON_1	, &nCommStatus); // avaliable
		WRITE_DIGITAL(dvAMHS_CM_EStop			, eON_1	, &nCommStatus); // not Emergency stop
		WRITE_DIGITAL(dvAMHS_CM_L_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_U_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_Ready			, eOFF_0, &nCommStatus);
		break;
	case -2 : // Manual Posible
		WRITE_DIGITAL(dvAMHS_CM_HostAvailable	, eOFF_0, &nCommStatus); // not avaliable
		WRITE_DIGITAL(dvAMHS_CM_EStop			, eON_1	, &nCommStatus); // not Emergency stop
		WRITE_DIGITAL(dvAMHS_CM_L_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_U_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_Ready			, eOFF_0, &nCommStatus);
		break;
	case -3 : // Emergency
		WRITE_DIGITAL(dvAMHS_CM_HostAvailable	, eOFF_0, &nCommStatus); // not avaliable
		WRITE_DIGITAL(dvAMHS_CM_EStop			, eOFF_0, &nCommStatus); // Emergency stop
		WRITE_DIGITAL(dvAMHS_CM_L_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_U_Request		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_Ready			, eOFF_0, &nCommStatus);
		break;
	case eMON_LOAD_AGV_WAIT_VALID_4 :
		break;
	case eMON_LOAD_AGV_WAIT_TRREQ_5 :
		break;
	case eMON_LOAD_AGV_WAIT_BUSY_6 :
		break;
	case eMON_LOAD_AGV_WAIT_CARRIER_7 :
		break;
	case eMON_LOAD_AGV_WAIT_COMPLETE_8 :
		break;
	case eMON_LOAD_AGV_WAIT_COMPLETE2_9 :
		break;
	//====
	case eMON_LOAD_AGV_WAIT_FINISH_11 : 
		break;
	case eMON_UNLOAD_AGV_WAIT_VALID_13 : 
		break;
	case eMON_UNLOAD_AGV_WAIT_TRREQ_14 :
		break;
	case eMON_UNLOAD_AGV_WAIT_BUSY_15 :
		break;
	case eMON_UNLOAD_AGV_WAIT_CARRIER_16 : 
		break;
	case eMON_UNLOAD_AGV_WAIT_COMPLETE_17 : 
		break;
	case eMON_UNLOAD_AGV_WAIT_COMPLETE2_18 : 
		break;
	case eMON_UNLOAD_AGV_WAIT_CSOFF_19 : 
		break;
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int FOUP_SENSOR_CHECK( int *nCommStatus ) { // return 0 = None, 1 = Absent, 2 = Present

	int nPlaced, nPresent;
	int nCommStatus1 = 0;
	int nCommStatus2 = 0;

	nPlaced  = READ_DIGITAL( diLP_CM_Placed  , &nCommStatus1 ); // 0 = Absent, 1 = Present
	nPresent = READ_DIGITAL( diLP_CM_Present , &nCommStatus2 ); // 0 = Absent, 1 = Present

	if ( !nCommStatus1 || !nCommStatus2 ) {
		*nCommStatus = 0;
		return eNONE_0;
	}

	*nCommStatus = 1;

	if ( nPlaced == ePRESENT_1 && nPresent == ePRESENT_1 ) {
		return ePRESENT2_2;
	}

	if ( nPlaced == eABSENT_0 && nPresent == eABSENT_0 ) {
		return eABSENT2_1;
	}

	// Placed/Present signals are changing or do not agree.
	return eNONE_0;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int FOUP_SENSOR_CHECK_STABLE( int *nCommStatus , int nWaitTimeMs ) {

	int nElapsedTime = 0;
	int nCurrentState;
	int nPreviousState = eNONE_0;
	int nStableCount = 0;

	const int nCheckIntervalMs = 100;
	const int nRequiredStableCount = 3;

	while ( nElapsedTime <= nWaitTimeMs ) {

		nCurrentState = FOUP_SENSOR_CHECK( nCommStatus );

		if ( !(*nCommStatus) ) {
			return eNONE_0;
		}

		if ( nCurrentState == eNONE_0 ) {
			nPreviousState = eNONE_0;
			nStableCount = 0;
		}
		else {
			if ( nCurrentState == nPreviousState ) {
				nStableCount++;
			}
			else {
				nPreviousState = nCurrentState;
				nStableCount = 1;
			}

			if ( nStableCount >= nRequiredStableCount ) {
				return nCurrentState;
			}
		}

		Sleep( nCheckIntervalMs );
		nElapsedTime += nCheckIntervalMs;
	}

	return eNONE_0;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status FOUP_SENSOR_ERROR( int nExpectedState , BOOL bHO_AVAL , int Line ) {

	int nAlarm;
	int nAlmResult;
	int nCommStatus = 0;
	int nActualState;
	BOOL bCheck = FALSE;

	if ( nExpectedState == eABSENT2_1 ) {
		nAlarm = ALARM_FOUP_PRESENT;
	}
	else if ( nExpectedState == ePRESENT2_2 ) {
		nAlarm = ALARM_FOUP_ABSENT;
	}
	else {
		_iLOG_PRINTF(
			"  =>> FOUP_SENSOR_ERROR InvalidExpected[%d] Line[%d]",
			nExpectedState,
			Line
		);
		return SYS_ABORTED;
	}

	// Allow a short interval between Placed and Present signal changes.
	nActualState = FOUP_SENSOR_CHECK_STABLE( &nCommStatus , 1000 );

	if ( !nCommStatus ) {
		_iLOG_PRINTF(
			"  =>> FOUP_SENSOR_COMM_ERROR Exp[%d] Act[%d] Line[%d]",
			nExpectedState,
			nActualState,
			Line
		);

		if ( bHO_AVAL ) {
			AGV_INIT_SET_IO( -2 );
		}

		return SYS_ABORTED;
	}

	if ( nActualState == eNONE_0 ) {
		_iLOG_PRINTF(
			"  =>> FOUP_SENSOR_MISMATCH Exp[%d] Act[%d] Line[%d]",
			nExpectedState,
			nActualState,
			Line
		);

		if ( bHO_AVAL ) {
			AGV_INIT_SET_IO( -2 );
		}

		return SYS_ABORTED;
	}

	if ( nActualState == nExpectedState ) {
		return SYS_SUCCESS;
	}

	_iLOG_PRINTF(
		"  =>> FOUP_SENSOR_ERROR Exp[%d] Act[%d] Line[%d]",
		nExpectedState,
		nActualState,
		Line
	);

	if ( bHO_AVAL ) {
		bCheck = TRUE;
		AGV_INIT_SET_IO( -2 );
	}

	//ALARM_POST( nAlarm );

	//while ( TRUE ) {

	//	Sleep( 100 );
	//	nAlmResult = ALARM_STATUS( nAlarm );

	//	if ( nAlmResult != ALM_PAUSED ) {
	//		break;
	//	}

	//	if ( gnControl_Refresh ) {
	//		ALARM_CLEAR( nAlarm );
	//		return SYS_ABORTED;
	//	}
	//}

	if ( bHO_AVAL && bCheck ) {
		WRITE_DIGITAL(
			dvAMHS_CM_HostAvailable,
			eON_1,
			&nCommStatus
		);
	}

	if ( nAlmResult == ALM_IGNORE ) {
		return SYS_ERROR;
	}

	return SYS_ABORTED;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status AGV_RECOVERY_SCENARIO(int nLoadUnLoad) {

	int nCarrierSts = 0;
	int nCommStatus;
	int nData = 0;
	
	while( TRUE ) {
		Sleep(250);
		nData = FOUP_SENSOR_CHECK( &nCommStatus );
		if ( nCommStatus ) {
			if (nData == ePRESENT2_2) { nCarrierSts = ePRESENT_1; break; }
			else					{ nCarrierSts = eABSENT_0;  break; }
		}
	}
	
	switch( nLoadUnLoad ) {
	case eLOAD_0 :
		if( nCarrierSts ) return SYS_SUCCESS;//Load Complete;
		else 			 return SYS_ABORTED;//Load Request;	
	case eUNLOAD_1 :
		if( nCarrierSts ) return SYS_ABORTED;//UnLoad Request;
		else			 return SYS_SUCCESS;//UnLoad Complete;
	}
	
	return SYS_ABORTED;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status AGV_LOAD_CONTROL() {

	int nAlmResult;
	int nStatus, nCommStatus;
	double dTime;

	// if Only IGNORE Recovery is used , that is selete scenario
	//---------------------------------------------------
	// Checking Receive Carrier with PIO
	//---------------------------------------------------
	S_TIMER_READY();

	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_VALID_4 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
	
		nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

			if ( nStatus != SYS_SUCCESS ) return SYS_ERROR; // No Foup
			else { // SYS_SUCCESS, Foup Present
				//-------------------------------------------------------------------------------
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
				//-------------------------------------------------------------------------------
				return SYS_SUCCESS;
			}
		}
		
		if ( Read_Digital_True( dvAMHS_CM_Valid , &nCommStatus ) ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP1 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_TIMEOUT );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if (nStatus != SYS_SUCCESS){
					return SYS_ERROR; // No Foup
				}
				else { // SYS_SUCCESS, Foup Present
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}
	}
	WRITE_DIGITAL(dvAMHS_CM_L_Request, eON_1, &nCommStatus);

	S_TIMER_READY();
	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_TRREQ_5 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ERROR; // No Foup
				else { // SYS_SUCCESS, Foup Present
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}

		nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

			if ( nStatus != SYS_SUCCESS ) return SYS_ERROR; // No Foup
			else { // SYS_SUCCESS, Foup Present
				//-------------------------------------------------------------------------------
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
				//-------------------------------------------------------------------------------
				return SYS_SUCCESS;
			}
		}
		
		if ( Read_Digital_True( dvAMHS_CM_TR_Request , &nCommStatus ) ) break;
		dTime = (double) READ_DIGITAL( CTC_AMHS_TP1 , &nCommStatus );
		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TRREQ_TIMEOUT );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ERROR; // No Foup
				else { // SYS_SUCCESS, Foup Present
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}
	}

	// Before TB, return SYS_ERROR, After return SYS_ABORTED
	//-------------------------------------------------------------------------------
	sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV2 , gnCassette_Index );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//-------------------------------------------------------------------------------
	WRITE_DIGITAL(dvAMHS_CM_Ready, eON_1, &nCommStatus);

	S_TIMER_READY();
	while( TRUE ) {
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_BUSY_6 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}

		//if (READ_DIGITAL(TM_Light_Curtain, &nCommStatus) != eOFF_0) {

		//	AGV_INIT_SET_IO( -3 ); // recovery is only ignore and abort
		//	nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TM_Light_Curtain_ON );

		//	if ( nAlmResult == ALM_IGNORE ) {

		//		nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

		//		if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
		//		else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
		//	}
		//	else return SYS_ABORTED;
		//}

		nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED){
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
			
			if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
		}

		if ( Read_Digital_True( dvAMHS_CM_Busy , &nCommStatus ) ) break;
		dTime = (double) READ_DIGITAL( CTC_AMHS_TP2 , &nCommStatus );
		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_BUSY_TIMEOUT );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}

		Sleep(100);
	}

	S_TIMER_READY();
	while ( TRUE ) {
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_CARRIER_7 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 :
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );
			
			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}

		//if (READ_DIGITAL(TM_Light_Curtain, &nCommStatus) != eOFF_0) {

		//	AGV_INIT_SET_IO( -3 ); // recovery is only ignore and abort
		//	nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TM_Light_Curtain_ON );
		//	
		//	if ( nAlmResult == ALM_IGNORE ) {
		//	
		//		nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

		//		if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
		//		else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
		//	}
		//	else return SYS_ABORTED;
		//}

		if (FOUP_SENSOR_CHECK(&nCommStatus) == ePRESENT2_2) break;
		dTime = (double) READ_DIGITAL( CTC_AMHS_TP3 , &nCommStatus );
		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_CARRECV_TIMEOUT );
			
			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
				
				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}

		Sleep(100);
	}
	WRITE_DIGITAL(dvAMHS_CM_L_Request, eOFF_0, &nCommStatus);

	S_TIMER_READY();
	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_COMPLETE_8 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}

		nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );
		if		( nStatus == SYS_ABORTED ) return SYS_ABORTED;
		else if ( nStatus == SYS_ERROR   ){
			nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
			if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
		}
	
		if ( Read_Digital_True( dvAMHS_CM_Complete , &nCommStatus ) ) break;
//		dTime = (double) READ_DIGITAL( CTC_AMHS_TP4 , &nCommStatus ) / 2.;
		dTime = (double) READ_DIGITAL( CTC_AMHS_TP4 , &nCommStatus );
		//		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT / 2.;		
		if (dTime <= 0) dTime = DEFAULT_TIME_OUT;
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {
			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_COMPLETE_TIMEOUT );
			if ( nAlmResult == ALM_IGNORE ) {
				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}
	}
	WRITE_DIGITAL(dvAMHS_CM_Ready, eOFF_0, &nCommStatus);
	
	S_TIMER_READY();
	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_AGV_WAIT_CSOFF_10 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if ( READ_DIGITAL( dvAMHS_CM_Complete , &nCommStatus ) ) {
			if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {
				AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
				nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );
				if ( nAlmResult == ALM_IGNORE ) {
					nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
					if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
					else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
				}
				else return SYS_ABORTED;
			}
		}

		nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );
		if		( nStatus == SYS_ABORTED ) return SYS_ABORTED;
		else if ( nStatus == SYS_ERROR   ) {
			nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);
			if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
		}
		
		if ( Read_Digital_True( dvAMHS_CM_CS0 , &nCommStatus ) == 0 ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP5 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_CSOFF_TIMEOUT );

			if ( nAlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eLOAD_0);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // No Foup
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Present
			}
			else return SYS_ABORTED;
		}
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status AGV_UNLOAD_CONTROL( BOOL bUnClamp ) {

	int AlmResult, Result; 
	int nCommStatus, nStatus;
	double dTime;

	// if Only IGNORE Recovery is used , that is selete scenario
	//---------------------------------------------------
	// Checking Send Carrier with PIO
	//---------------------------------------------------
	S_TIMER_READY();

	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_VALID_13 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO( eUNLOAD_1 );

			if ( nStatus != SYS_SUCCESS ) return SYS_ERROR; // Foup Present
			else { // SYS_SUCCESS, Foup Absent
				//-------------------------------------------------------------------------------
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
				//-------------------------------------------------------------------------------
				return SYS_SUCCESS;
			}
		}
		
		if ( Read_Digital_True( dvAMHS_CM_Valid , &nCommStatus ) ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP1 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if (nStatus != SYS_SUCCESS) {
					return SYS_ERROR; // Foup Present
				}
				else { // SYS_SUCCESS, Foup Absent
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}
	}

	WRITE_DIGITAL(dvAMHS_CM_U_Request, eON_1, &nCommStatus);
	S_TIMER_READY();
	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_TRREQ_14 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if (nStatus != SYS_SUCCESS) {
					return SYS_ERROR; // Foup Present
				}
				else { // SYS_SUCCESS, Foup Absent
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}

		nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED){
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

			if (nStatus != SYS_SUCCESS) {
				return SYS_ERROR; // Foup Present
			}
			else { // SYS_SUCCESS, Foup Absent
				//-------------------------------------------------------------------------------
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
				//-------------------------------------------------------------------------------
				return SYS_SUCCESS;
			}
		}
		
		if ( Read_Digital_True( dvAMHS_CM_TR_Request , &nCommStatus ) ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP1 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TRREQ_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if (nStatus != SYS_SUCCESS) {
					return SYS_ERROR; // Foup Present
				}
				else { // SYS_SUCCESS, Foup Absent
					//-------------------------------------------------------------------------------
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					//-------------------------------------------------------------------------------
					return SYS_SUCCESS;
				}
			}
			else return SYS_ABORTED;
		}
	}

	// Before TB, return SYS_ERROR, After return SYS_ABORTED
	//-------------------------------------------------------------------------------
	sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//-------------------------------------------------------------------------------

	if ( bUnClamp == 0 ) {
		//---------------------------------------------------
		// Make UnClamp
		//---------------------------------------------------
		WRITE_DIGITAL(doLP_CM_Clamp, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_CM_UnClamp, eON_1, &nCommStatus);
		
		//---------------------------------------------------
		// Send Unclamped Message
		//---------------------------------------------------
		S_TIMER_READY();
		while ( TRUE ) {
			Sleep(250);
			//======================================================================
			if ( gnControl_Refresh ) return SYS_ABORTED;
			//======================================================================
			Monitor( eMON_UNLOAD_AGV_WAIT_UNCLAMPED_12 );
			
			// CTC_FA_STATUS_MDL_CM check of under part may be skipped in unload sequence auto
			switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
			case 1 : 
			case 4 : 
				break;
			default:
				AGV_INIT_SET_IO( -2 );
				return SYS_ABORTED;
				break;
			}
			
			if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

				AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
				AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

				if ( AlmResult == ALM_IGNORE ) {

					nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

					if (nStatus != SYS_SUCCESS) {
						return SYS_ABORTED; // Foup Present
					}
					else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
				}
				else return SYS_ABORTED;
			}
			
			nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );

			if (nStatus == SYS_ABORTED) {
				return SYS_ABORTED;
			}
			else if ( nStatus == SYS_ERROR   ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			
			Result = FALSE;

			//if ( Read_Digital_Double( UnClamp_Status , Clamp_Status , &nCommStatus ) == 2 ) Result = TRUE;
			if ( READ_DIGITAL( diLP_Clamp_Status , &nCommStatus ) == 0 ) Result = TRUE;

			if ( Result ) {
				// Send Unclamp Complete
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_UNCLAMP_COMPLETE , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
				break;
			}
			
			if ( S_TIMER_CHECK( 1 , DEFAULT_TIME_OUT ) ) {

				AGV_INIT_SET_IO( -2 );
				AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_UNCLAMP_TIMEOUT );

				if      ( AlmResult == ALM_IGNORE ) { 

					nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

					if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
					else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
				}
				else return SYS_ABORTED;
			}
		}
	}
	WRITE_DIGITAL(dvAMHS_CM_Ready, eON_1, &nCommStatus);

	S_TIMER_READY();
	while( TRUE ) {
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_BUSY_15 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 :
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}

		//if (READ_DIGITAL(TM_Light_Curtain, &nCommStatus) != eOFF_0) {

		//	AGV_INIT_SET_IO( -3 ); // recovery is only ignore and abort
		//	AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TM_Light_Curtain_ON );

		//	if ( AlmResult == ALM_IGNORE ) {

		//		nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

		//		if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
		//		else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
		//	}
		//	else return SYS_ABORTED;
		//}

		nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

			if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
		}
		
		if ( Read_Digital_True( dvAMHS_CM_Busy , &nCommStatus ) ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP2 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_BUSY_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}

		Sleep(100);
	}

	S_TIMER_READY();
	while ( TRUE ) {
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_CARRIER_16 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}

		//if (READ_DIGITAL(TM_Light_Curtain, &nCommStatus) != eOFF_0) {

		//	AGV_INIT_SET_IO( -3 ); // recovery is only ignore and abort
		//	AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_TM_Light_Curtain_ON );

		//	if ( AlmResult == ALM_IGNORE ) {

		//		nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

		//		if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
		//		else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
		//	}
		//	else return SYS_ABORTED;
		//}

		if (FOUP_SENSOR_CHECK(&nCommStatus) == eABSENT2_1) break;
		
		dTime = (double) READ_DIGITAL( CTC_AMHS_TP3 , &nCommStatus );
		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_CARREMOVE_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}

		Sleep(100);
	}
	WRITE_DIGITAL(dvAMHS_CM_U_Request, eOFF_0, &nCommStatus);

	S_TIMER_READY();
	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_COMPLETE_17 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

			AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}

		nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

			if (nStatus != SYS_SUCCESS) {
				return SYS_ABORTED; // Foup Present
			}
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
		}
		
		if ( Read_Digital_True( dvAMHS_CM_Complete , &nCommStatus ) ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP4 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_COMPLETE_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}
	}

	WRITE_DIGITAL(dvAMHS_CM_Ready, eOFF_0, &nCommStatus);
	S_TIMER_READY();

	while( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_AGV_WAIT_CSOFF_19 );
		
		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 : 
			break;
		default:
			AGV_INIT_SET_IO( -2 );
			return SYS_ABORTED;
			break;
		}
		
		if ( READ_DIGITAL( dvAMHS_CM_Complete , &nCommStatus ) ) {
			if (READ_DIGITAL(dvAMHS_CM_Valid, &nCommStatus) == eOFF_0) {

				AGV_INIT_SET_IO( -2 ); // recovery is only ignore and abort
				AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_VALID_OFF );

				if ( AlmResult == ALM_IGNORE ) {

					nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

					if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
					else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
				}
				else return SYS_ABORTED;
			}
		}

		nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , TRUE , __LINE__ );

		if (nStatus == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		else if ( nStatus == SYS_ERROR   ) {

			nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

			if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
			else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
		}
		
		if ( Read_Digital_True( dvAMHS_CM_CS0 , &nCommStatus ) == 0 ) break;

		dTime = (double) READ_DIGITAL( CTC_AMHS_TP5 , &nCommStatus );

		if ( dTime <= 0 ) dTime = DEFAULT_TIME_OUT;		
		
		if ( S_TIMER_CHECK( 1 , dTime ) ) {

			AGV_INIT_SET_IO( -2 );
			AlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_CSOFF_TIMEOUT );

			if ( AlmResult == ALM_IGNORE ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) return SYS_ABORTED; // Foup Present
				else return SYS_SUCCESS; // SYS_SUCCESS, Foup Absent
			}
			else return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Load_Control() {

	Module_Status nStatus;
	int nCommStatus;
	int nAlmResult, nResult;
	int nPresent = -1, nPresent_Old = -1;
	int nMode;

	printf( "[%s] Start CM%d Load : %s\n", PROGRAM_FUNCTION_READ() , gnCassette_Index , PROGRAM_PARAMETER_READ() );

	// if operator re-load after remove a carrier in unload routine,
	// because Loadport Monitor program clear a port information, when it is reported a event, a problem can occur
	// so under code is to clear a IO.

	sprintf( gszStrCarrierID , "" ); sprintf( gszStrJobID , "" ); 
	sprintf ( gszStrPPID     , "" ); sprintf( gszStrLotID , "" );

	WRITE_STRING( CARRIER_ID + gnCassette_Index - 1 , "" , &nCommStatus );
	WRITE_STRING( CARREAD_ID + gnCassette_Index - 1 , "" , &nCommStatus );
	WRITE_STRING( JOBNAME_ID + gnCassette_Index - 1 , "" , &nCommStatus );
	WRITE_STRING( JOBREAD_ID + gnCassette_Index - 1 , "" , &nCommStatus );
	WRITE_STRING( RECIPE_ID  + gnCassette_Index - 1 , "" , &nCommStatus );
	WRITE_STRING( LOTNAME_ID + gnCassette_Index - 1 , "" , &nCommStatus );

	if ( READ_DIGITAL( CM_EVENT_SERVICE , &nCommStatus ) ) {
		sprintf( gszStrBuffer , "EVENT_ON PORT_TRANSFER_STATE CM%d|6" , gnCassette_Index ); // From Ready to RTL : Outofservice => Inservice
		WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	}
	else {
		sprintf( gszStrBuffer , "%s%d" , gszEVENT_LOAD_REQUEST , gnCassette_Index );
		WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	}
	WRITE_DIGITAL(CM_EVENT_SERVICE, eOFF_0, &nCommStatus);

	WRITE_DIGITAL(doLP_Load_Lamp, eON_1, &nCommStatus);

	//---------------------------------------------------
	// Make UnClamp : because unclamp status may be not
	//---------------------------------------------------
	WRITE_DIGITAL(doLP_CM_Clamp, eOFF_0, &nCommStatus);
	WRITE_DIGITAL(doLP_CM_UnClamp, eON_1, &nCommStatus);

	//---------------------------------------------------
	// Send Unclamped Message
	//---------------------------------------------------
	S_TIMER_READY();
	while ( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================

		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			return SYS_ABORTED;
			break;
		}
		
		nResult = FALSE;

		//if ( Read_Digital_Double( UnClamp_Status , Clamp_Status , &nCommStatus ) == 2 ) nResult = TRUE;
		if ( READ_DIGITAL( diLP_Clamp_Status , &nCommStatus ) == 0 ) nResult = TRUE;

		if ( nResult ) break;

		if ( S_TIMER_CHECK( 1 , DEFAULT_TIME_OUT ) ) {
			nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_UNCLAMP_TIMEOUT );
			if      ( nAlmResult == ALM_IGNORE ) break;
			else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
			else {
				WRITE_DIGITAL(doLP_CM_Clamp, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(doLP_CM_UnClamp, eON_1, &nCommStatus);
	
				S_TIMER_READY();
			}
		}
	}
	
	while ( TRUE ) { // 1st while() : Carrier Load
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_WAIT_CARRIER_0 );

		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			return SYS_ABORTED;
			break;
		}

		if ( gbControl_AutoMode ) { // Auto Mode
			nPresent_Old = -1;
			WRITE_DIGITAL(doLP_Load_Lamp, eOFF_0, &nCommStatus);

			nStatus = FOUP_SENSOR_ERROR( eABSENT2_1 , FALSE , __LINE__ ); // Foup check before load
			if		( nStatus == SYS_ABORTED ) return SYS_ABORTED;	// retry recovery
			else if ( nStatus == SYS_ERROR   ) {
				nStatus = AGV_RECOVERY_SCENARIO( eLOAD_0 );
				if ( nStatus != SYS_SUCCESS ) continue; // No Foup
				else { // SYS_SUCCESS, Foup Present
					sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV , gnCassette_Index );
					WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					break; // 1st while out, complete recovery
				}
			}

			if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) { // OHT arrive
				WRITE_DIGITAL(dvAMHS_CM_Moving	, eON_1, &nCommStatus); // AGV Moving

				nStatus = AGV_LOAD_CONTROL();

				WRITE_DIGITAL(dvAMHS_CM_L_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_U_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_Ready	 , eOFF_0, &nCommStatus);
				if		( nStatus == SYS_ABORTED ) {
					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle
					return SYS_ABORTED;
				}
				else if ( nStatus == SYS_ERROR ) {
					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle
					WRITE_DIGITAL(dvAMHS_CM_HostAvailable, eON_1, &nCommStatus);
					continue;
				}
//				WRITE_DIGITAL( dvAMHS_CM_Moving , OFF , &nCommStatus ); // AGV Idle, Until Load is completed

				sprintf( gszStrBuffer , "EVENT_ON LP_NOTRESERVATION2 CM%d" , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );

				while ( TRUE ) { // wait foup present
					Sleep(250);
					//======================================================================
					if ( gnControl_Refresh ) return SYS_ABORTED;
					//======================================================================
					Monitor( eMON_LOAD_WAIT_CARRIER_0 );
					
					switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
					case 1 : case 4 : break;
					default:
						return SYS_ABORTED;
						break;
					}

					if (READ_DIGITAL(diLP_CM_Present, &nCommStatus) == ePRESENT_1) {
						if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == ePRESENT_1) {
							if ( nCommStatus ) {
								// AGV Load Complete
								break;
							}
						}
					}
					
//					if( MANAGER_ABORT() ) return SYS_ABORTED;
				}
				break; // 1st while out
			}
		}
		else { // Manual Mode
			while( TRUE ) {
				if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) { // if OHT arrives in Manual Mode
					nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_AHMS_MANUAL );
					if      ( nAlmResult == ALM_IGNORE ) break;
					else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
				}
				else break;
			}
		
			nPresent = READ_DIGITAL(diLP_CM_Placed, &nCommStatus );
			if (nPresent == ePRESENT_1)	{
				WRITE_DIGITAL(dvAMHS_CM_Moving, eON_1, &nCommStatus); // AGV Moving
				if ( nCommStatus ) {
					// Manual Load Complete
					if ( nPresent != nPresent_Old ) {
						sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_LOAD_RECV , gnCassette_Index );
						WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
					}
					//
					WRITE_DIGITAL(doLP_Load_Lamp, eBLINK_2, &nCommStatus);
				}
				if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == ePRESENT_1) break;
			}
			else {
				if (nPresent_Old == ePRESENT_1) { // after foup is present, when foup is absent, to restart
					Sleep( 1000 );
					if (READ_DIGITAL(diLP_CM_Present, &nCommStatus) == eABSENT_0) {
						Sleep( 1000 );
						if (READ_DIGITAL(diLP_CM_Present, &nCommStatus) == eABSENT_0) {
							return SYS_ABORTED;
						}
					}
				}
				//
				WRITE_DIGITAL(doLP_Load_Lamp, eON_1, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Moving
			}
			nPresent_Old = nPresent;
		}

//		if ( MANAGER_ABORT() ) return SYS_ABORTED;
	} // End of 1st while()

	while( TRUE ) { // 2nd while() : Clamp Action
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_WAIT_CLAMP_1 );

		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			return SYS_ABORTED;
			break;
		}

		if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
			Sleep( 1000 );
			if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
				Sleep( 1000 );
				if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
					return SYS_ABORTED;
				}
			}
		}
		
		if ( gbControl_AutoMode ) {
			WRITE_DIGITAL(doLP_CM_UnClamp, eOFF_0, &nCommStatus);
			WRITE_DIGITAL(doLP_CM_Clamp, eON_1, &nCommStatus);
			break;
		}
		else { // Wait until Pushing Load Button
			WRITE_DIGITAL(doLP_Load_Lamp, eON_1, &nCommStatus);

			if (READ_DIGITAL(Load_Button, &nCommStatus) == eON_1) {
				WRITE_DIGITAL(doLP_CM_UnClamp, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(doLP_CM_Clamp, eON_1, &nCommStatus);
				break;	
			}
		}
		
		nResult = FALSE;

		//if ( Read_Digital_Double( UnClamp_Status , Clamp_Status , &nCommStatus ) == 1 ) nResult = TRUE;
		if ( READ_DIGITAL( diLP_Clamp_Status , &nCommStatus ) == 1 ) nResult = TRUE;

		if ( nResult ) break;
	}
	WRITE_DIGITAL(doLP_Load_Lamp, eOFF_0, &nCommStatus);
//	WRITE_DIGITAL( dvAMHS_CM_Moving	, OFF , &nCommStatus ); // AGV Moving

	nMode = 0;
	while ( TRUE ) { // 3rd while() : Wait & Check Clamped Status
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_LOAD_WAIT_CLAMP_1 );

		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : case 4 : break;
		default:
			return SYS_ABORTED;
			break;
		}
		
		if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
			Sleep( 1000 );
			if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
				Sleep( 1000 );
				if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0) {
					return SYS_ABORTED;
				}
			}
		}

		if ( nMode == 0 ) {
			if (READ_DIGITAL(doLP_CM_Clamp, &nCommStatus) == eON_1) {
				nMode = 1;
				S_TIMER_READY();
			}
			else {
				nMode = 0;
			}
		}

		nResult = FALSE;

		//if ( Read_Digital_Double( UnClamp_Status , Clamp_Status , &nCommStatus ) == 1 ) nResult = TRUE;
		if ( READ_DIGITAL( diLP_Clamp_Status , &nCommStatus ) == 1 ) nResult = TRUE;

		if ( nResult )	{
			sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_CLAMP_COMPLETE , gnCassette_Index );
			WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
			break;
		}
		
		if ( nMode == 1 ) {
			if ( S_TIMER_CHECK( 1 , DEFAULT_TIME_OUT ) ) {
				nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_CLAMP_TIMEOUT );
				if      ( nAlmResult == ALM_IGNORE ) break;
				else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
				else {
					WRITE_DIGITAL(doLP_CM_UnClamp, eOFF_0, &nCommStatus);
					WRITE_DIGITAL(doLP_CM_Clamp, eON_1, &nCommStatus);
					nMode = 0;
				}
			}
		}
	}
	WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Moving, MOVING is ON, AMHS Mode don't chagned 

	sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_LOAD_COMPLETE , gnCassette_Index );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );

	// in gbSimulation_Mode, When Clamp/LoadComplete Event happen, CarrierID isnot NULL because gbSimulation_Mode is very quick
	Sleep( 300 ); 
	
	if ( RUN_FUNCTION_for_CarrHandling( CM_MID_CONTROL , "MID_READ" ) == SYS_ABORTED ) return SYS_ABORTED; // Unload Request Happen

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Unload_Control() {

	Module_Status nStatus;
	int nCommStatus;
	int nResult, nAlmResult;
	int nMode;
	int nCancel = eOFF_0;

	printf( "[%s] Start CM%d Unload : %s\n" , PROGRAM_FUNCTION_READ() , gnCassette_Index , PROGRAM_PARAMETER_READ() );

	Sleep(100);

	// if operator re-load after remove a carrier in unload routine,
	// because Loadport Monitor program clear a port information, when it is reported a event in unload, a problem can occur
	// so under code is to set a previous IO value.
	sprintf( gszStrCarrierID , "" ); sprintf( gszStrJobID , "" ); 
	sprintf( gszStrPPID      , "" ); sprintf( gszStrLotID , "" );

	READ_STRING( CARRIER_ID + gnCassette_Index - 1 , gszStrCarrierID , &nCommStatus );
	READ_STRING( JOBNAME_ID + gnCassette_Index - 1 , gszStrJobID     , &nCommStatus );
	READ_STRING( RECIPE_ID  + gnCassette_Index - 1 , gszStrPPID      , &nCommStatus );
	READ_STRING( LOTNAME_ID + gnCassette_Index - 1 , gszStrLotID     , &nCommStatus );

	if ( READ_DIGITAL( CM_EVENT_SERVICE , &nCommStatus ) ) {
		sprintf( gszStrBuffer , "EVENT_ON PORT_TRANSFER_STATE CM%d|7" , gnCassette_Index ); // From Ready to RTU : Outofservice => Inservice
		WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	}
	else {
		sprintf( gszStrBuffer , "%s%d" , gszEVENT_UNLOAD_REQUEST , gnCassette_Index );
		WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	}

	WRITE_DIGITAL(CM_EVENT_SERVICE, eOFF_0, &nCommStatus);
	WRITE_DIGITAL(doLP_Unload_Lamp, eON_1, &nCommStatus);
	
	while ( TRUE ) { // 1st while() : Wait Unload SW or AGV
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_WAIT_UNCLAMP_2 );

		switch( READ_DIGITAL( CTC_FA_STATUS_MDL_CM , &nCommStatus ) ) {
		case 1 : 
		case 4 :
			break;
		default:
			return SYS_ABORTED;
			break;
		}

		if ( gbControl_AutoMode ) { // Auto Mode
			WRITE_DIGITAL(doLP_Unload_Lamp, eOFF_0, &nCommStatus);

			nStatus = FOUP_SENSOR_ERROR( ePRESENT2_2 , FALSE , __LINE__ ); // Foup check before unload

			if (nStatus == SYS_ABORTED) {
				return SYS_ABORTED;	// retry recovery
			}
			else if ( nStatus == SYS_ERROR   ) {

				nStatus = AGV_RECOVERY_SCENARIO(eUNLOAD_1);

				if ( nStatus != SYS_SUCCESS ) continue; // Foup Present
				else break; // SYS_SUCCESS, Foup Absent
			}

			if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) { // OHT arrive

				WRITE_DIGITAL(dvAMHS_CM_Moving, eON_1, &nCommStatus); // AGV Moving

				nStatus = AGV_UNLOAD_CONTROL( 0 );

				WRITE_DIGITAL(dvAMHS_CM_L_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_U_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_Ready, eOFF_0, &nCommStatus);

				if( nStatus == SYS_ABORTED ) {

					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle

					if (nCancel) WRITE_DIGITAL(dvOPER_CM_Cancel, eON_1, &nCommStatus);
					return SYS_ABORTED;
				}
				else if ( nStatus == SYS_ERROR ) {
					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle
					WRITE_DIGITAL(dvAMHS_CM_HostAvailable, eON_1, &nCommStatus);
					continue;
				}
//				WRITE_DIGITAL( dvAMHS_CM_Moving , OFF , &nCommStatus ); // AGV Idle, Until Load is completed

				break; // 1st while out
			}
		}
		else { // Manual Mode
			WRITE_DIGITAL(doLP_Unload_Lamp, eON_1, &nCommStatus);

			while( TRUE ) {
				if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) { // if OHT arrive in Manual Mode

					nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_AHMS_MANUAL );

					if      ( nAlmResult == ALM_IGNORE ) break;
					else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
				}
				else break;
			}


			
			if ( READ_DIGITAL( Unload_Clamp_Mode, &nCommStatus ) == eUNCLAMP_0 ) {
				WRITE_DIGITAL( Unload_Button, eON_1, &nCommStatus );
			}
			
			if (Read_Digital_True(Unload_Button, &nCommStatus) == eON_1) {

				WRITE_DIGITAL(doLP_Unload_Lamp, eBLINK_2, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_Moving, eON_1, &nCommStatus); // AGV Moving
				//
				sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_TR_UNLOAD_RECV , gnCassette_Index );
				WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );					
				WRITE_DIGITAL(doLP_CM_Clamp, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(doLP_CM_UnClamp, eON_1, &nCommStatus);
				
				nMode = 0;

				while ( TRUE ) {
					Sleep(250);
					//======================================================================
					if ( gnControl_Refresh ) return SYS_ABORTED;
					//======================================================================
					Monitor( eMON_UNLOAD_WAIT_UNCLAMP_2 );
					
					if ( nMode == 0 ) {
						if (READ_DIGITAL(doLP_CM_UnClamp, &nCommStatus) == eON_1) {
							nMode = 1;
							S_TIMER_READY();
						}
						else {
							nMode = 0;
						}
					}
					
					nResult = FALSE;
					//if (Read_Digital_Double(UnClamp_Status, Clamp_Status, &nCommStatus) == 2) nResult = TRUE;
					if ( READ_DIGITAL( diLP_Clamp_Status, &nCommStatus ) == 0 ) nResult = TRUE;

					if ( nResult ) {	// UnClamp
						sprintf( gszStrBuffer , "%s%d" , gszEVENT_STRING_UNCLAMP_COMPLETE , gnCassette_Index );
						WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
						break;
					}
					
					if ( nMode == 1 ) {
						if ( S_TIMER_CHECK( 1 , DEFAULT_TIME_OUT ) ) {

							nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_UNCLAMP_TIMEOUT );
							
							if      ( nAlmResult == ALM_IGNORE ) break;
							else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
							else {
								WRITE_DIGITAL(doLP_CM_Clamp, eOFF_0, &nCommStatus);
								WRITE_DIGITAL(doLP_CM_UnClamp, eON_1, &nCommStatus);
								nMode = 0;
							}
						}
					}
				}
				break; // 1st while out
			}
		}
	} // End of 1st while()

	while ( TRUE ) {
		Sleep(250);
		//======================================================================
		if ( gnControl_Refresh ) return SYS_ABORTED;
		//======================================================================
		Monitor( eMON_UNLOAD_WAIT_CARRIER_3 );

		if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0)	{
			if (READ_DIGITAL(diLP_CM_Present, &nCommStatus) == eABSENT_0) {
				if ( nCommStatus ) {
					// Unload Complete
					if ( gbControl_AutoMode ) break;
					else {
						Sleep(3000); // to prepare the re-placed immediately after foup remove FOUP
						if (READ_DIGITAL(diLP_CM_Placed, &nCommStatus) == eABSENT_0 &&
							READ_DIGITAL(diLP_CM_Present, &nCommStatus) == eABSENT_0) {
							break;
						}
					}
				}
			}
		}

		if ( gbControl_AutoMode ) { // Auto Mode

			if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) {

				WRITE_DIGITAL(dvAMHS_CM_Moving, eON_1, &nCommStatus); // AGV Moving

				nStatus = AGV_UNLOAD_CONTROL( 0 );

				WRITE_DIGITAL(dvAMHS_CM_L_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_U_Request, eOFF_0, &nCommStatus);
				WRITE_DIGITAL(dvAMHS_CM_Ready, eOFF_0, &nCommStatus);

				if( nStatus == SYS_ABORTED ) {
					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle
					return SYS_ABORTED;
				}
				else if ( nStatus == SYS_ERROR ) {
					WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Idle
					WRITE_DIGITAL(dvAMHS_CM_HostAvailable, eON_1, &nCommStatus);
					continue;
				}
//				WRITE_DIGITAL( dvAMHS_CM_Moving , OFF , &nCommStatus ); // AGV Idle, Until Load is completed
			}
		}
		else {			// Manual Mode
			WRITE_DIGITAL(doLP_Unload_Lamp, eBLINK_2, &nCommStatus);

			while( TRUE ) {
				if (Read_Digital_True(dvAMHS_CM_CS0, &nCommStatus) == eON_1) {

					nAlmResult = ALARM_MANAGE_for_CarrHandling( ALARM_AHMS_MANUAL );

					if      ( nAlmResult == ALM_IGNORE ) break;
					else if ( nAlmResult != ALM_RETRY  ) return SYS_ABORTED;
				}
				else break;
			}
		}

//		if ( MANAGER_ABORT() ) return SYS_ABORTED; 
	}

	WRITE_DIGITAL(doLP_Unload_Lamp, eOFF_0, &nCommStatus);
	WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus); // AGV Moving

	sprintf( gszStrBuffer , "%s%d|%s" , gszEVENT_STRING_UNLOAD_COMPLETE , gnCassette_Index , gszStrCarrierID );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//
	sprintf( gszStrBuffer , "%s%d|%s" , gszEVENT_STRING_STS_UNOCCUPIED , gnCassette_Index , gszStrCarrierID );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//
	sprintf( gszStrBuffer , "%s%d|%s" , gszEVENT_STRING_CARR_DELETED , gnCassette_Index , gszStrCarrierID );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//
	sprintf( gszStrBuffer , "%s%d|%s" , gszEVENT_STRING_STS_DELETED , gnCassette_Index , gszStrCarrierID );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );
	//
	sprintf( gszStrBuffer , "%s%d|%s" , gszEVENT_STRING_CARR_NOTASSOCIATED , gnCassette_Index , gszStrCarrierID );
	WRITE_STRING( SCHEDULER , gszStrBuffer , &nCommStatus );

	Sleep(500);

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub(){

	int nID;
	int nCommStatus;
	char szRunMsg[32]={0,}, szModule[32]={0,}, szParam[32]={0,};
	Module_Status MDLStatus;

	STR_SEPERATE(PROGRAM_PARAMETER_READ(), szRunMsg, szParam, 31);
	STR_SEPERATE(szParam, szModule, szParam, 31);

	if		(STRCMP_L(szParam, "DISABLE")) nID = 0;
	else if (STRCMP_L(szParam, "ENABLE")) nID = 1;
	else if (STRCMP_L(szParam, "ENABLE2")) nID = 2;
	else if (STRCMP_L(szParam, "ENABLE3")) nID = 3;
	else {
		printf("[%s] Not Supported Parameter [%s]\n", PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ());
		return SYS_ABORTED;
	}
	//--
	if (nID == 0) {
		gbControl_AutoMode = FALSE;
		AGV_INIT_SET_IO(-2);
	}
	else {
		gbControl_AutoMode = TRUE;
		AGV_INIT_SET_IO(-1);
	}
	//--
	WRITE_DIGITAL(dvAMHS_CM_Moving, eOFF_0, &nCommStatus);

	if (STRCMP_L(szRunMsg, "LOAD")) {

		WRITE_DIGITAL(dvOPER_CM_Cancel	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvPRCS_CM_End		, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvULOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvLOAD_CM_Signal	, eON_1	, &nCommStatus);
		WRITE_DIGITAL(doLP_Load_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Unload_Lamp	, eOFF_0, &nCommStatus);

		MDLStatus = Load_Control();

		WRITE_DIGITAL(dvULOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvLOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Load_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Unload_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_Moving	, eOFF_0, &nCommStatus);

	}
	else if (STRCMP_L(szRunMsg, "UNLOAD")) {

		WRITE_DIGITAL(dvPRCS_CM_End		, eON_1	, &nCommStatus);
		WRITE_DIGITAL(dvLOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvULOAD_CM_Signal	, eON_1	, &nCommStatus);
		WRITE_DIGITAL(doLP_Load_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Unload_Lamp	, eOFF_0, &nCommStatus);

		MDLStatus = Unload_Control();

		WRITE_DIGITAL(dvLOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvULOAD_CM_Signal	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Load_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(doLP_Unload_Lamp	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvAMHS_CM_Moving	, eOFF_0, &nCommStatus);
		WRITE_DIGITAL(dvPRCS_CM_End		, eOFF_0, &nCommStatus);

		if (MDLStatus == SYS_ABORTED) {
			WRITE_STRING(CARRIER_ID + gnCassette_Index - 1, gszStrCarrierID, &nCommStatus);
			WRITE_STRING(CARREAD_ID + gnCassette_Index - 1, gszStrCarrierID, &nCommStatus);
			WRITE_STRING(JOBNAME_ID + gnCassette_Index - 1, gszStrJobID, &nCommStatus);
			WRITE_STRING(JOBREAD_ID + gnCassette_Index - 1, gszStrJobID, &nCommStatus);
			WRITE_STRING(RECIPE_ID + gnCassette_Index - 1, gszStrPPID, &nCommStatus);
			WRITE_STRING(LOTNAME_ID + gnCassette_Index - 1, gszStrLotID, &nCommStatus);
		}

	}
	else {
		printf("[%s] Not Supported Parameter [%s]\n", PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ());
		MDLStatus = SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main() {

	Module_Status	MDLStatus;

	gnControl_Refresh = FALSE;
	Sleep(3000);
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
void Program_Enter_Code() {

	char  szModule[64]={0,}, szAlarm[64]={0,}, szIndex[64]={0,}, szArgument[64]={0,};
	
	if ( !KWIN_DIALOG_DRIVER_LOADING() ) MessageBeep(100) ;

	STR_SEPERATE(PROGRAM_ARGUMENT_READ(), szModule, szArgument, 63);
	STR_SEPERATE(szArgument, szAlarm, szIndex, 63);

	REPLACE_CHAR_SET(0, szModule);
	REPLACE_CHAR_SET(1, szIndex);

	gnCassette_Index = atoi(szIndex);
	gnALARM_START_INDEX = atoi(szAlarm);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Event_Message_Received() {

	char szEvent[32] , szParam[31];

	STR_SEPERATE(PROGRAM_EVENT_READ(), szEvent, szParam, 30);

	if      ( STRCMP_L( szEvent , "DISABLE" ) ) {
		gbControl_AutoMode = FALSE;
		AGV_INIT_SET_IO( -2 );
	}
	else if ( STRCMP_L( szEvent , "ENABLE" ) ) {
		gbControl_AutoMode = TRUE;
		AGV_INIT_SET_IO( -1 );
	}
	else if ( STRCMP_L( szEvent , "ENABLE2" ) ) {
		gbControl_AutoMode = TRUE;
		AGV_INIT_SET_IO( -1 );
	}
	else if ( STRCMP_L( szEvent , "ENABLE3" ) ) {
		gbControl_AutoMode = TRUE;
		AGV_INIT_SET_IO( -1 );
	}
	else if ( STRCMP_L( szEvent , "REFRESH" ) ) {
		gnControl_Refresh = TRUE;
	}
	else {
		gnMonitor_View = TRUE;
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code() {

	int nCommStatus;

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
