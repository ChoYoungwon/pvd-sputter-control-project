//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <kutlgui.h>
#include "iodefine.h"
#include "resource.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {

	{ "e:SCHEDULER"						, _K_S_IO	,	SCHEDULER						,	0	} ,
	{ "CTC.INFO_SAVE"					, _K_D_IO	,	INFO_SAVE						,	0	} ,
	//---------------------------------------------------------------------------------------------
	{ "SCHEDULER_ERROR"					, _K_F_IO	,	SCHEDULER_ERROR					,	0	} ,
	{ "CTC.TR_STATUS"					, _K_D_IO	,	CTC_TR_STATUS					,	0	} ,
	//---------------------------------------------------------------------------------------------
	{ "SCHEDULER_MAINT_PM1_ALL"			, _K_F_IO	,	SCHEDULER_MAINT_PM1			,	0	} ,
	{ "SCHEDULER_MAINT_PM2_ALL"			, _K_F_IO	,	SCHEDULER_MAINT_PM2				,	0	} ,
	//{ "SCHEDULER_MAINT_PM3_ALL"			, _K_F_IO	,	SCHEDULER_MAINT_PM3				,	0	} ,
	{ "SCHEDULER_MAINT_TM_ALL"			, _K_F_IO	,	SCHEDULER_MAINT_TM				,	0	} ,
	//---------------------------------------------------------------------------------------------
	{ "CTC.SYS_AL_CONTROL"				, _K_D_IO	,	CTC_SYS_AL_CONTROL				,	0	} ,
	{ "SYS_AL_Arm"						, _K_D_IO	,	SYS_AL_Arm						,	0	} ,
	{ "SYS_AL_Station"					, _K_D_IO	,	SYS_AL_Station					,	0	} ,
	{ "SYS_AL_Slot"						, _K_D_IO	,	SYS_AL_Slot						,	0	} ,
	
	//---------------------------------------------------------------------------------------------
	{ "CTC.TA_Wafer_Status"				, _K_D_IO	,	TA_Wafer_Status					,	0	} ,

	{ "CTC.PM1_Wafer_Status"			, _K_D_IO	,	PM1_Wafer_Status				,	0	} ,
	{ "CTC.PM2_Wafer_Status"			, _K_D_IO	,	PM2_Wafer_Status				,	0	} ,
	{ "CTC.PM3_Wafer_Status"			, _K_D_IO	,	PM3_Wafer_Status				,	0	} ,

	{ "CTC.FA_Wafer_Status"				, _K_D_IO	,	CTC_FA_Wafer_Status				,	0	} ,
	{ "CTC.FA_Wafer_Source"				, _K_D_IO	,	CTC_FA_Wafer_Source				,	0	} ,
	{ "CTC.FB_Wafer_Status"				, _K_D_IO	,	CTC_FB_Wafer_Status				,	0	} ,
	{ "CTC.FB_Wafer_Source"				, _K_D_IO	,	CTC_FB_Wafer_Source				,	0	} ,
	{ "CTC.FM_AL_Wafer_Status"			, _K_D_IO	,	CTC_FM_AL_Wafer_Status			,	0	} ,

	{ "CM1.C01_Wafer"					, _K_D_IO	,	CM1_C01_Wafer					,	0	} ,
	{ "CM1.C02_Wafer"					, _K_D_IO	,	CM1_C02_Wafer					,	0	} ,
	{ "CM1.C03_Wafer"					, _K_D_IO	,	CM1_C03_Wafer					,	0	} ,
	{ "CM1.C04_Wafer"					, _K_D_IO	,	CM1_C04_Wafer					,	0	} ,
	{ "CM1.C05_Wafer"					, _K_D_IO	,	CM1_C05_Wafer					,	0	} ,
	{ "CM1.C06_Wafer"					, _K_D_IO	,	CM1_C06_Wafer					,	0	} ,
	{ "CM1.C07_Wafer"					, _K_D_IO	,	CM1_C07_Wafer					,	0	} ,
	{ "CM1.C08_Wafer"					, _K_D_IO	,	CM1_C08_Wafer					,	0	} ,
	{ "CM1.C09_Wafer"					, _K_D_IO	,	CM1_C09_Wafer					,	0	} ,
	{ "CM1.C10_Wafer"					, _K_D_IO	,	CM1_C10_Wafer					,	0	} ,
	{ "CM1.C11_Wafer"					, _K_D_IO	,	CM1_C11_Wafer					,	0	} ,
	{ "CM1.C12_Wafer"					, _K_D_IO	,	CM1_C12_Wafer					,	0	} ,
	{ "CM1.C13_Wafer"					, _K_D_IO	,	CM1_C13_Wafer					,	0	} ,
	{ "CM1.C14_Wafer"					, _K_D_IO	,	CM1_C14_Wafer					,	0	} ,
	{ "CM1.C15_Wafer"					, _K_D_IO	,	CM1_C15_Wafer					,	0	} ,
	{ "CM1.C16_Wafer"					, _K_D_IO	,	CM1_C16_Wafer					,	0	} ,
	{ "CM1.C17_Wafer"					, _K_D_IO	,	CM1_C17_Wafer					,	0	} ,
	{ "CM1.C18_Wafer"					, _K_D_IO	,	CM1_C18_Wafer					,	0	} ,
	{ "CM1.C19_Wafer"					, _K_D_IO	,	CM1_C19_Wafer					,	0	} ,
	{ "CM1.C20_Wafer"					, _K_D_IO	,	CM1_C20_Wafer					,	0	} ,
	{ "CM1.C21_Wafer"					, _K_D_IO	,	CM1_C21_Wafer					,	0	} ,
	{ "CM1.C22_Wafer"					, _K_D_IO	,	CM1_C22_Wafer					,	0	} ,
	{ "CM1.C23_Wafer"					, _K_D_IO	,	CM1_C23_Wafer					,	0	} ,
	{ "CM1.C24_Wafer"					, _K_D_IO	,	CM1_C24_Wafer					,	0	} ,
	{ "CM1.C25_Wafer"					, _K_D_IO	,	CM1_C25_Wafer					,	0	} ,

	""
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status MAINT_INTERFACE_RUN( LPSTR szList , ... ) {

	va_list va;
	int nCommStatus;
	char szBuffer[256];
	char szBuffer2[256];
	va_start(va, szList);
	vsprintf(szBuffer2, szList, (LPSTR)va);
	va_end( va );

	WRITE_DIGITAL(CTC_TR_STATUS, 0, &nCommStatus);
	sprintf( szBuffer , "MAINT_INTERFACE %s" , szBuffer2 );
	WRITE_STRING(SCHEDULER, szBuffer, &nCommStatus);

	while ( TRUE ) {
		if ( !WAIT_SECONDS( 0.25 ) ) return SYS_ABORTED;

		switch (READ_DIGITAL(CTC_TR_STATUS, &nCommStatus)) {

		case SYS_ABORTED : return SYS_ABORTED;
		case SYS_ERROR   : return SYS_ERROR;
		case SYS_SUCCESS : return SYS_SUCCESS;
		}
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Program_Main() {

	int nResult , nCommStatus;
	int nSelect;

	if ( STRCMP_L( "SYS_AL_HOME" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 0 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_PICK" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 1 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_PICK BM1" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 1 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_PICK BM2" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 1 , 1 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_PLACE" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 2 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_ROTATE" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 3 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_EXTEND" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 4 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_RETRACT" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 5 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_UP" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 6 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_DOWN" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = Maint_Alignment( 1 , 7 , 0 );
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "SYS_AL_INIT" , PROGRAM_PARAMETER_READ() ) ) {
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Function is not defined!", "OK", 0, &nSelect );
		nResult = SYS_ERROR;
		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "GO_INIT" , PROGRAM_PARAMETER_READ() ) ) {

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM1 , "GO_INIT" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM2 , "GO_INIT" );
		//RUN_SET_FUNCTION( SCHEDULER_MAINT_PM3 , "GO_INIT" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_TM  , "GO_INIT" );

		while( TRUE ) {
			if ( !WAIT_SECONDS( 0.1 ) ) {

				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM1 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM2 );
				//RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM3 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_TM );
				break;

			}

			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM1 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM2 );	if ( nResult == SYS_RUNNING ) continue;
			//nResult = READ_FUNCTION( SCHEDULER_MAINT_PM3 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_TM );	if ( nResult == SYS_RUNNING ) continue;
			break;

		}

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );

	}
	else if ( STRCMP_L( "GO_MAINT" , PROGRAM_PARAMETER_READ() ) ) {

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM1 , "GO_MAINT" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM2 , "GO_MAINT" );
		//RUN_SET_FUNCTION( SCHEDULER_MAINT_PM3 , "GO_MAINT" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_TM  , "GO_MAINT" );

		while( TRUE ) {

			if ( !WAIT_SECONDS( 0.1 ) ) {

				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM1 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM2 );
				//RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM3 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_TM );
				break;

			}

			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM1 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM2 );	if ( nResult == SYS_RUNNING ) continue;
			//nResult = READ_FUNCTION( SCHEDULER_MAINT_PM3 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_TM );	if ( nResult == SYS_RUNNING ) continue;
			break;

		}

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else if ( STRCMP_L( "GO_STANDBY" , PROGRAM_PARAMETER_READ() ) ) {

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 1 , &nCommStatus );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM1 , "GO_STANDBY" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_PM2 , "GO_STANDBY" );
		//RUN_SET_FUNCTION( SCHEDULER_MAINT_PM3 , "GO_STANDBY" );
		RUN_SET_FUNCTION( SCHEDULER_MAINT_TM  , "GO_STANDBY" );

		while( TRUE ) {
			if ( !WAIT_SECONDS( 0.1 ) ) {

				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM1 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM2 );
				//RUN_FUNCTION_ABORT( SCHEDULER_MAINT_PM3 );
				RUN_FUNCTION_ABORT( SCHEDULER_MAINT_TM );
				break;

			}

			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM1 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_PM2 );	if ( nResult == SYS_RUNNING ) continue;
			//nResult = READ_FUNCTION( SCHEDULER_MAINT_PM3 );	if ( nResult == SYS_RUNNING ) continue;
			nResult = READ_FUNCTION( SCHEDULER_MAINT_TM );	if ( nResult == SYS_RUNNING ) continue;
			break;

		}

		WRITE_DIGITAL( CTC_SYS_AL_CONTROL , 0 , &nCommStatus );
	}

	else {
		nResult = SYS_ABORTED;
	}

	WRITE_DIGITAL( INFO_SAVE , 1 , &nCommStatus );
	return nResult;
}