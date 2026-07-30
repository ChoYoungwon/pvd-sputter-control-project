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
//#include "kutlCheck.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define		ALARM_COMM_ERROR				gnALARM_START_INDEX + 1
#define		ALARM_TIMEOUT					gnALARM_START_INDEX + 2
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eOff_0, eOn_1 };
enum { Off, On };
enum { eHoming, eNone };
enum { eIdle, eBusy };

enum { eOpen, eClose };

enum {
	ATM,
	Vacuum,
	GoATM,
	GoVacuum,
	Abort
}VACSTS;

enum {
	Unuse,	
	UnInit,	
	Go_Init,	
	Maint,
	Go_Standby, 
	Standby, 
	Go_Maint, 
	CtcInUse, 
	CtcMaint, 
	RunProcess, 
	RunMaint, 
	OffLine,
}VACSTS;

BOOL		gbSIMULATION_MODE	= FALSE;
int			gnALARM_START_INDEX = 0;

enum {
	WAIT_TIME1 = 1,
	WAIT_TIME2 = 2,

	WAIT_TIME20 = 20,

	WAIT_TIME30 = 30,
	WAIT_TIME60 = 60,

	WAIT_TIME120 = 120,
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// 하위 Function 
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{ "IO_Driver_Loding"					, _K_D_IO	,	IO_Driver_Loding						,	0	} ,
	
	{ "SEQ_SPUTTER_DEVICE_CONTROL"			, _K_F_IO	,	SeqSputterDeviceControl					,	0	},

	{ "dvHandShakeControlMode"				, _K_D_IO	,	HandShakeMode							,	0	} ,
	{ "PM2.dvUpperSendAble"					, _K_D_IO	,	UpperSendAble							,	0	} ,
	{ "PM2.dvUpperSendStart"				, _K_D_IO	,	UpperSendStart							,	0	} ,
	{ "PM2.dvUpperSendComplete"				, _K_D_IO	,	UpperSendComplete							,	0	} ,
	{ "PM2.dvLowerRecvAble"					, _K_D_IO	,	LowerRecvAble							,	0	} ,
	{ "PM2.dvLowerRecvStart"				, _K_D_IO	,	LowerRecvStart						,	0	} ,

	{ "PM2.dvLowerRecvComplete"				, _K_D_IO	,	LowerRecvComplete							,	0	} ,

	{	"diPM2GateUpSen"					, _K_D_IO	,	PM2GateCloseSensor			,	0	} ,
	{	"diPM2GateDownSen"					, _K_D_IO	,	PM2GateOpenSensor			,	0	} ,
	{	"doPM2Gate"							, _K_D_IO	,	PM2GateControl				,	0	} ,
	

	{	"avForelinePressure"				, _K_A_IO	,	GaugeForelinePressure				,	0	} ,
	{	"avCvgGaugePressure"				, _K_A_IO	,	avCvgGaugePressure				,	0	} ,
	{	"doPVDTurboReady"					, _K_D_IO	,	doPVDTurboReady				,	0	} ,

	{	"avIonGaugePressure"				, _K_A_IO	,	avIonGaugePressure				,	0	} ,
	{	"avPN2GasFlowPV"					, _K_A_IO	,	avPN2GasFlowPV				,	0	} ,
	{	"avTurboRPM"						, _K_A_IO	,	avTurboRPM				,	0	} ,


	{	"PM2.dvVacuumStatus"				, _K_D_IO	,	dvVacuumStatus				,	0	} ,
	{	"CTC.PM2_ControlStatus"			, _K_D_IO	,	ControlStatus				,	0	} ,


	{	"doPVDVacuum_LED"					, _K_D_IO	,	doPVDVacuum_LED				,	0	} ,
	{	"doPVDIdle_LED"						, _K_D_IO	,	doPVDIdle_LED				,	0	} ,
	{	"doPVDProcess_LED"					, _K_D_IO	,	doPVDProcess_LED				,	0	} ,



	""
};

Module_Status PM2_Home() {

	printf("[Debug] SeqPM2Control Home");

	return SYS_SUCCESS;
}

Module_Status PM2_Stop() {

	int nCommstatus;

	printf("[Debug] SeqPM2Control Stop");

	return SYS_SUCCESS;
}

Module_Status PM2_Idle() {

	printf("[Debug] SeqPM2Control Idle");

	return SYS_SUCCESS;
}

Module_Status PM2_Init() {

	printf("[Debug] SeqPM2Control Init");


	if (PM2_Home() == SYS_SUCCESS) {

	}

	return SYS_SUCCESS;
}

Module_Status PM2_Standby() {
	return SYS_SUCCESS;
}

Module_Status PumpDown() {

	double pressure;
	BOOL flag = FALSE;
	int commStatus, TurboReady;

	if (READ_DIGITAL(dvVacuumStatus, &commStatus) == Vacuum) return SYS_SUCCESS;

	WRITE_DIGITAL(doPVDIdle_LED, Off, &commStatus);
	WRITE_DIGITAL(dvVacuumStatus, GoVacuum, &commStatus);

	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "GAS_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "HIVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;

	if (RUN_FUNCTION(SeqSputterDeviceControl, "DRY_PUMP_ON") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "FORELINE_VALVE_OPEN") == SYS_ABORTED) return SYS_ABORTED;

	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pressure = READ_ANALOG(GaugeForelinePressure, &commStatus);
		if (pressure <= 1.0) {
			if (!flag) { flag = TRUE; S_TIMER_READY(); }
			else {
				if (S_TIMER_CHECK(1, WAIT_TIME2)) break;
			}
		}

		//printf("S_Timer_ELAPSED Check %lf\n", S_TIMER_ELAPSED);
		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] FORELINE_VALVE_OPEN Time Out\n");
			return SYS_ABORTED;
		}
	}

	if (RUN_FUNCTION(SeqSputterDeviceControl, "TURBO_PUMP_ON") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_OPEN") == SYS_ABORTED) return SYS_ABORTED;

	pressure = 0.0;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pressure = READ_ANALOG(avCvgGaugePressure, &commStatus);
		TurboReady = READ_DIGITAL(doPVDTurboReady, &commStatus);

		printf("[Debug PM2 Control] TurboReady %d\n", TurboReady);

		if ((pressure <= 1.0) && (TurboReady == On)) break;

		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] LOWVAC_VALVE_OPEN Time Out\n");
			return SYS_ABORTED;
		}
	}

	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "HIVAC_VALVE_OPEN") == SYS_ABORTED) return SYS_ABORTED;

	pressure = 0.0;
	flag = FALSE;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pressure = READ_ANALOG(avIonGaugePressure, &commStatus);
		if (pressure <= 1.0 * pow(10, -6)) {
			if (!flag) { flag = TRUE; S_TIMER_READY(); }
			else {
				if (S_TIMER_CHECK(1, WAIT_TIME2)) { 
					S_TIMER_READY(); 
					break;
				}
			}
		} 
		if (S_TIMER_ELAPSED() > WAIT_TIME60) {
			printf("[Debug PM2 Control] HIVAC_VALVE_OPEN Time Out\n");
			return SYS_ABORTED;
		}
	}

	WRITE_DIGITAL(dvVacuumStatus, Vacuum, &commStatus);
	WRITE_DIGITAL(doPVDVacuum_LED, On, &commStatus);
	WRITE_DIGITAL(doPVDIdle_LED, On, &commStatus);

	return SYS_SUCCESS;
}

Module_Status Vent() {

	double pv, pressure;
	int RPM;
	BOOL flag = FALSE;
	int commStatus;

	if (READ_DIGITAL(dvVacuumStatus, &commStatus) == ATM) return SYS_SUCCESS;

	WRITE_DIGITAL(dvVacuumStatus, GoATM, &commStatus);

	if (RUN_FUNCTION(SeqSputterDeviceControl, "GAS_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "HIVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;

	if (RUN_FUNCTION(SeqSputterDeviceControl, "TURBO_PUMP_OFF") == SYS_ABORTED) return SYS_ABORTED;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		RPM = READ_ANALOG(avTurboRPM, &commStatus);
		if (RPM == 0) break;
		
		//printf("S_Timer_ELAPSED Check %lf\n", S_TIMER_ELAPSED);
		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] TURBO_PUMP_OFF Time Out\n");
			return SYS_ABORTED;
		}
	}

	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_SLOW_OPEN 100") == SYS_ABORTED) return SYS_ABORTED;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pv = READ_ANALOG(avPN2GasFlowPV, &commStatus);
		pressure = READ_ANALOG(avCvgGaugePressure, &commStatus);

		if (pv > 90.0 || pv < 110.0) {
			if (pressure >= (1.0 * pow(10, -6) + 760.0) / 2)
				break;
		}
		
		//printf("S_Timer_ELAPSED Check %lf\n", S_TIMER_ELAPSED);
		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] VENT_SLOW_OPEN 100 Time Out\n");
			return SYS_ABORTED;
		}
	}

	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_FAST_OPEN 500") == SYS_ABORTED) return SYS_ABORTED;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pv = READ_ANALOG(avPN2GasFlowPV, &commStatus);
		pressure = READ_ANALOG(avCvgGaugePressure, &commStatus);

		if (pv > 450.0 || pv <= 550.0) {
			if(pressure >= 760.0)	break;
		}

		//printf("S_Timer_ELAPSED Check %lf\n", S_TIMER_ELAPSED);
		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] VENT_FAST_OPEN 500 Time Out\n");
			return SYS_ABORTED;
		}
	}
	
	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pv = READ_ANALOG(avPN2GasFlowPV, &commStatus);

		if (pv >= -1.0 || pv <= 1.0) break;

		//printf("S_Timer_ELAPSED Check %lf\n", S_TIMER_ELAPSED);
		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] VENT_FAST_OPEN 500 Time Out\n");
			return SYS_ABORTED;
		}
	}

	WRITE_DIGITAL(dvVacuumStatus, ATM, &commStatus);
	WRITE_DIGITAL(doPVDVacuum_LED, Off, &commStatus);
	WRITE_DIGITAL(doPVDIdle_LED, On, &commStatus);

	return SYS_SUCCESS;
}

Module_Status Process() {
	int commStatus;
	double pressure;

	WRITE_DIGITAL(ControlStatus, RunProcess, &commStatus);
	WRITE_DIGITAL(doPVDIdle_LED, Off, &commStatus);

	if (READ_DIGITAL(dvVacuumStatus, &commStatus) != Vacuum) 
		if (PumpDown() == SYS_ABORTED) return SYS_ABORTED;

	WRITE_DIGITAL(doPVDProcess_LED, On, &commStatus);

	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "HIVAC_VALVE_OPEN") == SYS_ABORTED) return SYS_ABORTED;
	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pressure = READ_ANALOG(avIonGaugePressure, &commStatus);
		if (pressure <= 1.0 * pow(10, -6)) break;

		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] Process HIVAC_VALVE_OPEN Time Out\n");
			return SYS_ABORTED;
		}
	}

	// 1.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "GAS_VALVE_OPEN 50") == SYS_ABORTED) return SYS_ABORTED;

	// 2.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_AR_FLOW") == SYS_ABORTED) return SYS_ABORTED;

	// 3.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_AR_PROCESS_PRESSURE 0.005") == SYS_ABORTED) return SYS_ABORTED;

	// 4.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "DC_POWER_RAMP_UP 1000,10") == SYS_ABORTED) return SYS_ABORTED;

	// 5.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_DC_POWER") == SYS_ABORTED) return SYS_ABORTED;

	// 6.PROCESS HOLD

	// END
	// 7.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "DC_POWER_RAMP_DOWN 5") == SYS_ABORTED) return SYS_ABORTED;

	// 8.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_DC_POWER 0") == SYS_ABORTED) return SYS_ABORTED;

	// 9.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "GAS_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;

	// 10.
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_AR_FLOW 0") == SYS_ABORTED) return SYS_ABORTED;

	WRITE_DIGITAL(ControlStatus, Maint, &commStatus);
	WRITE_DIGITAL(doPVDProcess_LED, Off, &commStatus);
	WRITE_DIGITAL(doPVDIdle_LED, On, &commStatus);

	return SYS_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() {
	int nCommStatus = 0;
	unsigned char *szParameter;
	char szRunMsg[32]={0,}, szParam1[32]={0,}, szParam2[32] = { 0, }, szParam3[32] = { 0, }, szBuffer[32] = { 0, };
	double dbSetPos, dbSetSpd, dbSetFlow;
	Module_Status MDLStatus = SYS_SUCCESS;

	szParameter = PROGRAM_PARAMETER_READ();

	STR_SEPERATE( szParameter, szRunMsg, szBuffer, 31 );	
	STR_SEPERATE( szBuffer, szParam1, szBuffer, 31 );
	STR_SEPERATE(szBuffer, szParam2, szParam3, 31);
	
	dbSetPos = (double)atoi(szParam1);
	dbSetSpd = (double)atoi(szParam2);
	dbSetFlow = (double)atoi(szParam3);

	printf("[Debug] SeqPM2Control start, szRunMsg : %s, szParam1 : %s, szParam2 : %s \n", 
		szRunMsg, szParam1, szParam2);
		
	if (STRCMP_L(szRunMsg, "HOME")) { MDLStatus = PM2_Home(); }
	else if (STRCMP_L(szRunMsg, "STOP")) { MDLStatus = PM2_Stop(); }
	else if (STRCMP_L(szRunMsg, "IDLE")) { MDLStatus = PM2_Idle(); }

	else if (STRCMP_L(szRunMsg, "GO_INIT")) { MDLStatus = PM2_Init(); }
	else if (STRCMP_L(szRunMsg, "GO_STANDBY")) { MDLStatus = PM2_Standby(); }

	// 그대로 카피
	else if (STRCMP_L(szRunMsg, "SEND_ABLE")) {
		WRITE_DIGITAL(UpperSendComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendAble, eOn_1, &nCommStatus);
		WRITE_DIGITAL(UpperSendStart, eOn_1, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "SEND_COMP")) {
		WRITE_DIGITAL(UpperSendComplete, eOn_1, &nCommStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(UpperSendAble, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendStart, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendComplete, eOff_0, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "RECV_ABLE")) {
		WRITE_DIGITAL(LowerRecvComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvAble, eOn_1, &nCommStatus);
		WRITE_DIGITAL(LowerRecvStart, eOn_1, &nCommStatus);

	}
	else if (STRCMP_L(szRunMsg, "RECV_COMP")) {
		WRITE_DIGITAL(LowerRecvComplete, eOn_1, &nCommStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(LowerRecvAble, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvStart, eOff_0, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "OPEN_GATE")) {
		WRITE_DIGITAL(PM2GateControl, eOpen, &nCommStatus);

		while (TRUE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (READ_DIGITAL(PM2GateOpenSensor, &nCommStatus) == eOn_1 &&
				READ_DIGITAL(PM2GateCloseSensor, &nCommStatus) == eOff_0) return SYS_SUCCESS;
		}
		WRITE_DIGITAL(LowerRecvAble, eOn_1, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "CLOSE_GATE")) {
		if (!WAIT_SECONDS(2)) return SYS_ABORTED;
		WRITE_DIGITAL(PM2GateControl, eClose, &nCommStatus);

		while (TRUE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (READ_DIGITAL(PM2GateOpenSensor, &nCommStatus) == eOff_0 &&
				READ_DIGITAL(PM2GateCloseSensor, &nCommStatus) == eOn_1) return SYS_SUCCESS;
		}
		WRITE_DIGITAL(LowerRecvAble, eOff_0, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "PUMP_DOWN"))	return PumpDown();
	else if (STRCMP_L(szRunMsg, "VENT"))	return Vent();
	else if (STRCMP_L(szRunMsg, "PROCESS"))	return Process();
	else {
		printf( "[%s] Not Supported Parameter[%s]\n" , PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ() );
		return SYS_ABORTED;
	}
	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main() {
	Module_Status MDLStatus;

	//==============================================================================================================
	//_iLOG_PRINTF( "  =>> START [%s]\n" , PROGRAM_PARAMETER_READ() );
	//==============================================================================================================

	MDLStatus = Program_Main_Sub();

	//==============================================================================================================
	//switch( MDLStatus ) {
	//	case SYS_SUCCESS	:	_iLOG_PRINTF( "  <<= END [%s] is SUCCESS\n" , PROGRAM_PARAMETER_READ() );			break;
	//	case SYS_ABORTED	:	_iLOG_PRINTF( "  <<= END [%s] is ABORTED\n" , PROGRAM_PARAMETER_READ() );			break;
	//	case SYS_ERROR		:	_iLOG_PRINTF( "  <<= END [%s] is ERROR\n"   , PROGRAM_PARAMETER_READ() );			break;
	//	default				:	_iLOG_PRINTF( "  <<= END [%s] is [%d]\n"    , PROGRAM_PARAMETER_READ() , MDLStatus );break;
	//}
	//==============================================================================================================

	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Program_Enter_Code() {

	char szModule[16]={0,}, szArgument[16]={0,};

	STR_SEPERATE( PROGRAM_ARGUMENT_READ(), szModule, szArgument, 15 );

	REPLACE_CHAR_SET( 0 , szModule );
	gnALARM_START_INDEX = atoi( szArgument );
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code() {
	int nCommStatus ;
	
	if (READ_DIGITAL(IO_Driver_Loding, &nCommStatus) == 0)	gbSIMULATION_MODE = TRUE;
	else													gbSIMULATION_MODE = FALSE;

	KWIN_DIALOG_DRIVER_LOADING();

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------