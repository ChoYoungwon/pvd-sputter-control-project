#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
#include "iodefine.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define TIMEOUT 10

#define CM1_EMPTY					155
#define GLASS_PRESENT_IN_CM			157
#define GLASS_EMPTY_IN_CM			158
#define GLASS_PRESENT_IN_ROBOT_HAND	165
#define GLASS_EMPTY_IN_ROBOT_HAND	166
#define GLASS_PRESENT_IN_PM1		167
#define GLASS_EMPTY_IN_PM1			168
#define GLASS_PRESENT_IN_PM2		169
#define GLASS_EMPTY_IN_PM2			170
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eOFF_0, eON_1 };
enum { Cassette, PM1, PM2 };

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL	_simulationMode = FALSE;

char	_moduleName[32] = { 0, };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{	"IO_Driver_Loding"			, _K_D_IO	,	IO_Driver_Loading		,	0	} ,

	{	"SCHEDULER"					, _K_F_IO + 1,	SCHEDULER				,	0	} ,

	{	"SEQ_ROBOT_MOTOR"			, _K_F_IO	,	SeqRobotMotor			,	0	} ,
	{	"SEQ_ROBOT_SIMULATION"		, _K_F_IO	,	SeqRobotSimulation		,	0	} ,
	{	"SEQ_PM1_DEVELOPER_CONTROL"	, _K_F_IO	,	SeqPM1DeveloperControl	,	0	} ,
	{	"SEQ_PM2_CHAMBER_CONTROL"	, _K_F_IO	,	SeqPM2ChamberControl	,	0	} ,

	{	"diLP_CM1_Placed"			, _K_D_IO	,	CM1Placed				,	0	} ,
	{	"CTC.TA_Wafer_Status"		, _K_D_IO	,	LowerArmWaferStatus		,	0	} ,
	{	"CTC.TB_Wafer_Status"		, _K_D_IO	,	UpperArmWaferStatus		,	0	} ,
	{	"CM1.C01_Wafer"				, _K_D_IO	,	CM1_C01_Wafer			,	0	} ,
	{	"CTC.PM1_Wafer_Status"		, _K_D_IO	,	PM1_Wafer_Status		,	0	} ,
	{	"CTC.PM2_Wafer_Status"		, _K_D_IO	,	PM2_Wafer_Status		,	0	} ,

	{	"TM.RB_SYNCH"				, _K_D_IO	,	TM_RB_SYNCH				,	0	} ,

	{	"dvHandShakeControlMode"		, _K_D_IO	,	HandShakeMode		,	0	} ,
	{	"Indexer.dvUpperSendAble"		, _K_D_IO	,	UpperSendAble		,	0	} ,
	{	"Indexer.dvUpperSendStart"		, _K_D_IO	,	UpperSendStart		,	0	} ,
	{	"Indexer.dvUpperSendComplete"	, _K_D_IO	,	UpperSendComplete	,	0	} ,
	{	"Indexer.dvLowerRecvAble"		, _K_D_IO	,	LowerRecvAble		,	0	} ,
	{	"Indexer.dvLowerRecvStart"		, _K_D_IO	,	LowerRecvStart		,	0	} ,
	{	"Indexer.dvLowerRecvComplete"	, _K_D_IO	,	LowerRecvComplete	,	0	} ,

	{	"PM1.dvLowerRecvStart"			, _K_D_IO	,	LowerRecvStart_PM1	,	0	} ,
	{	"PM1.dvLowerRecvComplete"		, _K_D_IO	,	LowerRecvComplete_PM1	,	0	} ,
	{	"PM1.dvUpperSendAble"			, _K_D_IO	,	UpperSendAble_PM1	,	0	} ,
	{	"PM1.dvUpperSendStart"			, _K_D_IO	,	UpperSendStart_PM1	,	0	} ,

	{	"PM2.dvLowerRecvStart"			, _K_D_IO	,	LowerRecvStart_PM2	,	0	} ,
	{	"PM2.dvLowerRecvComplete"		, _K_D_IO	,	LowerRecvComplete_PM2	,	0	} ,
	{	"PM2.dvUpperSendAble"			, _K_D_IO	,	UpperSendAble_PM2	,	0	} ,
	{	"PM2.dvUpperSendStart"			, _K_D_IO	,	UpperSendStart_PM2	,	0	} ,
	""
};

Module_Status CheckBeforePick(char* stationName)
{
	int commStatus;

	if (READ_DIGITAL(LowerArmWaferStatus, &commStatus) != 0)
	{
		AlarmStyle style = ALARM_MANAGE(GLASS_PRESENT_IN_ROBOT_HAND);
		if (style == ALM_RETRY) CheckBeforePick(stationName);
		else if (style == ALM_ABORT) return SYS_ABORTED;
	}

	if (STRCMP_L(stationName, "CM1") || STRCMP_L(stationName, "A_CM1"))
	{
		// Check 1. CM1 is empty or not......
		if (READ_DIGITAL(CM1Placed, &commStatus) == eOFF_0)
		{
			AlarmStyle style = ALARM_MANAGE(CM1_EMPTY);
			if (style == ALM_RETRY) CheckBeforePick(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}

		if (READ_DIGITAL(CM1_C01_Wafer, &commStatus) == 1)
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_EMPTY_IN_CM);
			if (style == ALM_RETRY) CheckBeforePick(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}
	else if (STRCMP_L(stationName, "PM1") || STRCMP_L(stationName, "A_PM1"))
	{
		if (READ_DIGITAL(PM1_Wafer_Status, &commStatus) == 0)
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_EMPTY_IN_PM1);
			if (style == ALM_RETRY) CheckBeforePick(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}
	else if (STRCMP_L(stationName, "PM2") || STRCMP_L(stationName, "A_PM2"))
	{
		if (READ_DIGITAL(PM2_Wafer_Status, &commStatus) == 0)
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_EMPTY_IN_PM2);
			if (style == ALM_RETRY) CheckBeforePick(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}

Module_Status CheckBeforePlace(char* stationName)
{
	int commStatus;

	if (READ_DIGITAL(LowerArmWaferStatus, &commStatus) == 0)
	{
		AlarmStyle style = ALARM_MANAGE(GLASS_EMPTY_IN_ROBOT_HAND);
		if (style == ALM_RETRY) CheckBeforePlace(stationName);
		else if (style == ALM_ABORT) return SYS_ABORTED;
	}

	if (STRCMP_L(stationName, "CM1") || STRCMP_L(stationName, "A_CM1"))
	{
		// Check 1. CM1 is empty or not......
		if (READ_DIGITAL(CM1Placed, &commStatus) == eOFF_0)
		{
			AlarmStyle style = ALARM_MANAGE(CM1_EMPTY);
			if (style == ALM_RETRY) CheckBeforePlace(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}

		if (READ_DIGITAL(CM1_C01_Wafer, &commStatus) != 1) // 1 : Absent
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_PRESENT_IN_CM);
			if (style == ALM_RETRY) CheckBeforePlace(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}
	else if (STRCMP_L(stationName, "PM1") || STRCMP_L(stationName, "A_PM1"))
	{
		if (READ_DIGITAL(PM1_Wafer_Status, &commStatus) != 0)
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_PRESENT_IN_PM1);
			if (style == ALM_RETRY) CheckBeforePlace(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}
	else if (STRCMP_L(stationName, "PM2") || STRCMP_L(stationName, "A_PM2"))
	{
		if (READ_DIGITAL(PM2_Wafer_Status, &commStatus) != 0)
		{
			AlarmStyle style = ALARM_MANAGE(GLASS_PRESENT_IN_PM2);
			if (style == ALM_RETRY) CheckBeforePlace(stationName);
			else if (style == ALM_ABORT) return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status PickRobot(char* stationName)
{
	int commStatus;
	char message[32];

	if (!STRCMP_L(stationName, "CM1") && !STRCMP_L(stationName, "PM1") &&
		!STRCMP_L(stationName, "A_CM1") && !STRCMP_L(stationName, "A_PM1") &&
		!STRCMP_L(stationName, "PM2") && !STRCMP_L(stationName, "A_PM2"))
	{
		return SYS_ABORTED;
	}

	if (CheckBeforePick(stationName) == SYS_ABORTED) return SYS_ABORTED;

	if (!STRCMP_L(stationName, "CM1") && !STRCMP_L(stationName, "A_CM1")) WRITE_DIGITAL(LowerRecvStart, eON_1, &commStatus);

	WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_RETRACT_START_4, &commStatus);
	sprintf(message, "PICK %s 1", stationName);
	RUN_SET_FUNCTION(SeqRobotSimulation, message);

	sprintf(message, "PICK %s", stationName);  // message == "PICK PM1"
	if (RUN_FUNCTION(SeqRobotMotor, message) == SYS_ABORTED) return SYS_ABORTED;

	while (TRUE) {
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_READY_0, &commStatus);
			return SYS_ABORTED;
		}
		if (READ_FUNCTION(SeqRobotSimulation) != SYS_RUNNING)
			break;
	}

	WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_SUCCESS_7, &commStatus);

	return SYS_SUCCESS;
}

Module_Status PlaceRobot(char* stationName)
{
	int commStatus;
	char message[32];

	if (!STRCMP_L(stationName, "CM1") && !STRCMP_L(stationName, "PM1") &&
		!STRCMP_L(stationName, "A_CM1") && !STRCMP_L(stationName, "A_PM1") &&
		!STRCMP_L(stationName, "PM2") && !STRCMP_L(stationName, "A_PM2"))
	{
		return SYS_ABORTED;
	}

	if (CheckBeforePlace(stationName) == SYS_ABORTED) return SYS_ABORTED;

	if (!STRCMP_L(stationName, "CM1") && !STRCMP_L(stationName, "A_CM1")) WRITE_DIGITAL(UpperSendStart, eON_1, &commStatus);

	WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_RETRACT_START_4, &commStatus);
	sprintf(message, "PLACE %s 1", stationName);
	RUN_SET_FUNCTION(SeqRobotSimulation, message);

	sprintf(message, "PLACE %s", stationName);
	if (RUN_FUNCTION(SeqRobotMotor, message) == SYS_ABORTED) return SYS_ABORTED;

	while (TRUE) {
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_READY_0, &commStatus);
			return SYS_ABORTED;
		}
		if (READ_FUNCTION(SeqRobotSimulation) != SYS_RUNNING)
			break;
	}

	WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_SUCCESS_7, &commStatus);

	return SYS_SUCCESS;
}

Module_Status HomeRobot()
{
	int commStatus;
	WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_RETRACT_START_4, &commStatus);
	RUN_SET_FUNCTION(SeqRobotSimulation, "HOME");

	if (RUN_FUNCTION(SeqRobotMotor, "HOME") == SYS_SUCCESS)
	{
		WRITE_DIGITAL(TM_RB_SYNCH, eROBOT_SYNCH_SUCCESS_7, &commStatus);
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main()
{
	char param1[16] = { 0, }, param2[16] = { 0, }, param3[16] = { 0, }, param4[16] = { 0, };
	int commStatus;

	printf("Normal Message : %s\n", PROGRAM_PARAMETER_READ());

	//"MANUAL_MOVE	CTC.TR_SRC_STATION	CTC.TR_SRC_SLOT	CTC.TR_TRG_STATION	CTC.TR_TRG_SLOT"
	//"PICK A_CM1 1 TRANSFER"
	STR_SEPERATE(PROGRAM_PARAMETER_READ(), param1, param2, 15);
	//param1 : "MANUAL_MOVE"
	//param2 : "CTC.TR_SRC_STATION	CTC.TR_SRC_SLOT	CTC.TR_TRG_STATION	CTC.TR_TRG_SLOT"
	STR_SEPERATE(param2, param3, param4, 15);
	//param3 : "CTC.TR_SRC_STATION"
	//param4 : "CTC.TR_SRC_SLOT	CTC.TR_TRG_STATION	CTC.TR_TRG_SLOT"

	if (STRCMP_L(param1, "MANUAL_PICK") || STRCMP_L(param1, "PICK"))
	{
		while (TRUE)
		{
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) break;
			else if (STRCMP_L(param3, "PM1") || STRCMP_L(param3, "A_PM1")) {
				if (READ_DIGITAL(UpperSendAble_PM1, &commStatus) == eON_1) break;
			}
			else if (STRCMP_L(param3, "PM2") || STRCMP_L(param3, "A_PM2")) {
				if (READ_DIGITAL(UpperSendAble_PM2, &commStatus) == eON_1) break;
			}
		}
		return PickRobot(param3);
	}
	else if (STRCMP_L(param1, "MANUAL_PLACE") || STRCMP_L(param1, "PLACE"))
	{
		while (TRUE)
		{
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) break;
			else if (STRCMP_L(param3, "PM1") || STRCMP_L(param3, "A_PM1")) {
				if (READ_DIGITAL(LowerRecvStart_PM1, &commStatus) == eON_1) break;
			}
			else if (STRCMP_L(param3, "PM2") || STRCMP_L(param3, "A_PM2")) {
				if (READ_DIGITAL(LowerRecvStart_PM2, &commStatus) == eON_1) break;
			}
		}
		return PlaceRobot(param3);
	}
	else if (STRCMP_L(param1, "HOME"))
	{
		return HomeRobot();
	}
	else if (STRCMP_L(param1, "MANUAL_MOVE"))
	{
		if (PickRobot(param3) == SYS_ABORTED) return SYS_ABORTED;
		STR_SEPERATE(param4, param2, param3, 15);
		//param2 : "CTC.TR_SRC_SLOT"
		//param3 : "CTC.TR_TRG_STATION	CTC.TR_TRG_SLOT"
		STR_SEPERATE(param3, param2, param4, 15);
		//param2 : "CTC.TR_TRG_STATION"
		//param4 : "CTC.TR_TRG_SLOT"
		return PlaceRobot(param2);
	}
	else if (STRCMP_L(param1, "SEND_ABLE"))
	{
		if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) return SYS_SUCCESS;
		WRITE_DIGITAL(UpperSendStart, eOFF_0, &commStatus);
		WRITE_DIGITAL(UpperSendComplete, eOFF_0, &commStatus);
		WRITE_DIGITAL(UpperSendAble, eON_1, &commStatus);
	}
	else if (STRCMP_L(param1, "SEND_COMP"))
	{
		if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) return SYS_SUCCESS;

		WRITE_DIGITAL(UpperSendComplete, eON_1, &commStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(UpperSendStart, eOFF_0, &commStatus);
		WRITE_DIGITAL(UpperSendComplete, eOFF_0, &commStatus);
		WRITE_DIGITAL(UpperSendAble, eOFF_0, &commStatus);
	}
	else if (STRCMP_L(param1, "RECV_ABLE"))
	{
		if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) return SYS_SUCCESS;
		WRITE_DIGITAL(LowerRecvStart, eOFF_0, &commStatus);
		WRITE_DIGITAL(LowerRecvComplete, eOFF_0, &commStatus);
		WRITE_DIGITAL(LowerRecvAble, eON_1, &commStatus);
	}
	else if (STRCMP_L(param1, "RECV_COMP"))
	{
		if (STRCMP_L(param3, "CM1") || STRCMP_L(param3, "A_CM1")) return SYS_SUCCESS;

		WRITE_DIGITAL(LowerRecvComplete, eON_1, &commStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(LowerRecvStart, eOFF_0, &commStatus);
		WRITE_DIGITAL(LowerRecvComplete, eOFF_0, &commStatus);
		WRITE_DIGITAL(LowerRecvAble, eOFF_0, &commStatus);
	}
	else if (STRCMP_L(param1, "OPEN_GATE") || STRCMP_L(param1, "CLOSE_GATE"))
	{
		if (STRCMP_L(param3, "PM1") || STRCMP_L(param3, "A_PM1"))
			return RUN_FUNCTION(SeqPM1DeveloperControl, PROGRAM_PARAMETER_READ());
		else if (STRCMP_L(param3, "PM2") || STRCMP_L(param3, "A_PM2"))
			return RUN_FUNCTION(SeqPM2ChamberControl, PROGRAM_PARAMETER_READ());
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Program_Enter_Code()
{
	char arg1[16] = { 0, }, arg2[16] = { 0, }, arg3[16] = { 0, }, arg4[16] = { 0, }, arg5[16] = { 0, };

	sprintf(arg1, PROGRAM_ARGUMENT_READ());

	STR_SEPERATE(PROGRAM_ARGUMENT_READ(), arg2, arg3, 15);
	STR_SEPERATE(arg3, arg4, arg5, 15);

	//strcat(_moduleName, arg1);

	printf("Enter [%s], [%s], [%s]\n", arg1, arg2, arg3);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code()
{
	int commStatus;

	//if (READ_DIGITAL(IO_Driver_Loading, &commStatus) == 0)	_simulationMode = TRUE;
	//else													_simulationMode = FALSE;

	SYSTEM_KILL_WITH_MESSAGE();

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
