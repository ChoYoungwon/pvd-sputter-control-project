#ifndef IODEFINE_H
#define IODEFINE_H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum {	eSYS_CM1_0, eSYS_PM1_1, eSYS_PM2_2, eSYS_PM3_3	};
enum {	eArmA_0	};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

typedef enum {
	SCHEDULER						,
	INFO_SAVE						,

	SCHEDULER_ERROR					,
	TMC_STANDBY_CONTROL				,
	CTC_TR_STATUS					,

	SCHEDULER_MAINT_PM1,
	SCHEDULER_MAINT_PM2				,
	//SCHEDULER_MAINT_PM3				,
	SCHEDULER_MAINT_TM				,

	CTC_SYS_AL_CONTROL				,
	SYS_AL_Arm						,
	SYS_AL_Station					,
	SYS_AL_Slot						,

	TA_Wafer_Status					,

	PM1_Wafer_Status				,
	PM2_Wafer_Status				,
	PM3_Wafer_Status				,

	CTC_FA_Wafer_Status				,
	CTC_FA_Wafer_Source				,
	CTC_FB_Wafer_Status				,
	CTC_FB_Wafer_Source				,
	CTC_FM_AL_Wafer_Status			,

	CM1_C01_Wafer					,
	CM1_C02_Wafer					,
	CM1_C03_Wafer					,
	CM1_C04_Wafer					,
	CM1_C05_Wafer					,
	CM1_C06_Wafer					,
	CM1_C07_Wafer					,
	CM1_C08_Wafer					,
	CM1_C09_Wafer					,
	CM1_C10_Wafer					,
	CM1_C11_Wafer					,
	CM1_C12_Wafer					,
	CM1_C13_Wafer					,
	CM1_C14_Wafer					,
	CM1_C15_Wafer					,
	CM1_C16_Wafer					,
	CM1_C17_Wafer					,
	CM1_C18_Wafer					,
	CM1_C19_Wafer					,
	CM1_C20_Wafer					,
	CM1_C21_Wafer					,
	CM1_C22_Wafer					,
	CM1_C23_Wafer					,
	CM1_C24_Wafer					,
	CM1_C25_Wafer					,

} IOPointIndex;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int MAINT_INTERFACE_RUN( LPSTR nList , ... );

//---------------------------
//Sys Align
//---------------------------
int Maint_Alignment( int nRun_Mode , int ntype , int noption );
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif
