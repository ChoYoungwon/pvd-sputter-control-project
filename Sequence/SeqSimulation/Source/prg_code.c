#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
#include <math.h>

#include <Kutlstr.h>
#include <Kutltime.h>

#include "cimseqnc.h"
#include "iodefine.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//#define		PRESSURE_SCALE	133.332		// PASCAL
#define		PRESSURE_SCALE	1		// TORR
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum	{	eOFF_0	,	eON_1		};
enum	{	eON_0	,	eOFF_1		};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {

	{ "IO_Driver_Loding"				, _K_D_IO	,	IO_Driver_Loding				,	0	} ,

	{ "INTLKS_CONTROL"					, _K_D_IO	,	INTLKS_CONTROL					,	0	} ,

	//
	//{ "dpDOOR_TM_SimulationTime"		, _K_D_IO	,	dpDOOR_TM_SimulationTime		,	0	} ,
	//{ "dpSIM_TM_GoVent_Time"			, _K_D_IO	,	dpSIM_TM_GoVent_Time			,	0	} ,
	//{ "dpSIM_TM_GoPump_Time"			, _K_D_IO	,	dpSIM_TM_GoPump_Time			,	0	} ,

	{ "diLP_CM1_Placed"					, _K_D_IO	,	diLP_CM1_Placed					,	0	} ,
	{ "diLP_CM1_Present"				, _K_D_IO	,	diLP_CM1_Present				,	0	} ,

	{ "diLP_CM1_Door_Status"			, _K_D_IO	,	diLP_CM1_Door_Status			,	0	} ,
	{ "diLP_CM1_Shuttle_Status"			, _K_D_IO	,	diLP_CM1_Shuttle_Status			,	0	} ,
	{ "diLP_CM1_Clamp_Status"			, _K_D_IO	,	diLP_CM1_Clamp_Status			,	0	} ,
//	{ "diLP_CM1_UnClamp_Status"			, _K_D_IO	,	diLP_CM1_UnClamp_Status			,	0	} ,
	{ "doLP_CM1_Door_Ctrl"				, _K_D_IO	,	doLP_CM1_Door_Ctrl				,	0	} ,
	{ "doLP_CM1_Docking"				, _K_D_IO	,	doLP_CM1_Docking				,	0	} ,
	{ "doLP_CM1_Clamp"					, _K_D_IO	,	doLP_CM1_Clamp					,	0	} ,
	{ "doLP_CM1_UnClamp"				, _K_D_IO	,	doLP_CM1_UnClamp				,	0	} ,

	//{ "doGATE_TMPM1_Open"				, _K_D_IO	,	doGATE_TMPM1_Open				,	0	} ,
	//{ "doGATE_TMPM1_Close"				, _K_D_IO	,	doGATE_TMPM1_Close				,	0	} ,
	//{ "doGATE_TMPM2_Open"				, _K_D_IO	,	doGATE_TMPM2_Open				,	0	} ,
	//{ "doGATE_TMPM2_Close"				, _K_D_IO	,	doGATE_TMPM2_Close				,	0	} ,
	//{ "doGATE_TMPM3_Open"				, _K_D_IO	,	doGATE_TMPM3_Open				,	0	} ,
	//{ "doGATE_TMPM3_Close"				, _K_D_IO	,	doGATE_TMPM3_Close				,	0	} ,

	//{ "diGATE_TMPM1_Opened"				, _K_D_IO	,	diGATE_TMPM1_Opened				,	0	} ,
	//{ "diGATE_TMPM1_Closed"				, _K_D_IO	,	diGATE_TMPM1_Closed				,	0	} ,
	//{ "diGATE_TMPM2_Opened"				, _K_D_IO	,	diGATE_TMPM2_Opened				,	0	} ,
	//{ "diGATE_TMPM2_Closed"				, _K_D_IO	,	diGATE_TMPM2_Closed				,	0	} ,
	//{ "diGATE_TMPM3_Opened"				, _K_D_IO	,	diGATE_TMPM3_Opened				,	0	} ,
	//{ "diGATE_TMPM3_Closed"				, _K_D_IO	,	diGATE_TMPM3_Closed				,	0	} ,

	//{ "diRB_TMPM1_Not_Extend"			, _K_D_IO	,	diRB_TMPM1_Not_Extend			,	0	} ,
	//{ "diRB_TMPM2_Not_Extend"			, _K_D_IO	,	diRB_TMPM2_Not_Extend			,	0	} ,
	//{ "diRB_TMPM3_Not_Extend"			, _K_D_IO	,	diRB_TMPM3_Not_Extend			,	0	} ,

	//{ "diAIR_TM_Pressure"				, _K_D_IO	,	diAIR_TM_Pressure				,	0	} ,

	//{ "dvVAC_TM_RoughDryPMP_Start"		, _K_D_IO	,	dvVAC_TM_RoughDryPMP_Start		,	0	} ,
	//{ "dvVAC_TM_RoughDryPMP"			, _K_D_IO	,	dvVAC_TM_RoughDryPMP			,	0	} ,
	//{ "dvVAC_TM_RoughDryPMP_Alarm"		, _K_D_IO	,	dvVAC_TM_RoughDryPMP_Alarm		,	0	} ,
	//{ "aiVAC_TM_CG_Foreline_Pressure"	, _K_A_IO	,	aiVAC_TM_CG_Foreline_Pressure	,	0	} ,

	//{ "dioVAC_TM_SlowRoughingValve"		, _K_D_IO	,	dioVAC_TM_SlowRoughingValve		,	0	} ,
	//{ "dioVAC_TM_RoughingValve"			, _K_D_IO	,	dioVAC_TM_RoughingValve			,	0	} ,
	//{ "dioVAC_TM_ProfiledVentValve"		, _K_D_IO	,	dioVAC_TM_ProfiledVentValve		,	0	} ,
	//{ "dioVAC_TM_FastVentValve"			, _K_D_IO	,	dioVAC_TM_FastVentValve			,	0	} ,
	//{ "diVAC_TM_ATM_Sensor"				, _K_D_IO	,	diVAC_TM_ATM_Sensor				,	0	} ,
	//{ "aiVAC_TM_CG_Pressure"			, _K_A_IO	,	aiVAC_TM_CG_Pressure			,	0	} ,
	//{ "apVAC_TM_FastPump_Pressure"		, _K_A_IO	,	apVAC_TM_FastPump_Pressure		,	0	} ,

	//{ "diPRESS_TM_N2_High"				, _K_D_IO	,	diPRESS_TM_N2_High				,	0	} ,
	//{ "diPRESS_TM_N2_Low"				, _K_D_IO	,	diPRESS_TM_N2_Low				,	0	} ,
	//{ "diLID_TM_Closed"					, _K_D_IO	,	diLID_TM_Closed					,	0	} ,
	//{ "diWTR_TM_Flow"					, _K_D_IO	, 	diWTR_TM_Flow					,	0	} ,
	""
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Pressure_Find_2(int nPressure_IO, int nT_ROUGH) {

	double dr , ndr;
	int nCommStatus;

	if (nT_ROUGH != 0) {
		dr = READ_ANALOG(nPressure_IO, &nCommStatus) / PRESSURE_SCALE;
		ndr = pow(10, log10(dr) + (nT_ROUGH * 0.1));
		if ( ndr < 0.002  ) ndr = 0.002 + ( clock() % 100 ) * 0.00001;
		if ( ndr > 765    ) ndr = 765;
		WRITE_ANALOG(nPressure_IO, ndr * PRESSURE_SCALE, &nCommStatus);
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Pressure_Find_1(int nPressure_IO, int nATMSS_IO, int s, int t) {

	double dbPressure_Value , dbPressure_CalValue;
	int ts , nCommStatus;

	if      ( s == 0 && t == 0 ) return;
	else if ( s == 0 && t != 0 ) ts = t;
	else if ( s != 0 && t == 0 ) ts = s;
	else if ( s <  0 && t <  0 ) ts = t;
	else                         ts = s + t;
	if ( ts != 0 ) {
		dbPressure_Value = READ_ANALOG(nPressure_IO, &nCommStatus) / PRESSURE_SCALE;
		if (dbPressure_Value <= 0.000000001) dbPressure_Value = 0.000000001;
		dbPressure_CalValue = pow(10, log10(dbPressure_Value) + (ts * 0.1));
		if (dbPressure_CalValue >= 765) {
			WRITE_DIGITAL(nATMSS_IO, 1, &nCommStatus);
			dbPressure_CalValue = 765;
		}
		else {
			WRITE_DIGITAL(nATMSS_IO, 0, &nCommStatus);
		}
		if ( ts <= -6 ) {
			if (dbPressure_CalValue < 0.00000002) {
				dbPressure_CalValue = 0.00000002 + (clock() % 100) * 0.0000000001;
				WRITE_ANALOG(nPressure_IO, 0 * PRESSURE_SCALE, &nCommStatus);
			}
			else if (dbPressure_CalValue < 0.0002) {
				WRITE_ANALOG(nPressure_IO, 0 * PRESSURE_SCALE, &nCommStatus);
			}
			else {
				WRITE_ANALOG(nPressure_IO, dbPressure_CalValue * PRESSURE_SCALE, &nCommStatus);
			}
		}
		else {
			if (dbPressure_CalValue < 0.002) {
				dbPressure_CalValue = 0.002 + (clock() % 100) * 0.00001;
			}
			WRITE_ANALOG(nPressure_IO, dbPressure_CalValue * PRESSURE_SCALE, &nCommStatus);
		}
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void CM_Simulate() {

	int nCommStatus;
	int i, nData;

	while(TRUE) {
		//==========================================================================================
		_sleep(500);
		//==========================================================================================
		//for ( i=0 ; i<4 ; i++ ) {
			nData = READ_DIGITAL(doLP_CM1_Door_Ctrl /* + (i * 7)*/, &nCommStatus);
			WRITE_DIGITAL(diLP_CM1_Door_Status /* + (i * 7)*/, nData, &nCommStatus);
		
			nData = READ_DIGITAL(doLP_CM1_Docking /* + (i * 7)*/, &nCommStatus);
			WRITE_DIGITAL(diLP_CM1_Shuttle_Status/* + (i * 7)*/, nData*2, &nCommStatus);

			nData = READ_DIGITAL(doLP_CM1_Clamp /* + (i * 7)*/, &nCommStatus);
			if ( nData == eON_1 ) {
				WRITE_DIGITAL(diLP_CM1_Clamp_Status /* + (i * 7)*/, 1, &nCommStatus);
			}
			nData = READ_DIGITAL(doLP_CM1_UnClamp /* + (i * 7)*/, &nCommStatus);
			if ( nData == eON_1 ) {
				WRITE_DIGITAL(diLP_CM1_Clamp_Status/* + (i * 7)*/, 0, &nCommStatus);
			}

		//}
		
		WRITE_DIGITAL( diLP_CM1_Present , READ_DIGITAL( diLP_CM1_Placed , &nCommStatus ) , &nCommStatus );
		//==========================================================================================
	}
	_endthread();
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM1_Door_Simulate() {

//	int nCommStatus;
//	int nBM1_DoorSts = dioDOOR_BM1_Open;
//	int nData, nData2;
//	int nTimeData;

//	while(TRUE) {
		//==========================================================================================
//		nTimeData = READ_DIGITAL(dpDOOR_BM1_SimulationTime, &nCommStatus);
		//==========================================================================================
//		if (nTimeData <= 0) {
//			_sleep(100);
//			continue;
//		}
//		_sleep(nTimeData / 2);
		//==========================================================================================
//		nData = ( READ_DIGITAL( nBM1_DoorSts , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_DoorSts + 1 , &nCommStatus );
//		if      ( nData == 1 ) {
//			nData2 = ( READ_DIGITAL( nBM1_DoorSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_DoorSts + 3 , &nCommStatus );
//			if ( nData2 != 1 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM1_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_DoorSts + 3 , 1 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM1_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_DoorSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
//		else if ( nData == 2 ) {
//			nData2 = ( READ_DIGITAL( nBM1_DoorSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_DoorSts + 3 , &nCommStatus );
//			if ( nData2 != 2 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM1_DoorSts + 2 , 1 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_DoorSts + 3 , 0 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM1_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_DoorSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM2_Door_Simulate() {

//	int nCommStatus;
//	int nBM2_DoorSts = dioDOOR_BM2_Open;
//	int nData, nData2;
//	int timedata;

//	while(TRUE) {
		//==========================================================================================
//		timedata = READ_DIGITAL( dpDOOR_BM2_SimulationTime , &nCommStatus );
		//==========================================================================================
//		if ( timedata <= 0 ) {
//			_sleep(100);
//			continue;
//		}
//		_sleep( timedata / 2 );
		//==========================================================================================
//		nData = ( READ_DIGITAL( nBM2_DoorSts , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_DoorSts + 1 , &nCommStatus );
//		if      ( nData == 1 ) {
//			nData2 = ( READ_DIGITAL( nBM2_DoorSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_DoorSts + 3 , &nCommStatus );
//			if ( nData2 != 1 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM2_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_DoorSts + 3 , 1 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM2_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_DoorSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
//		else if ( nData == 2 ) {
//			nData2 = ( READ_DIGITAL( nBM2_DoorSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_DoorSts + 3 , &nCommStatus );
//			if ( nData2 != 2 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM2_DoorSts + 2 , 1 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_DoorSts + 3 , 0 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM2_DoorSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_DoorSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM1_Pin_Simulate() {

//	int nCommStatus;
//	int nBM1_PinSts  = dioPIN_BM1_Up , nData , nData2;
//	int nTimeData;

//	while(TRUE) {
		//==========================================================================================
//		nTimeData = READ_DIGITAL( dpPIN_BM1_SimulationTime , &nCommStatus );
		//==========================================================================================
//		if ( nTimeData <= 0 ) {
//			_sleep(100);
//			continue;
//		}
//		_sleep( nTimeData / 2 );
		//==========================================================================================
//		nData = ( READ_DIGITAL( nBM1_PinSts , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_PinSts + 1 , &nCommStatus );
//		if      ( nData == 1 ) {
//			nData2 = ( READ_DIGITAL( nBM1_PinSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_PinSts + 3 , &nCommStatus );
//			if ( nData2 != 1 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM1_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_PinSts + 3 , 1 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM1_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_PinSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
//		else if ( nData == 2 ) {
//			nData2 = ( READ_DIGITAL( nBM1_PinSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM1_PinSts + 3 , &nCommStatus );
//			if ( nData2 != 2 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM1_PinSts + 2 , 1 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_PinSts + 3 , 0 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM1_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM1_PinSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM2_Pin_Simulate() {

//	int nCommStatus;
//	int nBM2_PinSts  = dioPIN_BM2_Up , nData , nData2;
//	int nTimenData;

//	while(TRUE) {
		//==========================================================================================
//		nTimenData = READ_DIGITAL( dpPIN_BM2_SimulationTime , &nCommStatus );
		//==========================================================================================
//		if ( nTimenData <= 0 ) {
//			_sleep(100);
//			continue;
//		}
//		_sleep( nTimenData / 2 );
		//==========================================================================================
//		nData = ( READ_DIGITAL( nBM2_PinSts , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_PinSts + 1 , &nCommStatus );
//		if      ( nData == 1 ) {
//			nData2 = ( READ_DIGITAL( nBM2_PinSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_PinSts + 3 , &nCommStatus );
//			if ( nData2 != 1 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM2_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_PinSts + 3 , 1 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM2_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_PinSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
//		else if ( nData == 2 ) {
//			nData2 = ( READ_DIGITAL( nBM2_PinSts + 2 , &nCommStatus ) * 2 ) + READ_DIGITAL( nBM2_PinSts + 3 , &nCommStatus );
//			if ( nData2 != 2 ) {
//				if ( nData2 == 0 ) {
//					WRITE_DIGITAL( nBM2_PinSts + 2 , 1 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_PinSts + 3 , 0 , &nCommStatus );
//				}
//				else {
//					WRITE_DIGITAL( nBM2_PinSts + 2 , 0 , &nCommStatus );
//					WRITE_DIGITAL( nBM2_PinSts + 3 , 0 , &nCommStatus );
//				}
//			}
//		}
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void TM_Door_Simulate() {
//
//	int nCommStatus;
//	int i , nData , nData2;
//	int nTimeData;
//
//	while(TRUE) {
//		//==========================================================================================
//		nTimeData = READ_DIGITAL( dpDOOR_TM_SimulationTime , &nCommStatus );
//		//==========================================================================================
//		if ( nTimeData <= 0 ) {
//			_sleep(100);
//			continue;
//		}
//		_sleep( nTimeData / 2 );
//		//==========================================================================================
//		for ( i = 0 ; i < 3 ; i++ ) {
//			nData = ( READ_DIGITAL( doGATE_TMPM1_Open + ( i*2 ) , &nCommStatus ) * 2 ) + READ_DIGITAL( doGATE_TMPM1_Close + ( i*2 ) , &nCommStatus );
//			if      ( nData == 1 ) {
//				nData2 = ( READ_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , &nCommStatus ) * 2 ) + READ_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , &nCommStatus );
//				if ( nData2 != 1 ) {
//					if ( nData2 == 0 ) {
//						WRITE_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , 0 , &nCommStatus );
//						WRITE_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , 1 , &nCommStatus );
//					}
//					else {
//						WRITE_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , 0 , &nCommStatus );
//						WRITE_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , 0 , &nCommStatus );
//					}
//				}
//			}
//			else if ( nData == 2 ) {
//				nData2 = ( READ_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , &nCommStatus ) * 2 ) + READ_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , &nCommStatus );
//				if ( nData2 != 2 ) {
//					if ( nData2 == 0 ) {
//						WRITE_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , 1 , &nCommStatus );
//						WRITE_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , 0 , &nCommStatus );
//					}
//					else {
//						WRITE_DIGITAL( diGATE_TMPM1_Opened + ( i*2 ) , 0 , &nCommStatus );
//						WRITE_DIGITAL( diGATE_TMPM1_Closed + ( i*2 ) , 0 , &nCommStatus );
//					}
//				}
//			}
//		}
//			
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM1_Vac_Simulate() {

//	int nCommStatus;
//	int nRoughDryPump_Start , VA , VB , VC , VD , S_C = 0 , T_C;
//	double VE;
//	int nTimeData;

//	while(TRUE) {
		//==========================================================================================
//		if ( S_C > 0 ) {
			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_BM1_GoVent_Time , &nCommStatus );
			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else if ( S_C < 0 ) {
			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_BM1_GoPump_Time , &nCommStatus );
			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else {
//			nTimeData = 100;
//		}
//		_sleep( nTimeData );
		//==========================================================================================
//		if ( READ_DIGITAL( dpPUMP_BM1_Control , &nCommStatus ) == 2 ) {
//			nRoughDryPump_Start = READ_DIGITAL(dvVAC_TM_RoughDryPMP_Start, &nCommStatus);
//		}
//		else {
//			nRoughDryPump_Start = READ_DIGITAL(dvVAC_BM1_RoughDryPMP_Start, &nCommStatus);
//		}
		//==========================================================================================
//		S_C = 0;
//		T_C = 0;

//		VA = READ_DIGITAL( dioVAC_BM1_SlowRoughingValve , &nCommStatus );
//		VB = READ_DIGITAL( dioVAC_BM1_RoughingValve , &nCommStatus );
//		VC = READ_DIGITAL( dioVAC_BM1_ProfiledVentValve , &nCommStatus );
//		VD = READ_DIGITAL( dioVAC_BM1_FastVentValve , &nCommStatus );
//		VE = READ_ANALOG( aiVAC_BM1_CG_Pressure , &nCommStatus ) / PRESSURE_SCALE;

//		if (nRoughDryPump_Start) {
//			if      (         VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   5 ) S_C = +1;	else if ( VE >   5 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 500 ) S_C = +1;	else if ( VE > 500 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  50 ) S_C = +1;	else if ( VE >  50 ) S_C = -1;	}
//			}
//			else if (         VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   1 ) S_C = +1;	else if ( VE >   1 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 100 ) S_C = +1;	else if ( VE > 100 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   2 ) S_C = +1;	else if ( VE >   2 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 200 ) S_C = +1;	else if ( VE > 200 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   6 ) S_C = +1;	else if ( VE >   6 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 600 ) S_C = +1;	else if ( VE > 600 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  60 ) S_C = +1;	else if ( VE >  60 ) S_C = -1;	}
//			}
//			else if ( !VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +6; }
//				else				 {	S_C = +6; }
//			}
//			else if ( !VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +3; }
//				else				 {	S_C = +3; }
//			}
//			else if (  VA &&  VB && !VC && !VD ) {
//				S_C = -3;
//			}
//			else if ( !VA &&  VB && !VC && !VD ) {
//				S_C = -2;
//			}
//			else if (  VA && !VB && !VC && !VD ) {
//				S_C = -1;
//			}
//			else if ( !VA && !VB && !VC && !VD ) {
//			}
//		}
//		else {
//			if ( VA ) { S_C += 1; }
//			if ( VB ) { S_C += 3; }
//			if ( VC ) { S_C += 1; }
//			if ( VD ) { S_C += 3; }
//		}
//		Pressure_Find_1( aiVAC_BM1_CG_Pressure       , diVAC_BM1_ATM_Sensor , S_C , T_C );
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void BM2_Vac_Simulate() {

//	int nCommStatus;
//	int nRoughDryPump_Start, VA, VB, VC, VD, S_C = 0, T_C;
//	int nTimeData;
//	double VE;

//	while(TRUE) {
		//==========================================================================================
//		if ( S_C > 0 ) {
			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_BM2_GoVent_Time , &nCommStatus );
			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else if ( S_C < 0 ) {
			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_BM2_GoPump_Time , &nCommStatus );
			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else {
//			nTimeData = 100;
//		}
//		_sleep( nTimeData );
		//==========================================================================================
//		if ( READ_DIGITAL( dpPUMP_BM2_Control , &nCommStatus ) == 2 ) {
//			nRoughDryPump_Start = READ_DIGITAL(dvVAC_TM_RoughDryPMP_Start, &nCommStatus);
//		}
//		else {
//			nRoughDryPump_Start = READ_DIGITAL(dvVAC_BM2_RoughDryPMP_Start, &nCommStatus);
//		}
		//==========================================================================================
//		S_C = 0;
//		T_C = 0;

//		VA = READ_DIGITAL( dioVAC_BM2_SlowRoughingValve , &nCommStatus );
//		VB = READ_DIGITAL( dioVAC_BM2_RoughingValve , &nCommStatus );
//		VC = READ_DIGITAL( dioVAC_BM2_ProfiledVentValve , &nCommStatus );
//		VD = READ_DIGITAL( dioVAC_BM2_FastVentValve , &nCommStatus );
//		VE = READ_ANALOG( aiVAC_BM2_CG_Pressure , &nCommStatus ) / PRESSURE_SCALE;

//		if (nRoughDryPump_Start) {
//			if      (         VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   5 ) S_C = +1;	else if ( VE >   5 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 500 ) S_C = +1;	else if ( VE > 500 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  50 ) S_C = +1;	else if ( VE >  50 ) S_C = -1;	}
//			}
//			else if (         VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   1 ) S_C = +1;	else if ( VE >   1 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 100 ) S_C = +1;	else if ( VE > 100 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   2 ) S_C = +1;	else if ( VE >   2 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 200 ) S_C = +1;	else if ( VE > 200 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   6 ) S_C = +1;	else if ( VE >   6 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 600 ) S_C = +1;	else if ( VE > 600 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  60 ) S_C = +1;	else if ( VE >  60 ) S_C = -1;	}
//			}
//			else if ( !VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +6; }
//				else				 {	S_C = +6; }
//			}
//			else if ( !VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +3; }
//				else				 {	S_C = +3; }
//			}
//			else if (  VA &&  VB && !VC && !VD ) {
//				S_C = -3;
//			}
//			else if ( !VA &&  VB && !VC && !VD ) {
//				S_C = -2;
//			}
//			else if (  VA && !VB && !VC && !VD ) {
//				S_C = -1;
//			}
//			else if ( !VA && !VB && !VC && !VD ) {
//			}
//		}
//		else {
//			if ( VA ) { S_C += 1; }
//			if ( VB ) { S_C += 3; }
//			if ( VC ) { S_C += 1; }
//			if ( VD ) { S_C += 3; }
//		}
//		Pressure_Find_1( aiVAC_BM2_CG_Pressure       , diVAC_BM2_ATM_Sensor , S_C , T_C );
		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//void TM_Vac_Simulate() {
//
//	int nCommStatus;
//	int nRoughDryPump_Start, VA, VB, VC, VD, S_C = 0, T_C;
//	int nTimeData;
//	double VE;
//
//	while(TRUE) {
//		//==========================================================================================
//		if ( S_C > 0 ) {
//			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_TM_GoVent_Time , &nCommStatus );
//			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else if ( S_C < 0 ) {
//			//==========================================================================================
//			nTimeData = READ_DIGITAL( dpSIM_TM_GoPump_Time , &nCommStatus );
//			//==========================================================================================
//			if ( nTimeData <= 0 ) {
//				_sleep(100);
//				continue;
//			}
//		}
//		else {
//			nTimeData = 100;
//		}
//		_sleep( nTimeData );
//
//
//		//==========================================================================================
//		nRoughDryPump_Start = READ_DIGITAL(dvVAC_TM_RoughDryPMP_Start, &nCommStatus);
//		//==========================================================================================
//
//		S_C = 0;
//		T_C = 0;
//
//		VA = READ_DIGITAL( dioVAC_TM_SlowRoughingValve , &nCommStatus );
//		VB = READ_DIGITAL( dioVAC_TM_RoughingValve , &nCommStatus );
//		VC = READ_DIGITAL( dioVAC_TM_ProfiledVentValve , &nCommStatus );
//		VD = READ_DIGITAL( dioVAC_TM_FastVentValve , &nCommStatus );
//		VE = READ_ANALOG(aiVAC_TM_CG_Pressure, &nCommStatus) / PRESSURE_SCALE;
//
//		if (nRoughDryPump_Start) {
//			if      (         VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   5 ) S_C = +1;	else if ( VE >   5 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 500 ) S_C = +1;	else if ( VE > 500 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  50 ) S_C = +1;	else if ( VE >  50 ) S_C = -1;	}
//			}
//			else if (         VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   1 ) S_C = +1;	else if ( VE >   1 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 100 ) S_C = +1;	else if ( VE > 100 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   2 ) S_C = +1;	else if ( VE >   2 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 200 ) S_C = +1;	else if ( VE > 200 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//			}
//			else if (  VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <   6 ) S_C = +1;	else if ( VE >   6 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	T_C = 0;	if ( VE < 600 ) S_C = +1;	else if ( VE > 600 ) S_C = -1;	}
//				else				 {	T_C = 0;	if ( VE <  60 ) S_C = +1;	else if ( VE >  60 ) S_C = -1;	}
//			}
//			else if ( !VA && !VB &&         VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  20 ) S_C = +1;	else if ( VE >  20 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +6; }
//				else				 {	S_C = +6; }
//			}
//			else if ( !VA && !VB &&  VC && !VD ) {
//				if      ( T_C < 0 ) {	T_C = 0;	if ( VE <  10 ) S_C = +1;	else if ( VE >  10 ) S_C = -1;	}
//				else if ( T_C > 0 ) {	S_C = +3; }
//				else				 {	S_C = +3; }
//			}
//			else if (  VA &&  VB && !VC && !VD ) {
//				S_C = -3;
//			}
//			else if ( !VA &&  VB && !VC && !VD ) {
//				S_C = -2;
//			}
//			else if (  VA && !VB && !VC && !VD ) {
//				S_C = -1;
//			}
//			else if ( !VA && !VB && !VC && !VD ) {
//			}
//		}
//		else {
//			if ( VA ) { S_C += 1; }
//			if ( VB ) { S_C += 3; }
//			if ( VC ) { S_C += 1; }
//			if ( VD ) { S_C += 3; }
//		}
//		Pressure_Find_1(aiVAC_TM_CG_Pressure, diVAC_TM_ATM_Sensor, S_C, T_C);
//		//==========================================================================================
//	}
//	_endthread();
//}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Program_Main() {

	int nCommStatus;
	int nRoughDryPump_Start , nT_ROUGH;

	if ( READ_DIGITAL( IO_Driver_Loding , &nCommStatus ) == 1 ) return SYS_SUCCESS;

	//-------------------------------------------------------------------------------------

	WRITE_DIGITAL( diLP_CM1_Placed			, 0 , &nCommStatus );

	WRITE_DIGITAL( diLP_CM1_Door_Status		, 0 , &nCommStatus );
//	WRITE_DIGITAL( diDOOR_CM1_Downed		, 0 , &nCommStatus );
	WRITE_DIGITAL( diLP_CM1_Shuttle_Status	, 0 , &nCommStatus );
//	WRITE_DIGITAL( diSHUTT_CM1_Out			, 1 , &nCommStatus );
	WRITE_DIGITAL( diLP_CM1_Clamp_Status	, 0 , &nCommStatus );
//	WRITE_DIGITAL( diLP_CM1_UnClamp_Status	, 1 , &nCommStatus );

	WRITE_DIGITAL( doLP_CM1_Door_Ctrl		, 0 , &nCommStatus );
	WRITE_DIGITAL( doLP_CM1_Docking			, 0 , &nCommStatus );
	WRITE_DIGITAL( doLP_CM1_Clamp			, 0 , &nCommStatus );
	WRITE_DIGITAL( doLP_CM1_UnClamp			, 1 , &nCommStatus );
	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------
	//WRITE_DIGITAL( diRB_TMPM1_Not_Extend , 1 , &nCommStatus );
	//WRITE_DIGITAL( diRB_TMPM2_Not_Extend , 1 , &nCommStatus );
	//WRITE_DIGITAL( diRB_TMPM3_Not_Extend , 1 , &nCommStatus );
	//-------------------------------------------------------------------------------------
	//WRITE_DIGITAL( diAIR_TM_Pressure , 1 , &nCommStatus );
	//-------------------------------------------------------------------------------------
	//WRITE_DIGITAL( dvVAC_TM_RoughDryPMP_Alarm  , 1 , &nCommStatus );
	//	
	//WRITE_ANALOG( aiVAC_TM_CG_Foreline_Pressure , 100 * PRESSURE_SCALE , &nCommStatus );
	//-------------------------------------------------------------------------------------
	//WRITE_ANALOG(aiVAC_TM_CG_Pressure, 100 * PRESSURE_SCALE, &nCommStatus);
	//-------------------------------------------------------------------------------------
	//WRITE_DIGITAL( diPRESS_TM_N2_High	, 1 , &nCommStatus );
	//WRITE_DIGITAL( diPRESS_TM_N2_Low	, 1 , &nCommStatus );
	//WRITE_DIGITAL( diLID_TM_Closed		, 1 , &nCommStatus );
	//-------------------------------------------------------------------------------------
	WRITE_DIGITAL( INTLKS_CONTROL	, 0 , &nCommStatus );


	_beginthread( (void *) CM_Simulate , 0 , 0 );
	//
	//
	//_beginthread( (void *) TM_Door_Simulate , 0 , 0 );
	//
	//
	//_beginthread( (void *) TM_Vac_Simulate , 0 , 0 );


//	while ( TRUE ) {
//		_sleep( 500 );
//		//==========================================================================================
//		//nRoughDryPump_Start = READ_DIGITAL( dvVAC_TM_RoughDryPMP_Start , &nCommStatus );
//		//WRITE_DIGITAL( dvVAC_BM1_RoughDryPMP , nRoughDryPump_Start , &nCommStatus );
//		//==========================================================================================
//		if ( nRoughDryPump_Start ) {
//			nT_ROUGH = -4;
//		}
//		else {
//			nT_ROUGH = 4;
//		}
////		Pressure_Find_2( aiVAC_BM1_CG_Foreline_Pressure , nT_ROUGH );
//		//==========================================================================================
//		//==========================================================================================
//		//nRoughDryPump_Start = READ_DIGITAL( dvVAC_TM_RoughDryPMP_Start , &nCommStatus );
//		//WRITE_DIGITAL( dvVAC_BM2_RoughDryPMP , nRoughDryPump_Start , &nCommStatus );
//		//==========================================================================================
//		if ( nRoughDryPump_Start ) {
//			nT_ROUGH = -4;
//		}
//		else {
//			nT_ROUGH = 4;
//		}
////		Pressure_Find_2( aiVAC_BM2_CG_Foreline_Pressure , nT_ROUGH );
//		//==========================================================================================
//		//==========================================================================================
//		//nRoughDryPump_Start = READ_DIGITAL( dvVAC_TM_RoughDryPMP_Start , &nCommStatus );
//		//WRITE_DIGITAL( dvVAC_TM_RoughDryPMP , nRoughDryPump_Start , &nCommStatus );
//		//==========================================================================================
//		if ( nRoughDryPump_Start ) {
//			nT_ROUGH = -4;
//		}
//		else {
//			nT_ROUGH = 4;
//		}
//		//Pressure_Find_2( aiVAC_TM_CG_Foreline_Pressure , nT_ROUGH );
//		//==========================================================================================
//	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Screen_Nav_Info( int no ) {

}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
BOOL Program_Init_Code() {
return TRUE;
}
