#ifndef IODEFINE_H
#define IODEFINE_H
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
typedef enum {
	IO_Driver_Loding,

	OBJ_DEVELOPER_FLOW,

	doDevNoz_HomeMove,
	doDevNoz_Current,
	doDevNoz_Stop,
	doDevNoz_Set_Spd,
	doDevNoz_Set_HomeSpd,

	diDevNoz_Homing_Sts,
	diDevNoz_Busy_Sts,
	diDevNoz_HomeSen,

	dpDevNoz_HomeMode,

	aoDevNoz_Set_Pos,
	aoDevNoz_Cur_Pos,
	aoDevNoz_Abs_Pos,
	aoDevNoz_Rel_Pos,
	aoDevNoz_Reset_Pos,

	aiDevNoz_Cur_Pos,
	aiDevNoz_Cmd_Pos,

	aoDevNoz_Set_Start_Spd,
	aoDevNoz_Set_Acc_Spd,
	aoDevNoz_Set_Dec_Spd,
	aoDevNoz_Set_Drive_Spd,

	apDevNoz_Set_Start_Spd,
	apDevNoz_Set_Acc_Spd,
	apDevNoz_Set_Dec_Spd,
	apDevNoz_Set_Drive_Spd,

	aoDevNoz_Home_Spd0,
	aoDevNoz_Home_Spd1,
	aoDevNoz_Home_Spd2,
	aoDevNoz_Offset_Spd,

	apDevNoz_Home_Spd0,
	apDevNoz_Home_Spd1,
	apDevNoz_Home_Spd2,
	apDevNoz_Offset_Spd,

	apDevNoz_Start_Pos,
	apDevNoz_Edge1_Pos,
	apDevNoz_Center_Pos,
	apDevNoz_Edge2_Pos,
	apDevNoz_End_Pos,

	apDevNoz_Move_Timeout,
	apDevNoz_Swing_Timeout,

	HandShakeMode,
	UpperSendAble,
	UpperSendStart,
	UpperSendComplete,
	LowerRecvAble,
	LowerRecvStart,
	LowerRecvComplete,

	PM1GateCloseSensor,
	PM1GateOpenSensor,
	PM1GateControl,

} IOPointIndex;
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#endif
