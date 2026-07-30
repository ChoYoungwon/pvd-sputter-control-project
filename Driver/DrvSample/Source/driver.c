#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "CimRs232.h"
//-----------------------------------------------------------------------------------
#include "kutletc.h"
#include "kutlanal.h"
#include "kutlstr.h"
//-----------------------------------------------------------------------------------
#include "driver.h"
#include "Define.h"
#include "process.h"
#include "CimSeqnc.h"

int  _timeOut;
int	 _address;
char _header;
char _sendStart[2], _sendTerm[2], _recvTerm[2];

#define START_CODE		0x01

CRITICAL_SECTION        _crSection;

char	_drvName[10] = { 0, };
int _disConnect = 0;
int		_commPort = 0;
BOOL	_log = FALSE;
BOOL	_display = FALSE;
char	_time[32] = { 0, };

// Variable for Thread
int _threadRunning = FALSE;
int _threadTerminate = FALSE;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
char* GetTime(char* szTime)
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	sprintf(szTime, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return szTime;
}
//-------------------------------------------------------------------------------------------------
BOOL SaveCommLogFile(char* msg, int msgLength, int type)
{
	char	timeCode[32];
	char	logFileName[256];
	char	logData[1024];

	FILE* fp = NULL;

	SYSTEMTIME	st;

	GetLocalTime(&st);

	memset(timeCode, 0, sizeof(char) * 32);
	memset(logFileName, 0, sizeof(char) * 256);
	memset(logData, 0, sizeof(char) * 1024);

	mkdir("History");
	mkdir("History/Datalog");
	mkdir("History/Datalog/DrvSample");
	sprintf(logFileName, "History/Datalog/DrvSample/DrvSample_%04d%02d%02d%02d.log", st.wYear, st.wMonth, st.wDay, st.wHour);

	sprintf(timeCode, "[%02d:%02d:%02d:%03d]", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	if (type == eSEND_0) { sprintf(logData, "%s [SEND] %s\n", timeCode, msg); }
	else if (type == eRECV_1) { sprintf(logData, "%s [RECV] %s\n", timeCode, msg); }
	else if (type == eERROR_2) { sprintf(logData, "%s [ERR] %s\n", timeCode, msg); }
	else if (type == eTIMEOUT_3) { sprintf(logData, "%s [TIMEOUT] %s\n", timeCode, msg); }
	else { sprintf(logData, "%s [OTHER] %s\n", timeCode, msg); }
	//
	if (_log)
	{
		fp = fopen(logFileName, "a+");
		if (fp == NULL)
		{
			printf("[%s] [%8s] %s Log File Open Fail [%d]\n", GetTime(_time), _drvName, logData, GetLastError());
			return FALSE;
		}
		fprintf(fp, "%s", logData);
		fclose(fp);
	}
	//
	if (_display)
	{
		printf("[%s] [%8s] %s", GetTime(_time), _drvName, logData);
	}
	//
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL Device_Attach(char* Name, char* File, int ID1, int ID2, int ID3, int ID4, int ID5, int ID6, int ID7, int ID8, int ID9, int ID10)
{
	printf("Name : %s\n", Name);
	printf("File : %s\n", File);
	printf("ID1 : %d, ID2 : %d,ID3 : %d,ID4 : %d,ID5 : %d,ID6 : %d,ID7 : %d,ID8 : %d,ID9 : %d,ID10 : %d\n",
		ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, ID9, ID10);

	BOOL connStatus;
	char logMsg[256] = { 0, };

	//_sendTerm[0] = CR;	//0x0D
	//_sendTerm[1] = LF;
	_recvTerm[0] = CR;	//0x0D
	_recvTerm[1] = LF;

	strcpy(_drvName, Name);
	_log = TRUE;
	_display = TRUE;

	if (ID10 < 100) _ackTimeOut = 500;
	else            _ackTimeOut = ID10;

	connStatus = RS232_Connect_Port(ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8);

	if (connStatus == TRUE)
	{
		sprintf(logMsg, "DrvSample Driver Loading OK!\n");
		SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);
		sprintf(logMsg, "Load Info Port[%d] Baudrate[%d]\n", ID1, ID2);
		SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);

		InitializeCriticalSection(&_crSection);

		return TRUE;
	}
	else return FALSE;

	//int	res;
	//int	elapsedTime = 0;
	//char fileName[256] = "";
	//char logMsg[256] = { 0, };
	//BOOL connection = FALSE;

	//if (ID10 < 100) _ackTimeOut = 500;
	//else            _ackTimeOut = ID10;

	//strcpy(_drvName, Name);
	//_log = TRUE;
	//_display = TRUE;

	//_header = START_CODE; // 0x01
	//_address = 0;
	//_sendStart[0] = 's';
	//_sendStart[1] = 0;
	//_sendTerm[0] = CR;	//0x0D
	//_sendTerm[1] = LF;
	//_recvTerm[0] = CR;	//0x0D
	//_recvTerm[1] = LF;

	//connection = RS232_Connect_Port(ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8);

	//if (!connection)
	//{
	//	printf("---> [%8s] : Device Attach : Serial_Connect_Port() Error...\n", _drvName);
	//	return FALSE;
	//}

	////
	//_commStatus = TRUE;
	////

	//sprintf(logMsg, "DrvSample Driver Loading OK!\n");
	//SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);
	////sprintf(logMsg, "Load Info IP[%d.%d.%d.%d] Port[%d] Server[%d]\n", ID1, ID2, ID3, ID4, ID5, ID6);
	////SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);

	//InitializeCriticalSection(&_crSection);
	////_beginthread((void *)Polling, 0, 0);
	////SendReceiveCommand(HOMING);

	////while (TRUE)
	////{
	////	if (_threadRunning == TRUE) break;
	////	if (elapsedTime++ > 50) break;
	////	_sleep(100);
	////}

	////if (_threadRunning != TRUE)
	////{
	////	sprintf(logMsg, "Failed to wait thread initialize\n");
	////	SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);
	////	res = FALSE;
	////}
	////else
	////{
	////	res = TRUE;
	////}

	//_log = FALSE;
	//_display = FALSE;

	//return res;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL Device_Detach(char* Name, char* File, int ID1, int ID2, int ID3, int ID4, int ID5, int ID6, int ID7, int ID8, int ID9, int ID10)
{
	char	logMsg[256] = { 0, };

	printf("Name : %s\n", Name);
	printf("File : %s\n", File);
	printf("ID1 : %d, ID2 : %d,ID3 : %d,ID4 : %d,ID5 : %d,ID6 : %d,ID7 : %d,ID8 : %d,ID9 : %d,ID10 : %d\n",
		ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, ID9, ID10);

	RS232_Disconnect_Port();

	sprintf(logMsg, "[Device_Detach] Communication Close\n");
	SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);

	//Delete Critical Section
	DeleteCriticalSection(&_crSection);

	return TRUE;

	//int elapsedTime = 0;
	//char	logMsg[256] = { 0, };
	//_threadTerminate = TRUE;   // terminate polling thread

	//// wait thread terminate
	//while (TRUE)
	//{
	//	if (_threadRunning == FALSE) break;

	//	if (elapsedTime++ > 50)
	//	{
	//		sprintf(logMsg, "Failed to End thread.\n");
	//		SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);
	//		break;
	//	}
	//	Sleep(100);
	//}

	//RS232_Disconnect_Port();

	//sprintf(logMsg, "[Device_Detach] Communication Close\n");
	//SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);

	////Delete Critical Section
	//DeleteCriticalSection(&_crSection);

	//return TRUE;
}

void CalcurateChecksum(char* data, int length, char* crc)
{
	unsigned int sum = 0;
	int len = strlen(data);

	sum = length;
	while (len--)
	{
		sum += *data++;
	};

	sum &= 0xff;

	char checksum[3];
	sprintf(checksum, "%x%x", (sum >> 4) & 0xf, sum & 0xf);

	strcpy(crc, checksum);
	//char *str = (char*)malloc(3);
	//str[0] = checksum[0];
	//str[1] = checksum[1];
	//str[2] = '\0';

	//return str;
}

//void MakeCommand(char* command, char* data)
//{
//	char tempBuffer[256] = { 0, };
//
//	////////////////////////////////////////////////////////////
//	// Calcurate Checksum
//	////////////////////////////////////////////////////////////
//	sprintf(tempBuffer, "%02d%s;", _address, data); // address = 0x30, 0x30
//	//sprintf(tempBuffer, "00%s;", data); // address = 0x30, 0x30
//	printf("[MakeCommand-1] Command [%s]\n", tempBuffer);
//	int len = 2 + strlen(tempBuffer); // command + checksum(2)
//
//	len &= 0xff;
//	char hexLen[3];
//	sprintf(hexLen, "%x%x", (len >> 4) & 0xf, len & 0xf);
//	//sprintf(hexLen, "%x%x", 0x00, 0x0E);
//	printf("[MakeCommand-2] hexLen [%x],[%x]\n", hexLen[0], hexLen[1]);
//
//	char checksum[5] = { 0, };
//	CalcurateChecksum(tempBuffer, len, checksum);
//	printf("[MakeCommand-3] Checksum [%s]\n", checksum);
//
//	////////////////////////////////////////////////////////////
//	// Make command
//	////////////////////////////////////////////////////////////
//	//sprintf(buffer, "%c%s%s%c%c%c", _header, hexLen, tempBuffer, checksum[0], checksum[1], _sendTerm);
//	//sprintf(buffer, "%s%s%s", _sendStart, tempBuffer, _sendTerm);
//	//sprintf(buffer, "%c%x%x%s%c%c%s", _header, hexLen[0], hexLen[1], tempBuffer, checksum[0], checksum[1], _sendTerm);
//	sprintf(command, "%c%c%c%s%c%c%s", _header, hexLen[0], hexLen[1], tempBuffer, checksum[0], checksum[1], _sendTerm);
//	//sprintf(command, "%c%c%c%s%c%c%c", _header, hexLen[0], hexLen[1], tempBuffer, checksum[0], checksum[1], CR);
//	printf("[MakeCommand-4] Command [%s], length [%d]\n", command, strlen(command));
//	//memcpy(command, buffer, sizeof(buffer));
//}

/////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int SendData(char* send)
{
	int len;
	int res;
	char SendCommand[DEFAULT_LENGTH] = { 0, };
	char logMsg[256] = { 0, };

	sprintf(SendCommand, "%s%s", send, _sendTerm);
	len = strlen(SendCommand);

	if (!RS232_Write_String(SendCommand, len))
	{
		res = FALSE;
		_commStatus = FALSE;
		sprintf(logMsg, "Write String Error[%s][%d]\n", SendCommand, len);
		SaveCommLogFile(logMsg, strlen(logMsg), eSEND_0);
	}
	else
	{
		printf("[SendData] Command :[%s]\n", SendCommand);
		res = TRUE;
		_commStatus = TRUE;
		SaveCommLogFile(SendCommand, strlen(SendCommand), eSEND_0);
	}

	printf("[SendData] SendCommand :[%s], [%d], [%d]\n", SendCommand, len, res);

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL RecvData(char* receiveData)
{
	int readCount;
	RESPONSETYPE res;
	DWORD startTime;
	char	logMsg[256] = { 0, };

	memset(receiveData, 0, sizeof(receiveData));
	startTime = GetTickCount64();
	while (TRUE)
	{
		printf("------------- Before receiving ---------------\n");
		//if (RS232_Read_String(receiveData, _recvTerm, strlen(receiveData), TIMEOUT1, &readCount))
		if (RS232_Read_String(receiveData, _recvTerm, 255, _ackTimeOut, &readCount))
		//if (RS232_Read_Count_Char(receiveData, 10, &readCount, TIMEOUT1))
		{
			printf("Recv. - 1[%s]\n", receiveData);
			//SaveCommLogFile(receiveData, strlen(receiveData), eRECV_1);
			//res = AnalysisResponseData(receiveData, readCount);
			//if (res == ACKMSG) return TRUE;
			//else if (res == NAKMSG) return FALSE;
			//else if (res == ERRORMSG) return FALSE;
			break;
		}
		
		//if (RS232_Read_Char(receiveData))
		//{
		//	printf("[Recv. - 2] [%s]\n", receiveData);
		//	break;
		//}

		if (GetTickCount64() >= startTime + (_ackTimeOut * 2))
		{
			sprintf(logMsg, "Wait AckNak Receive Timeout[%d]n", _runFlag);
			SaveCommLogFile(logMsg, strlen(logMsg), eTIMEOUT_3);
			return FALSE;
		}
		_sleep(100);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//void RecvEventData(void)
//{
//	char receiveData[DEFAULT_LENGTH] = "";
//	int  readCount = 0;
//
//	EnterCriticalSection(&_crSection);
//	//if ( Net_Read_Char(receiveData) )
//	//{
//	//	if ( RecvData( &receiveData[1], 500, &readCount ) )
//	//	{
//	//		SaveCommLogFile(receiveData, strlen(receiveData) , eRECV_1 );
//	//		AnalysisResponseData (receiveData, readCount );
//	//	}
//	//}
//	LeaveCriticalSection(&_crSection);
//}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//void Polling(void)
//{
//	int i;
//	_threadRunning = TRUE;
//
//	while (TRUE)
//	{
//		if (_threadTerminate == TRUE) break;
//
//		SendReceiveCommand(GET_STATE);
//		_sleep(300);
//		//for (i = 0; i < 5; i++)
//		//{
//			//if (_threadTerminate == TRUE) break;
//		//RecvEventData();
//		//_sleep(100);
//	//}
//	//_sleep(500);
//
//	}
//	_threadRunning = FALSE;
//
//	_endthread();
//}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//int WaitACKNAK(void)
//{
//	int readCount = 0;
//	char receiveData[DEFAULT_LENGTH] = "";
//	RESPONSETYPE res;
//	DWORD startTime;
//	char	logMsg[256] = { 0, };
//
//	startTime = GetTickCount();
//	while (TRUE)
//	{
//		if (RecvData(receiveData, _ackTimeOut, &readCount))
//		{
//			SaveCommLogFile(receiveData, strlen(receiveData), eRECV_1);
//			res = AnalysisResponseData(receiveData, readCount);
//			if (res == ACKMSG) return TRUE;
//			else if (res == NAKMSG) return FALSE;
//			else if (res == ERRORMSG) return FALSE;
//		}
//		if (GetTickCount() - startTime > _ackTimeOut)
//		{
//			sprintf(logMsg, "Wait AckNak Receive Timeout[%d]n", _runFlag);
//			SaveCommLogFile(logMsg, strlen(logMsg), eERROR_2);
//			return FALSE;
//		}
//	}
//
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL SendReceiveCommand(char* Send, char* Recv)
{
	BOOL res = 0;
	//int readCount = 0;
	//char receiveData[DEFAULT_LENGTH] = "";
	//RESPONSETYPE res;
	//DWORD startTime;
	//char	logMsg[256] = { 0, };

	EnterCriticalSection(&_crSection);

	if (SendData(Send))
	{
		//_sleep(300);

		res = RecvData(Recv);

		//if (strncmp(Send, GET_STATE, 6) != 0)
		//{
		//	if (res) _runFlag = Running;
		//}
	}
	LeaveCriticalSection(&_crSection);

	return res;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
double Analog_In(int ID1, int ID2, int ID3, int* Result)
{
	double	readData = 0.0;
	char	logMsg[256] = { 0, };
	char receiveData[DEFAULT_LENGTH] = { 0, };

	*Result = TRUE;

	if (ID1 == 0)
	{

	}
	else if (ID1 == 1)
	{
		if (SendReceiveCommand(GETTEMP, receiveData))
			readData = atof(receiveData);
	}

	if (!(*Result))
	{
		printf("[%s] [%8s] DI Error - ID1[%d] ID2[%d] ID3[%d]\n", GetTime(_time), _drvName, ID1, ID2, ID3);
	}

	return readData;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Analog_Out(int ID1, int ID2, int ID3, double Data, int* Result)
{
	*Result = FALSE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int Digital_In(int ID1, int ID2, int ID3, int ID4, int* Result)
{
	int	readData = 0;
	char	logMsg[256] = { 0, };
	char receiveData[DEFAULT_LENGTH] = { 0, };
	char sendData[DEFAULT_LENGTH] = { 0, };

	*Result = TRUE;

	if (ID1 == 0)
	{
		if(SendReceiveCommand(GETSTATE, receiveData) == TRUE)
			readData = atoi(receiveData);
	}
	else if (ID1 == 1)
	{
		if (SendReceiveCommand(HOMING, receiveData))
			readData = atoi(receiveData);
	}
	else if (ID1 >= 101 && ID1 <= 105)
	{
		sprintf(sendData, "DOUTSTATUS:%d", ID1 - 100);
		if (SendReceiveCommand(sendData, receiveData))
			readData = atoi(receiveData);
	}
	else if (ID1 == 300 || ID1 == 301)
	{
		sprintf(sendData, "GETVALVE:%d", ID1 - 299);
		if (SendReceiveCommand(sendData, receiveData))
			readData = atoi(receiveData);
	}

	if (!(*Result))
	{
		printf("[%s] [%8s] DI Error - ID1[%d] ID2[%d] ID3[%d] ID4[%d]\n", GetTime(_time), _drvName, ID1, ID2, ID3, ID4);
	}

	return readData;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Digital_Out(int ID1, int ID2, int ID3, int ID4, int Data, int* Result)
{
	*Result = TRUE;
	char sendData[DEFAULT_LENGTH] = { 0, };
	char receiveData[DEFAULT_LENGTH] = { 0, };

	switch (ID1) //CommStatus & Log Check Part
	{
	case 0:
		if (ID2 == 1)
		{	// CommLog Save
			_log = Data;
			break;
		}
		else if (ID2 == 2)
		{	// CommLog Print to Debugview
			_display = Data;
			break;
		}

	case 1:
		sprintf(sendData, "%s:%d", ROTATIONFAN, Data);
		if (SendReceiveCommand(sendData, receiveData) == TRUE)
		*Result = 1;
		break;

	case 2:
		sprintf(sendData, "%s:%d", DIRENTIONFAN, Data);
		if (SendReceiveCommand(sendData, receiveData) == TRUE)
		*Result = 1;
		break;

	case 3:
		sprintf(sendData, "%s:%d", FANSPEED, Data);
		if (SendReceiveCommand(sendData, receiveData) == TRUE)
		*Result = 1;
		break;

	case 200:
	case 201:
	case 202:
	case 203:
	case 204:
		sprintf(sendData, "DINSTATUS:%d:%d", ID1-199, Data);
		if (SendReceiveCommand(sendData, receiveData) == TRUE)
			*Result = 1;
		break;

	case 300:
	case 301:
		sprintf(sendData, "SETVALVE:%d:%d", ID1 - 299, Data);
		if (SendReceiveCommand(sendData, receiveData) == TRUE)
			*Result = 1;
		break;

	default:
		*Result = FALSE;
		return;
	}

	if (!(*Result))
	{
		printf("[%s] [%8s] DO Error - ID1[%d] ID2[%d] ID3[%d] ID4[%d]\n", GetTime(_time), _drvName, ID1, ID2, ID3, ID4);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void String_In(int ID1, char* data, int* Result)
{
	int		i = 0;
	char	mappedData[26];

	*Result = TRUE;

	switch (ID1)
	{
	case 2:	// H/W 메모리에서 가장 최근의 Mapping 결과값을 가져온다. 																				
		// Mapping Data Format = "Mxxxxxxxx,xxxxxxxx,xxxxxxxx"
		//                         Preset  , Cross  , Double
		sprintf(data, "%s", _mappingData);
		break;

	case 3:	//가장 마지막에 발생한 Error Code를 가져온다. (Dura Port Response Data : Exxx cccccccccccccccccccc) 
		strcpy(data, _errorCode);
		break;

	case 4:	//가장 마지막에 발생한 Error Message를 가져온다. (Dura Port Response Data : Exxx cccccccccccccccccccc) 																					
		strcpy(data, _errorMassage);
		break;

	default:
		*Result = FALSE;
		return;
	}
	if (!(*Result))
	{
		printf("[%s] [%8s] SI Error - ID1[%d]\n", GetTime(_time), _drvName, ID1);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void String_Out(int ID1, char* data, int* Result)
{
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
double Analog_Name_In(char* Name, int* Result)
{
	*Result = TRUE;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Analog_Name_Out(char* Name, double data, int* Result)
{
	*Result = TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int Digital_Name_In(char* Name, int* Result)
{
	*Result = TRUE;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void Digital_Name_Out(char* Name, int data, int* Result)
{
	*Result = TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void String_Name_In(char* Name, char* data, int* Result)
{
	*Result = TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void String_Name_Out(char* Name, char* data, int* Result)
{
	*Result = TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
