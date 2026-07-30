//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#include <kutlgui.h>
#include "iodefine.h"
#include "resource.h"
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Alignment( int nRun_Mode , int nType , int nOption ) {

	int nCommStatus;
	int nArm;
	int nStation;
	int nSlot;
	int nSelect;
	int nResult;

	nArm     = READ_DIGITAL( SYS_AL_Arm	, &nCommStatus );
	nStation = READ_DIGITAL( SYS_AL_Station	, &nCommStatus );
	nSlot    = READ_DIGITAL( SYS_AL_Slot		, &nCommStatus ) + 1;

	switch( nType ) {
	case 0 : // HOME
		switch( nStation ) {
		case eSYS_CM1_0:		//	CM1
	
			if ( nArm == eArmA_0 ) {
				if ( MAINT_INTERFACE_RUN( "HOME_FM|%d|CM1|%d|0" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			else {
				if ( MAINT_INTERFACE_RUN( "HOME_FM|%d|CM1|0|%d" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

		case eSYS_PM1_1:		//	PM1
		case eSYS_PM2_2:		//	PM2
		case eSYS_PM3_3:		//	PM3

			if ( MAINT_INTERFACE_RUN( "HOME_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		default :
			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station Information Error!", "OK", 0, &nSelect );
			return SYS_ABORTED;
		}
		break;

	case 1 : // PICK
		switch( nStation ) {

		case eSYS_CM1_0 :		//	CM1

			if ( READ_DIGITAL( CTC_FA_Wafer_Status , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( nArm == eArmA_0 ) {
				if ( MAINT_INTERFACE_RUN( "PICK_FM|%d|CM1|%d|0" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			else {
				if ( MAINT_INTERFACE_RUN( "PICK_FM|%d|CM1|0|%d" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( READ_DIGITAL( TA_Wafer_Status + nArm , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PICK_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( READ_DIGITAL( TA_Wafer_Status + nArm , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PICK_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( READ_DIGITAL( TA_Wafer_Status + nArm , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PICK_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

	case 2 : // PLACE
		switch( nStation ) {

		case eSYS_CM1_0 :		//	CM1

			if (READ_DIGITAL(CM1_C01_Wafer + nSlot - 1, &nCommStatus) != 1) return SYS_ABORTED;
			if ( nArm == eArmA_0 ) {
				if ( MAINT_INTERFACE_RUN( "PLACE_FM|%d|CM1|%d|0" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			else {
				if ( MAINT_INTERFACE_RUN( "PLACE_FM|%d|CM1|0|%d" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( READ_DIGITAL( PM1_Wafer_Status , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PLACE_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( READ_DIGITAL( PM2_Wafer_Status , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PLACE_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( READ_DIGITAL( PM3_Wafer_Status , &nCommStatus ) != 0 ) return SYS_ABORTED;
			if ( MAINT_INTERFACE_RUN( "PLACE_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

	case 3 : // Rotate
		switch( nStation ) {

		case eSYS_CM1_0 :		//	CM1
			if ( nArm == eArmA_0 ) {
				if ( MAINT_INTERFACE_RUN( "ROTATE_FM|%d|CM1|%d|0" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			else {
				if ( MAINT_INTERFACE_RUN( "ROTATE_FM|%d|CM1|0|%d" , nRun_Mode , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
			}
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( MAINT_INTERFACE_RUN( "ROTATE_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( MAINT_INTERFACE_RUN( "ROTATE_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( MAINT_INTERFACE_RUN( "ROTATE_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

	case 4 : // EXTEND
		switch( nStation ) {
		case eSYS_CM1_0 :		//	CM1

			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station is not supported!", "OK", 0, &nSelect );
			return SYS_ABORTED;
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( MAINT_INTERFACE_RUN( "EXTEND_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( MAINT_INTERFACE_RUN( "EXTEND_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( MAINT_INTERFACE_RUN( "EXTEND_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		default :
			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station Information Error!", "OK", 0, &nSelect );
			return SYS_ABORTED;
		}
		break;

	case 5 : // RETRACT
		switch( nStation ) {
		case eSYS_CM1_0:		//	CM1

			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station is not supported!", "OK", 0, &nSelect );
			return SYS_ABORTED;
			break;


		case eSYS_PM1_1 :		//	PM1

			if ( MAINT_INTERFACE_RUN( "RETRACT_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( MAINT_INTERFACE_RUN( "RETRACT_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( MAINT_INTERFACE_RUN( "RETRACT_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		default :
			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station Information Error!", "OK", 0, &nSelect );
			return SYS_ABORTED;
		}
		break;
	case 6 : // UP
		switch( nStation ) {
		case eSYS_CM1_0:		//	CM1

			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station is not supported!", "OK", 0, &nSelect );
			return SYS_ABORTED;
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( MAINT_INTERFACE_RUN( "UP_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( MAINT_INTERFACE_RUN( "UP_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( MAINT_INTERFACE_RUN( "UP_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		default :
			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station Information Error!", "OK", 0, &nSelect );
			return SYS_ABORTED;
		}
		break;
	case 7 : // DOWN
		switch (nStation) {
		case eSYS_CM1_0:		//	CM1

			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station is not supported!", "OK", 0, &nSelect );
			return SYS_ABORTED;
			break;

		case eSYS_PM1_1 :		//	PM1

			if ( MAINT_INTERFACE_RUN( "DOWN_TM|%d|PM1|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM2_2 :		//	PM2

			if ( MAINT_INTERFACE_RUN( "DOWN_TM|%d|PM2|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		case eSYS_PM3_3 :		//	PM3

			if ( MAINT_INTERFACE_RUN( "DOWN_TM|%d|PM3|%c|1|1" , nRun_Mode , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
			break;

		default :
			nResult = MODAL_MESSAGE_BOX( GET_MAIN_WINDOW(), "Information", "Station Information Error!", "OK", 0, &nSelect );
			return SYS_ABORTED;
		}
		break;
	}

	return SYS_SUCCESS;
	}

}