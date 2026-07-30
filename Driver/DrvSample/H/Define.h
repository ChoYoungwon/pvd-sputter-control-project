#ifndef __Define__H__
#define __Define__H__


//----------------------------------------------------------------------------------
#define		CR						   	0x0d
#define		LF							0x0a	
#define		ERROR_MESSAGE				"E"
#define		ACK_MESSAGE					"ACK"
#define		NAK_MESSAGE					"NAK"
#define		INTERLOCK_MESSAGE			"NAK:CLOAD/INTER/"
#define		EVENT_MESSAGE_TYPE1			"INF"               // Event Message Type1
#define		EVENT_MESSAGE_TYPE2			"ABS"               // Event Message Type2 
#define		STATUS_MESSAGE				"ACK:STATE/"
#define		STATUS_INDICATOR_MESSAGE	"I"					// Ixxxxxxx  Indicator Status 
#define		COMMAND_COMPLETE_MESSAGE	"O"                 
#define		MAPPINGD_DATA_MESSAGE		"ACK:MAPRD/"        // From Down Slot
#define		MAPPINGT_DATA_MESSAGE		"ACK:MAPDT/"		// From Top Slot
#define		DEFAULT_LENGTH				256
//----------------------------------------------------------------------------------

//------- Command ------------------------------------------------------------------
#define		GETSTATE					"GET:STATE"
#define		HOMING						"MOV:ORGSH"
#define		GETTEMP						"GET:TEMP"
#define		ROTATIONFAN					"DOUT:0"
#define		DIRENTIONFAN				"DOUT:1"
#define		FANSPEED					"DOUT:2"
//----------------------------------------------------------------------------------

//------- Interlock Code -----------------------------------------------------------
#define		INTL_NOT_AVAILABLE			"INTER/CBUSY"
#define		INTL_NOT_FOUP_MOUNT			"INTER/FPILG"
#define		INTL_NOT_HOMED				"INTER/ORGYT"
#define		INTL_NOT_LOAD_COMP			"INTER/CLOAD"
#define		INTL_NOT_CLAMP_FAULT		"INTER/FPCLP"
#define		INTL_NOT_DOCK_COMP			"INTER/YPOSI"
#define		INTL_NOT_DOOR_VAC_FAULT		"INTER/DVACM"
#define		INTL_NOT_LATCH_FAULT		"INTER/LATCH"
#define		INTL_NOT_DOOR_CTRL_FAULT	"INTER/DPOSI"
#define		INTL_NOT_MAP_MOTOR_FAULT	"INTER/MPARM"
#define		INTL_NOT_ZAXIS_POS_FAULT	"INTER/ZPOSI"
#define		INTL_NOT_LOAD_LATCH_FAULT	"INTER/CLDDK"
//----------------------------------------------------------------------------------

//---------Mapping Data---------------------------------------------------------------------
#define		WAFER_ABSENT		0
#define		WAFER_PRESENT		1
#define		WAFER_DOUBLE		2
#define		WAFER_CROSS			3
#define		WAFER_UNKNOWN		4
#define		WAFER_DIFTHICK		5
//------------------------------------------------------------------------------------------

//--------- Timeout ---------------------------------------------------------------------
#define		TIMEOUT1			500		// 0.5 sec.
#define		TIMEOUT2			10000	// 10 sec.
//------------------------------------------------------------------------------------------

//------------Log---------------------------------------------------------------------------
// #define		LOG_INFORM				"[INF] "
// #define		LOG_ERROR				"[ERROR]"
// #define		LOG_WARNING				"[WARNING]"
// #define		LOG_FOLD				"History/%04d%02d%02d/"						
// #define		LOG_FILE				"%s%04d%02d%02d-LoadPort%d(Cymechs).log"	
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
typedef enum  { Unclamp			, Clamp			, ClmapUnknown		}EClampStatus;
typedef enum  { HomeNotComplete , HomeComplete	, Other				}EHomeCompleteStatus;
typedef enum  { Stop, Busy } EActionStatus;
typedef enum  { MotorOff		, MotorOn							}EMotorStatus;
typedef enum  { DoorClose		, DoorOpen		, DoorUnknown		}EDoorStatus;
typedef enum  { UnitStatusBusy	, UnitStatusIdle, UnitStatusError	}EUnitStatus;
typedef enum  { MaintDisable	, MaintEnable						}EMaintStatus;
typedef enum  { Undocking		, DockingUnknown, Docking			}EDockingStatus;
typedef enum  { MappingDisable	, MappingEnable						}EMappingModeStatus;
typedef enum  { SwitchDisable	, SwitchEnable						}ESwitchModeStatus;
typedef enum  { PlacementOff	, PlacementOn						}EPlacementStatus;
typedef enum  { PresenceOff		, PresenceOn						}EPresenceStatus;
typedef enum  { PosUnknown, PosDown, PosUp }DoorPosition;
typedef enum  { LampOff			, LampOn		, LampBlink			}ELampStatus;
typedef enum  { LogOff			, LogOn								}ELogMode;
typedef	enum  { LogCommunication, LogInform		, LogError			}ELogMethod;
typedef	enum  { ManualMode		, AutoMode		, MaintenanceMode	}EPortMode;
typedef	enum  { NoVac		, Vac		}EVacStatus;
typedef	enum  { NoLatch		, Latch		}ELatchStatus;
typedef	enum  { NoUnLatch	, UnLatch   }EUnLatchStatus;
typedef enum  { ACKMSG, NAKMSG, ERRORMSG, COMPLETEMSG, DATAMSG, EVTMSG, TIMEOUT } RESPONSETYPE;
typedef enum  { Complete, Running } CMDCOMPLETE;
typedef enum { AlarmOff, AlarmOn } EAlarmStatus;
typedef	enum { Off, On }EOffOn;

//-------------------------------------------------------------------------------------------
int					_commlogFileIndex;										//Log File Index
//-------------------------------------------------------------------------------------------
int					_ackTimeOut;													//Ack Timeout(Serial Communication)
char					_logFileName[DEFAULT_LENGTH];					//Log File Name for Control
char					_commLogFileName[DEFAULT_LENGTH];			//Log File Name for Control
char					_mappingData[DEFAULT_LENGTH];					//Mapping Data
char					_errorMassage[DEFAULT_LENGTH];					//Error Message
char					_errorCode[DEFAULT_LENGTH];						//Error Code

BOOL									_commStatus;								//Communication Fail or Serial Data Abnormal
CMDCOMPLETE					_runFlag;										//Command Complete Status

EHomeCompleteStatus		_homeComplete;						//Home Complete
EDoorStatus							_doorStatus;								//Door Status
EActionStatus						_isBusy;											//Action Status
BOOL									_isMaintMode;								//Maint. Mode Condition
ELatchStatus					    _latchStatus;	                            //Latch Status
EUnLatchStatus					_unLatchStatus;                           //UnLatch Status
EClampStatus						_clampStatus;								//Clamp Status
EDockingStatus					_dockingStatus;							//Docking Status
EVacStatus							_vacStatus;                                  //Vacuum Status
BOOL									_isAutoMode;								//Auto Mode Condition
EMappingModeStatus			_mappingFunctionMode;			//Mapping Function Enable/Disable
ESwitchModeStatus				_loadUnloadSwitchMode;			//Load/Unload Switch Mode
DoorPosition						_doorPosition;								//Door Position
EPlacementStatus				_placementStatus;						//Placement Sensor Status
EPresenceStatus					_presenceStatus;							//Presence Sensor Status
EAlarmStatus						_alarmStatus;								//Alarm Status

EMotorStatus						_motorDriverStatus ;					//Motor Driver Status 0:OFF 1:ON
EUnitStatus							_loadPortUnitStatus ;					//Load Port Status
EMaintStatus						_maintenanceMode ;					//Maintenance Mode Status

 
ELampStatus						_autoLampStatus;						//Auto Lamp Status
ELampStatus						_manualLampStatus;					//Manual Lamp Status
ELampStatus						_loadLampStatus;						//Load Lamp Status
ELampStatus						_unloadLampStatus;					//Unload Lamp Status
ELampStatus						_reserveLampStatus;					//Reserve Lamp Status
ELampStatus						_buttonLampStatus;					//Button Lamp Status

ELogMode							_errLog;										//Error Status Log
ELogMode							_commLog;									//Communication Log
ELogMode							_commLogPrint;							//Communication Log Console Print
ELogMode							_errConsolePrint;							//Error Status Print

EPortMode							_loadPortMode;							// Load Port Mode
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// #define		            Load_Status					"C00000004"
// #define	              	Unload_Status				"C00000008"
// #define                  DATA_LEN                     1024
//-------------------------------------------------------------------------------------------

typedef	enum  { None, Push } EButtonMode;
typedef enum  { Disconnect,	Connect	}ETPConnectStatus;
typedef enum  { Release, EmergencyStop	}EEStopStatus;

EButtonMode						_loadButton;								//Operation Access Button - Load Button Push or None
EButtonMode						_unloadButton;							//Operation Access Button - Unload Button Push or None
ETPConnectStatus				_tpConnectStatus;						// Pendant Connect Status
EEStopStatus						_eStopStatus;								// Emergency Stop Status

HANDLE                  _hThread;

enum	{	eSEND_0 , eRECV_1 , eERROR_2, eTIMEOUT_3, eOTHER_4	};

#endif