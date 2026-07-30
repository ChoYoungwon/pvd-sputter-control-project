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
enum { eOFF_0, eON_1 };
enum { eShuttle_0, eOneWay_1 };
enum { eHOME_0, eEDGE1_1, eCENTER_2, eEDGE2_3 };

BOOL		gbSIMULATION_MODE = FALSE;
int			gnALARM_START_INDEX = 0;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{ "IO_Driver_Loding"				, _K_D_IO	,	IO_Driver_Loding				,	0	} ,

	{ "doDevNozLED"						, _K_D_IO	,	doDevNoz_LED						,	0	} ,
	//{ "aoDevNoz_Flow"						, _K_A_IO	,	aoDevNoz_Flow						,	0	} ,
	//{ "aiDevNoz_Cur_Flow"					, _K_A_IO	,	aiDevNoz_Cur_Flow					,	0	} ,	

	{ "apDevNoz_Flow_Timeout"				, _K_A_IO	,	apDevNoz_Flow_Timeout				,	0	} ,

	""
};

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
Module_Status Developer_Flow_Init() {

	int nCommstatus, nAlmResult;

	//WRITE_ANALOG(aoDevNoz_Flow, 0, &nCommstatus);
	WRITE_DIGITAL(doDevNoz_LED, eOFF_0, &nCommstatus);
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Developer_Flow(double dbSetFlow) {

	int nCommstatus, nAlmResult;
	double dbData;
	//WRITE_ANALOG(aoDevNoz_Flow, dbSetFlow, &nCommstatus);
	if (dbSetFlow == 0) WRITE_DIGITAL(doDevNoz_LED, eOFF_0, &nCommstatus);
	else if (dbSetFlow > 0) WRITE_DIGITAL(doDevNoz_LED, eON_1, &nCommstatus);


	S_TIMER_READY();
	while (1) {
		if (gbSIMULATION_MODE) {
			//현재는 LED 방식으로 사용할 것으로 보여 조금씩 올려주는 것보다는 한번에 Set하는게 맞는 것으로 보여 아래 사용
			//추후에 실제로 Flow를 사용하게 된다면 주석처리된 구문 사용하여 simulation 구현해도 무방 25.04.03 hjh
			// dbData = READ_ANALOG(aiDevNoz_Cur_Flow, &nCommstatus);
			//if		(dbData < dbSetFlow) dbData += 5 ;
			//else if (dbData > dbSetFlow) dbData -= 5 ;
			//WRITE_ANALOG(aiDevNoz_Cur_Flow, dbData, &nCommstatus);
			//WRITE_ANALOG(aiDevNoz_Cur_Flow, dbSetFlow, &nCommstatus);
		}
		//if (READ_ANALOG(aiDevNoz_Cur_Flow, &nCommstatus) == dbSetFlow) break;

		if (dbSetFlow == 0) {
			if (READ_DIGITAL(doDevNoz_LED, &nCommstatus) == eOFF_0) break;
		}
		else if (dbSetFlow > 0) {
			if (READ_DIGITAL(doDevNoz_LED, &nCommstatus) == eON_1) break;
		}
		else break;

		if (S_TIMER_ELAPSED() > READ_ANALOG(apDevNoz_Flow_Timeout, &nCommstatus)) {
			nAlmResult = ALARM_MANAGE(ALARM_TIMEOUT);
			if (nAlmResult == ALM_IGNORE) break;
			else if (nAlmResult != ALM_RETRY) return SYS_ABORTED;
			else S_TIMER_READY();
		}
		if (!WAIT_SECONDS(0.1)) return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() {
	int nCommStatus = 0;
	unsigned char* szParameter;
	char szRunMsg[32] = { 0, }, szParam[32] = { 0, };
	double dbSetFlow;
	Module_Status MDLStatus = SYS_SUCCESS;

	szParameter = PROGRAM_PARAMETER_READ();

	STR_SEPERATE(szParameter, szRunMsg, szParam, 31);
	dbSetFlow = atoi(szParam);

	if (STRCMP_L(szRunMsg, "INIT")) { MDLStatus = Developer_Flow_Init(); }
	else if (STRCMP_L(szRunMsg, "SET")) { MDLStatus = Developer_Flow(dbSetFlow); }

	else {
		printf("[%s] Not Supported Parameter[%s]\n", PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ());
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
	printf("  =>> START [%s]\n", PROGRAM_PARAMETER_READ());
	//==============================================================================================================

	MDLStatus = Program_Main_Sub();

	//==========================================
	// ====================================================================
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
void Program_Enter_Code()
{
	char szModule[16] = { 0, }, szArgument[16] = { 0, };

	STR_SEPERATE(PROGRAM_ARGUMENT_READ(), szModule, szArgument, 15);

	REPLACE_CHAR_SET(0, szModule);
	gnALARM_START_INDEX = atoi(szArgument);
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
