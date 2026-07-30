#include "NMC2.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include "c_cpp_mix.h"

#define BOOL int

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

static short m_nDev_no;
static char gsModule_Name[8];
static BOOL m_bIsOpen;

enum DEVICE { IO, MOTOR };

typedef struct {
	double Start_Spd;
	double Acc_Spd;
	double Dec_Spd;
	double Drv_Spd;
} CurveSpeed;

typedef struct {
	double Home_Spd0;
	double Home_Spd1;
	double Home_Spd2;
	double Offset_Spd;
} HomeSpeed;

CurveSpeed CurSpd[10] = { 0 };
HomeSpeed HomeSpd[10] = { 0 };

BOOL Device_Attach(char* Name, char* File, int* ID1, int ID2, int ID3, int ID4, int ID5, int ID6, int ID7, int ID8, int ID9, int ID10) {

	printf("%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", Name, File, ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, ID9, ID10);

	m_nDev_no = (short)ID4;
	nmc_PingCheck(m_nDev_no, 10);

	nmc_OpenDevice(m_nDev_no);

	int dataType;

	nmc_GetDeviceType(m_nDev_no, &dataType);
	printf("%d\n", dataType);

	return TRUE;

	//m_nDev_no = (short)ID4;
	////gsModule_Name = Name;

	//if (fn_nmc_PingCheck(m_nDev_no, 10) != 0) {

	//	printf("Ping Check Error\n");
	//	return FALSE;
	//}

	//if (fn_nmc_OpenDevice(m_nDev_no) == 0) {

	//	printf("[%s] DEVICE_ATTACH SUCCESS\n", Name);
	//	m_bIsOpen = TRUE;
	//}

	//else {

	//	printf("DEVICE_ATTACH FAIL\n");
	//	m_bIsOpen = FALSE;
	//}

	//return m_bIsOpen;
}

BOOL Device_Detach(char* Name, char* File, int ID1, int ID2, int ID3, int ID4, int ID5, int ID6, int ID7, int ID8, int ID9, int ID10) {

	printf("DEVICE_DETACH START\n");

	m_nDev_no = (short)ID4;

	fn_nmc_CloseDevice(m_nDev_no);
	//m_bIsOpen = FALSE;

	printf("DEVICE_DETACH SUCCESS\n");

	return TRUE;
}

int Digital_In(int ID1, int ID2, int ID3, int ID4, int* Result) {
	int ReadData = 0;
	short sPinData[64];
	short s_Result;
	short nRet;
	NMCAXESEXPR NmcData;

	if (ID1 == (int)IO) {
		*Result = TRUE;
		s_Result = fn_nmc_GetDIOInput(m_nDev_no, sPinData);

		ReadData = sPinData[ID2];

	}

	else if (ID1 == (int)MOTOR) {
		*Result = TRUE;

		memset(&NmcData, 0x00, sizeof(NMCAXESEXPR));
		nRet = fn_nmc_GetAxesExpress(m_nDev_no, &NmcData);

#ifdef __DEBUG__
		printf("%d Home=%d , Busy = %d \n", nRet, NmcData.nHome[ID3], NmcData.nBusy[ID3]);
#endif

		switch (ID2) {

		case 1:                                         // Home Status        < Home Search 동작 상태(0=동작중, 1=None)
			ReadData = NmcData.nHome[ID3];
			break;

		case 2:                                         // Run Status or BusySignal        < 펄스 출력 상태(0=Idle, 1=Busy)
			ReadData = NmcData.nBusy[ID3];
			break;

		default:
			break;

		}

	}
	else {

	}
	return ReadData;
}

void Digital_Out(int ID1, int ID2, int ID3, int ID4, int Data, int* Result) {
	char Command[32] = { 0, };
	char Response[32] = { 0, };
	*Result = FALSE;

	if (ID1 == (int)IO) {

		*Result = TRUE;
		fn_nmc_SetDIOOutPin(m_nDev_no, (short)ID2, (short)Data);

		//short WINAPI nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);

	}
	else if (ID1 == (int)MOTOR) {

		*Result = TRUE;

		switch (ID2) {

		case 0:												// 지정 축의 Servo On을 설정
			fn_nmc_SetServoOn(m_nDev_no, (short)ID3, (short)Data); // chuck
			break;

		case 1:												// HOME (0=+Limit, 1=-Limit, 2=+Near, 3=-Near, 4= -Z, 5= +Z)
			fn_nmc_HomeMove(m_nDev_no, (short)ID3, (short)Data, 0, 0, 0);
			break;

		case 2:												// 지정 축의 Current On을 설정 -> 1축, 2축의 Current는 Off로 고정
			fn_nmc_SetCurrentOn(m_nDev_no, (short)ID3, (short)Data);
			break;

		case 3:												// 구동 중인 축을 긴급 정지합니다
			fn_nmc_SuddenStop(m_nDev_no, (short)ID3);
			break;

		case 4:

			fn_nmc_JogMove(m_nDev_no, (short)ID3, (short)Data);
			break;

		case 5:
			//printf("||%d Axis CurSpd : %f, %f, %f, %f\n", ID3, CurSpd[ID3].Start_Spd, CurSpd[ID3].Acc_Spd, CurSpd[ID3].Dec_Spd, CurSpd[ID3].Drv_Spd);
			fn_nmc_SetSpeed(m_nDev_no, (short)ID3, CurSpd[ID3].Start_Spd, CurSpd[ID3].Acc_Spd, CurSpd[ID3].Dec_Spd, CurSpd[ID3].Drv_Spd);		// 구동 속도
	
			break;

		case 6:
			//printf("||%d Axis HomeSpd : %f, %f, %f, %f\n", ID3, HomeSpd[ID3].Home_Spd0, HomeSpd[ID3].Home_Spd1, HomeSpd[ID3].Home_Spd2, HomeSpd[ID3].Offset_Spd);
			fn_nmc_SetHomeSpeed(m_nDev_no, (short)ID3, HomeSpd[ID3].Home_Spd0, HomeSpd[ID3].Home_Spd1, HomeSpd[ID3].Home_Spd2, HomeSpd[ID3].Offset_Spd);	// 홈 속도
			break;

		default:
			break;

		}
	}
	else {

	}
}

double Analog_In(int ID1, int ID2, int ID3, int* Result) {
	double dData = 0;
	char Command[32] = { 0, };
	char Response[32] = { 0, };
	short nRet;
	NMCAXESEXPR pNmcData;

	if (ID1 == (int)IO) {

		dData = 0;
	}

	else if (ID1 == (int)MOTOR) {

		*Result = TRUE;

		//fn_nmc_GetAxesExpress(m_nDev_no, pNmcData);

		memset(&pNmcData, 0x00, sizeof(NMCAXESEXPR));

		nRet = fn_nmc_GetAxesExpress(m_nDev_no, &pNmcData);

#ifdef __DEBUG__
		printf("%d, Enc = %d,Cmd = %d \n", nRet, pNmcData.dEnc[ID3], pNmcData.dCmd[ID3]);
#endif
		switch (ID2) {

		case 0: //Encoder             /*!< 엔코더 위치(UnitPerPulse 적용) */
			dData = pNmcData.dEnc[ID3];
			break;

		case 1: //CmdPos            /*!< 지령 위치(UnitPerPulse 적용) */
			dData = pNmcData.dCmd[ID3];
			break;

		default:
			dData = 0;
			*Result = FALSE;
			break;
		}
	}

	else {

	}

	////------------------------------------------------
	//// Comm Log
	////------------------------------------------------

	//if ((gLogMode == (int)EnumLog_Mode._COMM_LOG_ERROR) || (gLogMode == (int)EnumLog_Mode._COMM_LOG_ALL))
	//{
	// if (Result != (int)Result_Comm.TRUE)
	// {
	//  strLogData = string.Format("[ERR] AI [{0:D3},{1:D3},{2:D3}] -> {3}", ID1, ID2, ID3, strSubLogData);
	//  _LOG_WRITE(strLogData, EnumLog._LOGERR);
	// }
	//}

	////------------------------------------------------
	//// Command Log
	////------------------------------------------------

	//if ((gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_READ) || (gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_ALL))
	//{
	// strLogData = string.Format("[READ] AI [{0:D3},{1:D3},{2:D3}] -> ReturnData : {3}", ID1, ID2, ID3, dData);
	// _LOG_WRITE(strLogData, EnumLog._LOGOPER);
	//}

	return dData;
}

void Analog_Out(int ID1, int ID2, int ID3, double Data, int* Result) {

	//string strLogData = "";
	//string strSubLogData = "";

	//string Command = string.Empty;
	//string Response = string.Empty;
	char Command[32] = { 0, };
	char Response[32] = { 0, };
	int AxisNo, SpeedNo;

	if (ID1 == (int)IO) {

		*Result = TRUE;
		fn_nmc_SetDIOOutPin(m_nDev_no, (short)ID2, (short)Data);


	}

	else if (ID1 == (int)MOTOR) {

		*Result = TRUE;

		switch (ID2)
		{

		case 0:
			fn_nmc_SetCmdPos(m_nDev_no, (short)ID3, (double)Data);
			break;

		case 1:
			fn_nmc_SetEncPos(m_nDev_no, (short)ID3, (double)Data);
			break;

		case 2:
			fn_nmc_AbsMove(m_nDev_no, (short)ID3, (double)Data);
			break;

		case 3:
			fn_nmc_RelMove(m_nDev_no, (short)ID3, (double)Data);
			break;

		case 4:
			fn_nmc_SetCmdEncPos(m_nDev_no, (short)ID3, (double)Data);
			break;

		case 5:
			AxisNo = ID3 / 10;
			SpeedNo = ID3 % 10;
			if		(SpeedNo == 0)	CurSpd[AxisNo].Start_Spd = (double)Data;
			else if (SpeedNo == 1)	CurSpd[AxisNo].Acc_Spd = (double)Data;
			else if (SpeedNo == 2)	CurSpd[AxisNo].Dec_Spd = (double)Data;
			else if (SpeedNo == 3)	CurSpd[AxisNo].Drv_Spd = (double)Data;

			//printf("%d Axis CurSpd : %f, %f, %f, %f\n", AxisNo, CurSpd[AxisNo].Start_Spd, CurSpd[AxisNo].Acc_Spd, CurSpd[AxisNo].Dec_Spd, CurSpd[AxisNo].Drv_Spd);

			break;

		case 6:
			AxisNo = ID3 / 10;
			SpeedNo = ID3 % 10;
			if (SpeedNo == 0)	HomeSpd[AxisNo].Home_Spd0 = (double)Data;
			else if (SpeedNo == 1)	HomeSpd[AxisNo].Home_Spd1 = (double)Data;
			else if (SpeedNo == 2)	HomeSpd[AxisNo].Home_Spd2 = (double)Data;
			else if (SpeedNo == 3)	HomeSpd[AxisNo].Offset_Spd = (double)Data;

			//printf("%d Axis HomeSpd : %f, %f, %f, %f\n", AxisNo, HomeSpd[AxisNo].Home_Spd0, HomeSpd[AxisNo].Home_Spd1, HomeSpd[AxisNo].Home_Spd2, HomeSpd[AxisNo].Offset_Spd);

			break;

		default:
			break;


		}
	}

	////------------------------------------------------
	//// Comm Log
	////------------------------------------------------

	//if ((gLogMode == (int)EnumLog_Mode._COMM_LOG_ERROR) || (gLogMode == (int)EnumLog_Mode._COMM_LOG_ALL))
	//{
	// if (Result != (int)Result_Comm.TRUE)
	// {
	//  strLogData = string.Format("[ERR] AO [{0:D3},{1:D3},{2:D3}] -> {3}", ID1, ID2, ID3, strSubLogData);
	//  _LOG_WRITE(strLogData, EnumLog._LOGERR);
	// }
	//}

	////------------------------------------------------
	//// Command Log
	////------------------------------------------------

	//if ((gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_READ) || (gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_ALL))
	//{
	// strLogData = string.Format("[SET] AO [{0:D3},{1:D3},{2:D3}] -> SetData : {3}", ID1, ID2, ID3, Data);
	// _LOG_WRITE(strLogData, EnumLog._LOGOPER);
	//}
}

//------------------------------------------------------------------------------------
void String_In(int ID1, char* Data, int* Result) {

	char Command[32] = { 0, };
	char Response[32] = { 0, };

	//string Command = string.Empty;
	//string Response = string.Empty;

	//string strLogData = "";
	//string strSubLogData = "";

	switch (ID1)
	{


	default:
		*Result = FALSE;
		printf("Wrong IO Error\n");
		break;

	}
	////------------------------------------------------
	//// Comm Log
	////------------------------------------------------

	//if ((gLogMode == (int)EnumLog_Mode._COMM_LOG_ERROR) || (gLogMode == (int)EnumLog_Mode._COMM_LOG_ALL))
	//{
	// if (Result != (int)Result_Comm.TRUE)
	// {
	//  strLogData = string.Format("[ERR] AI [{0:D3},{1:D3},{2:D3}] -> {3}", ID1, strSubLogData);
	//  _LOG_WRITE(strLogData, EnumLog._LOGERR);

	// }
	//}
	////------------------------------------------------
	//// Command Log
	////------------------------------------------------

	//if ((gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_READ) || (gCmdLogMode == (int)EnumCommand_Log_Mode._COMMAND_LOG_ALL))
	//{
	// strLogData = string.Format("[READ] AI [{0:D3},{1:D3},{2:D3}] -> ReturnData : {3}", ID1, Data);
	// _LOG_WRITE(strLogData, EnumLog._LOGOPER);
	//}
}
//------------------------------------------------------------------------------------
void String_Out(int ID1, char* Data, int* Result) {

	char Command[32] = { 0, };
	char Response[32] = { 0, };

	//string Command = string.Empty;
	//string Response = string.Empty;
	//string strData = Marshal.PtrToStringAnsi(Data);

}

int Digital_Name_In(char* Name, int* Result) {

	*Result = TRUE;

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
void Digital_Name_Out(char* Name, int data, int* Result) {

	*Result = TRUE;
}
//-------------------------------------------------------------------------------------------------
double Analog_Name_In(char* Name, int* Result) {

	*Result = TRUE;

	return 0;
}
//-------------------------------------------------------------------------------------------------
void Analog_Name_Out(char* Name, double data, int* Result) {

	*Result = TRUE;

}
//-------------------------------------------------------------------------------------------------
void String_Name_In(char* Name, char* data, int* Result) {
	*Result = TRUE;


}
//-------------------------------------------------------------------------------------------------
void String_Name_Out(char* Name, char* data, int* Result) {
	*Result = TRUE;

}