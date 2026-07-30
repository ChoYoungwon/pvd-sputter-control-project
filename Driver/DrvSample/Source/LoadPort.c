#include "cimrs232.h"

#include "kutletc.h"
#include "kutlanal.h"
#include "kutlstr.h"
#include "LoadPort.h"
#include "Driver.h"
#include "Define.h"

extern	char _drvName[10];
extern  char _time[32];

///////////////////////////
// Setting Default Value //
///////////////////////////
void SetDefaultValue()
{
	_ackTimeOut = 2000;

	_autoLampStatus = LampOff;
	_manualLampStatus = LampOff;
	_loadLampStatus = LampOff;
	_unloadLampStatus = LampOff;
	_reserveLampStatus = LampOff;

	_clampStatus = ClmapUnknown;

	_commLog = LogOff;
	_commLogPrint = LogOff;
	_commlogFileIndex = -1;

	_dockingStatus = DockingUnknown;
	_doorStatus = DoorUnknown;

	_errConsolePrint = LogOff;
	_errLog = LogOff;

	_homeComplete = HomeNotComplete;
	_isMaintMode = 0;
	_isAutoMode = 0;
	_loadPortUnitStatus = UnitStatusIdle;
	_loadUnloadSwitchMode = SwitchDisable;
	_maintenanceMode = MaintDisable;
	_mappingFunctionMode = MappingDisable;
	_motorDriverStatus = MotorOff;
	_placementStatus = PlacementOff;
	_presenceStatus = PresenceOff;

	_runFlag = Complete;

	_loadButton = None;
	_unloadButton = None;
	_tpConnectStatus = Disconnect;
	_eStopStatus = Release;

	strcpy(_logFileName, "");
	strcpy(_mappingData, "");
	strcpy(_errorMassage, "");
	strcpy(_errorCode, "");

}

//////////////////////////////////////////////////////////////////////////
//
void EventDataReset()
{
	_loadButton = None;
	_unloadButton = None;
	_eStopStatus = Release;
}
////////////////////////////////////////////////////////////////////////
int ControlLoadPort(int ID2, int Data)
{
	int readCount = 0;
	BOOL result = FALSE;
	char sendBuf[DEFAULT_LENGTH] = "";
	int sendMsgFlag = TRUE;
	BOOL noneMsg = FALSE;

	switch (ID2)
	{
	case 0: // Abort
		if (Data == 1) sprintf(sendBuf, "ABORT");
		else	noneMsg = TRUE;
		break;

	case 1:	//Clamp 
		if (Data == 1) sprintf(sendBuf, "MOV:PODCL;");
		else	noneMsg = TRUE;
		break;

	case 2:	//Unclamp
		if (Data == 1) sprintf(sendBuf, "MOV:PODOP;");
		else	noneMsg = TRUE;
		break;

	case 3:	//Dock || Undock
		if (Data == 0)
			sprintf(sendBuf, "MOV:DOROP;");
		else
			sprintf(sendBuf, "MOV:DORCL;");
		break;

	case 4:	//Load (Clamp->Dock->Lock->Door Open(Mapping)
		if (Data == 1) sprintf(sendBuf, "OPEN");
		else	noneMsg = TRUE;
		break;

	case 5:	//Unload(Door Close(Mapping) -> Unlock -> Undock -> Unclamp)
		if (Data == 1) sprintf(sendBuf, "CLOSE");
		else	noneMsg = TRUE;
		break;

	case 6:	//Home
		if (Data == 1) sprintf(sendBuf, "HOM");
		else	noneMsg = TRUE;
		break;

	case 7: // Down Mapping을 수행한다. 반드시 POD가 Open된 상태에서 수행해야 한다.
		if (Data == 1) sprintf(sendBuf, "SCAN DN");
		else	noneMsg = TRUE;
		break;

	case 8: // Up Mapping을 수행한다. 반드시 POD가 Open된 상태에서 수행해야 한다.
		if (Data == 1) sprintf(sendBuf, "SCAN UP");
		else	noneMsg = TRUE;
		break;

	case 9: // TCLOSE 상태에서 다시 Door를 Mapping 없이 Open하는데 사용한다. 
		if (Data == 1) sprintf(sendBuf, "MOV:DORBK;");
		else	noneMsg = TRUE;
		break;

	case 10: // LOAD 상태에서 임시로 Door를 Close 시키고 대기한다.
		if (Data == 1) sprintf(sendBuf, "MOV:DORFW;");
		else	noneMsg = TRUE;
		break;

	case 11: // Unload without mapping
		if (Data == 1) sprintf(sendBuf, "CLOSE_MAPOFF");
		else	noneMsg = TRUE;
		break;

	case 12: // Initialize Map Data
		if (Data == 1) sprintf(sendBuf, "MAPHOM");
		else noneMsg = TRUE;
		break;

	case 13:	// Ignore Door Interlock, Manual Door Close/Open
		if (Data == 0) sprintf(sendBuf, "MDOR_OPN OFF");
		else sprintf(sendBuf, "MDOR_OPN ON");
		break;

	case 14: // Ignore Door Interlock, Manual Door Down/Up
		if (Data == 0) sprintf(sendBuf, "MLIFT_DN ON");
		else sprintf(sendBuf, "MLIFT_DN OFF");
		break;

	case 15: //Vacuum On || Off
		if (Data == 0) sprintf(sendBuf, "MOV:VACOF;");
		else sprintf(sendBuf, "MOV:VACON;");
		break;

	case 16:	//LATCH || UNLATCH, On/Off 명령에 따른 상태가 타사와 반대
		//if (Data == 0) sprintf(sendBuf, "Latch OFF");
		//else sprintf(sendBuf, "Latch ON");
		if (Data == 0) sprintf(sendBuf, "MOV:DORCL;");
		else sprintf(sendBuf, "MOV:DOROP;");
		break;

	case 17: // Load/Unload UpMapping Enable
		if (Data == 0) sprintf(sendBuf, "CFG_MAP OFF");
		else sprintf(sendBuf, "CFG_MAP ON");
		break;

	case 18:	//Reset
		strcpy(_errorMassage, "");
		strcpy(_errorCode, "");
		sprintf(sendBuf, "RESET");
		break;

	case 20:
		if (Data == 0) sprintf(sendBuf, "CFG_IDSW OFF");
		else sprintf(sendBuf, "CFG_IDSW ON");
		break;

	default:
		sendMsgFlag = FALSE;
		printf("[%s] [%8s] DO Error - ID2[%d] Line[%d]\n", GetTime(_time), _drvName, ID2, __LINE__);
		break;
	}

	if (sendMsgFlag) {
		if (noneMsg)	result = TRUE;
		else			result = SendReceiveCommand(sendBuf);
	}
	else result = FALSE;

	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int AsciiToInt(char H)
{
	switch (H)
	{
	case 'A':
	case 'a': return 10;
	case 'B':
	case 'b': return 11;
	case 'C':
	case 'c': return 12;
	case 'D':
	case 'd': return 13;
	case 'E':
	case 'e': return 14;
	case 'F':
	case 'f': return 15;
	default: return H - 48;
	}
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
DWORD ConvertStrToBinary(char *StatusData)
{
	int i;
	DWORD BinaryData = 0;

	for (i = 0; i < 8; i++)
	{
		BinaryData = BinaryData << 4;
		BinaryData = BinaryData | AsciiToInt(StatusData[i]);
	}
	return BinaryData;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int ReadBit(DWORD BinaryData, int Bit)
{
	DWORD MaskBit = 1;
	MaskBit = MaskBit << Bit;

	return (BinaryData & MaskBit) >> Bit;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void UpdateErrorInform(char *errorInform, int count)
{
	char temp1[DEFAULT_LENGTH] = "";
	char temp2[DEFAULT_LENGTH] = "";
	char errorMessage[DEFAULT_LENGTH] = "";
	char errorNo[DEFAULT_LENGTH] = "";

	STR_SEPERATE_CHAR(errorInform, ' ', temp1, errorMessage, count);	//응답 메세지에서 Error번호와 Message를 분류 시킨다.(DuraPort Response : Exxx xxxxx)
	strcpy(_errorMassage, errorMessage);								//Error Message
	STR_SEPERATE_CHAR(temp1, 'E', temp2, errorNo, strlen(temp1));		//메시지에서 'E'문자를 제거한 번호를 뽑아낸다.
	strcpy(_errorCode, errorNo);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int GetMappingDataFromMemory(int ID2)
{
	return _mappingData[ID2 - 1];
}

//////////////////////////////////////////////////////////////////////////
// Update Mapping Data
// Mapping Data Format = "Mxxxxxxxx,xxxxxxxx,xxxxxxxx"
//                         Preset  , Cross  , Double
//////////////////////////////////////////////////////////////////////////
int UpdateMappingData(char *mappingData, int count)
{
	int step = 0;
	char WaferPreset[DEFAULT_LENGTH] = "";
	//char WaferCross[DEFAULT_LENGTH] = "";
	//char WaferDouble[DEFAULT_LENGTH] = "";
	char temp1[DEFAULT_LENGTH] = "";

	//DWORD Bin_WaferPreset;
	//DWORD Bin_WaferCross;
	//DWORD Bin_WaferDouble;

	STR_SEPERATE_CHAR(mappingData, '/', temp1, mappingData, count);
	STR_SEPERATE_CHAR(mappingData, ';', WaferPreset, temp1, count);

	strcpy(_mappingData, WaferPreset);
	int len = strlen(_mappingData);

	for (step = 0; step < 26; step++)
	{
		if (WaferPreset[step] == '0')_mappingData[step] = WAFER_ABSENT;
		else if (WaferPreset[step] == '1')_mappingData[step] = WAFER_PRESENT;
		else if (WaferPreset[step] == '2')_mappingData[step] = WAFER_CROSS;
		else if (WaferPreset[step] == 'W')_mappingData[step] = WAFER_DOUBLE;
		else _mappingData[step] = WAFER_UNKNOWN; // ? = Unkown

		printf("[UpdateMappingData] Slot [%d], Result [%d]\n", step + 1, _mappingData[step]);
	}

	////STR_SEPERATE_CHAR(temp1, ',', WaferCross, WaferDouble, count);

	//Bin_WaferPreset = strtoul(WaferPreset, NULL, 16);
	//Bin_WaferCross = strtoul(WaferCross, NULL, 16);
	//Bin_WaferDouble = strtoul(WaferDouble, NULL, 16);

	//for (step = 0; step < 26; step++)
	//{
	//	if (ReadBit(Bin_WaferPreset, step))
	//	{
	//		if (ReadBit(Bin_WaferCross, step))
	//		{
	//			_mappingData[step] = WAFER_CROSS;
	//		}
	//		else if (ReadBit(Bin_WaferDouble, step))
	//		{
	//			_mappingData[step] = WAFER_DOUBLE;
	//		}
	//		else
	//		{
	//			_mappingData[step] = WAFER_PRESENT;
	//		}
	//	}
	//	else _mappingData[step] = WAFER_ABSENT;

	//	printf("[UpdateMappingData] Slot [%d], Result [%d]\n", step + 1, _mappingData[step]);
	//}

	memset(temp1, 0, sizeof(char) * DEFAULT_LENGTH);

	// 	for(step = 0 ; step < 26 ; step++)
	// 	{
	// 		temp1[step] = _mappingData[step] + 48;
	// 	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Load Port Status Message Format
// "Sxxxxxxxx"
//////////////////////////////////////////////////////////////////////////
int UpdateLoadPortStatus(char *statusData, int count)
{
	int step = 0;
	char szReturnData[DEFAULT_LENGTH] = "";
	char temp1[DEFAULT_LENGTH] = "";
	DWORD BinaryData;
	int PStatus, NStatus;

	STR_SEPERATE_CHAR(statusData, 'S', temp1, szReturnData, count);
	//BinaryData = ConvertStrToBinary( szReturnData );
	BinaryData = strtoul(szReturnData, NULL, 16);
	//printf("[UpdateLoadPortStatus] BinaryData : [%lx]\n", BinaryData);

	//BIT 2  Door Open Status, BIT 3 Door Close Status
	PStatus = ReadBit(BinaryData, 2);
	NStatus = ReadBit(BinaryData, 3);
	//printf("[UpdateLoadPortStatus] Door Open Status : [%d], Door Close Status : [%d]\n", PStatus, NStatus);
	_doorStatus = (PStatus && !NStatus) ? DoorOpen : (!PStatus && NStatus) ? DoorClose : DoorUnknown;

	//BIT 6 Maint. Mode Condition
	_isMaintMode = ReadBit(BinaryData, 6);

	//BIT 7 Latch Locked
	_latchStatus = ReadBit(BinaryData, 7) ? Latch : NoLatch;

	//printf("[UpdateLoadPortStatus] Latch Lock Status : [%d]\n", _latchStatus);

	//BIT 8 Latch Unlocked
	_unLatchStatus = ReadBit(BinaryData, 8) ? UnLatch : NoUnLatch;

	//printf("[UpdateLoadPortStatus] Latch Unlock Status : [%d]\n", _unLatchStatus);

	//BIT 9 Clamp Status 1:Clamp  //2
	//BIT 10 Unclamp Status 1:UnClamp //2
	PStatus = ReadBit(BinaryData, 9);
	NStatus = ReadBit(BinaryData, 10);
	//printf("[UpdateLoadPortStatus] Clamp Status : [%d], Unclamp Status : [%d]\n", PStatus, NStatus);
	_clampStatus = (PStatus && !NStatus) ? Clamp : (!PStatus && NStatus) ? Unclamp : ClmapUnknown;

	//BIT 11 Docking Status 1:Docked //2
	//BIT 12 Undocking Status 1:UnDocked //2
	PStatus = ReadBit(BinaryData, 11);
	NStatus = ReadBit(BinaryData, 12);
	//printf("[UpdateLoadPortStatus] Dock Status : [%d],Undock Status : [%d]\n", PStatus, NStatus);
	_dockingStatus = (PStatus && !NStatus) ? Docking : (!PStatus && NStatus) ? Undocking : DockingUnknown;

	//BIT 15 Vacuum Status
	_vacStatus = ReadBit(BinaryData, 15) ? Vac : NoVac;

	//BIT 22 Mapping Function Usage 1:Enable
	_mappingFunctionMode = ReadBit(BinaryData, 22) ? MappingEnable : MappingDisable;

	//BIT 23 Auto Mode Condition
	_isAutoMode = ReadBit(BinaryData, 23);
	//printf("[UpdateLoadPortStatus] Auto Mode Status  : [%d]\n", _isAutoMode);

	//BIT 24 Load / Unload ID Switch Usage yes/no
	_loadUnloadSwitchMode = ReadBit(BinaryData, 24) ? SwitchEnable : SwitchDisable;

	//BIT 26 Door Up Position, BIT 27 Door Down Position
	PStatus = ReadBit(BinaryData, 26);
	NStatus = ReadBit(BinaryData, 27);
	_doorPosition = (PStatus && !NStatus) ? PosUp : (!PStatus && NStatus) ? PosDown : PosUnknown;

	//BIT 28 Placement Sensor Condition 1:ON 0:OFF
	_placementStatus = ReadBit(BinaryData, 28) ? PlacementOn : PlacementOff;
	//printf("[UpdateLoadPortStatus] Placement Sensor  : [%d]\n", _placementStatus);

	//BIT 29 Present Sensor Condition 1:=ON 0:FF
	_presenceStatus = ReadBit(BinaryData, 29) ? PresenceOn : PresenceOff;
	//printf("[UpdateLoadPortStatus] Present Sensor  : [%d]\n", _presenceStatus);

	//BIT 31 Alarm Status
	_alarmStatus = ReadBit(BinaryData, 31) ? AlarmOn : AlarmOff;

	//BIT 0  Home Complete
	_homeComplete = ReadBit(BinaryData, 0) ? HomeComplete : HomeNotComplete;
	//printf("[UpdateLoadPortStatus] Home Complete Status : [%d]\n", _homeComplete);

	//BIT 4 Moving Status
	_isBusy = ReadBit(BinaryData, 4) ? Busy : Stop;
	//printf("[UpdateLoadPortStatus] Busy Status : [%d]\n", _isBusy);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Dura Port Indicator Status Message Format
// "Ixxxxxxxx"
// 0번부터 4비트씩 할당 
// NotUse   NotUse   Button    Reserve  Manual  Auto   Unload  Load 
//  4Bit	 4Bit	  4Bit  	4Bit	 4Bit	4Bit	4Bit   4Bit
//////////////////////////////////////////////////////////////////////////
void UpdateIndicatorStatus(char *statusData)
{
	//LOAD Indicator Status  
	_loadLampStatus = (statusData[8] == '0') ? LampOff :
		(statusData[8] == '1') ? LampOn : LampBlink;
	//UnLOAD Indicator Status
	_unloadLampStatus = (statusData[7] == '0') ? LampOff :
		(statusData[7] == '1') ? LampOn : LampBlink;
	//AUTO Indicator Status
	_autoLampStatus = (statusData[6] == '0') ? LampOff :
		(statusData[6] == '1') ? LampOn : LampBlink;
	//MANUAL Indicator Status
	_manualLampStatus = (statusData[5] == '0') ? LampOff :
		(statusData[5] == '1') ? LampOn : LampBlink;
	//RESERVE Indicator Status
	_reserveLampStatus = (statusData[4] == '0') ? LampOff :
		(statusData[4] == '1') ? LampOn : LampBlink;
	//Button Indicator Status
	_buttonLampStatus = (statusData[3] == '0') ? LampOff :
		(statusData[3] == '1') ? LampOn : LampBlink;
}

//////////////////////////////////////////////////////////////////////////
// Event Data Format
// "C00000000"
//-----------------------------------------------------------------------------------
/*
	 아래 내용만 Event로 전달 됨. Response "C"
	 Data는 8자리 ASCII 문자로 구성되어 있음.
	 ================================================================================================
	 [BIT  0 : 0x00000001] TP Connected              : TP 연결
	 [BIT  1 : 0x00000002] TP Disconnected           : TP 제거
	 [BIT  2 : 0x00000004] Load/Unload Button Pushed : Load/Unload Switch 입력
	 [BIT  4 : 0x00000010] POD IN                    : POD가 Port에 놓여짐
	 [BIT  5 : 0x00000020] POD OUT                   : POD가 Port에서 제거됨
	 [BIT  8 : 0x00000100] Start Reset All           : System Reset Switch가 눌러짐.
	 [BIT  9 : 0x00000200] End Reset All             : System Reset 동작이 종료 됨.
	 [BIT 16 : 0x00010000] FOUP Started              : System에 전원이 공급되고 Program 시작
	 [BIT 17 : 0x00020000] FOUP Incorrect POS        : Load 상태에서 외부 요인에 따른 Docking 위치 이탈 발생
	 [BIT 24 : 0x01000000] EStop Pushed              : E-Stop Button이 눌렸을 때
	 ================================================================================================
*/
//////////////////////////////////////////////////////////////////////////
int UpdateEventData(char *eventData, int count)
{
	int step = 0;
	char szReturnData[DEFAULT_LENGTH] = "";
	char temp1[DEFAULT_LENGTH] = "";
	DWORD BinaryData;
	int PStatus, NStatus;

	STR_SEPERATE_CHAR(eventData, 'C', temp1, szReturnData, count);
	BinaryData = strtoul(szReturnData, NULL, 16);
	//BIT 0  Home Complete
	if (ReadBit(BinaryData, 0))	_tpConnectStatus = Connect;
	else if (ReadBit(BinaryData, 1))	_tpConnectStatus = Disconnect;

	_loadButton = ReadBit(BinaryData, 2);
	_unloadButton = ReadBit(BinaryData, 2);

	_eStopStatus = ReadBit(BinaryData, 24);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Get Current Load Port Status
//////////////////////////////////////////////////////////////////////////
// int GetDuraPortStatus()
// {
// 	int result = 0;
// 
// 	//result = RS232_IO( STATUS );
// 				
// 	return result;
// }
// 
//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
int ControlLEDLamp(int ID2, int Data)
{
	int result = FALSE;

	if (ID2 == 0)			//Lamp Remote Mode										
	{
		if (Data == 0) result = SendReceiveCommand("LED_SETCONT OFF");
		else result = SendReceiveCommand("LED_SETCONT ON");
	}
	else if (ID2 == 1)			//ManualLamp Control											
	{
		if (Data == 0) result = SendReceiveCommand("LED_STATE STATE1 OFF");
		else if (Data == 1) result = SendReceiveCommand("LED_STATE STATE1 ON");
		else if (Data == 2) result = SendReceiveCommand("LED_STATE STATE1 BLINK");
	}
	else if (ID2 == 2)		//Auto Lamp Control											
	{
		if (Data == 0) result = SendReceiveCommand("LED_STATE STATE2 OFF");
		else if (Data == 1) result = SendReceiveCommand("LED_STATE STATE2 ON");
		else if (Data == 2) result = SendReceiveCommand("LED_STATE STATE2 BLINK");
	}
	else if (ID2 == 3)		//Load  Lamp Control											
	{
		if (Data == 0) result = SendReceiveCommand("LED_STATE STATE3 OFF");
		else if (Data == 1) result = SendReceiveCommand("LED_STATE STATE3 ON");
		else if (Data == 2) result = SendReceiveCommand("LED_STATE STATE3 BLINK");
	}
	else if (ID2 == 4)		//Unload Lamp Control												
	{
		if (Data == 0) result = SendReceiveCommand("LED_STATE STATE4 OFF");
		else if (Data == 1) result = SendReceiveCommand("LED_STATE STATE4 ON");
		else if (Data == 2) result = SendReceiveCommand("LED_STATE STATE4 BLINK");
	}
	else if (ID2 == 5)		//Reserved Lamp Control												
	{
		if (Data == 0) result = SendReceiveCommand("LED_STATE STATE5 OFF");
		else if (Data == 1) result = SendReceiveCommand("LED_STATE STATE5 ON");
		else if (Data == 2) result = SendReceiveCommand("LED_STATE STATE5 BLINK");
	}


	if (!result)
	{
		printf("[%s] [%8s] DO Error - ID2[%d] Line[%d]\n", GetTime(_time), _drvName, ID2, __LINE__);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int UpdateLampStatus(int ID2, int ID3, int ID4)
{
	// Load Lamp Control Success (Lamp Status Update)
	if (ID2 == 1)
	{
		_loadLampStatus = (ID3 == 1 && ID4 == 1) ? LampOn :
			(ID3 == 1 && ID4 == 2) ? LampOff :
			(ID3 == 2 && ID4 == 1) ? LampBlink : LampOff;
	}
	// UnLoad Lamp Control Success (Lamp Status Update)
	else if (ID2 == 2)
	{
		_unloadLampStatus = (ID3 == 1 && ID4 == 1) ? LampOn :
			(ID3 == 1 && ID4 == 2) ? LampOff :
			(ID3 == 2 && ID4 == 1) ? LampBlink : LampOff;
	}
	// Auto Lamp Control Success (Lamp Status Update)
	else if (ID2 == 3)
	{
		_autoLampStatus = (ID3 == 1) ? LampOn : LampOff;
	}
	// Manual Lamp Control Success (Lamp Status Update)
	else if (ID2 == 4)
	{
		_manualLampStatus = (ID3 == 1) ? LampOn : LampOff;
	}

	return TRUE;
}
 
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
RESPONSETYPE AnalysisResponseData(char *recv, int count)
{
	RESPONSETYPE res;
	char logMsg[256] = { 0, };

	//printf("[AnalysisResponseData] recv [%s]\n", strResponse);
	sprintf(logMsg, "[AnalysisResponseData] Response [%s]\n", recv);
	SaveCommLogFile(logMsg, strlen(logMsg), eRECV_1);

	if (strncmp(recv, NAK_MESSAGE, 1) == 0)
	{
		res = NAKMSG;
		_runFlag = Complete;
	}
	else if (strncmp(recv, INTERLOCK_MESSAGE, 21) == 0)				// Interlock Event
	{
		//UpdateErrorInform(recv, count);
		//_runFlag = Complete;
		//res = ERRORMSG;
	}
	else if (strncmp(recv, ERROR_MESSAGE, 1) == 0)					//응답이 Error일 경우
	{
		//UpdateErrorInform(recv, count);
		//_runFlag = Complete;
		//res = ERRORMSG;
	}
	else if (strncmp(recv, STATUS_MESSAGE, 1) == 0)
	{
		UpdateLoadPortStatus(recv, count);
		res = DATAMSG;
	}
	else if (strncmp(recv, STATUS_INDICATOR_MESSAGE, 1) == 0)	//응답이 Indicator Status Data 경우
	{
		//UpdateIndicatorStatus(recv);
		//_runFlag = Complete;
		//res = DATAMSG;
	}
	else if (strncmp(recv, EVENT_MESSAGE_TYPE1, 1) == 0)				//응답이 Type1 Event Data 경우
	{
		//UpdateEventData(recv, count);
		//_runFlag = Complete;
		////sprintf( LogMsg , "Read String Error[%s]\n", strResponse );
		////SaveCommLogFile( LogMsg , strlen(LogMsg) , eRECV_1 );

		res = EVTMSG;
	}
	else if (strncmp(recv, EVENT_MESSAGE_TYPE2, 1) == 0)				//응답이 Type2 Event Data 경우
	{
		//UpdateEventData(strResponse, count);
		//_runFlag = Complete;
		////sprintf( LogMsg , "Read String Error[%s]\n", strResponse );
		////SaveCommLogFile( LogMsg , strlen(LogMsg) , eRECV_1 );

		res = EVTMSG;
	}
	else if (strncmp(recv, MAPPINGD_DATA_MESSAGE, 1) == 0)			//응답이 Mapping Data 경우
	{
		UpdateMappingData(recv, count);
		res = DATAMSG;
		_runFlag = Complete;
	}
	else if (strncmp(recv, COMMAND_COMPLETE_MESSAGE, 1) == 0)		//응답이 Command Complete 경우
	{
		////printf("[AnalysisResponseData] complete [%s]\n", recv);
		//res = COMPLETEMSG;
		//_runFlag = Complete;
	}
	else if (strncmp(recv, ACK_MESSAGE, 1) == 0)
	{
		//res = ACKMSG;
	}
	else
	{
		//printf("[AnalysisResponseData] else [%s]\n", recv);
	}


	return res;
}
