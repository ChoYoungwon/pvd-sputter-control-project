$$$ Enum

NotATMATM  	NotATM		ATM
CloseOpen  	Close		Open
OffOn      	Off		On
OnOff     	On		Off
NotOkOK  	NotOK		OK
OffOnBlink	Off		On		Blink

RETEX		Retract		Extend
DownUp		Down		Up
OpenClose	Open		Close

DisableEnable	Disable		Enable
EnableDisable	Enable		Disable

VacSta		Vacuum		Atm		IntVA		IntAV
VacStatus1    	Idle 	 	GoATM		GoVacuum	Abort
VacStatus2    	Vacuum 		ATM  		GoATM  		GoVacuum	Abort

ServiceMode	InService	OutofService
MID_RD_Mode	0		1		2		3		4		5		6		7		8		9
Clmp_Mode	UnClamp		Clamp

HRFCommStatus	Offline		Online															
HRFErrStatus	None		AutoFail	ExFail		WriteFail	NoTag		Invalid		UnKnown		UnConfig	Check		VoidAckn	Locked		MsgLen		Invalid2	NoAckn			
HRFPageCtrl	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17
HRFPageRead	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17
HRFRunIntlk	Run			

VAL			X_VAL	Z1_VAL	Z2_VAL		GATE_VAL_LL	GATE_VAL_PM1		GATE_VAL_PM2		GATE_VAL_PM3

HomeMode	+Limit		-Limit		+Near		-Near		-Z		+Z

JogPos			CW		CCW

eHoming			Homing	None		
eBusy			Idle		Busy
eErr				None		Error

NotUseUse	NotUse	Use
NoneRun		None Run
StartStop	Start Stop
StopStart	Stop Start

UseNotUse	Use NotUse
RunStop	Run	Stop
OffOn2		OFF	ON	ON2
IdleRun		Idle Run
OffOnline	Offline Online
ReadyRun	Ready Run
	
#MTRControl	Off   On    Hold   Restart	Start  Skip	Zero
#MTRStatus	None  PwrOn CurrLimit Running Hold Complete  Near  InPos
MTRAlarm	Alm_None  Alm_Servo  Alm_NS600  Alm_POT  Alm_NOT  Alm_PLS Alm_NLS  Alm_SVOFF  Alm_Hold  Alm_Inpos  Alm_Near  Alm_SVON  Alm_Etc
MTRComplete	Ready	Complete
MTRStop		Hold	Skip
#TBLStop	Reset	Stop
#TBLStatus	Other	Stop	
	
PrcsLastCtl		Idle		Pause	Resume	Stop		Abort
PrcsStatus		Idle		Running
PrcsStartEnd	End		Start
WfrReslt		Present	Success	Failure	Processingu
ControlSts		Unuse	UnInit	Go-Init	Maint		Go-Standby Standby Go-Maint CtcInUse CtcMaint RunProcess RunMaint OffLine
VACSTS		    ATM     Vacuum 		 GoATM  		GoVacuum	Abort
	
AxisCnt	None	2axis	4axis	6axis	8axis
TypeDIO	None	16by16	32by32	48by48	64by64
TypeExDIO	None	In16	Out16
TypeMDIO	None	8by8



	
	
eOFFON		OFF	ON
ClOp		Close	Open
eStart		Start	Stop




CommSts2	NotUse 		GoInit		GoMaint		GoStandby	RunProcess	RunMaint


Type	SVL SV SL
UpDown	Up	Down


InMode	Auto Manual


DOOR_LOCK	LOCK		UNLOCK
ALMLEVEL		NONE		INFORM		WARNING		ALARM		ERROR		FATAL		EMS			6			7			8			9
STEPSTS		IDLE		ING			SUCCESS		FAIL
PMEQSTS		AUTO		MANUAL		GO_MANUAL
PMEQBTN		AUTO		MANUAL		DISABLE
PMEQSTD		NOTSTD	STANDBY		DISABLE
EQSTS		NONE		MAINT		MANUAL		AUTO
UNIT_ENABLE	ENABLE	DISABLE
ctcGlsReslt		PRESENT	SUCCESS		FAILURE		PROCESSING
MELSEC_OFFON	OFF		ON
O3STS		STOP		ING			RUN
HSMODE		MODE1	MODE2
PIOSTA		ON		OFF			TMO:4
UPDN2_OPCL		NONE0		NONE1		NONE2		ERROR3		NONE4		OPEN		NONE6		ERROR7		NONE8		NONE9		CLOSE		ERROR11		ERROR12		ERROR13		ERROR14		ERROR15	
HS_STS	NONE	RECV	SEND	RESET

LPCOMMSTS	Offline	Online
LPRUN	  	Run
LPMODE 		Manual		Auto
LPCLOP		Close	  	Open
LPCLOPED	Closed		Opened	  	Other
LPLOCK	  	Unlock		Lock
LPCLAMP		Release		Clamp	   	CloNoRel
LPOFFONBLK	Off    		On	     	Blink
LPDCKING	UnDocking	Docking
LPHOMESTS	NotHome		Home	  	Other
LPREADYSTS	Busy	 	Ready	  	Error
LPRUNSTS	Complete 	Running
LPMAPWFRINF 	Absent		Present		Double		Cross		Unknown
LPPODPS	  	Undocked	Unknown		Docked
LPPUSH		None		Push
LPTPCONNECT	Disconnect	Connect
LPESTOP		Release		EStop