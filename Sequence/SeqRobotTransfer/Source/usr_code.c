#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include "cimseqnc.h"
#include "iodefine.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Screen_Nav_Info( int no ) {
	if ( no == -1 ) {
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Event_Message_Received() {
	//
	int nCommStatus;
	//
	if (STRCMP_L(PROGRAM_EVENT_READ(), "ABORTTR")) {
		//
		WRITE_FUNCTION_EVENT(SCHEDULER, "ABORTTR");
		//
		MANAGER_ABORT_TRUE();
		//
		//WRITE_DIGITAL(CTC_TR_CONTROL, Idle, &nCommStatus);
	}
	else if (STRCMP_L(PROGRAM_EVENT_READ(), "PAUSETR")) {

//		if (READ_DIGITAL(CTC_TR_CONTROL, &nCommStatus) == Running) {
//			if (READ_DIGITAL(CTC_TR_CONTROL, &nCommStatus) == 1) {
//				WRITE_FUNCTION_EVENT(SCHEDULER, "PAUSETR");
//			}
//			gbStop = TRUE;
//			WRITE_DIGITAL(CTC_TR_CONTROL, Paused, &nCommStatus);
//		}

	}
	else if (STRCMP_L(PROGRAM_EVENT_READ(), "CONTINUETR")) {

//		if (READ_DIGITAL(CTC_TR_CONTROL, &nCommStatus) == Paused) {
//			if (READ_DIGITAL(CTC_TR_CONTROL, &nCommStatus) == 3) {
//				WRITE_FUNCTION_EVENT(SCHEDULER, "CONTINUETR");
//			}
//			gbStop = FALSE;
//			WRITE_DIGITAL(CTC_TR_CONTROL, Running, &nCommStatus);
//		}
	}
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------