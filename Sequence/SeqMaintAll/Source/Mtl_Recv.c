//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#include "iodefine.h"
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_CM( int nRunMode , int nMode , int nCass , int nSlot ) {

	int i;
	int nIndex;
	int nCommStatus;

	if ( nMode == 0 ) {
		if ( MAINT_INTERFACE_RUN( "MAPPING|%d|CM%d" , nRunMode , nCass ) == SYS_ABORTED ) return SYS_ABORTED;
	}
	else if ( nMode == 1 ) {

		if (nCass == 1)	nIndex = CM1_C01_Wafer;
		else if (nCass == 2)	nIndex = CM1_C02_Wafer;
		else if (nCass == 3)	nIndex = CM1_C03_Wafer;
		else if (nCass == 4)	nIndex = CM1_C04_Wafer;
		else return SYS_ABORTED;

		if ( nSlot <= 0 ) {
			for ( i = 0 ; i < 30 ; i++ ) {
				WRITE_DIGITAL( nIndex + i - 1 , 2 , &nCommStatus );
			}
		}
		else {
			WRITE_DIGITAL( nIndex + nSlot - 1 , 2 , &nCommStatus );
		}
	}
	else {

		if      (nCass == 1)	nIndex =  CM1_C01_Wafer;
		else if (nCass == 2)	nIndex =  CM1_C02_Wafer;
		else if (nCass == 3)	nIndex =  CM1_C03_Wafer;
		else if (nCass == 4)	nIndex =  CM1_C04_Wafer;
		else return SYS_ABORTED;

		if ( nSlot <= 0 ) {
			for ( i = 0 ; i < 30 ; i++ ) {
				WRITE_DIGITAL( nIndex + i - 1 , 1 , &nCommStatus );
			}
		}
		else {
			WRITE_DIGITAL( nIndex + nSlot - 1 , 1 , &nCommStatus );
		}
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_AL( int nRunMode , int nMode , int nSlot ) {

	int nCommStatus;
	int nGuiWafer, nRealWafer;
	int i;

	if ( nMode == 0 ) {
		nGuiWafer = READ_DIGITAL( CTC_FM_AL_Wafer_Status , &nCommStatus );
		
		for ( i = 0 ; i < 5 ; i++ ) {
			WRITE_DIGITAL( dioAL_FM_Wafer_Sensor , 1 , &nCommStatus );
			nRealWafer = READ_DIGITAL( dioAL_FM_Wafer_Sensor , &nCommStatus );
			if ( nRealWafer ) break;
			_sleep(100);
		}
		if ( i ==5 ) return SYS_ABORTED;

		switch( nRealWafer ) {
		case 1   :
			if ( nGuiWafer > 0 ) {
				// Clear Wafer
				WRITE_DIGITAL( CTC_FM_AL_Wafer_Status , 0 , &nCommStatus );
			}
			break;
		default :
			if ( nGuiWafer <= 0 ) {
				// Make Wafer
				WRITE_DIGITAL( CTC_FM_AL_Wafer_Status , nSlot , &nCommStatus );
			}
			break;
		}
	}
	else if ( nMode == 1 ) {
		WRITE_DIGITAL( CTC_FM_AL_Wafer_Status , nSlot , &nCommStatus );
	}
	else {
		WRITE_DIGITAL( CTC_FM_AL_Wafer_Status , 0 , &nCommStatus );
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_FM( int nRunMode , int nMode , int nFinger , int nSlot , int *nSel ) {

	int nGuiWafer;
	int nCommStatus;

	if ( nMode == 0 ) {

		nGuiWafer = READ_DIGITAL( CTC_FA_Wafer_Status + nFinger , &nCommStatus );
		if ( RUN_FUNCTION( FM_WAFER_CHECK_CONTROL , "CHECK 1 0" ) == SYS_ABORTED ) return SYS_ABORTED;

		switch( READ_DIGITAL( FM_Wafer_Result , &nCommStatus ) ) {
		case 0   :
			if ( nGuiWafer > 0 ) {
				// Clear Wafer
				WRITE_DIGITAL( CTC_FA_Wafer_Status + nFinger , 0 , &nCommStatus );
			}
			if ( *nSel < 0 ) *nSel = nFinger;
			break;
		default :
			if ( nGuiWafer <= 0 ) {
				// Make Wafer
				WRITE_DIGITAL( CTC_FA_Wafer_Status + nFinger , nSlot , &nCommStatus );
			}
			break;
		}
	}
	else if ( nMode == 1 ) {
		WRITE_DIGITAL( CTC_FA_Wafer_Status + nFinger , nSlot , &nCommStatus );
	}
	else {
		WRITE_DIGITAL( CTC_FA_Wafer_Status + nFinger , 0 , &nCommStatus );
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_TM( int nRunMode , int nMode , int nFinger , int nSlot , int *nSel ) {

	int  nGuiWafer;
	int nCommStatus;

	if ( nMode == 0 ) {

		nGuiWafer = READ_DIGITAL( TA_Wafer_Status + nFinger , &nCommStatus );

		if (RUN_FUNCTIONF(TMC_TM_ROBOT_CONTROL, "CHECKLOAD %c_PM1", nFinger + 'A') == SYS_ABORTED){
			return SYS_ABORTED;
		}

		switch( READ_DIGITAL( TM_Wafer_Result , &nCommStatus ) ) {

		case 0   :
			if ( nGuiWafer > 0 ) {
				// Clear Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nFinger , 0 , &nCommStatus );
			}
			if ( *nSel < 0 ) *nSel = nFinger;
			break;

		default :
			if ( nGuiWafer <= 0 ) {
				// Make Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nFinger , nSlot , &nCommStatus );
			}
			break;
		}
	}
	else if ( nMode == 1 ) {
		WRITE_DIGITAL( TA_Wafer_Status + nFinger , nSlot , &nCommStatus );
	}
	else {
		WRITE_DIGITAL( TA_Wafer_Status + nFinger , 0 , &nCommStatus );
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_TM_PM( int nRunMode , int nMode , int Station , int nFinger , int nSlot , int *nSel ) {

	int nGuiWafer;
	int nCommStatus;

	if ( nMode == 0 ) {

		nGuiWafer = READ_DIGITAL( TA_Wafer_Status + nFinger , &nCommStatus );

		if (RUN_FUNCTIONF(TMC_ISOGATE_CONTROL, "OPEN %c_PM%d", nFinger + 'A', Station) == SYS_ABORTED) {
			return SYS_ABORTED;
		}
		if (RUN_FUNCTIONF(TMC_TM_ROBOT_CONTROL, "CHECKLOAD %c_PM%d", nFinger + 'A', Station) == SYS_ABORTED){
			return SYS_ABORTED;
		}

		switch( READ_DIGITAL( TM_Wafer_Result , &nCommStatus ) ) {

		case 0   :
			if ( nGuiWafer > 0 ) {
				// Clear Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nFinger , 0 , &nCommStatus );
			}
			if ( *nSel < 0 ) *nSel = nFinger;
			break;

		default :
			if ( nGuiWafer <= 0 ) {
				// Make Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nFinger , nSlot , &nCommStatus );
			}
			break;
		}
	}
	else if ( nMode == 1 ) {
		WRITE_DIGITAL( TA_Wafer_Status + nFinger , nSlot , &nCommStatus );
	}
	else {
		WRITE_DIGITAL( TA_Wafer_Status + nFinger , 0 , &nCommStatus );
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_PM( int nRunMode , int nMode , int nChamber , int nArm , int nSlot ) {

	int nGuiWafer;
	int nCommStatus;

	if ( nMode == 0 ) {

		nGuiWafer = READ_DIGITAL( PM1_Wafer_Status + nChamber - 1 , &nCommStatus );

		if (MAINT_INTERFACE_RUN("PICK_TM|%d|PM%d|%c|1|%d", nRunMode, nChamber, nArm + 'A', 1) == SYS_ABORTED){
			return SYS_ABORTED;
		}

		switch( READ_DIGITAL( TM_Wafer_Result , &nCommStatus ) ) {

		case 0   :
			if ( nGuiWafer > 0 ) {
				// Clear Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nArm , 0 , &nCommStatus );
			}
			break;

		default :
			if ( nGuiWafer <= 0 ) {
				// Make Wafer
				WRITE_DIGITAL( TA_Wafer_Status + nArm , nSlot , &nCommStatus );
			}
			if (MAINT_INTERFACE_RUN("PLACE_TM|%d|PM%d|%c|1|%d", nRunMode, nChamber, nArm + 'A', 1) == SYS_ABORTED){
				return SYS_ABORTED;
			}
			break;
		}
	}
	else if ( nMode == 1 ) {
		WRITE_DIGITAL( PM1_Wafer_Status + nChamber - 1 , nSlot , &nCommStatus );
	}
	else {
		WRITE_DIGITAL( PM1_Wafer_Status + nChamber - 1 , 0 , &nCommStatus );
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Check_BM( int nRunMode , int nMode , int nSide , int nChamber , int nArm , int nSlots , int nSlote , int nSlot ) {

	int nIndex;
	int j;
	int nGuiWafer;
	int nCommStatus;

	if ( nChamber == 1 ) nIndex = BM1_Wafer_Status;
	else			 nIndex = BM2_Wafer_Status;

	if ( nMode == 0 ) {

		if ( nSide == 0 ) {

			if (MAINT_INTERFACE_RUN("WAITING_FOR_FM|%d|BM%d", nRunMode, nChamber) == SYS_ABORTED){
				return SYS_ABORTED;
			}

			for ( j = nSlots ; j <= nSlote ; j++ ) {

				nGuiWafer = READ_DIGITAL( nIndex + j - 1 , &nCommStatus );

				if ( nArm == 0 ) {

					if (MAINT_INTERFACE_RUN("PICK_FM|%d|BM%d|%d|0", nRunMode, nChamber, j) == SYS_ABORTED){
						return SYS_ABORTED;
					}

					switch( READ_DIGITAL( FM_Wafer_Result , &nCommStatus ) ) {

					case 0   :
						if ( nGuiWafer > 0 ) {
							// Clear Wafer
							WRITE_DIGITAL( CTC_FA_Wafer_Status + nArm , 0 , &nCommStatus );
						}
						break;

					default :
						if ( nGuiWafer <= 0 ) {
							// Make Wafer
							WRITE_DIGITAL( CTC_FA_Wafer_Status + nArm , nSlot , &nCommStatus );
						}
						if (MAINT_INTERFACE_RUN("PLACE_FM|%d|BM%d|%d|0", nRunMode, nChamber, j) == SYS_ABORTED){
							return SYS_ABORTED;
						}
						break;
					}
				}
				else {
					if (MAINT_INTERFACE_RUN("PICK_FM|%d|BM%d|0|%d", nRunMode, nChamber, j) == SYS_ABORTED){
						return SYS_ABORTED;
					}

					switch( READ_DIGITAL( FM_Wafer_Result , &nCommStatus ) ) {

					case 0   :
						if ( nGuiWafer > 0 ) {
							// Clear Wafer
							WRITE_DIGITAL( CTC_FA_Wafer_Status + nArm , 0 , &nCommStatus );
						}
						break;

					default :
						if ( nGuiWafer <= 0 ) {
							// Make Wafer
							WRITE_DIGITAL( CTC_FA_Wafer_Status + nArm , nSlot , &nCommStatus );
						}
						if (MAINT_INTERFACE_RUN("PLACE_FM|%d|BM%d|0|%d", nRunMode, nChamber, j) == SYS_ABORTED){
							return SYS_ABORTED;
						}
						break;
					}
				}
			}
		}
		else {
			if ( MAINT_INTERFACE_RUN( "WAITING_FOR_TM|%d|BM%d" , nRunMode , nChamber ) == SYS_ABORTED ) return SYS_ABORTED;
			for ( j = nSlots ; j <= nSlote ; j++ ) {
				nGuiWafer = READ_DIGITAL( nIndex + j - 1 , &nCommStatus );
				if ( MAINT_INTERFACE_RUN( "PICK_TM|%d|BM%d|%c|1|%d" , nRunMode , nChamber , nArm + 'A' , j ) == SYS_ABORTED ) return SYS_ABORTED;
				if ( RUN_FUNCTIONF( TMC_TM_ROBOT_CONTROL , "CHECKLOAD %c_PM5" , nArm + 'A' ) == SYS_ABORTED ) return SYS_ABORTED;
				switch( READ_DIGITAL( TM_Wafer_Result , &nCommStatus ) ) {
				case 0   :
					if ( nGuiWafer > 0 ) {
						// Clear Wafer
						WRITE_DIGITAL( TA_Wafer_Status + nArm , 0 , &nCommStatus );
					}
					break;
				default :
					if ( nGuiWafer <= 0 ) {
						// Make Wafer
						WRITE_DIGITAL( TA_Wafer_Status + nArm , nSlot , &nCommStatus );
					}
					if ( MAINT_INTERFACE_RUN( "PLACE_TM|%d|BM%d|%c|1|%d" , nRunMode , nChamber , nArm + 'A' , j ) == SYS_ABORTED ) return SYS_ABORTED;
					break;
				}
			}
		}
	}
	else if ( nMode == 1 ) {
		for ( j = nSlots ; j <= nSlote ; j++ ) {
			WRITE_DIGITAL( nIndex + j - 1 , nSlot , &nCommStatus );
		}
	}
	else {
		for ( j = nSlots ; j <= nSlote ; j++ ) {
			WRITE_DIGITAL( nIndex + j - 1 , 0 , &nCommStatus );
		}
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Map( int nRunMode , int nType ) {

	int nCommStatus;
	int nRec_Arm = 0;
	int nRunType;
	int nArm;
	int nStation;
	int nSlot;
	int ss, es;

	nArm     = READ_DIGITAL( MTL_RECV_Arm	, &nCommStatus );
	nStation = READ_DIGITAL( MTL_RECV_Station, &nCommStatus );
	nSlot     = READ_DIGITAL( MTL_RECV_Slot	, &nCommStatus ) + 1;

	if (nType == 0)	nRunType = 0;
	else if (nType == 1) nRunType = 0;
	else if (nType == 2) nRunType = 1;
	else			nRunType = 2;

	switch( nStation ) {

	case eMTL_CM1_0:	//	CM1

		if (nType == 0) {
			ss = -1;
		}
		else {
			if (nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT)	return SYS_ABORTED;
			ss = nSlot;
		}

		if (Maint_Material_Recovery_Check_CM(nRunMode, nRunType, 1, ss) == SYS_ABORTED) return SYS_ABORTED;
		break;

	case eMTL_CM2_1:	//	CM2

		if (nType == 0) {
			ss = -1;
		}
		else {
			if (nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT)	return SYS_ABORTED;
			ss = nSlot;
		}

		if (Maint_Material_Recovery_Check_CM(nRunMode, nRunType, 2, ss) == SYS_ABORTED) return SYS_ABORTED;
		break;

	case eMTL_CM3_2:	//	CM3

		if (nType == 0) {
			ss = -1;
		}
		else {
			if (nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT)	return SYS_ABORTED;
			ss = nSlot;
		}

		if (Maint_Material_Recovery_Check_CM(nRunMode, nRunType, 3, ss) == SYS_ABORTED) return SYS_ABORTED;
		break;

	case eMTL_CM4_3:	//	CM4

		if (nType == 0) {
			ss = -1;
		}
		else {
			if (nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT)	return SYS_ABORTED;
			ss = nSlot;
		}

		if (Maint_Material_Recovery_Check_CM(nRunMode, nRunType, 4, ss) == SYS_ABORTED) return SYS_ABORTED;
		break;

	case eMTL_ATR7_4 :	//ATR8

		nRec_Arm = -1;
		if ( Maint_Material_Recovery_Check_FM( nRunMode , nRunType , 0 , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_Aligner_5 :	//Aligner

		if ( Maint_Material_Recovery_Check_AL( nRunMode , nRunType , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_BM1_ATM_6 :	//	BM1

		if (nType == 0) {
			ss = 1;
			es = MTLRECV_BUFFER_MAX_SLOT;
		}
		else {
			if ( nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT ) return SYS_ABORTED;
			ss = nSlot;
			es = nSlot;
		}

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_FM( nRunMode , nRunType , 0 , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nRec_Arm < 0 ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_BM( nRunMode , nRunType , 0 , 1 , 0 , ss , es , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_BM2_ATM_7 :	//	BM2

		if (nType == 0) {
			ss = 1;
			es = MTLRECV_BUFFER_MAX_SLOT;
		}
		else {
			if ( nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT ) return SYS_ABORTED;
			ss = nSlot;
			es = nSlot;
		}

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_FM( nRunMode , nRunType , 0 , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nRec_Arm < 0 ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_BM( nRunMode , nRunType , 0 , 2 , 0 , ss , es , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_BM1_VAC_8 :	//	BM1

		if (nType == 0) {
			ss = 1;
			es = MTLRECV_BUFFER_MAX_SLOT;
		}
		else {
			if ( nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT ) return SYS_ABORTED;
			ss = nSlot;
			es = nSlot;
		}

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM( nRunMode , nRunType , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_BM( nRunMode , nRunType , 1 , 1 , nArm , ss , es , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_BM2_VAC_9 :	//	BM2

		if (nType == 0) {
			ss = 1;
			es = MTLRECV_BUFFER_MAX_SLOT;
		}
		else {
			if ( nSlot < 0 || nSlot > MTLRECV_BUFFER_MAX_SLOT ) return SYS_ABORTED;
			ss = nSlot;
			es = nSlot;
		}

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM( nRunMode , nRunType , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_BM( nRunMode , nRunType , 1 , 2 , nArm , ss , es , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_MTR5_10 :	//	Mtr5

		nRec_Arm = -1;
		if ( Maint_Material_Recovery_Check_TM( nRunMode , nRunType , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_PM1_11 :	//	PM1

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM_PM( nRunMode , nRunType , 1 , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_PM( nRunMode , nRunType , 1 , nArm , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_PM2_12 :	//	PM2

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM_PM( nRunMode , nRunType , 2 , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_PM( nRunMode , nRunType , 2 , nArm , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_PM3_13 :	//	PM3

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM_PM( nRunMode , nRunType , 3 , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}
		if ( Maint_Material_Recovery_Check_PM( nRunMode , nRunType , 3 , nArm , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	case eMTL_PM4_14 :	//	PM4

		if ( nRunType == 0 ) {
			nRec_Arm = -1;
			if ( Maint_Material_Recovery_Check_TM_PM( nRunMode , nRunType , 4 , nArm , nSlot , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
			if ( nArm != nRec_Arm ) return SYS_ABORTED;
		}

		if ( Maint_Material_Recovery_Check_PM( nRunMode , nRunType , 4 , nArm , nSlot ) == SYS_ABORTED ) return SYS_ABORTED;
		break;

	default :
		return SYS_ABORTED;
	}

	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_Empty_PM() {

	if ( Maint_Material_Recovery_Check_PM( 2 , 2 , 1 , 0 , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_PM( 2 , 2 , 2 , 0 , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_PM( 2 , 2 , 3 , 0 , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_PM( 2 , 2 , 4 , 0 , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_BM( 2 , 2 , 0 , 1 , 0 , 1 , MTLRECV_BUFFER_MAX_SLOT , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_BM( 2 , 2 , 0 , 2 , 0 , 1 , MTLRECV_BUFFER_MAX_SLOT , 0 ) == SYS_ABORTED ) return SYS_ABORTED;
	return SYS_SUCCESS;

}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
Module_Status Maint_Material_Recovery_All( int nRunMode ) {

	int nRec_Arm;
	int i;

	//---------------------------------------------------------------------------------------------------------------------
	// CM Check
	//---------------------------------------------------------------------------------------------------------------------
	if ( Maint_Material_Recovery_Check_CM( nRunMode , 0 , 1 , -1 ) == SYS_ABORTED ) return SYS_ABORTED;
	if ( Maint_Material_Recovery_Check_CM( nRunMode , 0 , 2 , -1 ) == SYS_ABORTED ) return SYS_ABORTED;

	//---------------------------------------------------------------------------------------------------------------------
	// AL Check
	//---------------------------------------------------------------------------------------------------------------------
	if ( Maint_Material_Recovery_Check_AL( nRunMode , 0 , 1 ) == SYS_ABORTED ) return SYS_ABORTED;
	//---------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------
	// TM Robot Arm Check
	//---------------------------------------------------------------------------------------------------------------------
	nRec_Arm = -1;
	for ( i = 0 ; i < 2 ; i++ ) {
		if ( Maint_Material_Recovery_Check_TM( nRunMode , 0 , i , 1 , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
	}
	//-------------------------------------------------------
	if ( nRec_Arm < 0 ) return SYS_ABORTED;
	//-------------------------------------------------------

	//---------------------------------------------------------------------------------------------------------------------
	// FM Robot Arm Check
	//---------------------------------------------------------------------------------------------------------------------
	nRec_Arm = -1;
	if ( Maint_Material_Recovery_Check_FM( nRunMode , 0 , 0 , 1 , &nRec_Arm ) == SYS_ABORTED ) return SYS_ABORTED;
	//-------------------------------------------------------
	if ( nRec_Arm < 0 ) return SYS_ABORTED;
	//-------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------------------------------
	// BM All Check
	//---------------------------------------------------------------------------------------------------------------------
/*	for ( i = 0 ; i < 2 ; i++ ) {
		if ( Maint_Material_Recovery_Check_BM( nRunMode , 0 , 0 , i + 1 , nRec_Arm , 1 , MTLRECV_BUFFER_MAX_SLOT , 1 ) == SYS_ABORTED ) return SYS_ABORTED;
	}
*/
	return SYS_SUCCESS;
}
