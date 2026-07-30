#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
#include "iodefine.h"

#define TIMEOUT 10
#define ALARM_ACTION_TIMEOUT 164

//-------------------------------------------------------------------------------------------------
BOOL	_simulationMode = FALSE;

char	_moduleName[32] = { 0, };
//-------------------------------------------------------------------------------------------------

IO_Name_String_Map IO_STR_Map_Table[] = {
	{	"IO_Driver_Loding"			, _K_D_IO	,	IO_Driver_Loading		,	0	} ,
	{	"doRbXRetExt"				, _K_D_IO	,	RobotRetractExtend		,	0	} ,
	{	"diRbBusySts"				, _K_D_IO	,	RobotBusy				,	0	} ,
	{	"diRbExtendSen"				, _K_D_IO	,	RobotExtendSensor		,	0	} ,
	{	"diRbRetractSen"			, _K_D_IO	,	RobotRetractSensor		,	0	} ,

	{	"diRbZUpSen"				, _K_D_IO	,	RobotZUpSen				,	0	} ,
	{	"diRbZDownSen"				, _K_D_IO	,	RobotZDownSen			,	0	} ,
	{	"doRbZDnup"					, _K_D_IO	,	RobotZUp				,	0	} ,

	{	"doRbSetSpd"				, _K_D_IO	,	RobotSetSpeed			,	0	} ,
	{	"doRbSetHomeSpd"			, _K_D_IO	,	RobotSetHomeSpeed		,	0	} ,
	{	"doRbStop"					, _K_D_IO	,	RobotStop				,	0	} ,

	{	"dpRB_HomeMode"				, _K_D_IO	,	RobotHomeMode			,	0	} ,
	{	"diRbHomingSts"				, _K_D_IO	,	RobotHomeStatus			,	0	} ,
	{	"doRbHomeMove"				, _K_D_IO	,	RobotHomeStart			,	0	} ,

	{ "apRB_Start_Spd_Move"			, _K_A_IO	,	apRB_Start_Spd_Move		,	0	} ,
	{ "apRB_dACC_Move"				, _K_A_IO	,	apRB_dACC_Move			,	0	} ,
	{ "apRB_dDec_Move"				, _K_A_IO	,	apRB_dDec_Move			,	0	} ,
	{ "apRB_Drive_Spd_Move"			, _K_A_IO	,	apRB_Drive_Spd_Move		,	0	} ,

	{ "apRB_Home_Spd0"				, _K_A_IO	,	apRB_Home_Spd0			,	0	} ,
	{ "apRB_Home_Spd1"				, _K_A_IO	,	apRB_Home_Spd1			,	0	} ,
	{ "apRB_Home_Spd2"				, _K_A_IO	,	apRB_Home_Spd2			,	0	} ,
	{ "apRB_Offset_Spd"				, _K_A_IO	,	apRB_Offset_Spd			,	0	} ,

	{ "apRB_Pos_Indexer"			, _K_A_IO	,	apRbPos_Indexer			,	0	} ,
	{ "apRB_Pos_PM1"				, _K_A_IO	,	apRbPos_PM1				,	0	} ,
	{ "apRB_Pos_PM2"				, _K_A_IO	,	apRbPos_PM2				,	0	} ,

	{ "aoRbSetStartSpd"				, _K_A_IO	,	aoRB_Set_Start_Spd		,	0	} ,
	{ "aoRbSetAccelSpd"				, _K_A_IO	,	aoRB_Set_Accel_Spd		,	0	} ,
	{ "aoRbSetDecelSpd"				, _K_A_IO	,	aoRB_Set_Decel_Spd		,	0	} ,
	{ "aoRbSetDrvSpd"				, _K_A_IO	,	aoRB_Set_Drv_Spd		,	0	} ,

	{ "aoRbHomeSpd0"				, _K_A_IO	,	aoRB_Home_Spd0			,	0	} ,
	{ "aoRbHomeSpd1"				, _K_A_IO	,	aoRB_Home_Spd1			,	0	} ,
	{ "aoRbHomeSpd2"				, _K_A_IO	,	aoRB_Home_Spd2			,	0	} ,
	{ "aoRbOffsetSpd"				, _K_A_IO	,	aoRB_Offset_Spd			,	0	} ,

	{ "aoRbAbsPos"					, _K_A_IO	,	aoRbAbsPos				,	0	} ,
	{ "aoRbSetPos"					, _K_A_IO	,	aoRbSetPos				,	0	} ,
	{ "aoRbCurPos"					, _K_A_IO	,	aoRbCurPos				,	0	} ,

	{ "aiRbCurPos"					, _K_A_IO	,	aiRbCurrPos				,	0	} ,


	
	""
};

// 센싱 대기 루틴
Module_Status wait_routine(IOPointIndex IOAction, OnOFF onoff) {
	int commStatus;
	S_TIMER_READY();

	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		if (READ_DIGITAL(IOAction, &commStatus) == onoff) break;

		double elapsed = S_TIMER_ELAPSED();
		if (elapsed > TIMEOUT) {
			AlarmStyle style = ALARM_MANAGE(ALARM_ACTION_TIMEOUT);
			if (style == ALM_RETRY) { S_TIMER_READY(); }
			else if (style == ALM_CLEAR) break;
			else return SYS_ABORTED;
		}
	}
	return SYS_SUCCESS;
}

Module_Status SetConfigParameter() {

	int commStatus;
	WRITE_ANALOG(aoRB_Set_Start_Spd, READ_ANALOG(apRB_Start_Spd_Move, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Set_Accel_Spd, READ_ANALOG(apRB_dACC_Move, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Set_Decel_Spd, READ_ANALOG(apRB_dDec_Move, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Set_Drv_Spd, READ_ANALOG(apRB_Drive_Spd_Move, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Home_Spd0, READ_ANALOG(apRB_Home_Spd0, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Home_Spd1, READ_ANALOG(apRB_Home_Spd1, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Home_Spd2, READ_ANALOG(apRB_Home_Spd2, &commStatus), &commStatus);
	WRITE_ANALOG(aoRB_Offset_Spd, READ_ANALOG(apRB_Offset_Spd, &commStatus), &commStatus);

	printf("Busy Signal : %d\n", READ_DIGITAL(RobotBusy, &commStatus));
	if (READ_DIGITAL(RobotBusy, &commStatus) == Moving) return SYS_ABORTED;

	// RobotExtend -> RETRACT 실행
	if (READ_DIGITAL(RobotExtendSensor, &commStatus) == eON_1) {
		WRITE_DIGITAL(RobotRetractExtend, Retract, &commStatus);
		if (wait_routine(RobotRetractSensor, eON_1) == SYS_ABORTED)	return SYS_ABORTED;
	}

	// Down 동작
	printf("[ROBOT MOTOR] Down\n");
	if (READ_DIGITAL(RobotZUpSen, &commStatus) == eON_1) {
		WRITE_DIGITAL(RobotZUp, Down, &commStatus);
		if (wait_routine(RobotZUpSen, eOFF_0) == SYS_ABORTED)	return SYS_ABORTED;
	}

	// Home 동작
	printf("[ROBOT MOTOR] HOME\n");
	WRITE_DIGITAL(RobotSetSpeed, eON_1, &commStatus);
	WRITE_DIGITAL(RobotSetHomeSpeed, eON_1, &commStatus);

	WRITE_DIGITAL(RobotHomeStart, READ_DIGITAL(RobotHomeMode, &commStatus), &commStatus);
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) {

			WRITE_DIGITAL(RobotStop, eON_1, &commStatus);
			return SYS_ABORTED;
		}

		if (READ_DIGITAL(RobotHomeStatus, &commStatus) == HomeComplete &&
			READ_DIGITAL(RobotBusy, &commStatus) == Idle) break;
	}

	WRITE_ANALOG(aoRbSetPos, 0, &commStatus);
	WRITE_ANALOG(aoRbCurPos, 0, &commStatus);


	return SYS_SUCCESS;
}

Module_Status Home() {
	return SetConfigParameter();
}

int commStatus;

// PM1, CM1 이동 동작 실행
Module_Status ActionReadyStatus(char* stationName) {
	int commStatus;

	// Busy Signal Check
	printf("[ROBOT MOTOR] Busy Signal : %d\n", READ_DIGITAL(RobotBusy, &commStatus));
	if (READ_DIGITAL(RobotBusy, &commStatus) == Moving) return SYS_ABORTED;

	// RobotExtend -> RETRACT 실행
	if (READ_DIGITAL(RobotExtendSensor, &commStatus) == eON_1) {
		WRITE_DIGITAL(RobotRetractExtend, Retract, &commStatus);

		if (wait_routine(RobotRetractSensor, eON_1) == SYS_ABORTED)	return SYS_ABORTED;
	}

	// RobotSetSpeed 설정
	WRITE_DIGITAL(RobotSetSpeed, eON_1, &commStatus);

	// ROBOT 위치 이동
	if (STRCMP_L(stationName, "CM1") || STRCMP_L(stationName, "A_CM1")) {
		WRITE_ANALOG(aoRbAbsPos, READ_ANALOG(apRbPos_Indexer, &commStatus), &commStatus);
	}
	else if (STRCMP_L(stationName, "PM1") || STRCMP_L(stationName, "A_PM1")) {
		WRITE_ANALOG(aoRbAbsPos, READ_ANALOG(apRbPos_PM1, &commStatus), &commStatus);
	}
	else if (STRCMP_L(stationName, "PM2") || STRCMP_L(stationName, "A_PM2")) {
		WRITE_ANALOG(aoRbAbsPos, READ_ANALOG(apRbPos_PM2, &commStatus), &commStatus);
	}
	else return SYS_ABORTED;

	// ROBOT 위치 확인
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) {
			WRITE_DIGITAL(RobotStop, eON_1, &commStatus);
			return SYS_ABORTED;
		}

		if (READ_DIGITAL(RobotBusy, &commStatus) == Idle) {
			if (fabs(READ_ANALOG(aoRbAbsPos, &commStatus) - READ_ANALOG(aiRbCurrPos, &commStatus)) < 2)	break;
			else {
				printf("[ROBOT MOTOR : ABORTED] Robot Position MISS\n");
				return SYS_ABORTED;
			}
		}
	}
	return SYS_SUCCESS;
}

// PICK 동작 수행
Module_Status ActionPick(char* stationName) {
	int commStatus;

	// 초기 동작(위치 설정)
	if (ActionReadyStatus(stationName) == SYS_ABORTED) return SYS_ABORTED;

	// Extend 동작 실행
	printf("[ROBOT MOTOR] Extend\n");
	WRITE_DIGITAL(RobotRetractExtend, Extend, &commStatus);
	if (wait_routine(RobotExtendSensor, eON_1) == SYS_ABORTED)	return SYS_ABORTED;

	if (!WAIT_SECONDS(1)) return SYS_ABORTED;

	// Up 동작 실행
	printf("[ROBOT MOTOR] Up\n");
	WRITE_DIGITAL(RobotZUp, Up, &commStatus);
	if (wait_routine(RobotZUpSen, eON_1) == SYS_ABORTED)	return SYS_ABORTED;

	// Retract 동작 실행
	printf("[ROBOT MOTOR] Retract\n");
	WRITE_DIGITAL(RobotRetractExtend, Retract, &commStatus);
	if (wait_routine(RobotRetractSensor, eON_1) == SYS_ABORTED)	return SYS_ABORTED;

	if (!WAIT_SECONDS(1)) return SYS_ABORTED;

	return SYS_SUCCESS;
}

// PLACE 동작 수행
Module_Status ActionPlace(char* stationName) {

	// 초기 동작(위치 설정)
	if (ActionReadyStatus(stationName) == SYS_ABORTED) return SYS_ABORTED;

	// Up 동작 실행
	printf("[ROBOT MOTOR] Up\n");
	WRITE_DIGITAL(RobotZUp, Up, &commStatus);
	if (wait_routine(RobotZUpSen, eON_1) == SYS_ABORTED)	return SYS_ABORTED;

	// Extend 동작
	printf("[ROBOT MOTOR] Extend\n");
	WRITE_DIGITAL(RobotRetractExtend, Extend, &commStatus);
	if (wait_routine(RobotExtendSensor, eON_1) == SYS_ABORTED)	return SYS_ABORTED;

	if (!WAIT_SECONDS(1)) return SYS_ABORTED;

	// Down 동작
	printf("[ROBOT MOTOR] Down\n");
	WRITE_DIGITAL(RobotZUp, Down, &commStatus);
	if (wait_routine(RobotZUpSen, eOFF_0) == SYS_ABORTED)	return SYS_ABORTED;

	// Retract 동작
	WRITE_DIGITAL(RobotRetractExtend, Retract, &commStatus);
	printf("[ROBOT MOTOR] Retract\n");

	if (wait_routine(RobotExtendSensor, eOFF_0) == SYS_ABORTED) return SYS_ABORTED;

	return SYS_SUCCESS;
}


Module_Status ActionRobotXAxis(int action) {

	int commStatus, axisStatus;

	printf("Busy Signal : %d\n", READ_DIGITAL(RobotBusy, &commStatus));

	if (READ_DIGITAL(RobotBusy, &commStatus) == Moving)
	{
		return SYS_ABORTED;
	}

	switch (action) {
	case Retract:
		WRITE_DIGITAL(RobotRetractExtend, Retract, &commStatus);
		while (TRUE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			axisStatus = READ_DIGITAL(RobotRetractSensor, &commStatus);
			printf("XAxis Status : %d\n", axisStatus);
			if (axisStatus == eON_1) break;
		}
		break;
	case Extend:
		WRITE_DIGITAL(RobotRetractExtend, Extend, &commStatus);
		while (TRUE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;
			axisStatus = READ_DIGITAL(RobotExtendSensor, &commStatus);
			printf("XAxis Status : %d\n", axisStatus);
			if (axisStatus == eON_1) break;
		}
		break;
	default:
		return SYS_SUCCESS;
	}

	return SYS_SUCCESS;
}


Module_Status Program_Main()
{
	char param1[16] = { 0, }, param2[16] = { 0, }, param3[16] = { 0, }, param4[16] = { 0, };

	//printf("Normal Message : %s\n", PROGRAM_PARAMETER_READ());

	// source(파라미터를 읽는 함수) 
	STR_SEPERATE(PROGRAM_PARAMETER_READ(), param1, param2, 15);

	//printf("%s  %s\n", param1, param2);

	if (STRCMP_L(param1, "RET")) {
		return ActionRobotXAxis(Retract);
	}
	else if (STRCMP_L(param1, "EXT")) {
		return ActionRobotXAxis(Extend);
	}
	else if (STRCMP_L(param1, "PICK")) {
		printf("[Debug] param1 : %s, param2 : %s\n", param1, param2);
		return ActionPick(param2);
	}
	else if (STRCMP_L(param1, "PLACE")) {
		return ActionPlace(param2);
	}
	else if (STRCMP_L(param1, "HOME")) {
		return Home();
	}


	return SYS_SUCCESS;
	//Module_Status moduleStatus;
	//int commStatus;


	//return moduleStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// 로딩할때 불러짐
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

// 
BOOL Program_Init_Code()
{
	//int commStatus;

	//if (READ_DIGITAL(IO_Driver_Loading, &commStatus) == 0)	_simulationMode = TRUE;
	//else													_simulationMode = FALSE;

	SYSTEM_KILL_WITH_MESSAGE();

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
