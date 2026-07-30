#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
#include "iodefine.h"
//#include "../Library/kutlCheck.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define		MAXSTEP				200
#define		MAX_RECIPENAME_LEN	255
#define		MAX_STEP_COUNT		199
#define		MAX_RECIPE_ITEM		200
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eOFF_0, eON_1 };
enum { Off, On };
enum { eON_0, eOFF_1 };
enum { eCLOSE_0, eOPEN_1 };
enum { eOPEN_0, eCLOSE_1 };
enum { eShuttle_0, eOneWay_1, eMove_2, eIdle_3 };
enum { eSTART_0, eEDGE1_1, eCENTER_2, eEDGE2_3, eEND_4 };

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

enum {
	ATM,
	Vacuum,
	GoATM,
	GoVacuum,
	Abort
}VACSTS;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL	gbSIMULATION_MODE = FALSE;
int		gnALARM_START_INDEX = 0;

RecipeStepTemplateM		RECIPE[MAXSTEP];
char	szItemName[MAX_RECIPE_ITEM][64];
char	szItemValue[MAX_RECIPE_ITEM][64];


int		gnTotal_Step_Count = 0;	// RCP Step
int		gnWAFER_ID;
int		gnSTEP_COUNT;

int		gnRecipeType;
int		gnWafer_Information;
int		gnWaferSrc, gnWaferSts;

char	gszRECIPE_NAME[256];
char	gszWaferID[32];
char	gszRecipeType[32];

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
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{	"IO_Driver_Loding"					, _K_D_IO	,	IO_Driver_Loading				,	0	} ,

	{ "SEQ_SPUTTER_DEVICE_CONTROL"			, _K_F_IO	,	SeqSputterDeviceControl					,	0	},
	{ "SEQ_PM2_CHAMBER_CONTROL"				, _K_F_IO	,	SeqPM2ChamberControl					,	0	},


	{	"$0.dvRCP_MaxStep"					, _K_D_IO	,	dvRCP_MaxStep					,	0	} ,
	{	"$0.dvRCP_StepNumber"				, _K_D_IO	,	dvRCP_StepNumber				,	0	} ,

	{	"$0.dvRCP_Process_DataLog"			, _K_D_IO	,	dvRCP_Process_DataLog			,	0	} ,
	{	"$0.dvRCP_Process_Status"			, _K_D_IO	,	dvRCP_Process_Status			,	0	} ,
	{	"$0.dvRCP_Process_Control"			, _K_D_IO	,	dvRCP_Process_Control			,	0	} ,
	{	"$0.dvRCP_Process_Run"    			, _K_D_IO	,	dvRCP_Process_Run	    		,	0	} ,

	{	"$0.avRCP_Total_RunTime"			, _K_A_IO	,	avRCP_Total_RunTime				,	0	} ,
	{	"$0.avRCP_Total_SetTime"			, _K_A_IO	,	avRCP_Total_SetTime				,	0	} ,
	{	"$0.avRCP_Step_RunTime"				, _K_A_IO	,	avRCP_Step_RunTime				,	0	} ,
	{	"$0.avRCP_Step_SetTime"				, _K_A_IO	,	avRCP_Step_SetTime				,	0	} ,

	{	"$0.avRCP_Progress"		 			, _K_A_IO	,	avRCP_Progress		 			,	0	} ,

	{	"$0.svRCP_Name"		 				, _K_S_IO	,	svRCP_Name     		 			,	0	} ,
	{	"$0.svRCP_StepName"		 			, _K_S_IO	,	svRCP_StepName		 			,	0	} ,
	{	"$0.svRCP_Process_Message"		 	, _K_S_IO	,	svRCP_Process_Message		 	,	0	} ,

	{	"$0.PROCESS_LOG"					, _K_F_IO	,	PROCESS_LOG						,	0	} ,


	{	"$0.dvPM_Wafer_Status"				, _K_D_IO	,	dvPM_Wafer_Status				,	0	} ,
	{	"$0.dvPM_Wafer_Source"				, _K_D_IO	,	dvPM_Wafer_Source				,	0	} ,
	{	"$0.dvPM_Wafer_Result"				, _K_D_IO	,	dvPM_Wafer_Result				,	0	} ,


	{	"avForelinePressure"				, _K_A_IO	,	GaugeForelinePressure				,	0	} ,
	{	"avCvgGaugePressure"				, _K_A_IO	,	avCvgGaugePressure				,	0	} ,
	{	"doPVDTurboReady"					, _K_D_IO	,	doPVDTurboReady				,	0	} ,
	{	"avIonGaugePressure"				, _K_A_IO	,	avIonGaugePressure				,	0	} ,
	{	"avPN2GasFlowPV"					, _K_A_IO	,	avPN2GasFlowPV				,	0	} ,
	{	"avTurboRPM"						, _K_A_IO	,	avTurboRPM				,	0	} ,
	{	"PM2.dvVacuumStatus"				, _K_D_IO	,	dvVacuumStatus				,	0	} ,
	{	"CTC.PM2_ControlStatus"				, _K_D_IO	,	ControlStatus				,	0	} ,
	{	"doPVDVacuum_LED"					, _K_D_IO	,	doPVDVacuum_LED				,	0	} ,
	{	"doPVDIdle_LED"						, _K_D_IO	,	doPVDIdle_LED				,	0	} ,
	{	"doPVDProcess_LED"					, _K_D_IO	,	doPVDProcess_LED				,	0	} ,

	{	"avDcTargetPowerSP"					, _K_A_IO	,	avDcTargetPowerSP				,	0	} ,

	

	""
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Recipe_Data_Log()
{
	int i;
	int nCount = 0;

	for (i = 0; i < MAX_RECIPE_ITEM; i++) {
		if (strlen(szItemName[i]) > 0) nCount++;
	}

	for (i = 0; i < nCount; i++) {
		printf("  [%s]\t[%s]\n", szItemName[i], szItemValue[i]);
	}
	//
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Search_WaferID() {

	int nCommStatus;
	char szWaferSrc[32], szWaferSts[32];

	gnWaferSts = READ_DIGITAL(dvPM_Wafer_Status, &nCommStatus);
	gnWaferSrc = READ_DIGITAL(dvPM_Wafer_Source, &nCommStatus);

	itoa(gnWaferSts, szWaferSts, 10);
	itoa(gnWaferSrc, szWaferSrc, 10);

	if (strcmp(szWaferSrc, "0") == TRUE)	strcpy(szWaferSrc, "A");
	else if (strcmp(szWaferSrc, "1") == TRUE)	strcpy(szWaferSrc, "B");
	else if (strcmp(szWaferSrc, "2") == TRUE)	strcpy(szWaferSrc, "C");
	else if (strcmp(szWaferSrc, "3") == TRUE)	strcpy(szWaferSrc, "D");

	strcpy(gszWaferID, szWaferSrc);
	strcat(gszWaferID, ":");
	strcat(gszWaferID, szWaferSts);
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Search_RecipeType() {

	itoa(gnRecipeType, gszRecipeType, 10);

	if (STRCMP(gszRecipeType, "0") == TRUE)	strcpy(gszRecipeType, "MainProcess");	// RecipeType Sort
	else if (STRCMP(gszRecipeType, "1") == TRUE)	strcpy(gszRecipeType, "PostProcess");
	else if (STRCMP(gszRecipeType, "2") == TRUE)	strcpy(gszRecipeType, "PreProcess");
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Recipe_Reading(BOOL* appenddata) {
	int	iii, i = 0, k, l, idx = 0;
	int ii_return;
	double db_return;
	//char str_return[16];
	int nLen;
	unsigned char* pszEd;
	char	szNameList[1025];
	char	szValueList[1025];
	BOOL	bFindName = FALSE, bDataStart = FALSE;

	pszEd = PROGRAM_PARAMETER_READ();
	nLen = strlen(pszEd);

	*appenddata = FALSE;

	//for (iii = 0; iii < nLen; iii++)
	//{
	//	printf("%d - [%c]\n", iii, pszEd[iii]);			
	//}

	//return TRUE;

	if ((pszEd[0] != SEP_LEFT_BLACKET) || (pszEd[1] != '$') || (pszEd[2] != 'A') || (pszEd[3] != SEP_NAME_DATA)) {
		gnSTEP_COUNT = 0;
	}

	for (iii = 0; iii < nLen; iii++) {
		if (MANAGER_ABORT()) return SYS_ABORTED;
		//SEP_LEFT_BLACKET = 11 : START_DELEMETER(STD) of the Packet Format(Refer to Appendix N-4-3)
		if (pszEd[iii] == SEP_LEFT_BLACKET) { bDataStart = TRUE; bFindName = FALSE; i = 0; }
		//SEP_RIGHT_BLACKET = 12 : END_DELEMETER(ED) of the Packet Format(Refer to Appendix N-4-3)
		else if (pszEd[iii] == SEP_RIGHT_BLACKET) {
			i = 0;
			bDataStart = FALSE;
			if (bFindName) {
				//$A , $B , $I, $R, $M, $T, $C, $F, $P, $S, $E : Fixed Type Indicator
				//$A : More Data start
				if (strcmp("$A", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] \n", szNameList );
					*appenddata = FALSE;
				}
				//$B : Append Data
				else if (strcmp("$B", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] \n", szNameList );
					*appenddata = TRUE;
				}
				else if (strcmp("$I", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
						gnWAFER_ID = ii_return;
					}
				}
				//$R : Recipe Name
				else if (strcmp("$R", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					l = strlen(szValueList);
					for (k = l - 1; k >= 0; k--) {
						if ((szValueList[k] == '/') || (szValueList[k] == '\\')) {
							k++;
							break;
						}
					}
					strncpy(gszRECIPE_NAME, szValueList + k, MAX_RECIPENAME_LEN);

					printf("[%s] Recipe Name[%s]\n", PROGRAM_FUNCTION_READ(), gszRECIPE_NAME);
				}
				//$T : Total Step Count
				else if (strcmp("$T", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
						gnTotal_Step_Count = ii_return;
						printf("[%s] Total Step Count[%d]\n", PROGRAM_FUNCTION_READ(), gnTotal_Step_Count);
					}
				}
				//$S : Step Start
				else if (strcmp("$S", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
						gnSTEP_COUNT = ii_return;
						printf("[%s] Step Number[%d]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT);
					}
				}
				//$M : Recipe Type
				else if (strcmp("$M", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
						gnRecipeType = ii_return;
						printf("[%s] RecipeType [%d]\n", PROGRAM_FUNCTION_READ(), gnRecipeType);
					}
				}
				//$P : Delay Time in TM
				else if (strcmp("$P", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
					}
				}
				//$C : Cassette Chamber
				else if (strcmp("$C", szNameList) == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) {
					}
				}
				//$E : Step End
				else if (strcmp(szNameList, "$E") == 0) {
					strcpy(szItemValue[idx], szValueList);
					strcpy(szItemName[idx++], szNameList);
					//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
				}
				else {
					//User Define Indicator Part
					//Single Step Part
					if (gnSTEP_COUNT == 0) {
					}
					//Multi Step Part
					else if (gnSTEP_COUNT <= MAX_STEP_COUNT) {

						if (strcmp(szNameList, "STEP_USE") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].STEP_USE = ii_return;
							//printf("[%s] [%d] STEP_USE[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].STEP_USE);
						}
						else if (strcmp(szNameList, "AR_FLOW") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].AR_FLOW = ii_return;
							//printf("[%s] [%d] AR_FLOW[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].AR_FLOW);
						}
						else if (strcmp(szNameList, "PROC_PRESS") == 0) {
							if (DB_NUM_CHECK(szValueList, &db_return)) RECIPE[gnSTEP_COUNT - 1].PROC_PRESS = db_return;
							//printf("[%s] [%d] PROC_PRESS[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].PROC_PRESS);
						}
						else if (strcmp(szNameList, "DC_POWER") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].DC_POWER = ii_return;
							//printf("[%s] [%d] DC_POWER[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].DC_POWER);
						}
						else if (strcmp(szNameList, "RAMP_UP_TIME") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].RAMP_UP_TIME = ii_return;
							//printf("[%s] [%d] RAMP_UP_TIME[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].RAMP_UP_TIME);
						}
						else if (strcmp(szNameList, "PROC_TIME") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].PROC_TIME = ii_return;
							//printf("[%s] [%d] PROC_TIME[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].PROC_TIME);
						}
						else if (strcmp(szNameList, "RAMP_DOWN_TIME") == 0) {
							if (INT_NUM_CHECK(szValueList, 0, 0, &ii_return)) RECIPE[gnSTEP_COUNT - 1].RAMP_DOWN_TIME = ii_return;
							//printf("[%s] [%d] RAMP_DOWN_TIME[%.1lf]\n", PROGRAM_FUNCTION_READ(), gnSTEP_COUNT, RECIPE[gnSTEP_COUNT - 1].RAMP_DOWN_TIME);
						}

						strcpy(szItemValue[idx], szValueList);
						strcpy(szItemName[idx++], szNameList);
						//_iLOG_PRINTF( "  [%s] [%s]\n", szNameList, szValueList );
					}
				}
			}
		}
		else {
			if (bDataStart) {
				if (bFindName) { szValueList[i] = pszEd[iii]; szValueList[i + 1] = 0x00;  i++; }
				else {
					//SEP_NAME_DATA = 13 : SEPERATE_DELEMETER(SPD) of the Packet Format(Refer to Appendix N-4-3)
					if (pszEd[iii] == SEP_NAME_DATA) { szNameList[i] = 0x00; i = 0; bFindName = TRUE; }
					else { szNameList[i] = pszEd[iii]; i++; }
				}
			}
		}
	}
	if (!(*appenddata) && (gnTotal_Step_Count != gnSTEP_COUNT)) {
		printf("[%s] Recipe Data has a Error , therefore STEP_COUNT = 0\n", PROGRAM_FUNCTION_READ());
		gnSTEP_COUNT = 0;
	}
	if (MANAGER_ABORT()) return FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Process() {

	int i, nCommStatus;
	int nCurrStep;
	int commStatus;;

	double	dbStepRunTime, dbTotalRunTime = 0;
	double	dbTotalRecipeTime = 0, dbProgress = 0;
	double dbOldPos = 0, dbNewPos = 0, pressure = 0.0;

	char	szCurrStep[16];
	char	szPlotterEvent[64] = { 0, };
	char	szTempBuffer[256] = { 0, };
	char	szCmdBuffer[256] = { 0, };

	Module_Status	MS_DEVELOP;
	Module_Status	MS_DEVELOP_FLOW;

	//printf("[Debug] 여기서 오류2 \n");

	WRITE_STRING(svRCP_Name, gszRECIPE_NAME, &nCommStatus);
	WRITE_DIGITAL(dvRCP_MaxStep, gnTotal_Step_Count, &nCommStatus);

	WRITE_ANALOG(avRCP_Progress, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Total_SetTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Total_RunTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Step_RunTime, 0, &nCommStatus);
	WRITE_DIGITAL(dvRCP_StepNumber, 0, &nCommStatus);

	for (i = 0; i < gnTotal_Step_Count; i++) {
		dbTotalRecipeTime += (double)RECIPE[i].PROC_TIME;
	}
	WRITE_ANALOG(avRCP_Total_SetTime, (int)dbTotalRecipeTime, &nCommStatus);

	sprintf(szPlotterEvent, "START %d|%d|%s|%s", 100, 100, gszRECIPE_NAME, "Manual");
	//WRITE_FUNCTION_EVENT( SCREEN_PLOTTER, szPlotterEvent );

	// 펌핑 다운 실행
	printf("[Debug ProcessPM2] Pre_Process 펌핑 동작실행\n");
	if (READ_DIGITAL(dvVacuumStatus, &nCommStatus) != Vacuum)
		if (RUN_FUNCTION(SeqPM2ChamberControl, "PUMP_DOWN") == SYS_ABORTED) return SYS_ABORTED;

	// 초기 밸브 동작
	if (RUN_FUNCTION(SeqSputterDeviceControl, "VENT_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "LOWVAC_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;
	if (RUN_FUNCTION(SeqSputterDeviceControl, "HIVAC_VALVE_OPEN") == SYS_ABORTED) return SYS_ABORTED;

	S_TIMER_READY();
	while (TRUE) {
		if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

		pressure = READ_ANALOG(avIonGaugePressure, &nCommStatus);
		if (pressure <= 1.0 * pow(10, -6)) break;

		if (S_TIMER_ELAPSED() > WAIT_TIME120) {
			printf("[Debug PM2 Control] Process HIVAC_VALVE_OPEN Time Out\n");
			return SYS_ABORTED;
		}
	}
	
	// 메인 동작
	WRITE_DIGITAL(ControlStatus, RunProcess, &commStatus);
	WRITE_DIGITAL(doPVDIdle_LED, Off, &commStatus);
	WRITE_DIGITAL(doPVDProcess_LED, On, &commStatus);

	//printf("[Debug] 여기서 오류2-1 \n");

	for (i = 0; i < gnTotal_Step_Count; i++) {

		sprintf(szTempBuffer, "STEP %d", i + 1);
		WRITE_FUNCTION_EVENT(PROCESS_LOG, szTempBuffer);

		nCurrStep = i + 1;
		itoa(nCurrStep, szCurrStep, 10);

		WRITE_ANALOG(avRCP_Step_SetTime, RECIPE[i].PROC_TIME, &nCommStatus);
		WRITE_ANALOG(avRCP_Step_RunTime, 0, &nCommStatus);
		WRITE_DIGITAL(dvRCP_StepNumber, i + 1, &nCommStatus);
		//WRITE_STRING(svRCP_StepName, i + 1, &nCommStatus);

		//printf("[Debug] 여기서 오류2-2 \n");

		if (RECIPE[i].STEP_USE == 0) continue;


		// 1
		char buffer[200];

		printf("[Debug ProcessPM2] Step : %d, GAS_VALVE_OPEN %d\n", i + 1, RECIPE[i].AR_FLOW);
		sprintf(buffer, "GAS_VALVE_OPEN %d", RECIPE[i].AR_FLOW);
		if (RUN_FUNCTION(SeqSputterDeviceControl, buffer) == SYS_ABORTED) return SYS_ABORTED;

		// 2.
		printf("[Debug ProcessPM2] Step : %d, WAIT_AR_FLOW\n", i + 1);
		if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_AR_FLOW") == SYS_ABORTED) return SYS_ABORTED;

		// 3.
		printf("[Debug ProcessPM2] Step : %d, WAIT_AR_PROCESS_PRESSURE %lf\n", i + 1, RECIPE[i].PROC_PRESS);
		sprintf(buffer, "WAIT_AR_PROCESS_PRESSURE %lf", RECIPE[i].PROC_PRESS);
		if (RUN_FUNCTION(SeqSputterDeviceControl, buffer) == SYS_ABORTED) return SYS_ABORTED;

		// 4.
		if (RECIPE[i].DC_POWER > READ_ANALOG(avDcTargetPowerSP, &commStatus)) {
			printf("[Debug ProcessPM2] Step : %d, DC_POWER_RAMP_UP %d,%d\n", i + 1, RECIPE[i].DC_POWER, RECIPE[i].RAMP_UP_TIME);
			sprintf(buffer, "DC_POWER_RAMP_UP %d,%d", RECIPE[i].DC_POWER, RECIPE[i].RAMP_UP_TIME);
			if (RUN_FUNCTION(SeqSputterDeviceControl, buffer) == SYS_ABORTED) return SYS_ABORTED;
		}
		else {
			printf("[Debug ProcessPM2] Step : %d, DC_POWER_RAMP_DOWN %d,%d\n", i + 1, RECIPE[i].DC_POWER, RECIPE[i].RAMP_DOWN_TIME);
			sprintf(buffer, "DC_POWER_RAMP_UP %d,%d", RECIPE[i].DC_POWER, RECIPE[i].RAMP_DOWN_TIME);
			if (RUN_FUNCTION(SeqSputterDeviceControl, buffer) == SYS_ABORTED) return SYS_ABORTED;
		}

		// 5.
		printf("[Debug ProcessPM2] Step : %d, WAIT_DC_POWER\n", i + 1);
		if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_DC_POWER") == SYS_ABORTED) return SYS_ABORTED;

		// 6.PROCESS HOLD
		printf("[Debug ProcessPM2] Step : %d, PROCESS HOLD : %d\n", i + 1, RECIPE[i].PROC_TIME);
		S_TIMER_READY();
		while (TRUE) {
			if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

			if (S_TIMER_ELAPSED() > RECIPE[i].PROC_TIME) break;
		}

		S_TIMER_READY();
		while (TRUE) {

			MS_DEVELOP = READ_FUNCTION(SeqSputterDeviceControl);

			if (MS_DEVELOP == SYS_SUCCESS) {
				dbStepRunTime = S_TIMER_ELAPSED();

				if (!WAIT_SECONDS(0.5)) return SYS_ABORTED;

				dbProgress = ((dbTotalRunTime + dbStepRunTime) / dbTotalRecipeTime) * 100;
				WRITE_ANALOG(avRCP_Progress, dbProgress, &nCommStatus);
				WRITE_ANALOG(avRCP_Total_RunTime, dbTotalRunTime + dbStepRunTime, &nCommStatus);
				WRITE_ANALOG(avRCP_Step_RunTime, dbStepRunTime, &nCommStatus);
				if (dbStepRunTime >= (double)RECIPE[i].PROC_TIME) { // Flow Time >= StepTime
					dbTotalRunTime += dbStepRunTime;	// Total Time += Flow Time
					break;
				}
			}
			else if (MS_DEVELOP == SYS_ABORTED) {

				return SYS_ABORTED;
			}

			else {
				dbStepRunTime = S_TIMER_ELAPSED();

				WRITE_ANALOG(avRCP_Step_RunTime, dbStepRunTime, &nCommStatus);
				dbProgress = ((dbTotalRunTime + dbStepRunTime) / dbTotalRecipeTime) * 100;
				WRITE_ANALOG(avRCP_Progress, dbProgress, &nCommStatus);
				WRITE_ANALOG(avRCP_Total_RunTime, dbTotalRunTime + dbStepRunTime, &nCommStatus);
				WRITE_ANALOG(avRCP_Step_RunTime, dbStepRunTime, &nCommStatus);
			}

			if (!WAIT_SECONDS(0.001)) {
				ALARM_POST(956);
				return SYS_ABORTED;
			}
		}
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Pre_Process() {

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Process_Main() {

	int	nCommStatus;
	Module_Status	MDLStatus;

	//printf("[Debug] 여기서 오류1 \n");
	WRITE_DIGITAL(dvRCP_Process_Run, 1, &nCommStatus);
	WRITE_DIGITAL(dvRCP_Process_Status, 1, &nCommStatus);

	// Main Process
	MDLStatus = Process();


	//printf("[Debug] 여기서 오류3 \n");
	WRITE_DIGITAL(dvRCP_Process_Run, 0, &nCommStatus);
	WRITE_DIGITAL(dvRCP_Process_Status, 0, &nCommStatus);


	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Post_Process() {
	int nCommStatus;


	double startPos;
	char message[256];

	// 7.
	printf("[Debug ProcessPM2] DC_POWER_RAMP_DOWN 5\n");
	if (RUN_FUNCTION(SeqSputterDeviceControl, "DC_POWER_RAMP_DOWN 5") == SYS_ABORTED) return SYS_ABORTED;

	// 8.
	printf("[Debug ProcessPM2] WAIT_DC_POWER 0\n");
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_DC_POWER 0") == SYS_ABORTED) return SYS_ABORTED;

	// 9.
	printf("[Debug ProcessPM2] GAS_VALVE_CLOSE\n");
	if (RUN_FUNCTION(SeqSputterDeviceControl, "GAS_VALVE_CLOSE") == SYS_ABORTED) return SYS_ABORTED;

	// 10.
	printf("[Debug ProcessPM2] WAIT_AR_FLOW 0\n");
	if (RUN_FUNCTION(SeqSputterDeviceControl, "WAIT_AR_FLOW 0") == SYS_ABORTED) return SYS_ABORTED;


	WRITE_DIGITAL(dvRCP_MaxStep, 0, &nCommStatus);
	WRITE_DIGITAL(dvRCP_StepNumber, 0, &nCommStatus);

	WRITE_ANALOG(avRCP_Total_RunTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Total_SetTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Step_RunTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Step_SetTime, 0, &nCommStatus);
	WRITE_ANALOG(avRCP_Progress, 0, &nCommStatus);

	WRITE_STRING(svRCP_Name, "", &nCommStatus);
	WRITE_STRING(svRCP_StepName, "", &nCommStatus);
	WRITE_STRING(svRCP_Process_Message, "", &nCommStatus);

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() {

	int	nCommStatus = 0;
	int	nAppendData = -1;
	Module_Status	MDLStatus;

	if (!Recipe_Reading(&nAppendData)) {
		printf("[%s] Recipe_Reading() Fail\n", PROGRAM_FUNCTION_READ());
		return SYS_ABORTED;
	}

	Recipe_Data_Log();

	Search_RecipeType();
	Search_WaferID();

	MDLStatus = Pre_Process();
	if (MDLStatus != SYS_SUCCESS) {
		return SYS_ABORTED;
	}
	//
	MDLStatus = Process_Main();
	//

	MDLStatus = Post_Process();
	if (MDLStatus != SYS_SUCCESS) {
		return SYS_ABORTED;
	}

	//if ( MDLStatus == SYS_SUCCESS ) {
	//	MDLStatus = Post_Process();
	//	if ( MDLStatus != SYS_SUCCESS )	{
	//		return SYS_ABORTED;
	//	}
	//}
	//else {
	//	//
	//	// Error Code Here
	//	//
	//	MDLStatus = Post_Process();
	//	if ( MDLStatus != SYS_SUCCESS )	{
	//		return SYS_ABORTED;
	//	}
	//}

	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main() {

	Module_Status MDLStatus;

	//==============================================================================================================
	printf("  =>> START\n");
	//==============================================================================================================

	MDLStatus = Program_Main_Sub();

	//==============================================================================================================
	switch (MDLStatus) {
	case SYS_SUCCESS:	printf("  <<= END is SUCCESS\n");			break;
	case SYS_ABORTED:	printf("  <<= END is ABORTED\n");			break;
	case SYS_ERROR:	printf("  <<= END is ERROR\n");			break;
	default:	printf("  <<= END is [%d]\n", MDLStatus); break;
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

	if (READ_DIGITAL(IO_Driver_Loading, &nCommStatus) == 0)	gbSIMULATION_MODE = TRUE;
	else															gbSIMULATION_MODE = FALSE;

	SYSTEM_KILL_WITH_MESSAGE();

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
