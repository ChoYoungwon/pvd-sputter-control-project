//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include "iodefine.h"
//-------------------------------------------------------------------------------------------------
//---------------------------------------------+--------------------+
//  nCommStatus                                |  Option            |
//---------------------------------------------+--------------------+
//  0 : Unuse                                  |  Normal        or  |
//  1 : Local                                  |  Expand        or  |
//  2 : Remote  : Disconnect                   |  Expand+Abort      |
//  3 : Remote  : Enable(Connect)              |                    |
//  4 : Loc.Rem : Disable                      |                    |
//  5 : Loc.Rem : Disable HW                   |                    |
//  6 : Loc.Rem : Enable PM                    |                    |
//---------------------------------------------+--------------------+
//  7 : Loc.Rem : CTC Use(Enable)              |  Expand        or  |
//  8 : Loc.Rem : CTC Use(Disable)(WaferYes)   |  Expand+Abort      |
//  9 : Loc.Rem : CTC Use(Disble)(WaferNo)     |                    |
// 10 : Loc.Rem : CTC Use(Enable PM)           |                    |
// 11 : Loc.Rem : CTC Use(Disable HW)(WaferYes)|                    |
// 12 : Loc.Rem : CTC Use(Disble HW)(WaferNo)  |                    |
//---------------------------------------------+--------------------+
// 13 : Loc.Rem : Enable.Abort                 |  Expand+Abort      |
// 14 : Loc.Rem : Disable.Abort                |                    |
// 15 : Loc.Rem : Disble HW.Abort              |                    |
// 16 : Loc.Rem : Enable PM.Abort              |                    |
//---------------------------------------------+--------------------+
//---------------------------------------------+--------------------+
//  ControlStatus                              |  Transaction       |
//---------------------------------------------+--------------------+
//  0 : Unuse                                  | Unuse              |
//  1 : Disable HW                             | UnInit             |
//  2 : Go Init                                | Go Init            |
//  3 : Disable                                | Init or Maint      |
//  4 : Go Standby                             | Go Standby         |
//  5 : Enable                                 | Standby            |
//  6 : Go Maint                               | Go Maint           |
//  7 : CTC Use(Enable)                        | CTC Use            |
//  8 : CTC Use(Disable)                       | CTC Use            |
//  9 : Processing                             | Processing         |
// 10 : Homing                                 | Homing             |
// 11 : Disconnect                             | Offline            |
//-------------------------------------------------------------------------------------------------
char _stationName[3][4] = {"CM1", "PM1", "PM2"};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
IO_Name_String_Map IO_STR_Map_Table[] = {
	{ "e:SCHEDULER"					, _K_S_IO		,	SCHEDULER			,	0	} ,
	{ "SCHEDULER"					, _K_F_IO + 1	,	SeqSCHEDULER		,	0	} ,
	//
	{ "CTC.$0_CommStatus"			, _K_D_IO	,	M_STATUS			,	0	} ,
	//
	{ "CTC.$0_ControlStatus"		, _K_D_IO	,	CONTROL_STATUS		,	0	} ,
	//
	//{ "SCHEDULER_MAINT_$0_SUB"		, _K_F_IO	,	M_FUNCTION			,	0	} ,
	{ "SCHEDULER_MAINT_$0"			, _K_F_IO	,	M_FUNCTION			,	0	} ,
	{ "PM1.dvControlStatus"			, _K_D_IO	,	PM1_dvControlStatus	,	0	} ,
	{ "PM2.dvControlStatus"			, _K_D_IO	,	PM2_dvControlStatus		,	0	} ,
	{ "CTC.TR_STATUS"				, _K_D_IO	,	CTC_TR_STATUS		,	0	} ,
	{ "CTC.TR_SRC_STATION"			, _K_D_IO	,	CTC_TR_SRC_STATION	,	0	} ,
	{ "CTC.TR_SRC_SLOT"				, _K_D_IO	,	CTC_TR_SRC_SLOT		,	0	} ,
	{ "CTC.TR_TRG_STATION"			, _K_D_IO	,	CTC_TR_TRG_STATION	,	0	} ,
	{ "CTC.TR_TRG_SLOT"				, _K_D_IO	,	CTC_TR_TRG_SLOT		,	0	} ,

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// LED Control
	/////////////////////////////////////////////////////////////////////////////////////////////////
	{ "doPVDIdle_LED"				, _K_D_IO	,	PM2_Idle_LED		,	0	} ,
	{ "doPVDProcess_LED"			, _K_D_IO	,	PM2_Process_LED		,	0	} ,

	/////////////////////////////////////////////////////////////////////////////////////////////////

	""
};
//------------------------------------------------------------------------------------------
char ModuleIndex[32];
char PM[24];
int Maint_Run = 0;
//------------------------------------------------------------------------------------------
enum { OFF, ON };
//------------------------------------------------------------------------------------------
int Maint_Interface_Function(char* message) {
	int Result;
	WRITE_FUNCTION_EVENT(M_FUNCTION, "SKIP_FUNCTION_PART");
	while (TRUE) {
		if (READ_FUNCTION_EVENT(M_FUNCTION) != SYS_RUNNING) break;
		_sleep(10);
	}
	Result = RUN_FUNCTION(M_FUNCTION, message);
	WRITE_FUNCTION_EVENT(M_FUNCTION, "RUN_FUNCTION_PART");
	return Result;
}
//------------------------------------------------------------------------------------------
void Make_Module_Status(char* message) {
	int CommStatus;
	char Buffer[64];
	sprintf(Buffer, "SET_MODULE_INFO_LOCAL %s|%s", PROGRAM_ARGUMENT_READ(), message);
	WRITE_STRING(SCHEDULER, Buffer, &CommStatus);
}
//------------------------------------------------------------------------------------------
Module_Status MAINT_INTERFACE_RUN(LPSTR list, ...)
{
	va_list va;
	int nCommStatus;
	char szBuffer[256];
	char szBuffer2[256];
	va_start(va, list);
	vsprintf(szBuffer2, list, (LPSTR)va);
	va_end(va);
	//
	WRITE_DIGITAL(CTC_TR_STATUS, 0, &nCommStatus);
	sprintf(szBuffer, "MAINT_INTERFACE %s", szBuffer2);
	WRITE_FUNCTION_EVENT(SeqSCHEDULER, szBuffer);
	while (TRUE) {
		if (!WAIT_SECONDS(0.25)) return SYS_ABORTED;
		switch (READ_DIGITAL(CTC_TR_STATUS, &nCommStatus)) {
		case SYS_ABORTED: return SYS_ABORTED;
		case SYS_ERROR: return SYS_ERROR;
		case SYS_SUCCESS: return SYS_SUCCESS;
		}
	}
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void Program_Enter_Code() {

	REPLACE_CHAR_SET(0, PROGRAM_ARGUMENT_READ());
	sprintf(PM, PROGRAM_ARGUMENT_READ());
}
//------------------------------------------------------------------------------------------
Module_Status Make_Maintenance(int Status) {//9

	int CommStatus;

	if (STRCMP_L("GO_INIT", PROGRAM_PARAMETER_READ())) {
		switch (Status) {
		case 4: // Disable
		case 5: // Disable(H/W)
		case 14: // Disable.Abort
		case 15: // Disable(H/W).Abort
			Maint_Run = 1;
			if (Maint_Interface_Function("GO_INIT") == SYS_SUCCESS) {
				Make_Module_Status("DISABLE");
			}
			else {
				Make_Module_Status("DISABLEHW");
			}
			break;
		default:
			return SYS_ABORTED;
			break;
		}
	}
	else if (STRCMP_L("GO_MAINT", PROGRAM_PARAMETER_READ())) {
		switch (Status) {
		case 1: // Enable(Local)
		case 3: // Enable(Remote)
		case 7: // CTC Use(Enable)
		case 13: // Enable.Abort 
			Maint_Run = 2;
			Maint_Interface_Function("GO_MAINT");
			Make_Module_Status("DISABLE");
			break;
		case 5: // Disable(H/W)
		case 15: // Disable(H/W).Abort
			Maint_Run = 2;
			if (Maint_Interface_Function("GO_MAINT") == SYS_SUCCESS) {
				Make_Module_Status("DISABLE");
			}
			break;
		default:
			return SYS_ABORTED;
			break;
		}
	}
	else if (STRCMP_L("GO_STANDBY", PROGRAM_PARAMETER_READ())) {
		switch (Status) {
		case 4: // Disable
		case 14: // Disable.Abort
			Maint_Run = 3;
			if (Maint_Interface_Function("GO_STANDBY") == SYS_SUCCESS) {
				Make_Module_Status("ENABLE");
			}
			break;
		case 8: // Loc.Rem : CTC Use(Disable)(WaferYes)   |  Expand+Abort      |
		case 9: // Loc.Rem : CTC Use(Disble)(WaferNo)     |                    |
			Make_Module_Status("ENABLE");
			if (STRCMP_L(PM, "PM1"))	WRITE_DIGITAL(PM1_dvControlStatus, 5, &CommStatus);
			else if (STRCMP_L(PM, "PM2"))	WRITE_DIGITAL(PM2_dvControlStatus, 5, &CommStatus);
			break;
		default:
			return SYS_ABORTED;
			break;
		}
	}
	else if (STRCMP_L("PROCESS", PROGRAM_PARAMETER_READ())) {
		switch (Status) {
		case 1: // Enable(Local)
		case 3: // Enable(Remote)
		case 13: // Enable.Abort 
			Maint_Run = 4;
			if (Maint_Interface_Function("PROCESS") != SYS_SUCCESS) {
				Make_Module_Status("DISABLE");
			}
			break;
		default:
			return SYS_ABORTED;
			break;
		}
	}
	else {
		Maint_Run = 5;
		if (STRCMP_L(PROGRAM_PARAMETER_READ(), "HOME")) {
			return Maint_Interface_Function(PROGRAM_PARAMETER_READ());
		}
		else if (STRCMP_L(PROGRAM_PARAMETER_READ(), "MANUAL_PICK"))
		{
			int stationNum = READ_DIGITAL(CTC_TR_SRC_STATION, &CommStatus);
			if (MAINT_INTERFACE_RUN("PICK_TM|1|%s|A|1|1", _stationName[stationNum]) == SYS_ABORTED) return SYS_ABORTED;
		}
		else if (STRCMP_L(PROGRAM_PARAMETER_READ(), "MANUAL_PLACE"))
		{
			int stationNum = READ_DIGITAL(CTC_TR_TRG_STATION, &CommStatus);
			if (MAINT_INTERFACE_RUN("PLACE_TM|1|%s|A|1|1", _stationName[stationNum]) == SYS_ABORTED) return SYS_ABORTED;
		}
		else if (STRCMP_L(PROGRAM_PARAMETER_READ(), "MANUAL_MOVE"))
		{
			int stationNum = READ_DIGITAL(CTC_TR_SRC_STATION, &CommStatus);
			if (MAINT_INTERFACE_RUN("PICK_TM|1|%s|A|1|1", _stationName[stationNum]) == SYS_ABORTED) return SYS_ABORTED;

			stationNum = READ_DIGITAL(CTC_TR_TRG_STATION, &CommStatus);
			if (MAINT_INTERFACE_RUN("PLACE_TM|1|%s|A|1|1", _stationName[stationNum]) == SYS_ABORTED) return SYS_ABORTED;
		}
		else {
			switch (Status) {
			case 4: // Disable
			case 14: // Disable.Abort
				return Maint_Interface_Function(PROGRAM_PARAMETER_READ());
				break;
			default:
				return SYS_ABORTED;
				break;
			}
		}
	}
	return SYS_SUCCESS;
}

void Make_Control_Ststus_Monitor() {
	int Status, res, CommStatus;

	_sleep(3000);

	while (TRUE)
	{
		switch (Maint_Run)
		{
		case 0: // Normal
			Status = READ_DIGITAL(M_STATUS, &CommStatus);
			switch (Status)
			{
			case 0: // Unuse
				res = 0; // Unuse
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 1: // Local
				res = 5; // Enable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, ON, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 2: // Disconnect
				res = 11; // Disconnect
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 3: // Enable
				res = 5; // Enable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, ON, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, ON, &CommStatus); }
				break;
			case 4: // Disable
				res = 3; // Disable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 5: // Disable HW 
				res = 1; // Disable HW 
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 6: // Enable PM
				res = 5; // Enable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, ON, &CommStatus); }
				break;
			case 7: // CTC Use(Enable)
				res = 7; // CTC Use(Enable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, ON, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, ON, &CommStatus); }
				break;
			case 8: // CTC Use(Disable)(WaferYes)
				res = 8; // CTC Use(Disable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 9: // CTC Use(Disble)(WaferNo)
				res = 8; // CTC Use(Disable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 10: // CTC Use(Enable PM)
				res = 7; // CTC Use(Enable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, ON, &CommStatus); }
				break;
			case 11: // CTC Use(Disable HW)(WaferYes)
				res = 8; // CTC Use(Disable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 12: // CTC Use(Disble HW)(WaferNo)
				res = 8; // CTC Use(Disable)
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 13: // Enable.Abort
				res = 5; // Enable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 14: // Disable.Abort
				res = 3; // Disable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 15: // Disble HW.Abort
				res = 1; // Disable HW 
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			case 16: // Enable PM.Abort
				res = 5; // Enable
				if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
				break;
			}
			break;
		case 1: // Go Init
			res = 2;
			if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
			break;
		case 2: // Go Maint
			res = 6;
			if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, ON, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
			break;
		case 3: // Go Standby
			res = 4;
			if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, ON, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
			break;
		case 4: // Processing
			res = 9;
			if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, ON, &CommStatus); }
			break;
		case 5: // Homing
			res = 10;
			if (STRCMP_L(PM, "PM2")) { WRITE_DIGITAL(PM2_Idle_LED, OFF, &CommStatus); WRITE_DIGITAL(PM2_Process_LED, OFF, &CommStatus); }
			break;
		}
		//printf("Threading [%s] : [% d][%d][%d]\n", PM, res, Maint_Run, Status);
		WRITE_DIGITAL(CONTROL_STATUS, res, &CommStatus);
		_sleep(25);
	}
	_endthread();
}

Module_Status Program_Main() {
	int res, Status, CommStatus;
	Status = READ_DIGITAL(M_STATUS, &CommStatus);
	if (Status == 0) return SYS_SUCCESS;
	res = Make_Maintenance(Status);
	Maint_Run = 0;
	return res;
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
BOOL Program_Init_Code() {
	_beginthread((void*)Make_Control_Ststus_Monitor, 0, 0);
	return TRUE;
}
//---------------------------------------------------------------------------------------
