#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>
#include <direct.h>
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include <KutlAnal.h>
#include <KutlEtc.h>
#include <KutlFile.h>
#include <Kutlstr.h>
#include <Kutltime.h>
#include <kutlgui.h>
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include "cimseqnc.h"
#include "iodefine.h"
#include "resource.h"
#include "../Library/kutlCheck.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define		Max_Slot_Num					25
#define		Max_Module_Count				23
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum { eOFF_0 , eON_1 };
enum { MOVE_A_POS , EXTEND_A_POS , ROTATE_A_POS , MOVE_A_RNG , EXTEND_A_RNG , ROTATE_A_RNG , MOVE_A_DLY , EXTEND_A_DLY , ROTATE_A_DLY ,
	   MOVE_B_POS , EXTEND_B_POS , ROTATE_B_POS , MOVE_B_RNG , EXTEND_B_RNG , ROTATE_B_RNG , MOVE_B_DLY , EXTEND_B_DLY , ROTATE_B_DLY ,
	   RET_RNG , RET_DLY , UPDOWN_DLY , ROTATION_STYLE , ARM_STYLE , BASE_LIMIT };
enum { CM1 , CM2 , CM3 , CM4 , PM1 , PM2 , PM3 , PM4 , PM5 , PM6 , PM7 , PM8 , PM9 , PM10 , PM11 , PM12 , AL , IC , BM1 , BM2 , BM3 , BM4 , HOM };
enum { CCW_HOMEBASED , CW_HOMEBASED , CCW_ROTATION , CW_ROTATION , MINIMUM };
enum { SETUP , HOME , PICK , PLACE , ROTATE , EXTEND , RETRACT, PLACE_AL }; 
enum { NORMAL_STATUS , UP_STATUS , DOWN_STATUS };
enum { USING_A , USING_B , USING_AB };
enum { FINGER_A , FINGER_B , FINGER_AB };
enum { NORMAL , INCREASE , DECREASE };
enum { ROBOT_SYNCH_READY , ROBOT_SYNCH_ROTATE_START , ROBOT_SYNCH_EXTEND_START , ROBOT_SYNCH_UPDATE , ROBOT_SYNCH_RETRACT_START , ROBOT_SYNCH_RESET_REVERSE , ROBOT_SYNCH_RESET_ONLY , ROBOT_SYNCH_SUCCESS };

enum { eA, eB, eC, eD, eF, eG, eH, eI, eK=10 };

enum { PRESENT, SUCCESS , FAILURE, PROCESSING };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//												 CM1     CM2 	CM3     CM4     PM1     PM2     PM3     PM4     PM5     PM6     PM7     PM8     PM9    PM10    PM11    PM12     AL      IC     BM1     BM2     BM3      BM4      HOM
double	MOVA[Max_Module_Count]= {0}; //    = {   -64 ,     0 ,    63 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,  70.5 ,   -55 ,   -45 ,    25 ,     0 ,     0 ,     20 };
double	EXTA[Max_Module_Count]= {0}; //    = {    46 ,	46 ,    46 ,	 0 ,    91 ,    91 ,    95 ,    95 ,    95 ,    95 ,    95 ,    91 ,    95 ,    95 ,    95 ,    95 ,    61 ,    31 ,    85 ,    60 ,     0 ,     0 ,     20 };
double	ROTA[Max_Module_Count]= {0}; //    = {	 0 ,	 0 ,     0 ,    30 ,   180 ,   180 ,    89 ,    89 ,    89 ,    89 ,    89 ,   180 ,    89 ,    89 ,    89 ,    89 ,    90 ,   241 ,   180 ,   180 ,     0 ,     0 ,     20 };
double	MOVB[Max_Module_Count]= {0}; //    = {   -64 ,     0 ,    63 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,  70.5 ,   -55 ,   -45 ,    25 ,     0 ,     0 ,     20 };
double	EXTB[Max_Module_Count]= {0}; //    = {    46 ,	46 ,    46 ,	 0 ,    91 ,    91 ,    95 ,    95 ,    95 ,    95 ,    95 ,    91 ,    95 ,    95 ,    95 ,    95 ,    61 ,    31 ,    85 ,    60 ,     0 ,     0 ,     20 };
double	ROTB[Max_Module_Count]= {0}; //    = {	 0 ,	 0 ,     0 ,    30 ,   180 ,   180 ,    89 ,    89 ,    89 ,    89 ,    89 ,   180 ,    89 ,    89 ,    89 ,    89 ,    90 ,   241 ,   180 ,   180 ,     0 ,     0 ,     20 };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

double	MOVA_Rng[Max_Module_Count]= {0}; // = {    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 };
double	EXTA_Rng[Max_Module_Count]= {0}; // = {     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 };
double	ROTA_Rng[Max_Module_Count]= {0}; // = {     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 };
double	MOVB_Rng[Max_Module_Count]= {0}; // = {    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 ,    10 };
double	EXTB_Rng[Max_Module_Count]= {0}; // = {     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 };
double	ROTB_Rng[Max_Module_Count]= {0}; // = {     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 ,     7 };
double	Ret_Rng[2]				 = {     7 ,     7 };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
double	MOVA_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	EXTA_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	ROTA_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	MOVB_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	EXTB_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	ROTB_Dly[Max_Module_Count]= {0}; // = {  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 ,  0.05 };
double	Ret_Dly[2]				 = {  0.05 ,  0.05 };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
double	gdbUPDOWN_DELAY[2]	= { 0.3 , 0.3 };
double	gdbBaseLimit[2]		= { 0   , 0   };
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int		ROTATION_TYPE	= CCW_HOMEBASED;
int		ARM_TYPE		= USING_AB;
int		UPDOWNSTATUS	= NORMAL_STATUS;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//enum {                      CM1 ,  CM2  , CM3   , CM4   , PM1   , PM2   , PM3   , PM4   , PM5   , PM6   , PM7   , PM8   , PM9   , PM10   , PM11   , HOM   , AL   , IC   , BM1   , BM2   , BM3   , BM4   , FAL   , FIC  };
char	Chamber[23][10] = { "CM1" , "CM2" , "CM3" , "CM4" , "PM1" , "PM2" , "PM3" , "PM4" , "PM5" , "PM6" , "PM7" , "PM8" , "PM9" , "PM10" , "PM11" , "PM12" , "AL" , "IC" , "BM1" , "BM2" , "BM3" , "BM4" , "HOM" };
char	ArmType[2][2]  = { "A" , "B" };
char	gszPathSetup[256] = ".\\Sequence\\SeqRobotAnimation\\Setup_Robot.cfg";
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = 
{
	{ "CTC.TA_Wafer_Status"				, _K_D_IO	,	TA_Wafer_Status					,	0	} ,
	{ "CTC.TB_Wafer_Status"				, _K_D_IO	,	TB_Wafer_Status					,	0	} ,
	
	{ "CTC.PM1_Wafer_Status"			, _K_D_IO	,	PM1_Wafer_Status				,	0	} ,
	{ "CTC.PM2_Wafer_Status"			, _K_D_IO	,	PM2_Wafer_Status				,	0	} ,
	{ "CTC.PM3_Wafer_Status"			, _K_D_IO	,	PM3_Wafer_Status				,	0	} ,
	{ "CTC.PM4_Wafer_Status"			, _K_D_IO	,	PM4_Wafer_Status				,	0	} ,
	{ "CTC.PM5_Wafer_Status"			, _K_D_IO	,	PM5_Wafer_Status				,	0	} ,
	{ "CTC.PM6_Wafer_Status"			, _K_D_IO	,	PM6_Wafer_Status				,	0	} ,

	{ "CTC.BM1_Wafer_Status"			, _K_D_IO	,	BM1_Wafer_Status				,	0	} ,
	{ "CTC.BM1_Wafer_Status2"			, _K_D_IO	,	BM1_Wafer_Status2				,	0	} ,

	{ "CTC.BM2_Wafer_Status"			, _K_D_IO	,	BM2_Wafer_Status				,	0	} ,
	{ "CTC.BM2_Wafer_Status2"			, _K_D_IO	,	BM2_Wafer_Status2				,	0	} ,

	{ "CTC.TA_Wafer_Source"				, _K_D_IO	,	TA_Wafer_Source					,	0	} ,
	{ "CTC.TB_Wafer_Source"				, _K_D_IO	,	TB_Wafer_Source					,	0	} ,
	
	{ "CTC.PM1_Wafer_Source"			, _K_D_IO	,	PM1_Wafer_Source				,	0	} ,
	{ "CTC.PM2_Wafer_Source"			, _K_D_IO	,	PM2_Wafer_Source				,	0	} ,
	{ "CTC.PM3_Wafer_Source"			, _K_D_IO	,	PM3_Wafer_Source				,	0	} ,
	{ "CTC.PM4_Wafer_Source"			, _K_D_IO	,	PM4_Wafer_Source				,	0	} ,
	{ "CTC.PM5_Wafer_Source"			, _K_D_IO	,	PM5_Wafer_Source				,	0	} ,
	{ "CTC.PM6_Wafer_Source"			, _K_D_IO	,	PM6_Wafer_Source				,	0	} ,

	{ "CTC.BM1_Wafer_Source"			, _K_D_IO	,	BM1_Wafer_Source				,	0	} ,
	{ "CTC.BM1_Wafer_Source2"			, _K_D_IO	,	BM1_Wafer_Source2				,	0	} ,

	{ "CTC.BM2_Wafer_Source"			, _K_D_IO	,	BM2_Wafer_Source				,	0	} ,
	{ "CTC.BM2_Wafer_Source2"			, _K_D_IO	,	BM2_Wafer_Source2				,	0	} ,

	{ "CTC.TA_Wafer_Result"				, _K_D_IO	,	TA_Wafer_Result					,	0	} ,
	{ "CTC.TB_Wafer_Result"				, _K_D_IO	,	TB_Wafer_Result					,	0	} ,

	{ "CTC.PM1_Wafer_Result"			, _K_D_IO	,	PM1_Wafer_Result				,	0	} ,
	{ "CTC.PM2_Wafer_Result"			, _K_D_IO	,	PM2_Wafer_Result				,	0	} ,
	{ "CTC.PM3_Wafer_Result"			, _K_D_IO	,	PM3_Wafer_Result				,	0	} ,
	{ "CTC.PM4_Wafer_Result"			, _K_D_IO	,	PM4_Wafer_Result				,	0	} ,
	{ "CTC.PM5_Wafer_Result"			, _K_D_IO	,	PM5_Wafer_Result				,	0	} ,
	{ "CTC.PM6_Wafer_Result"			, _K_D_IO	,	PM6_Wafer_Result				,	0	} ,

	{ "CTC.BM1_Wafer_Result"			, _K_D_IO	,	BM1_Wafer_Result				,	0	} ,
	{ "CTC.BM1_Wafer_Result2"			, _K_D_IO	,	BM1_Wafer_Result2				,	0	} ,

	{ "CTC.BM2_Wafer_Result"			, _K_D_IO	,	BM2_Wafer_Result				,	0	} ,
	{ "CTC.BM2_Wafer_Result2"			, _K_D_IO	,	BM2_Wafer_Result2				,	0	} ,

	{ "CTC.Rb_Extension"				, _K_A_IO	,	Rb_Extension					,	0	} ,
	{ "CTC.Rb_Extension2"				, _K_A_IO	,	Rb_Extension2					,	0	} ,
	{ "CTC.Rb_Rotation"					, _K_A_IO	,	Rb_Rotation						,	0	} ,
	{ "CTC.Rb_UpDown"					, _K_A_IO	,	Rb_UpDown						,	0	} ,
	{ "CTC.Rb_Movement"					, _K_A_IO	,	Rb_Movement						,	0	} ,

	{ "TM.ROBOT_PAUSE_CONTROL"			, _K_D_IO	,	ROBOT_PAUSE_CONTROL				,	0	} ,
	{ "TM.RB_SYNCH"						, _K_D_IO	,	RB_SYNCH						,	0	} ,

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

	{ "CTC.TA_Wafer_MID"				, _K_S_IO	,	TA_Wafer_MID					,	0	} ,
	{ "CTC.TB_Wafer_MID"				, _K_S_IO	,	TB_Wafer_MID					,	0	} ,

	{ "CTC.PM1_Wafer_MID"				, _K_S_IO	,	PM1_Wafer_MID					,	0	} ,
	{ "CTC.PM2_Wafer_MID"				, _K_S_IO	,	PM2_Wafer_MID					,	0	} ,
	{ "CTC.PM3_Wafer_MID"				, _K_S_IO	,	PM3_Wafer_MID					,	0	} ,
	{ "CTC.PM4_Wafer_MID"				, _K_S_IO	,	PM4_Wafer_MID					,	0	} ,
	{ "CTC.PM5_Wafer_MID"				, _K_S_IO	,	PM5_Wafer_MID					,	0	} ,
	{ "CTC.PM6_Wafer_MID"				, _K_S_IO	,	PM6_Wafer_MID					,	0	} ,
	
	""
};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Move_Template( int io , double trg , double rng , double dly )
 {
	int CommStatus , Synch , Data;
	double pos;

	pos = READ_ANALOG( io , &CommStatus );
	if ( pos > trg ) 
	{
		while ( TRUE ) 
		{
			Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
			if		( Synch == ROBOT_SYNCH_READY   ) 
				return SYS_ABORTED;
			else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
			{ 
				WRITE_ANALOG( io , trg , &CommStatus ); 
				return SYS_SUCCESS; 
			}

			while ( TRUE ) 
			{
				Data = READ_DIGITAL( ROBOT_PAUSE_CONTROL , &CommStatus );
				if ( Data != eON_1 ) 
					break;
				else 
				{
					if ( !WAIT_SECONDS( 0.2 ) ) 
					{ 
						WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); 
						return SYS_ABORTED; 
					}
				}
				Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
				if		( Synch == ROBOT_SYNCH_READY   ) 
				{ WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); return SYS_ABORTED; }
				else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
				{ WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); WRITE_ANALOG( io , trg , &CommStatus ); return SYS_SUCCESS; }
			}

			pos = pos - rng;
			if ( pos <= trg ) 
			{
				WRITE_ANALOG( io , trg , &CommStatus );
				break;
			}
			WRITE_ANALOG( io , pos , &CommStatus );
			if ( !WAIT_SECONDS( dly ) ) return SYS_ABORTED;
		}
	}
	else if ( pos < trg ) {
		while ( TRUE ) {
			Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
			if		( Synch == ROBOT_SYNCH_READY   ) return SYS_ABORTED;
			else if ( Synch == ROBOT_SYNCH_SUCCESS ) { WRITE_ANALOG( io , trg , &CommStatus ); return SYS_SUCCESS; }

			while ( TRUE ) {
				Data = READ_DIGITAL( ROBOT_PAUSE_CONTROL , &CommStatus );
				if ( Data != eON_1 ) break;
				else {
					if ( !WAIT_SECONDS( 0.2 ) ) { WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); return SYS_ABORTED; }
				}
				Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
				if		( Synch == ROBOT_SYNCH_READY   ) { WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); return SYS_ABORTED; }
				else if ( Synch == ROBOT_SYNCH_SUCCESS ) { WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); WRITE_ANALOG( io , trg , &CommStatus ); return SYS_SUCCESS; }
			}

			pos = pos + rng;
			if ( pos >= trg ) {
				WRITE_ANALOG( io , trg , &CommStatus );
				break;
			}
			WRITE_ANALOG( io , pos , &CommStatus );
			if ( !WAIT_SECONDS( dly ) ) return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Move_Template_AB( int io , double trg , double rng , double dly , int io2 , double trg2 , double rng2 , double dly2 )
 {
	int CommStatus , Synch , Data;
	double pos, pos2;

	pos = READ_ANALOG( io , &CommStatus );
	pos2 = READ_ANALOG( io2 , &CommStatus );


	if ( pos > trg ) 
	{
		while ( TRUE ) 
		{
			Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
			if		( Synch == ROBOT_SYNCH_READY   ) 
				return SYS_ABORTED;
			else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
			{ 
				WRITE_ANALOG( io , trg , &CommStatus ); 
				WRITE_ANALOG( io2 , trg2 , &CommStatus ); 
				return SYS_SUCCESS; 
			}

			while ( TRUE ) 
			{
				Data = READ_DIGITAL( ROBOT_PAUSE_CONTROL , &CommStatus );
				if ( Data != eON_1 ) 
					break;
				else 
				{
					if ( !WAIT_SECONDS( 0.2 ) ) 
					{ 
						WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); 
						return SYS_ABORTED; 
					}
				}
				Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
				if		( Synch == ROBOT_SYNCH_READY   ) 
				{ 
					WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); 
					return SYS_ABORTED; 
				}
				else if (Synch == ROBOT_SYNCH_SUCCESS)
				{
					WRITE_DIGITAL(ROBOT_PAUSE_CONTROL, eOFF_0, &CommStatus);
					WRITE_ANALOG(io, trg, &CommStatus);
					return SYS_SUCCESS;
				}
			}

			pos = pos - rng;
			pos2 = pos2 - rng2;
			if ( pos <= trg ) 
			{
				WRITE_ANALOG( io , trg , &CommStatus );
				break;
			}
			if ( pos2 <= trg2 ) 
			{
				WRITE_ANALOG( io2 , trg2 , &CommStatus );
				break;
			}
			WRITE_ANALOG( io , pos , &CommStatus );
			WRITE_ANALOG( io2 , pos2 , &CommStatus );
			if ( !WAIT_SECONDS( dly ) ) 
				return SYS_ABORTED;
		}
	}
	else if ( pos < trg ) 
	{
		while ( TRUE ) 
		{
			Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
			if		( Synch == ROBOT_SYNCH_READY   ) 
				return SYS_ABORTED;
			else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
			{ 
				WRITE_ANALOG( io , trg , &CommStatus ); 
				WRITE_ANALOG( io2 , trg2 , &CommStatus ); 
				return SYS_SUCCESS; 
			}

			while ( TRUE ) 
			{
				Data = READ_DIGITAL( ROBOT_PAUSE_CONTROL , &CommStatus );
				if ( Data != eON_1 ) 
					break;
				else 
				{
					if ( !WAIT_SECONDS( 0.2 ) ) 
					{ 
						WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus );
						return SYS_ABORTED; 
					}
				}
				Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
				if		( Synch == ROBOT_SYNCH_READY   ) 
				{ 
					WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); 
					return SYS_ABORTED; 
				}
				else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
				{ 
					WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); 
					WRITE_ANALOG( io , trg , &CommStatus ); 
					WRITE_ANALOG( io2 , trg2 , &CommStatus ); 
					return SYS_SUCCESS; 
				}
			}

			pos = pos + rng;
			pos2 = pos2 + rng2;
			if ( pos >= trg ) 
			{
				WRITE_ANALOG( io , trg , &CommStatus );
				break;
			}
			if ( pos2 >= trg2 ) 
			{
				WRITE_ANALOG( io2 , trg2 , &CommStatus );
				break;
			}
			WRITE_ANALOG( io , pos , &CommStatus );
			WRITE_ANALOG( io2 , pos2 , &CommStatus );
			if ( !WAIT_SECONDS( dly ) ) 
				return SYS_ABORTED;
		}
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Move_Template2( int io , double trg , double rng , int io2 , double trg2 , double rng2 , double dly ) 
{
	int CommStatus , Synch , Data , LimitRot = NORMAL;
	double pos , pos2 , m , m2 , TarRot , LastTrg , LastTrg2;
	BOOL md = TRUE , md2 = TRUE;


	LastTrg = trg; LastTrg2 = trg2;
	pos  = READ_ANALOG( io , &CommStatus );
	pos2 = READ_ANALOG( io2 , &CommStatus );

	if      ( pos > trg ) 
		m = -(rng);
	else if ( pos < trg ) 
		m = rng;
	else  
	{
		md = FALSE;
		m = 0;
	}

	if      ( pos2 > trg2 ) 
	{
		if ( pos2 < gdbBaseLimit[0] || trg2 >= gdbBaseLimit[0] ) 
			m2 = -(rng2);
		else 
		{ m2 = rng2; LimitRot = INCREASE; TarRot = trg2; trg2 = 359.9; }
	}
	else if ( pos2 < trg2 ) 
	{
		if ( trg2 <= gdbBaseLimit[0] || pos2 > gdbBaseLimit[0] ) 
			m2 = rng2;
		else 
		{ m2 = -(rng2); LimitRot = DECREASE; TarRot = trg2; trg2 = 0; }
	}
	else  
	{
		md2 = FALSE;
		m2 = 0;
	}

	while ( TRUE ) 
	{
		Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
		if		( Synch == ROBOT_SYNCH_READY   ) 
			return SYS_ABORTED;
		else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
		{ 
			WRITE_ANALOG( io  , LastTrg/*trg*/  , &CommStatus ); 
			WRITE_ANALOG( io2 , LastTrg2/*trg2*/ , &CommStatus ); 
			return SYS_SUCCESS; 
		}

		while ( TRUE ) 
		{
			Data = READ_DIGITAL( ROBOT_PAUSE_CONTROL , &CommStatus );
			if ( Data != eON_1 ) 
				break;
			else 
			{
				if ( !WAIT_SECONDS( 0.2 ) ) 
				{ WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); return SYS_ABORTED; }
			}
			Synch = READ_DIGITAL( RB_SYNCH , &CommStatus );
			if		( Synch == ROBOT_SYNCH_READY   ) 
			{ WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus ); return SYS_ABORTED; }
			else if ( Synch == ROBOT_SYNCH_SUCCESS ) 
			{ 
				WRITE_DIGITAL( ROBOT_PAUSE_CONTROL , eOFF_0 , &CommStatus );
				WRITE_ANALOG( io  , LastTrg/*trg*/  , &CommStatus ); 
				WRITE_ANALOG( io2 , LastTrg2/*trg2*/ , &CommStatus ); 
				return SYS_SUCCESS; 
			}
		}

		pos = pos + m;
		pos2 = pos2 + m2;

		if ( md ) 
		{
			if ( m < 0 ) 
			{
				if ( pos <= trg ) 
				{
					WRITE_ANALOG( io , trg , &CommStatus );
					md = FALSE;
					m = 0;
				}
				else 
				{
					WRITE_ANALOG( io , pos , &CommStatus );
//					printf("[%s] [Move_Template2(md):m < 0][pos:%.1lf]\n", PROGRAM_FUNCTION_READ(), pos );
				}
			}
			else if ( m > 0 ) 
			{
				if ( pos >= trg ) 
				{
					WRITE_ANALOG( io , trg , &CommStatus );
					md = FALSE;
					m = 0;
				}
				else 
				{
					WRITE_ANALOG( io , pos , &CommStatus );
//					printf("[%s] [Move_Template2(md):m > 0][pos:%f]\n", PROGRAM_FUNCTION_READ(), pos );
				}
			}
		}
		//
		if ( md2 ) 
		{
			if ( m2 < 0 ) 
			{
				if ( pos2 <= trg2 ) 
				{
					if		( LimitRot == DECREASE ) 
					{ 
						pos2 = 359.9;
						WRITE_ANALOG( io2 , 359.9 , &CommStatus );
						trg2 = TarRot;
						LimitRot = NORMAL;
					}
					else 
					{
						WRITE_ANALOG( io2 , trg2 , &CommStatus );
						md2 = FALSE;
						m2 = 0;
					}
				}
				else 
				{
					WRITE_ANALOG( io2 , pos2 , &CommStatus );
//					printf("[%s] [Move_Template2(md2):m2 < 0][pos:%f]\n", PROGRAM_FUNCTION_READ(), pos2 );
				}
			}
			else if ( m2 > 0 ) 
			{
				if ( pos2 >= trg2 )
				{
					if		( LimitRot == INCREASE ) 
					{ 
						pos2 = 0;
						WRITE_ANALOG( io2 , 0 , &CommStatus );
						trg2 = TarRot;
						LimitRot = NORMAL;
					}
					else 
					{
						WRITE_ANALOG( io2 , trg2 , &CommStatus );
						md2 = FALSE;
						m2 = 0;
					}
				}
				else {
					WRITE_ANALOG( io2 , pos2 , &CommStatus );
//					printf("[%s] [Move_Template2(md2):m2 > 0][pos:%f]\n", PROGRAM_FUNCTION_READ(), pos2 );
				}
			}
		}

		if ( !md && !md2 ) return SYS_SUCCESS;
		if ( !WAIT_SECONDS( dly ) ) return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Retract_Control( BOOL Rot ) 
{
	Move_Template( Rb_Extension  ,  0 , Ret_Rng[0] , Ret_Dly[0] );
	Move_Template( Rb_Extension2 ,  0 , Ret_Rng[0] , Ret_Dly[0] );
	if ( Rot ) {
		Move_Template( Rb_Rotation   , 90 , Ret_Rng[0] , Ret_Dly[0] );
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Extend_Control( int Station , int Arm ) 
{
	int io;
	double trg , rng , dly;
	Module_Status msResult = SYS_ABORTED;
	
	if ( Arm == FINGER_A ) 
	{
		io  = Rb_Extension;
		rng = EXTA_Rng[ Station ];
		trg = EXTA[ Station ];
		dly = EXTA_Dly[ Station ];
	}
	else 
	{
		io  = Rb_Extension2;
		rng = EXTB_Rng[ Station ];
		trg = EXTB[ Station ];
		dly = EXTB_Dly[ Station ];
	}

	msResult = Move_Template( io , trg , rng , dly );
	return msResult;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Move_Control( int Station , int Arm ) 
{
	int io, io2;
	double trg , rng , dly;
	double trg2 , rng2 , dly2;
	
	if ( Arm == FINGER_A ) 
	{
		io  = Rb_Movement;
		rng = MOVA_Rng[ Station ];
		trg = MOVA[ Station ];
		dly = MOVA_Dly[ Station ];
	}
	else if ( Arm == FINGER_B )
	{
		io  = Rb_Movement;
		rng = MOVB_Rng[ Station ];
		trg = MOVB[ Station ];
		dly = MOVB_Dly[ Station ];
	}
	else if ( Arm == FINGER_AB )
	{
		io  = Rb_Movement;
		rng = MOVA_Rng[ Station ];
		trg = MOVA[ Station ];
		dly = MOVA_Dly[ Station ];

		io2  = Rb_Movement;
		rng2 = MOVB_Rng[ Station ];
		trg2 = MOVB[ Station ];
		dly2 = MOVB_Dly[ Station ];
	}
	
	return ( Move_Template( io , trg , rng , dly ) );
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Rotate_Control( int Station , int Arm ) 
{
	int io, io2;
	double trg , rng , dly;
	double trg2 , rng2 , dly2;
	Module_Status msResult = SYS_ABORTED;
	

	if ( Arm == FINGER_A ) 
	{
		io  = Rb_Rotation;
		rng = ROTA_Rng[ Station ];
		trg = ROTA[ Station ];
		dly = ROTA_Dly[ Station ];
	}
	else if ( Arm == FINGER_B )
	{
		io  = Rb_Rotation;
		rng = ROTB_Rng[ Station ];
		trg = ROTB[ Station ];
		dly = ROTB_Dly[ Station ];
	}
	else if ( Arm == FINGER_AB )
	{
		io  = Rb_Rotation;
		rng = ROTA_Rng[ Station ];
		trg = ROTA[ Station ];
		dly = ROTA_Dly[ Station ];

		io2  = Rb_Rotation;
		rng2 = ROTB_Rng[ Station ];
		trg2 = ROTB[ Station ];
		dly2 = ROTB_Dly[ Station ];
	}
	if( Arm == FINGER_A || Arm == FINGER_B )
		msResult = Move_Template( io , trg , rng , dly );
	else if( Arm == FINGER_AB )
		msResult = Move_Template_AB( io , trg , rng , dly , io2 , trg2 , rng2 , dly2 );

	return msResult;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_MoveRotate_Control( int Station , int Arm ) 
{
	int io , io2;
	double trg , trg2 , rng , rng2 , dly;
	Module_Status msResult = SYS_ABORTED;
	
	
	if ( Arm == FINGER_A ) 
	{
		io  = Rb_Movement;
		rng = MOVA_Rng[ Station ];
		trg = MOVA[ Station ];
		dly = MOVA_Dly[ Station ];
		//
		io2  = Rb_Rotation;
		trg2 = ROTA[ Station ];
		rng2 = ROTA_Rng[ Station ];
	}
	else
	{
		io  = Rb_Movement;
		rng = MOVB_Rng[ Station ];
		trg = MOVB[ Station ];
		dly = MOVB_Dly[ Station ];
		//
		io2  = Rb_Rotation;
		trg2 = ROTB[ Station ];
		rng2 = ROTB_Rng[ Station ];
	}


	msResult = Move_Template2( io , trg , rng , io2 , trg2 , rng2 , dly );
	if( !msResult ) printf( "[%s] Robot_MoveRotate_Control Error!\n", PROGRAM_FUNCTION_READ() );
	return msResult;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status PickRobot( int Arm , int Station , int SlotNum , BOOL Rot ) 
{
	int CommStatus , w, s;
	int r=0, r2=0;
	char szIndex[4]={0,};

	char szMID[512];


	if ( Arm == FINGER_AB ) {
		return SYS_ABORTED;
	}

	if ( Robot_Retract_Control( FALSE            ) != SYS_SUCCESS ) return SYS_ABORTED;

	if ( Robot_MoveRotate_Control( Station , Arm ) != SYS_SUCCESS ) return SYS_ABORTED;

	if ( Robot_Extend_Control( Station , Arm     ) != SYS_SUCCESS ) return SYS_ABORTED;

	if ( !WAIT_SECONDS( gdbUPDOWN_DELAY[0] ) ) return SYS_ABORTED;
	

	switch( Station ) 
	{
		case CM1 :
					s = Station;
					w = SlotNum;
					if ( READ_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , &CommStatus ) == 1 ) w = 0; // absent
					break;
		case CM2 :
					s = Station;
					w = SlotNum;
					if ( READ_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , &CommStatus ) == 1 ) w = 0; // absent
					break;
		case CM3 :
					s = Station;
					w = SlotNum;
					if ( READ_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , &CommStatus ) == 1 ) w = 0; // absent
					break;
		case CM4 :
					s = Station;
					w = SlotNum;
					if ( READ_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , &CommStatus ) == 1 ) w = 0; // absent
					break;
		case PM1 :
					s = READ_DIGITAL( PM1_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM1_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM1_Wafer_Result, &CommStatus );
					READ_STRING ( PM1_Wafer_MID, szMID, &CommStatus );
					break;
		case PM2 :
					s = READ_DIGITAL( PM2_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM2_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM2_Wafer_Result, &CommStatus );
					READ_STRING ( PM2_Wafer_MID, szMID, &CommStatus );
					break;
		case PM3 :
					s = READ_DIGITAL( PM3_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM3_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM3_Wafer_Result, &CommStatus );
					READ_STRING ( PM3_Wafer_MID, szMID, &CommStatus );
					break;
		case PM4 :
					s = READ_DIGITAL( PM4_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM4_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM4_Wafer_Result, &CommStatus );
					READ_STRING ( PM4_Wafer_MID, szMID, &CommStatus );
					break;
		case PM5 :
					s = READ_DIGITAL( PM5_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM5_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM5_Wafer_Result, &CommStatus );
					READ_STRING ( PM5_Wafer_MID, szMID, &CommStatus );
					break;
		case PM6 :
					s = READ_DIGITAL( PM6_Wafer_Source, &CommStatus );
					w = READ_DIGITAL( PM6_Wafer_Status, &CommStatus );
					r = READ_DIGITAL( PM6_Wafer_Result, &CommStatus );
					READ_STRING ( PM6_Wafer_MID, szMID, &CommStatus );
					break;
		case BM1 : 
					s = READ_DIGITAL( BM1_Wafer_Source + SlotNum - 1 , &CommStatus );
					w = READ_DIGITAL( BM1_Wafer_Status + SlotNum - 1 , &CommStatus );
					r = READ_DIGITAL( BM1_Wafer_Result + SlotNum - 1 , &CommStatus );
					READ_STRING ( BM1_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;

		case BM2 : 
					s = READ_DIGITAL( BM2_Wafer_Source + SlotNum - 1 , &CommStatus );
					w = READ_DIGITAL( BM2_Wafer_Status + SlotNum - 1 , &CommStatus );
					r = READ_DIGITAL( BM2_Wafer_Result + SlotNum - 1 , &CommStatus );
					READ_STRING ( BM2_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;
		default  :
					return SYS_ABORTED;
					break;
	}

	if ( Arm == FINGER_A ) 
	{
		WRITE_DIGITAL( TA_Wafer_Source , s , &CommStatus );
		WRITE_DIGITAL( TA_Wafer_Status , w , &CommStatus );
		WRITE_DIGITAL( TA_Wafer_Result , r , &CommStatus );
		WRITE_STRING ( TA_Wafer_MID,  szMID, &CommStatus );
	}
	else
	{
		WRITE_DIGITAL( TB_Wafer_Source , s , &CommStatus );
		WRITE_DIGITAL( TB_Wafer_Status , w , &CommStatus );
		WRITE_DIGITAL( TB_Wafer_Result , r , &CommStatus );
		WRITE_STRING ( TB_Wafer_MID,  szMID, &CommStatus );
	}


	switch( Station ) 
	{
		case CM1 :
					WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					break;
		case CM2 :
					WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					break;
		case CM3 :
					WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					break;
		case CM4 :
					WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					break;
		case PM1 :
					WRITE_DIGITAL( PM1_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM1_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM1_Wafer_MID,     "", &CommStatus );
					break;
		case PM2 :
					WRITE_DIGITAL( PM2_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM2_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM2_Wafer_MID,     "", &CommStatus );
					break;
		case PM3 :
					WRITE_DIGITAL( PM3_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM3_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM3_Wafer_MID,     "", &CommStatus );
					break;
		case PM4 :
					WRITE_DIGITAL( PM4_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM4_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM4_Wafer_MID,     "", &CommStatus );
					break;
		case PM5 :
					WRITE_DIGITAL( PM5_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM5_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM5_Wafer_MID,     "", &CommStatus );
					break;
		case PM6 :
					WRITE_DIGITAL( PM6_Wafer_Source , 0, &CommStatus );
					WRITE_DIGITAL( PM6_Wafer_Status , 0, &CommStatus );
					WRITE_STRING ( PM6_Wafer_MID,     "", &CommStatus );
					break;
		case BM1 :
					WRITE_DIGITAL( BM1_Wafer_Source + SlotNum -1 , 0, &CommStatus );
					WRITE_DIGITAL( BM1_Wafer_Status + SlotNum -1 , 0, &CommStatus );
					WRITE_STRING ( BM1_Wafer_MID + SlotNum -1,     "", &CommStatus );
					break;
		case BM2 :
					WRITE_DIGITAL( BM2_Wafer_Source + SlotNum -1 , 0, &CommStatus );
					WRITE_DIGITAL( BM2_Wafer_Status + SlotNum -1 , 0, &CommStatus );
					WRITE_STRING ( BM2_Wafer_MID + SlotNum -1,     "", &CommStatus );
					break;
		default  :
					return SYS_ABORTED;
					break;
	}

	if ( Robot_Retract_Control( Rot ) != SYS_SUCCESS ) { 

		return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status PlaceRobot( int Arm , int Station , int SlotNum , BOOL Rot ) 
{
	int CommStatus , w , s,  r;
	char szMID[512];


	if ( Arm == FINGER_AB ) {
		return SYS_ABORTED;
	}

	if ( Robot_Retract_Control( FALSE ) == SYS_ABORTED ) return SYS_ABORTED;
	
	if ( Robot_MoveRotate_Control( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;
	
	if ( Robot_Extend_Control    ( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;

	if ( !WAIT_SECONDS( gdbUPDOWN_DELAY[0] ) ) return SYS_ABORTED;

	if ( Arm == FINGER_A ) 
	{
		s = READ_DIGITAL( TA_Wafer_Source , &CommStatus );
		w = READ_DIGITAL( TA_Wafer_Status , &CommStatus );
		r = READ_DIGITAL( TA_Wafer_Result , &CommStatus );
		READ_STRING ( TA_Wafer_MID,  szMID, &CommStatus );
	}
	else
	{
		s = READ_DIGITAL( TB_Wafer_Source , &CommStatus );
		w = READ_DIGITAL( TB_Wafer_Status , &CommStatus );
		r = READ_DIGITAL( TB_Wafer_Result , &CommStatus );
		READ_STRING ( TB_Wafer_MID,  szMID, &CommStatus );
	}

	switch( Station ) 
	{
		case CM1 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM2 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM3 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM4 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;

		case PM1 :
					WRITE_DIGITAL( PM1_Wafer_Source, s,	 &CommStatus );
					WRITE_DIGITAL( PM1_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM1_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM1_Wafer_MID, szMID, &CommStatus );
					break;
		case PM2 :
					WRITE_DIGITAL( PM2_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM2_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM2_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM2_Wafer_MID, szMID, &CommStatus );
					break;
		case PM3 :
					WRITE_DIGITAL( PM3_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM3_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM3_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM3_Wafer_MID, szMID, &CommStatus );
					break;
		case PM4 :
					WRITE_DIGITAL( PM4_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM4_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM4_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM4_Wafer_MID, szMID, &CommStatus );
					break;
		case PM5 :
					WRITE_DIGITAL( PM5_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM5_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM5_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM5_Wafer_MID, szMID, &CommStatus );
					break;
		case PM6 :
					WRITE_DIGITAL( PM6_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM6_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM6_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM6_Wafer_MID, szMID, &CommStatus );
					break;
		case BM1 :
					WRITE_DIGITAL( BM1_Wafer_Source + SlotNum - 1, s , &CommStatus );
					WRITE_DIGITAL( BM1_Wafer_Status + SlotNum - 1, w , &CommStatus );
					WRITE_DIGITAL( BM1_Wafer_Result + SlotNum - 1, r , &CommStatus );
					WRITE_STRING ( BM1_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;

		case BM2 :
					WRITE_DIGITAL( BM2_Wafer_Source + SlotNum - 1, s , &CommStatus );
					WRITE_DIGITAL( BM2_Wafer_Status + SlotNum - 1, w , &CommStatus );
					WRITE_DIGITAL( BM2_Wafer_Result + SlotNum - 1, r , &CommStatus );
					WRITE_STRING ( BM2_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;
		default  :
					return SYS_ABORTED;
					break;
	}

	if ( Arm == FINGER_A ) 
	{
		WRITE_DIGITAL( TA_Wafer_Source , 0 , &CommStatus );
		WRITE_DIGITAL( TA_Wafer_Status , 0 , &CommStatus );
		WRITE_STRING ( TA_Wafer_MID,     "", &CommStatus );
	}
	else
	{
		WRITE_DIGITAL( TB_Wafer_Source , 0 , &CommStatus );
		WRITE_DIGITAL( TB_Wafer_Status , 0 , &CommStatus );
		WRITE_STRING ( TA_Wafer_MID,     "", &CommStatus );
	}

	if ( Robot_Retract_Control( Rot ) != SYS_SUCCESS ) {
		return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status PlaceRobot_AL ( int Arm , int Station , int SlotNum , BOOL Rot ) 
{
	int CommStatus , w , s,  r;
	char szMID[512];


	if ( Arm == FINGER_AB ) {
		return SYS_ABORTED;
	}

	if ( Robot_Extend_Control    ( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;

	if ( !WAIT_SECONDS( gdbUPDOWN_DELAY[0] ) ) return SYS_ABORTED;

	if ( Arm == FINGER_A ) 
	{
		s = READ_DIGITAL( TA_Wafer_Source , &CommStatus );
		w = READ_DIGITAL( TA_Wafer_Status , &CommStatus );
		r = READ_DIGITAL( TA_Wafer_Result , &CommStatus );
		READ_STRING ( TA_Wafer_MID,  szMID, &CommStatus );
	}
	else
	{
		s = READ_DIGITAL( TB_Wafer_Source , &CommStatus );
		w = READ_DIGITAL( TB_Wafer_Status , &CommStatus );
		r = READ_DIGITAL( TB_Wafer_Result , &CommStatus );
		READ_STRING ( TB_Wafer_MID,  szMID, &CommStatus );
	}

	switch( Station ) 
	{
		case CM1 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM1_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM2 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM2_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM3 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM3_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;
		case CM4 :
					if ( w > 0 ) {
						if		( r == 0 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 2 , &CommStatus );
						}
						else if ( r == 1 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 3 , &CommStatus );
						}
						else if ( r == 2 ) {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 4 , &CommStatus );
						}
						else {
							WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 0 , &CommStatus );
						}
					}
					else {
						WRITE_DIGITAL( CM4_C01_Wafer + SlotNum - 1 , 1 , &CommStatus );
					}
					break;

		case PM1 :
					WRITE_DIGITAL( PM1_Wafer_Source, s,	 &CommStatus );
					WRITE_DIGITAL( PM1_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM1_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM1_Wafer_MID, szMID, &CommStatus );
					break;
		case PM2 :
					WRITE_DIGITAL( PM2_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM2_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM2_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM2_Wafer_MID, szMID, &CommStatus );
					break;
		case PM3 :
					WRITE_DIGITAL( PM3_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM3_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM3_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM3_Wafer_MID, szMID, &CommStatus );
					break;
		case PM4 :
					WRITE_DIGITAL( PM4_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM4_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM4_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM4_Wafer_MID, szMID, &CommStatus );
					break;
		case PM5 :
					WRITE_DIGITAL( PM5_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM5_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM5_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM5_Wafer_MID, szMID, &CommStatus );
					break;
		case PM6 :
					WRITE_DIGITAL( PM6_Wafer_Source, s,  &CommStatus );
					WRITE_DIGITAL( PM6_Wafer_Status, w,  &CommStatus );
					WRITE_DIGITAL( PM6_Wafer_Result, r,  &CommStatus );
					WRITE_STRING ( PM6_Wafer_MID, szMID, &CommStatus );
					break;
		case BM1 :
					WRITE_DIGITAL( BM1_Wafer_Source + SlotNum - 1, s , &CommStatus );
					WRITE_DIGITAL( BM1_Wafer_Status + SlotNum - 1, w , &CommStatus );
					WRITE_DIGITAL( BM1_Wafer_Result + SlotNum - 1, r , &CommStatus );
					WRITE_STRING ( BM1_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;

		case BM2 :
					WRITE_DIGITAL( BM2_Wafer_Source + SlotNum - 1, s , &CommStatus );
					WRITE_DIGITAL( BM2_Wafer_Status + SlotNum - 1, w , &CommStatus );
					WRITE_DIGITAL( BM2_Wafer_Result + SlotNum - 1, r , &CommStatus );
					WRITE_STRING ( BM2_Wafer_MID + SlotNum - 1, szMID, &CommStatus );
					break;
		default  :
					return SYS_ABORTED;
					break;
	}

	if ( Arm == FINGER_A ) 
	{
		WRITE_DIGITAL( TA_Wafer_Source , 0 , &CommStatus );
		WRITE_DIGITAL( TA_Wafer_Status , 0 , &CommStatus );
		WRITE_STRING ( TA_Wafer_MID,     "", &CommStatus );
	}
	else
	{
		WRITE_DIGITAL( TB_Wafer_Source , 0 , &CommStatus );
		WRITE_DIGITAL( TB_Wafer_Status , 0 , &CommStatus );
		WRITE_STRING ( TA_Wafer_MID,     "", &CommStatus );
	}

	if ( Robot_Retract_Control( Rot ) != SYS_SUCCESS ) {
		return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status RotateRobot( int Arm , int Station , int SlotNum ) {

	if ( Robot_Retract_Control( FALSE ) == SYS_ABORTED ) return SYS_ABORTED;

	if ( Robot_MoveRotate_Control( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status ExtendRobot( int Arm , int Station , int SlotNum ) {

	if ( Robot_Retract_Control( FALSE ) == SYS_ABORTED ) return SYS_ABORTED;

	if ( Robot_MoveRotate_Control( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;

	if ( Robot_Extend_Control( Station , Arm ) == SYS_ABORTED ) return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status RetractRobot() {

	if ( Robot_Retract_Control( FALSE ) == SYS_ABORTED ) return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status HomeRobot() {

	if ( Robot_Retract_Control( FALSE ) == SYS_ABORTED ) {
		return SYS_ABORTED;
	}
	if ( Robot_MoveRotate_Control( HOM , FINGER_A ) == SYS_ABORTED ) {
		return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Config(char *Buffer , double *Title , int Length , double Min , double Max , int Line , char *FileName) 
{
	int nCount;
	double dbValue;
	char Read_Char[256];


	for( nCount = 1 ; nCount < Length ; nCount++ ) 
	{
		if ( !Get_Data_From_String( Buffer , Read_Char , nCount ) ) 
		{
			printf( "[%s] [%s] Format Error [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ() , FileName , Buffer, Line );
			return SYS_ERROR;
		}

		dbValue = atof( Read_Char );
		if ( dbValue < Min || dbValue > Max ) 
		{
			printf( "[%s] [%s] Range Error [%s] %f ( %f ~ %f ) Line[%d]\n" , PROGRAM_FUNCTION_READ() , FileName , Buffer, dbValue , Min , Max , Line );
			return SYS_ERROR;
		}
		Title[nCount - 1] = dbValue;
	}

	return SYS_SUCCESS;
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
Module_Status Robot_Setup_Control ( char* FileName ) 
{
	FILE	*Fpt;
	char	szFilePath[256]={0,};
	char	szBuffer[256] , szRead_Char[256];
	int		nTitle , nReadCnt , nLine;
	BOOL	FileEnd = TRUE;
	Module_Status	MDLStatus=SYS_SUCCESS;


	if ( ( Fpt = fopen( FileName , "r+t" ) ) == NULL ) 
	{
		printf( "[%s] File is not found![%s]\n" , PROGRAM_FUNCTION_READ() , FileName );
		return SYS_ERROR;
	}

	for ( nLine = 1 ; FileEnd ; nLine++ ) 
	{
		FileEnd = Get_Line_String_From_File( Fpt , szBuffer , &nReadCnt );
		if ( nReadCnt > 0 ) 
		{
			if ( nReadCnt != 2 && nReadCnt != Max_Module_Count + 1 ) 
			{
				printf( "[%s] File Format Error [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , nLine );
				MDLStatus = SYS_ERROR;
				break;
			}
			
			if ( !Get_Data_From_String( szBuffer , szRead_Char , 0 ) ) 
			{
				printf( "[%s] File Format Error [%s] Data[%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , szBuffer , nLine );
				MDLStatus = SYS_ERROR;
				break;
			}
			if		( STRCMP_L( szRead_Char , "MOVE_A_POS"     ) ) nTitle = MOVE_A_POS;
			else if ( STRCMP_L( szRead_Char , "EXTEND_A_POS"   ) ) nTitle = EXTEND_A_POS;
			else if ( STRCMP_L( szRead_Char , "ROTATE_A_POS"   ) ) nTitle = ROTATE_A_POS;
			else if	( STRCMP_L( szRead_Char , "MOVE_A_RNG"     ) ) nTitle = MOVE_A_RNG;
			else if ( STRCMP_L( szRead_Char , "EXTEND_A_RNG"   ) ) nTitle = EXTEND_A_RNG;
			else if ( STRCMP_L( szRead_Char , "ROTATE_A_RNG"   ) ) nTitle = ROTATE_A_RNG;
			else if	( STRCMP_L( szRead_Char , "MOVE_A_DLY"     ) ) nTitle = MOVE_A_DLY;
			else if ( STRCMP_L( szRead_Char , "EXTEND_A_DLY"   ) ) nTitle = EXTEND_A_DLY;
			else if ( STRCMP_L( szRead_Char , "ROTATE_A_DLY"   ) ) nTitle = ROTATE_A_DLY;
			else if	( STRCMP_L( szRead_Char , "MOVE_B_POS"     ) ) nTitle = MOVE_B_POS;
			else if ( STRCMP_L( szRead_Char , "EXTEND_B_POS"   ) ) nTitle = EXTEND_B_POS;
			else if ( STRCMP_L( szRead_Char , "ROTATE_B_POS"   ) ) nTitle = ROTATE_B_POS;
			else if	( STRCMP_L( szRead_Char , "MOVE_B_RNG"     ) ) nTitle = MOVE_B_RNG;
			else if ( STRCMP_L( szRead_Char , "EXTEND_B_RNG"   ) ) nTitle = EXTEND_B_RNG;
			else if ( STRCMP_L( szRead_Char , "ROTATE_B_RNG"   ) ) nTitle = ROTATE_B_RNG;
			else if	( STRCMP_L( szRead_Char , "MOVE_B_DLY"     ) ) nTitle = MOVE_B_DLY;
			else if ( STRCMP_L( szRead_Char , "EXTEND_B_DLY"   ) ) nTitle = EXTEND_B_DLY;
			else if ( STRCMP_L( szRead_Char , "ROTATE_B_DLY"   ) ) nTitle = ROTATE_B_DLY;
			else if ( STRCMP_L( szRead_Char , "RET_RNG"        ) ) nTitle = RET_RNG;
			else if ( STRCMP_L( szRead_Char , "RET_DLY"        ) ) nTitle = RET_DLY;
			else if ( STRCMP_L( szRead_Char , "UPDOWNDELAY"    ) ) nTitle = UPDOWN_DLY;
			else if ( STRCMP_L( szRead_Char , "ROTATION_STYLE" ) ) nTitle = ROTATION_STYLE;
			else if ( STRCMP_L( szRead_Char , "ARM_STYLE"      ) ) nTitle = ARM_STYLE;
			else if ( STRCMP_L( szRead_Char , "BASE_LIMIT"     ) ) nTitle = BASE_LIMIT;
			else 
			{
				printf( "[%s] File Unknown Option [%s][%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , szRead_Char , nLine );
				MDLStatus = SYS_ERROR;
				break;
			}

			switch( nTitle ) 
			{
				case MOVE_A_POS		:
					if ( Config( szBuffer , MOVA      , Max_Module_Count + 1 , -130 , 500 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_A_POS	:
					if ( Config( szBuffer , EXTA      , Max_Module_Count + 1 ,  -50 , 100 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_A_POS	:
					if ( Config( szBuffer , ROTA      , Max_Module_Count + 1 ,   0 , 360 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case MOVE_A_RNG		:
					if ( Config( szBuffer , MOVA_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_A_RNG	:
					if ( Config( szBuffer , EXTA_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_A_RNG	:
					if ( Config( szBuffer , ROTA_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case MOVE_A_DLY		:
					if ( Config( szBuffer , MOVA_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_A_DLY	:
					if ( Config( szBuffer , EXTA_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_A_DLY	:
					if ( Config( szBuffer , ROTA_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case MOVE_B_POS		:
					if ( Config( szBuffer , MOVB      , Max_Module_Count + 1 , -130 , 500 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_B_POS	:
					if ( Config( szBuffer , EXTB      , Max_Module_Count + 1 ,  -50 , 100 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_B_POS	:
					if ( Config( szBuffer , ROTB      , Max_Module_Count + 1 ,   0 , 360 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case MOVE_B_RNG		:
					if ( Config( szBuffer , MOVB_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_B_RNG	:
					if ( Config( szBuffer , EXTB_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_B_RNG	:
					if ( Config( szBuffer , ROTB_Rng  , Max_Module_Count + 1 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case MOVE_B_DLY		:
					if ( Config( szBuffer , MOVB_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case EXTEND_B_DLY	:
					if ( Config( szBuffer , EXTB_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATE_B_DLY	:
					if ( Config( szBuffer , ROTB_Dly  , Max_Module_Count + 1 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case RET_RNG		:
					if ( Config( szBuffer , Ret_Rng ,  2 ,   0 , 50 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case RET_DLY		:
					if ( Config( szBuffer , Ret_Dly ,  2 ,   0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case UPDOWN_DLY			:
					if ( Config( szBuffer , gdbUPDOWN_DELAY , 2 , 0 , 10 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case BASE_LIMIT		:
					if ( Config( szBuffer , gdbBaseLimit , 2 , 0 , 359 , nLine , FileName ) == SYS_ERROR ) MDLStatus = SYS_ERROR;
					break;
				case ROTATION_STYLE :
					if ( !Get_Data_From_String( szBuffer , szRead_Char , 1 ) ) {
						printf( "[%s] [ROTATION_STYLE] Format Error [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , nLine );
						MDLStatus = SYS_ERROR;
						break;
					}
					
					if      ( STRCMP_L( szRead_Char , "CCW_HOMEBASED"         ) ) ROTATION_TYPE = CCW_HOMEBASED;
					else if ( STRCMP_L( szRead_Char , "CW_HOMEBASED"          ) ) ROTATION_TYPE = CW_HOMEBASED;
					else if ( STRCMP_L( szRead_Char , "CCW_ROTATION"          ) ) ROTATION_TYPE = CCW_ROTATION;
					else if ( STRCMP_L( szRead_Char , "CW_ROTATION"           ) ) ROTATION_TYPE = CW_ROTATION;
					else if ( STRCMP_L( szRead_Char , "MINIMUM"               ) ) ROTATION_TYPE = MINIMUM;
					else {
						printf( "[%s] [ROTATION_STYLE] not supported Style [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , nLine );
						MDLStatus = SYS_ERROR;
					}
					break;
				case ARM_STYLE		:
					if ( !Get_Data_From_String( szBuffer , szRead_Char , 1 ) ) {
						printf( "[%s] [ARM_STYLE] Format Error [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , nLine );
						MDLStatus = SYS_ERROR;
						break;
					}
					
					if      ( STRCMP_L( szRead_Char , "USING_A"          ) ) 	ARM_TYPE = USING_A; 
					else if ( STRCMP_L( szRead_Char , "USING_B"          ) ) 	ARM_TYPE = USING_B;
					else if ( STRCMP_L( szRead_Char , "USING_AB"         ) ) 	ARM_TYPE = USING_AB;
					else {
						printf( "[%s] [ARM_STYLE] not supported Style [%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName , nLine );
						MDLStatus = SYS_ERROR;
					}
					break;
				default				:
					printf( "[%s] File Unknown Option [%s][%s] Line[%d]\n" , PROGRAM_FUNCTION_READ(), FileName, szRead_Char , nLine );
					MDLStatus = SYS_ERROR;
					break;
			}
			if ( MDLStatus != SYS_SUCCESS )	break;
		}
	}
	fclose(Fpt);

	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Home_Control( ) 
{
	if ( HomeRobot() == SYS_ABORTED )
	{
		return SYS_ABORTED;
	}
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Robot_Run_Control( int Mode , int Arm , int Station , int SlotNum ) 
{
	if		( Mode == PICK	) 
	{
		if ( PickRobot ( Arm , Station , SlotNum , FALSE ) == SYS_ABORTED ) 
		{
			return SYS_ABORTED;
		}
	}
	else if ( Mode == PLACE	)
	{
		if ( PlaceRobot( Arm , Station , SlotNum , FALSE ) == SYS_ABORTED ) 
		{
			return SYS_ABORTED;
		}
	}
	else if ( Mode == PLACE_AL	)
	{
		if ( PlaceRobot_AL ( Arm , Station , SlotNum , FALSE ) == SYS_ABORTED ) 
		{
			return SYS_ABORTED;
		}
	}
	else if ( Mode == ROTATE ) 
	{
		if ( RotateRobot( Arm , Station , SlotNum ) == SYS_ABORTED )
		{
			return SYS_ABORTED;
		}
	}
	else if ( Mode == EXTEND ) 
	{
		if ( ExtendRobot( Arm , Station , SlotNum ) == SYS_ABORTED )
		{
			return SYS_ABORTED;
		}
	}
	else if ( Mode == RETRACT ) 
	{
		if ( RetractRobot() == SYS_ABORTED )
		{
			return SYS_ABORTED;
		}
	}
	else return SYS_ABORTED;

	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Select_Action( int Command , int Arm , int Station , int SlotNum  ) 
{
	Module_Status	MDLStatus;

	MDLStatus = Robot_Run_Control( Command , Arm , Station , SlotNum );

	if ( MDLStatus!= SYS_SUCCESS ) 
	{
		printf("[%s] [Select_Action][Robot_Run_Control]Error Command[%d][%d][%d][%d]\n" , PROGRAM_FUNCTION_READ(), Command, Arm, Station, SlotNum);
	}
	return MDLStatus;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Station_And_Slot ( char *TempStr, int* nStation, int* nSlot, int* nArm )
{
	char szArmStation[61];
	char szStation[61];
	char szSlot[61];
	char szArm[61];

	STR_SEPERATE      ( TempStr, szArmStation, szSlot, 60 );
	STR_SEPERATE_CHAR ( szArmStation, '_', szArm, szStation, 60 );

	if		( STRCMP_L( szStation , "CM1"	) ) { *nStation = CM1; *nSlot = atoi ( szSlot ); }
	else if	( STRCMP_L( szStation , "CM2"	) ) { *nStation = CM2; *nSlot = atoi ( szSlot ); }
	else if	( STRCMP_L( szStation , "CM3"	) ) { *nStation = CM3; *nSlot = atoi ( szSlot ); }
	else if	( STRCMP_L( szStation , "CM4"	) ) { *nStation = CM4; *nSlot = atoi ( szSlot ); }
	else if ( STRCMP_L( szStation , "BM1"   ) ) { *nStation = BM1; *nSlot = atoi ( szSlot ); }
	else if	( STRCMP_L( szStation , "BM2"   ) ) { *nStation = BM2; *nSlot = atoi ( szSlot ); }
	else if	( STRCMP_L( szStation , "PM1"   ) ) { *nStation = PM1; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "PM2"   ) ) { *nStation = PM2; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "PM3"   ) ) { *nStation = PM3; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "PM4"   ) ) { *nStation = PM4; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "PM5"   ) ) { *nStation = PM5; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "PM6"   ) ) { *nStation = PM6; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "AL"	) ) { *nStation = AL; *nSlot = 1;	}
	else if	( STRCMP_L( szStation , "IC"	) ) { *nStation = IC; *nSlot = 1;	}
	else {
		return FALSE;
	}
	
	if		( STRCMP_L( szArm , "A" ) ) { *nArm = FINGER_A; }
	else if	( STRCMP_L( szArm , "B" ) ) { *nArm = FINGER_B; }
	else {
		return FALSE;
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main_Sub() {
	int	nLength , Arm = FINGER_A , Station = -1 , SlotNum = 1 , Command;
	unsigned char *szParameter;
	char szRunMsg[256]={0,} , szParam[256]={0,};


	szParameter = PROGRAM_PARAMETER_READ();
	nLength = strlen( szParameter );
	szParameter[ nLength ] = 0x00;

	if ( nLength < 1 ) 
	{
		printf( "[%s] Parameter Length Error[%s]\n" , PROGRAM_FUNCTION_READ(), PROGRAM_PARAMETER_READ() );
		return SYS_ABORTED;
	}

	printf( "[%s] Start Animation : [%s]\n", PROGRAM_FUNCTION_READ() , szParameter );

	STR_SEPERATE( szParameter , szRunMsg , szParam , 255 );

	if		( STRCMP_L( szRunMsg , "SETUP"			) ) {
		return Robot_Setup_Control ( szParam );
	}
	else if ( STRCMP_L( szRunMsg , "HOME"				) ) {
		return Robot_Home_Control ( );
	}
	else { 
		if ( Station_And_Slot ( szParam, &Station, &SlotNum, &Arm ) != TRUE ) {
			printf( "[%s] Station & Slot Error [%s]\n", PROGRAM_FUNCTION_READ() , szParam );
			return SYS_ABORTED;
		}
		if		( STRCMP_L( szRunMsg , "PICK"			) ) Command = PICK;
		else if ( STRCMP_L( szRunMsg , "PLACE"			) ) Command = PLACE;
		else if ( STRCMP_L( szRunMsg , "PLACE_AL"		) ) Command = PLACE_AL;
		else if ( STRCMP_L( szRunMsg , "ROTATE"			) ) Command = ROTATE;
		else if ( STRCMP_L( szRunMsg , "ROTPIC"			) ) Command = ROTATE;
		else if ( STRCMP_L( szRunMsg , "ROTPLC"			) ) Command = ROTATE;
		else if ( STRCMP_L( szRunMsg , "EXTEND"			) ) Command = EXTEND;
		else if ( STRCMP_L( szRunMsg , "EXTPIC"			) ) Command = EXTEND;
		else if ( STRCMP_L( szRunMsg , "EXTPLC"			) ) Command = EXTEND;
		else if ( STRCMP_L( szRunMsg , "RETRACT"		) ) Command = RETRACT;
		else if ( STRCMP_L( szRunMsg , "RETPIC"			) ) Command = RETRACT;
		else if ( STRCMP_L( szRunMsg , "RETPLC"			) ) Command = RETRACT;
		else {
			printf( "[%s] Not Supported Parameter [%s]\n" , PROGRAM_FUNCTION_READ() , PROGRAM_PARAMETER_READ() );
			return SYS_ABORTED;
		}
		return Select_Action( Command , Arm , Station , SlotNum  );
	}
	
	return SYS_SUCCESS;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Module_Status Program_Main() {
	
	Module_Status	MDLStatus;
		
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

	KWIN_DIALOG_DRIVER_LOADING();
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
BOOL Program_Init_Code() {
	int	nLength;
	int	nSelect=0;
	char	szErrorMsg[256]={0,};
	unsigned char *szArgument;
	
	szArgument = PROGRAM_ARGUMENT_READ();
	nLength = strlen( szArgument );
	szArgument[ nLength ] = 0x00;


	/*if ( nLength < 1 ) 
	{
		sprintf( szErrorMsg, "[%s] Argument Error! File path is not set!", PROGRAM_FUNCTION_READ() );
		MODAL_MESSAGE_BOX( / *GET_MAIN_WINDOW()* / NULL, "Warning", szErrorMsg, "OK", 0, &nSelect );
		return FALSE;
	}*/


	if ( Robot_Setup_Control( gszPathSetup ) == SYS_ABORTED ) {
		sprintf( szErrorMsg, "[%s] Setup File Error!", PROGRAM_FUNCTION_READ() );
		MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Error", szErrorMsg, "OK", 0, &nSelect );
		return FALSE;
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
