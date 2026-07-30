#ifndef PARAMETER_H
#define PARAMETER_H

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define     ALARM_NOT_HOME                  gnALARM_START_INDEX + 1
#define		ALARM_DOUBLE_WAFER				gnALARM_START_INDEX + 2
#define		ALARM_CROSS_WAFER				gnALARM_START_INDEX + 3
#define		ALARM_UNKNOWN_WAFER				gnALARM_START_INDEX + 4
#define		ALARM_HW_ERROR_NOTIFY			gnALARM_START_INDEX + 5
#define		ALARM_READ_COMM_ERROR			gnALARM_START_INDEX + 6
#define		ALARM_WRITE_COMM_ERROR			gnALARM_START_INDEX + 7
#define		ALARM_PORT_RUNNING				gnALARM_START_INDEX + 8
#define		ALARM_PORT_FOUP_ABSENT			gnALARM_START_INDEX + 9
//-------------------------------------------------------------------------------------------------
#define		ALARM_PORT_INIT_TIMEOUT			gnALARM_START_INDEX + 10
#define		ALARM_PORT_CLAMP_TIMEOUT		gnALARM_START_INDEX + 11
#define		ALARM_PORT_UNCLAMP_TIMEOUT		gnALARM_START_INDEX + 12
#define		ALARM_PORT_OPEN_TIMEOUT			gnALARM_START_INDEX + 13
#define		ALARM_PORT_CLOSE_TIMEOUT		gnALARM_START_INDEX + 14
#define		ALARM_PORT_LOAD_TIMEOUT			gnALARM_START_INDEX + 15
#define		ALARM_PORT_UNLOAD_TIMEOUT		gnALARM_START_INDEX + 16
#define		ALARM_PORT_MAPPING_TIMEOUT		gnALARM_START_INDEX + 17
//-------------------------------------------------------------------------------------------------
#define		ALARM_PORT_CLAMP_FAIL			gnALARM_START_INDEX + 18
#define		ALARM_PORT_UNCLAMP_FAIL			gnALARM_START_INDEX + 19
#define		ALARM_PORT_OPEN_FAIL			gnALARM_START_INDEX + 20
#define		ALARM_PORT_CLOSE_FAIL			gnALARM_START_INDEX + 21
#define		ALARM_PORT_LOAD_FAIL			gnALARM_START_INDEX + 22
#define		ALARM_PORT_UNLOAD_FAIL			gnALARM_START_INDEX + 23
#define		ALARM_PORT_MAPDATA_COMP_ERROR	gnALARM_START_INDEX + 24
#define		ALARM_ROBOT_NOT_RETRACTED		gnALARM_START_INDEX + 25
#define		ALARM_PORT_ALARM_CLEAR_FAIL		gnALARM_START_INDEX + 26
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define		RUN						0
#define		MAX_WAFER_SLOT			25

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum { OFF, ON } IOCON;
typedef enum { LAMPOFF, LAMPON, LAMPBLINK } LAMPCON;
typedef enum { PARMON, PARMOFF } PARAMETERONOFF;
typedef enum { UNCLAMP, CLAMP, UnknownCLAMP } CLAMPSTS;
typedef enum { Extended, Retracted } ROBOTARMSTS;
typedef enum { NHome , Home , Other	} HOMESTS;
typedef enum { RunBUSY, RunREADY, RunERROR } LPRUNSTS;
typedef enum { CLOSED, OPENED, UnknownDOORSTS } DOORSTS;
typedef enum { UNDOCKED	, UNKNOWNPS	, DOCKED } SHUTTLEPOS;
typedef enum { WUnknown, WAbsent, WPresent, WProcess, WFailure, WDOUBLE, WCROSS } CTCWFRINFO;
typedef enum { MAPAbsent, MAPPresent, MAPDouble, MAPCross, MAPUnknown } LPMAPINFO;
typedef enum { LPRUN_ABORT, LPRUN_RETRY, LPRUN_SUCCESS } LPRUNRESULT;
typedef enum { ActionComplete, ActionRunning } ACTRUNSTS;
typedef enum { InitMode, ClampMode, UnclampMode, LoadMode, UnloadMode, TCloseMode, TOpenMode } CONTROLMODE;
typedef enum { LPNot_Reserved, LPReserved } LPRESERVE;
typedef enum { AMHS_Manual,	AMHS_Auto } AMHSMODE;
typedef enum { LP_Manual, LP_Auto } LPMODE;
typedef enum { UnknownSts, LoadedSts, UnloadedSts } LPCTRL;

typedef enum 
{ 
	cIdle			,
	cRunning		,
	cPausing		,
	cPaused			,
	cAborting		,
	cDisable		,
	cWaiting		,
	cWait_HandOff	,

} CTCMAINCONTROL; 
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum
{ 
	sIdle			,	// 0
	sLoading		,	// 1
	sLoaded			,	// 2	
	sLoadFail		,	// 3
	sMapping		,	// 4
	sMapped			,	// 5
	sMapFail		,	// 6
	sStart			,	// 7					
	sHandOffIn		,	// 8
	sBegin			,	// 9
	sMapIn			,	// 10
	sWaiting		,	// 11
	sRunning		,	// 12
	sMapOut			,	// 13
	sHandOffOut		,	// 14
	sComplete		,	// 15
	sCancel			,	// 16
	sAborted		,	// 17
	sUnloading		,	// 18
	sUnloading_C	,	// 19
	sUnloaded		,	// 20
	sUnloadFail		,	// 21
	sUnloaded_C		,	// 22

} CTCMAINTSTATUS;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum	{	eOFF_0		,	eON_1	};
enum	{	eON_0		,	eOFF_1	};
enum	{	eNOTOK_0	,	eOK_1	};
enum	{	eUnload_0	,	eLoad_1	};
enum	{	eDISABLE_0	,	eENABLE_1	};

#endif