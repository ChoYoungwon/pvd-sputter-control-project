#ifndef IODEFINE_H
#define IODEFINE_H


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum { eOFF_0, eON_1 } OnOFF;
enum { Retract, Extend };
enum { Idle, Moving };
enum { Down, Up };
enum { Homing, HomeComplete };

typedef enum {
	IO_Driver_Loading,
	RobotRetractExtend,
	RobotBusy,
	RobotExtendSensor,
	RobotRetractSensor,

	RobotZUpSen,
	RobotZDownSen,
	RobotZUp,

	RobotSetSpeed,
	RobotSetHomeSpeed,
	RobotStop,
	RobotHomeMode,
	RobotHomeStatus,
	RobotHomeStart,

	apRB_Start_Spd_Move,
	apRB_dACC_Move,
	apRB_dDec_Move,
	apRB_Drive_Spd_Move,

	apRB_Home_Spd0,
	apRB_Home_Spd1,
	apRB_Home_Spd2,
	apRB_Offset_Spd,

	apRbPos_Indexer,
	apRbPos_PM1,
	apRbPos_PM2,

	aoRB_Set_Start_Spd,
	aoRB_Set_Accel_Spd,
	aoRB_Set_Decel_Spd,
	aoRB_Set_Drv_Spd,

	aoRB_Home_Spd0,
	aoRB_Home_Spd1,
	aoRB_Home_Spd2,
	aoRB_Offset_Spd,

	aoRbAbsPos,
	aoRbSetPos,
	aoRbCurPos,
	aiRbCurrPos,

} IOPointIndex;


#endif
