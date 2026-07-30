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
#define		ALARM_COMM_ERROR				gnALARM_START_INDEX + 1
#define		ALARM_TIMEOUT					gnALARM_START_INDEX + 2
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eShuttle_0, eOneWay_1 };
enum { eOff_0, eOn_1 };

enum { eHoming, eNone };
enum { eIdle, eBusy };
enum { eOpen, eClose };


BOOL		gbSIMULATION_MODE = FALSE;
int			gnALARM_START_INDEX = 0;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// 하위 Function 
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{ "IO_Driver_Loding"				, _K_D_IO	,	IO_Driver_Loding						,	0	} ,

	{ "SEQ_PM1_DEVELOPER_FLOW"			, _K_F_IO	,	OBJ_DEVELOPER_FLOW						,	0	} ,

	{ "doDevNozHomeMove"				, _K_D_IO	,	doDevNoz_HomeMove						,	0	} ,
	{ "doDevNozCurrent"					, _K_D_IO	,	doDevNoz_Current						,	0	} ,
	{ "doDevNozStop"					, _K_D_IO	,	doDevNoz_Stop							,	0	} ,
	{ "doDevNozSetSpd"					, _K_D_IO	,	doDevNoz_Set_Spd						,	0	} ,
	{ "doDevNozSetHomeSpd"				, _K_D_IO	,	doDevNoz_Set_HomeSpd					,	0	} ,

	{ "diDevNozHomingSts"				, _K_D_IO	,	diDevNoz_Homing_Sts						,	0	} ,
	{ "diDevNozBusySts"					, _K_D_IO	,	diDevNoz_Busy_Sts						,	0	} ,
	{ "diDevNozHomeSen"					, _K_D_IO	,	diDevNoz_HomeSen						,	0	} ,

	{ "dpDevNoz_HomeMode"				, _K_D_IO	,	dpDevNoz_HomeMode						,	0	} ,

	{ "aoDevNozSetPos"					, _K_A_IO	,	aoDevNoz_Set_Pos						,	0	} ,
	{ "aoDevNozCurPos"					, _K_A_IO	,	aoDevNoz_Cur_Pos						,	0	} ,
	{ "aoDevNozAbsPos"					, _K_A_IO	,	aoDevNoz_Abs_Pos						,	0	} ,
	{ "aoDevNozRelPos"					, _K_A_IO	,	aoDevNoz_Rel_Pos						,	0	} ,
	{ "aoDevNozResetPos"					, _K_A_IO	,	aoDevNoz_Reset_Pos						,	0	} ,

	{ "aiDevNozCurPos"					, _K_A_IO	,	aiDevNoz_Cur_Pos						,	0	} ,
	{ "aiDevNozCmdPos"					, _K_A_IO	,	aiDevNoz_Cmd_Pos						,	0	} ,

	{ "aoDevNozSetStartSpd"				, _K_A_IO	,	aoDevNoz_Set_Start_Spd					,	0	} ,
	{ "aoDevNozSetAccSpd"				, _K_A_IO	,	aoDevNoz_Set_Acc_Spd					,	0	} ,
	{ "aoDevNozSetDecSpd"				, _K_A_IO	,	aoDevNoz_Set_Dec_Spd					,	0	} ,
	{ "aoDevNozSetDriveSpd"				, _K_A_IO	,	aoDevNoz_Set_Drive_Spd					,	0	} ,

	{ "apDevNoz_Set_Start_Spd"			, _K_A_IO	,	apDevNoz_Set_Start_Spd					,	0	} ,
	{ "apDevNoz_Set_Acc_Spd"			, _K_A_IO	,	apDevNoz_Set_Acc_Spd					,	0	} ,
	{ "apDevNoz_Set_Dec_Spd"			, _K_A_IO	,	apDevNoz_Set_Dec_Spd					,	0	} ,
	{ "apDevNoz_Set_Drive_Spd"			, _K_A_IO	,	apDevNoz_Set_Drive_Spd					,	0	} ,

	{ "aoDevNozHomeSpd0"				, _K_A_IO	,	aoDevNoz_Home_Spd0						,	0	} ,
	{ "aoDevNozHomeSpd1"				, _K_A_IO	,	aoDevNoz_Home_Spd1						,	0	} ,
	{ "aoDevNozHomeSpd2"				, _K_A_IO	,	aoDevNoz_Home_Spd2						,	0	} ,
	{ "aoDevNozOffsetSpd"				, _K_A_IO	,	aoDevNoz_Offset_Spd						,	0	} ,

	{ "apDevNoz_Home_Spd0"				, _K_A_IO	,	apDevNoz_Home_Spd0						,	0	} ,
	{ "apDevNoz_Home_Spd1"				, _K_A_IO	,	apDevNoz_Home_Spd1						,	0	} ,
	{ "apDevNoz_Home_Spd2"				, _K_A_IO	,	apDevNoz_Home_Spd2						,	0	} ,
	{ "apDevNoz_Offset_Spd"				, _K_A_IO	,	apDevNoz_Offset_Spd						,	0	} ,

	{ "apDevNoz_Start_Pos"				, _K_A_IO	,	apDevNoz_Start_Pos						,	0	} ,
	{ "apDevNoz_Edge1_Pos"				, _K_A_IO	,	apDevNoz_Edge1_Pos						,	0	} ,
	{ "apDevNoz_Center_Pos"				, _K_A_IO	,	apDevNoz_Center_Pos						,	0	} ,
	{ "apDevNoz_Edge2_Pos"				, _K_A_IO	,	apDevNoz_Edge2_Pos						,	0	} ,
	{ "apDevNoz_End_Pos"				, _K_A_IO	,	apDevNoz_End_Pos						,	0	} ,

	{ "apDevNoz_Move_Timeout"			, _K_A_IO	,	apDevNoz_Move_Timeout					,	0	} ,
	{ "apDevNoz_Swing_Timeout"			, _K_A_IO	,	apDevNoz_Swing_Timeout					,	0	} ,

	{	"dvHandShakeControlMode"		, _K_D_IO	,	HandShakeMode		,	0	} ,
	{	"PM1.dvUpperSendAble"			, _K_D_IO	,	UpperSendAble		,	0	} ,
	{	"PM1.dvUpperSendStart"			, _K_D_IO	,	UpperSendStart		,	0	} ,
	{	"PM1.dvUpperSendComplete"		, _K_D_IO	,	UpperSendComplete	,	0	} ,
	{	"PM1.dvLowerRecvAble"			, _K_D_IO	,	LowerRecvAble		,	0	} ,
	{	"PM1.dvLowerRecvStart"			, _K_D_IO	,	LowerRecvStart		,	0	} ,
	{	"PM1.dvLowerRecvComplete"		, _K_D_IO	,	LowerRecvComplete	,	0	} ,

	{	"diPM1GateUpSen"				, _K_D_IO	,	PM1GateCloseSensor	,	0	} ,
	{	"diPM1GateDownSen"				, _K_D_IO	,	PM1GateOpenSensor	,	0	} ,
	{	"doPM1Gate"						, _K_D_IO	,	PM1GateControl		,	0	} ,

	""
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status WRITE_DIGITAL_TRUE(int io, int Data) {
	int i, nCommStatus;

	for (i = 0; i < 3; i++) {
		WRITE_DIGITAL(io, Data, &nCommStatus);
		if (!nCommStatus) {
			if (!WAIT_SECONDS(0.2)) return SYS_ABORTED;
		}
		else break;
	}

	if (!nCommStatus) {
		ALARM_POST(ALARM_COMM_ERROR);
		return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status WRITE_ANALOG_TRUE(int io, double Data) {
	int i, nCommStatus;

	for (i = 0; i < 3; i++) {
		WRITE_ANALOG(io, Data, &nCommStatus);
		if (!nCommStatus) {
			if (!WAIT_SECONDS(0.2)) return SYS_ABORTED;
		}
		else break;
	}

	if (!nCommStatus) {
		ALARM_POST(ALARM_COMM_ERROR);
		return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status READ_ANALOG_TRUE(int io, double* Data) {
	int i, nCommStatus;

	for (i = 0; i < 3; i++) {
		*Data = READ_ANALOG(io, &nCommStatus);
		if (!nCommStatus) {
			if (!WAIT_SECONDS(0.2)) return SYS_ABORTED;
		}
		else break;
	}

	if (!nCommStatus) {
		ALARM_POST(ALARM_COMM_ERROR);
		return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Speed_Setting(double StartSpd, double AccSpd, double DecSpd, double DriveSpd) {

	int nCommstatus;

	WRITE_ANALOG(aoDevNoz_Set_Start_Spd, StartSpd, &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Set_Acc_Spd, AccSpd, &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Set_Dec_Spd, DecSpd, &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Set_Drive_Spd, DriveSpd, &nCommstatus);

	WRITE_DIGITAL(doDevNoz_Set_Spd, eOn_1, &nCommstatus);

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Home() {

	int nCommstatus, nAlmResult;
	int HomeCnt = 0;

	//Current Off
	WRITE_DIGITAL(doDevNoz_Current, eOff_0, &nCommstatus);

	//Set HomeSpd
	WRITE_ANALOG(aoDevNoz_Home_Spd0, (double)READ_ANALOG(apDevNoz_Home_Spd0, &nCommstatus), &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Home_Spd1, (double)READ_ANALOG(apDevNoz_Home_Spd1, &nCommstatus), &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Home_Spd2, (double)READ_ANALOG(apDevNoz_Home_Spd2, &nCommstatus), &nCommstatus);
	WRITE_ANALOG(aoDevNoz_Offset_Spd, (double)READ_ANALOG(apDevNoz_Offset_Spd, &nCommstatus), &nCommstatus);

	WRITE_DIGITAL(doDevNoz_Set_HomeSpd, eOn_1, &nCommstatus);

	//Move Home
	WRITE_DIGITAL(doDevNoz_HomeMove, (int)READ_DIGITAL(dpDevNoz_HomeMode, &nCommstatus), &nCommstatus);

	S_TIMER_READY();

	while (1) {
		//Simulation
		if (gbSIMULATION_MODE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			WRITE_DIGITAL(diDevNoz_HomeSen, eOn_1, &nCommstatus);
			WRITE_DIGITAL(diDevNoz_Homing_Sts, eNone, &nCommstatus);
			WRITE_ANALOG(aiDevNoz_Cur_Pos, 0, &nCommstatus);
			WRITE_ANALOG(aiDevNoz_Cmd_Pos, 0, &nCommstatus);
		}

		//Manage Abort
		if (MANAGER_ABORT() == TRUE)	WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);

		//Homing Start
		while (READ_DIGITAL(diDevNoz_Homing_Sts, &nCommstatus) == eHoming) {

			//Check Idle
			if (READ_DIGITAL(diDevNoz_Homing_Sts, &nCommstatus) == eNone) break;

			//Timer Check
			if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
				nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
				if (nAlmResult == ALM_IGNORE) break;
				else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
				else S_TIMER_READY();
			}
			if (!WAIT_SECONDS(0.1)) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
				return SYS_ABORTED;
			}
		}
		//Homing End

		//HomeSen Check
		if (READ_DIGITAL(diDevNoz_HomeSen, &nCommstatus) == eOn_1) break;

		//Timer Check
		if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
			nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
			if (nAlmResult == ALM_IGNORE) break;
			else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
			else S_TIMER_READY();
		}
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
			return SYS_ABORTED;
		}
	}

	//Reset Pos
	if (!WAIT_SECONDS(0.1)) {
		WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
		if (gbSIMULATION_MODE) {
			WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
		}
		return SYS_ABORTED;
	}
	WRITE_ANALOG(aoDevNoz_Reset_Pos, 0, &nCommstatus);
	if (fabs(READ_ANALOG(aiDevNoz_Cmd_Pos, &nCommstatus) - 0) <= 2 && fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - 0) <= 2)	return SYS_SUCCESS;

	return SYS_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Stop() {

	int nCommstatus;

	WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
	if (gbSIMULATION_MODE) {
		WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
	}
	while (TRUE) {
		if (!WAIT_SECONDS(0.1)) return SYS_ABORTED;
		if (diDevNoz_Busy_Sts == eIdle) break;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Idle(double dbSetFlow) {

	int nCommstatus, nAlmResult;
	char szCmdBuffer[256] = { 0, };

	//DEV Stop
	if (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eBusy)
		WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);

	//LED Flow
	if (dbSetFlow != 0) {
		sprintf(szCmdBuffer, "SET %f", dbSetFlow);
		RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, szCmdBuffer); //LED Flow
	}
	else
		RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED OFF

	while (TRUE) {
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
			return SYS_ABORTED;
		}
		if (READ_FUNCTION(OBJ_DEVELOPER_FLOW) == SYS_SUCCESS && READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eIdle) break;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Move(double dbSetPos, double dbSetSpd, double dbSetFlow) {

	int nCommstatus, nAlmResult;
	double dbData;
	char szCmdBuffer[256] = { 0, };

	//Set Spd
	{
		double Spd1, Spd2, Spd3, Spd4;
		Spd1 = READ_ANALOG(apDevNoz_Set_Start_Spd, &nCommstatus);
		Spd2 = READ_ANALOG(apDevNoz_Set_Acc_Spd, &nCommstatus);
		Spd3 = READ_ANALOG(apDevNoz_Set_Dec_Spd, &nCommstatus);
		Spd4 = dbSetSpd;
		Developer_Speed_Setting(Spd1, Spd2, Spd3, Spd4);
	}

	//Pre-Check
	if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbSetPos) <= 2) return SYS_SUCCESS;

	//LED Flow
	if (dbSetFlow >= 0) {
		sprintf(szCmdBuffer, "SET %f", dbSetFlow);
		RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, szCmdBuffer); //LED Flow
	}
	//Move Pos
	WRITE_ANALOG(aoDevNoz_Abs_Pos, dbSetPos, &nCommstatus);
	//Simulation
	if (gbSIMULATION_MODE) {
		WRITE_DIGITAL(diDevNoz_Busy_Sts, eBusy, &nCommstatus);
	}

	S_TIMER_READY();
	while (1) {

		//Manage Abort
		if (MANAGER_ABORT() == TRUE) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
		}

		//Move Start

		while (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eBusy) {
			//Simulation
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);
				dbData = READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus);
				if (dbData < dbSetPos)	dbData += 4;
				else if (dbData > dbSetPos) dbData -= 4;
				WRITE_ANALOG(aiDevNoz_Cur_Pos, dbData, &nCommstatus);
				WRITE_ANALOG(aiDevNoz_Cmd_Pos, dbData, &nCommstatus);
				if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbSetPos) <= 2) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
					if (READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) == 0 && READ_ANALOG(aiDevNoz_Cmd_Pos, &nCommstatus) == 0)
					{
						WRITE_DIGITAL(diDevNoz_HomeSen, eOn_1, &nCommstatus);
					}

					else {
						WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);
					}
					if (!WAIT_SECONDS(0.1)) {
						WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
						if (gbSIMULATION_MODE) {
							WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
						}
						return SYS_ABORTED;
					}
				}
			}

			//Manage Abort
			if (MANAGER_ABORT() == TRUE) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
			}

			//Check Idle
			if (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eIdle) break;

			//Timer Check
			if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
				nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
				if (nAlmResult == ALM_IGNORE) break;
				else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
				else S_TIMER_READY();
			}
			if (!WAIT_SECONDS(0.1)) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
				return SYS_ABORTED;
			}
		}
		//Move End

		//Pos Check
		if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbSetPos) <= 2) break;

		//Timer Check
		if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
			nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
			if (nAlmResult == ALM_IGNORE) break;
			else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
			else S_TIMER_READY();
		}
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
			return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Swing(double dbSetEndPos, double dbSetSpd, double dbSetFlow, int SwingType) {

	int nCommstatus, nAlmResult;
	double dbStartPos;
	double dbData;
	int nSwingCnt = 0;
	char szCmdBuffer[256] = { 0, };

	//Start Pos 읽지 않고 Current Pos를 Start Pos로 저장
	dbStartPos = READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus);
	if (fabs(dbStartPos - dbSetEndPos) <= 2)	return SYS_SUCCESS; //Pre-Check

	//Set Spd
	{
		double Spd1, Spd2, Spd3, Spd4;
		Spd1 = READ_ANALOG(apDevNoz_Set_Start_Spd, &nCommstatus);
		Spd2 = READ_ANALOG(apDevNoz_Set_Acc_Spd, &nCommstatus);
		Spd3 = READ_ANALOG(apDevNoz_Set_Dec_Spd, &nCommstatus);
		Spd4 = dbSetSpd;
		Developer_Speed_Setting(Spd1, Spd2, Spd3, Spd4);
	}

	sprintf(szCmdBuffer, "SET %f", dbSetFlow);
	RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, szCmdBuffer); //LED Flow


	WRITE_ANALOG(aoDevNoz_Abs_Pos, dbSetEndPos, &nCommstatus); //Move Pos
	//Simulation
	if (gbSIMULATION_MODE) {
		WRITE_DIGITAL(diDevNoz_Busy_Sts, eBusy, &nCommstatus);
	}

	S_TIMER_READY();
	while (1) {

		//Manage Abort
		if (MANAGER_ABORT() == TRUE) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
			RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED OFF
		}

		//Move Start Pos -> End Pos
		while (nSwingCnt == 0)
		{
			//Manage Abort
			if (MANAGER_ABORT() == TRUE) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}

				RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED OFF
			}

			//Move Start
			while (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eBusy) {
				//Simulation
				if (gbSIMULATION_MODE) {
					dbData = READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus);
					if (dbData < dbSetEndPos) {
						dbData += 4;
						WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);

					}
					else if (dbData > dbSetEndPos) {
						dbData -= 4;
						WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);

					}
					WRITE_ANALOG(aiDevNoz_Cur_Pos, dbData, &nCommstatus);
					WRITE_ANALOG(aiDevNoz_Cmd_Pos, dbData, &nCommstatus);
					if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbSetEndPos) <= 2) {
						WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
						if (READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) == 0 && READ_ANALOG(aiDevNoz_Cmd_Pos, &nCommstatus) == 0) {
							WRITE_DIGITAL(diDevNoz_HomeSen, eOn_1, &nCommstatus);
						}
						else {
							WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);
						}
						if (!WAIT_SECONDS(0.1)) {
							WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
							if (gbSIMULATION_MODE) {
								WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
							}
							RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
							return SYS_ABORTED;
						}
					}

				}

				//Manage Abort
				if (MANAGER_ABORT() == TRUE) {

					WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
					if (gbSIMULATION_MODE) {
						WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
					}
					RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED OFF
				}

				//Check Idle
				if (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eIdle) break;

				//Timer Check
				if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
					nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
					if (nAlmResult == ALM_IGNORE) break;
					else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
					else S_TIMER_READY();
				}
				if (!WAIT_SECONDS(0.1)) {
					WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
					if (gbSIMULATION_MODE) {
						WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
					}
					RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
					return SYS_ABORTED;
				}
			}
			//Move End

			//Check Start Pos -> End Pos
			if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbSetEndPos) <= 2) {
				nSwingCnt++;
				if (SwingType == eOneWay_1) { RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); } //Oneway -> LED OFF
				WRITE_ANALOG(aoDevNoz_Abs_Pos, dbStartPos, &nCommstatus); //Move End Pos -> Start Pos

				//Simulation
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eBusy, &nCommstatus);
				}
				break;
			}

			//Timer Check
			if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Swing_Timeout, &nCommstatus)) {
				nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
				if (nAlmResult == ALM_IGNORE) break;
				else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
				else S_TIMER_READY();
			}
			if (!WAIT_SECONDS(0.1)) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
				RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
				return SYS_ABORTED;
			}
		}

		//Move Start
		while (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eBusy) {
			//Simulation
			if (gbSIMULATION_MODE) {
				dbData = READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus);
				if (dbData < dbStartPos)	dbData += 4;
				else if (dbData > dbStartPos) dbData -= 4;
				WRITE_ANALOG(aiDevNoz_Cur_Pos, dbData, &nCommstatus);
				WRITE_ANALOG(aiDevNoz_Cmd_Pos, dbData, &nCommstatus);
				if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbStartPos) <= 2) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
					if (READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) == 0 && READ_ANALOG(aiDevNoz_Cmd_Pos, &nCommstatus) == 0) WRITE_DIGITAL(diDevNoz_HomeSen, eOn_1, &nCommstatus);
					else {
						WRITE_DIGITAL(diDevNoz_HomeSen, eOff_0, &nCommstatus);
					}
					if (!WAIT_SECONDS(0.1)) {
						WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
						if (gbSIMULATION_MODE) {
							WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
						}
						RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
						return SYS_ABORTED;
					}
				}
			}

			//Manage Abort
			if (MANAGER_ABORT() == TRUE) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
				RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED OFF
			}

			//Check Idle
			if (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommstatus) == eIdle) break;

			//Timer Check
			if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Move_Timeout, &nCommstatus)) {
				nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
				if (nAlmResult == ALM_IGNORE) break;
				else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
				else S_TIMER_READY();
			}
			if (!WAIT_SECONDS(0.1)) {
				WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
				if (gbSIMULATION_MODE) {
					WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
				}
				RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
				return SYS_ABORTED;
			}
		}
		//Move End

		//Check End Pos -> Start Pos
		if (fabs(READ_ANALOG(aiDevNoz_Cur_Pos, &nCommstatus) - dbStartPos) <= 2) {
			//Swing Count Init
			nSwingCnt = 0;
			break;
		}

		//Timer Check
		if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Swing_Timeout, &nCommstatus)) {
			nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
			if (nAlmResult == ALM_IGNORE) break;
			else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
			else S_TIMER_READY();
		}
		if (!WAIT_SECONDS(0.1)) {
			WRITE_DIGITAL(doDevNoz_Stop, eOn_1, &nCommstatus);
			if (gbSIMULATION_MODE) {
				WRITE_DIGITAL(diDevNoz_Busy_Sts, eIdle, &nCommstatus);
			}
			RUN_SET_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0"); //LED 
			return SYS_ABORTED;
		}
	}

	//LED OFF
	RUN_FUNCTION(OBJ_DEVELOPER_FLOW, "SET 0");

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Init()
{
	if (Developer_Home() == SYS_SUCCESS)
	{
		// Door 처리 구문 예정
	}

	return SYS_SUCCESS;
}

Module_Status Developer_Standby()
{
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() {
	int nCommStatus = 0;
	unsigned char* szParameter;
	char szRunMsg[32] = { 0, }, szParam1[32] = { 0, }, szParam2[32] = { 0, }, szParam3[32] = { 0, }, szBuffer[32] = { 0, };
	double dbSetPos, dbSetSpd, dbSetFlow;
	Module_Status MDLStatus = SYS_SUCCESS;

	if (READ_DIGITAL(diDevNoz_Busy_Sts, &nCommStatus) == eBusy) {
		printf("[%s] Nozzle is now Busy[%s]\n", PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ());
		return SYS_ABORTED;
	}

	szParameter = PROGRAM_PARAMETER_READ();

	STR_SEPERATE(szParameter, szRunMsg, szBuffer, 31);
	STR_SEPERATE(szBuffer, szParam1, szBuffer, 31);
	STR_SEPERATE(szBuffer, szParam2, szParam3, 31);

	dbSetPos = (double)atoi(szParam1);
	dbSetSpd = (double)atoi(szParam2);
	dbSetFlow = (double)atoi(szParam3);

	if (STRCMP_L(szRunMsg, "HOME")) { MDLStatus = Developer_Home(); }
	else if (STRCMP_L(szRunMsg, "STOP")) { MDLStatus = Developer_Stop(); }
	else if (STRCMP_L(szRunMsg, "IDLE")) { MDLStatus = Developer_Idle((double)atoi(szParam1)); }
	else if (STRCMP_L(szRunMsg, "MOVE")) { MDLStatus = Developer_Move(dbSetPos, dbSetSpd, dbSetFlow); } //MOVE 500 100
	else if (STRCMP_L(szRunMsg, "SWING_ONEWAY")) { MDLStatus = Developer_Swing(dbSetPos, dbSetSpd, dbSetFlow, eOneWay_1); }	// SWING_ONEWAY 500 100
	else if (STRCMP_L(szRunMsg, "SWING_SHUTTLE")) { MDLStatus = Developer_Swing(dbSetPos, dbSetSpd, dbSetFlow, eShuttle_0); }	// SWING_SHUTTLE 500 100
	else if (STRCMP_L(szRunMsg, "GO_INIT")) { MDLStatus = Developer_Init(); }
	else if (STRCMP_L(szRunMsg, "GO_STANDBY")) { MDLStatus = Developer_Standby(); }
	else if (STRCMP_L(szRunMsg, "SEND_ABLE"))
	{
		WRITE_DIGITAL(UpperSendComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendAble, eOn_1, &nCommStatus);
		WRITE_DIGITAL(UpperSendStart, eOn_1, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "SEND_COMP"))
	{
		WRITE_DIGITAL(UpperSendComplete, eOn_1, &nCommStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(UpperSendAble, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendStart, eOff_0, &nCommStatus);
		WRITE_DIGITAL(UpperSendComplete, eOff_0, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "RECV_ABLE"))
	{
		WRITE_DIGITAL(LowerRecvComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvAble, eOn_1, &nCommStatus);
		WRITE_DIGITAL(LowerRecvStart, eOn_1, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "RECV_COMP"))
	{
		WRITE_DIGITAL(LowerRecvComplete, eOn_1, &nCommStatus);
		WAIT_SECONDS(2);
		WRITE_DIGITAL(LowerRecvAble, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvComplete, eOff_0, &nCommStatus);
		WRITE_DIGITAL(LowerRecvStart, eOff_0, &nCommStatus);
	}
	else if (STRCMP_L(szRunMsg, "OPEN_GATE"))
	{
		WRITE_DIGITAL(PM1GateControl, eOpen, &nCommStatus);
		if (gbSIMULATION_MODE)
		{
			WRITE_DIGITAL(PM1GateOpenSensor, eOn_1, &nCommStatus);
			WRITE_DIGITAL(PM1GateCloseSensor, eOff_0, &nCommStatus);
			return SYS_SUCCESS;
		}
		while (TRUE)
		{
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (READ_DIGITAL(PM1GateOpenSensor, &nCommStatus) == eOn_1 &&
				READ_DIGITAL(PM1GateCloseSensor, &nCommStatus) == eOff_0)
			{
				return SYS_SUCCESS;
			}
		}
	}
	else if (STRCMP_L(szRunMsg, "CLOSE_GATE"))
	{
		if (!WAIT_SECONDS(2)) return SYS_ABORTED;
		WRITE_DIGITAL(PM1GateControl, eClose, &nCommStatus);
		if (gbSIMULATION_MODE)
		{
			WRITE_DIGITAL(PM1GateOpenSensor, eOff_0, &nCommStatus);
			WRITE_DIGITAL(PM1GateCloseSensor, eOn_1, &nCommStatus);
			return SYS_SUCCESS;
		}
		while (TRUE)
		{
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			if (READ_DIGITAL(PM1GateOpenSensor, &nCommStatus) == eOff_0 &&
				READ_DIGITAL(PM1GateCloseSensor, &nCommStatus) == eOn_1)
			{
				return SYS_SUCCESS;
			}
		}
	}
	else {
		printf("[%s] Not Supported Parameter[%s]\n", PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ());
		return SYS_ABORTED;
	}
	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main() {
	Module_Status MDLStatus;

	//==============================================================================================================
	printf("  =>> START [%s]\n", PROGRAM_PARAMETER_READ());
	//==============================================================================================================

	MDLStatus = Program_Main_Sub();

	//==============================================================================================================
	switch (MDLStatus) {
	case SYS_SUCCESS:	printf("  <<= END [%s] is SUCCESS\n", PROGRAM_PARAMETER_READ());			break;
	case SYS_ABORTED:	printf("  <<= END [%s] is ABORTED\n", PROGRAM_PARAMETER_READ());			break;
	case SYS_ERROR:	printf("  <<= END [%s] is ERROR\n", PROGRAM_PARAMETER_READ());			break;
	default:	printf("  <<= END [%s] is [%d]\n", PROGRAM_PARAMETER_READ(), MDLStatus); break;
	}
	//==============================================================================================================

	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Program_Enter_Code() {

	char szModule[16] = { 0, }, szArgument[16] = { 0, };

	STR_SEPERATE(PROGRAM_ARGUMENT_READ(), szModule, szArgument, 15);

	REPLACE_CHAR_SET(0, szModule);
	gnALARM_START_INDEX = atoi(szArgument);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code() {
	int nCommStatus;

	if (READ_DIGITAL(IO_Driver_Loding, &nCommStatus) == 0)	gbSIMULATION_MODE = TRUE;
	else													gbSIMULATION_MODE = FALSE;

	SYSTEM_KILL_WITH_MESSAGE();

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
