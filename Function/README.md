## Function
FuncDef.dat -> Sequnece/F_SEQ.dat 파일을 연결하였습니다.
```
# FuncDef.dat 파일의 내용 중 일부
$$$ F	Sequence/F_SEQ.dat
```

### 1. Function Option  
Function들은 메모리에 Loading되어 Initial Part만 수행되고, Wait 상태에 들어가게 된다. Run Message가 올 때 까지 기다리게 됩니다.
- PROGRAM_ARGUMENT : 각 Function이 메모리에 로딩될 때 부터 전달되는 Data(로딩 타임)
- PROGRAM_PARAMETER : Run_Message에 의해 동작되었을 때 전달되는 Data(러닝 타임)

#### 1.1. Loading Part
- **Program Enter_Code()**
  - Function이 Loading 되면 수행됩니다.
  - IO Table에 사용자가 정해 놓은 IO의 내용을 Main Engine으로 부터 확인 후 사용가능 여부를 통보 받습니다.
- **Program_Init_Code()**
  - IO 정보의 확인 후 수행되게 되며 이 Procedure의 결과가 FALSE일 때는 이 Function은 Loading에 실패하게 됩니다.
  - Program_Enter_Code() 보다 더 큰 범위로 초기화가 필요할 경우 사용 되어지기 위해 존재한다.
#### 1.2. Running Part
- Server 역할을 하며 상위 Level의 Function으로 부터 Message를 기다립니다.
- **Run Message** : **Program_Main()** Procedure가 수행됩니다.
- **Event Message** : **Event_Message_Received()** Procedure가 수행됩니다.
#### 1.3. Function Read
- **PROGRAM_ARGUMENT_READ**
  - Loading Part에서 Module load시 configuration file에서 주어진 **Argument를** 읽을 때 사용합니다.
- **PROGRAM_PARAMETER_READ**
  - Running Part에서 상위에서 내려온 **Run Message를** 읽을 때 사용합니다.
- **PROGRAM_EVENT_READ**
  - Running Part에서 상위에서 내려온 **Event Message를** 읽을 때 사용합니다.

### 2. Function 구현 사항
#### 2.1. Main 화면 000 Mode 부분 클릭시
GUI 클릭  
-> SCHEDULER_MAINT_000_ALL (F_SEQ.dat)  
-> SeqMaintModule/Sequence.exe에 TM 또는 PM1 또는 PM2 인자 전달   
-> GO_INT 또는 GO_STANDBY 또는 GO_MAINT 또는 "" 메시지 전달  
-> Sequence/Interface_Maint_00.cfg에 메시지 전달   
-> **SEQ_ROBOT_TRANSFER** 또는 **SEQ_PM1_DEVELOPER_CONTROL** 또는 **SEQ_PM2_CHAMBER_CONTROL**에 메시지를 전달합니다.  

#### 2.2. 로봇팔 Function
GUI 클릭   
-> SCHEDULER_MAINT_TM_ALL(F_SEQ.dat)에 메시지 전달(ex "MANUAL_MOVE")  
-> SeqMaintModule/Sequence.exe에 TM인자 전달   
-> "PICK_TM|1|sationname|A|1|1"이 스케쥴러에 전달된다.  
-> SCHEDULER_MAINT_TM 에 메시지 전달  
-> Sequence/Interface_Maint_TM.cfg에 메시지 전달  
-> **SEQ_ROBOT_TRANSFER** 에 메시지 전달  
	("MANUAL_MOVE CTC.TR_SRC_STATION CTC.TR_SRC_SLOT CTC.TR_TRG_STATION	CTC.TR_TRG_SLOT")  
-> **SEQ_ROBOT_MOTOR**에서 동작 수행  

#### 2.3. PM1, PM2 Function
GUI 클릭
-> SCHEDULER_MAINT_PM00에 메시지 전송(ex "PUMP_DOWN")  
-> Sequence/Interface_Maint_TM.cfg에 메시지 전달  
-> **SEQ_PM1_DEVELOPER_CONTROL** or **SEQ_PM2_CHAMBER_CONTROL**에 메시지를 전달합니다.  
-> 동작을 수행합니다.  