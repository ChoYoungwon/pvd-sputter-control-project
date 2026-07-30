#ifndef IODEFINE_H
#define IODEFINE_H

enum {
	eROBOT_SYNCH_READY_0,
	eROBOT_SYNCH_ROTATE_START_1,
	eROBOT_SYNCH_EXTEND_START_2,
	eROBOT_SYNCH_UPDATE_3,
	eROBOT_SYNCH_RETRACT_START_4,
	eROBOT_SYNCH_RESET_REVERSE_5,
	eROBOT_SYNCH_RESET_ONLY_6,
	eROBOT_SYNCH_SUCCESS_7
};
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum {
	IO_Driver_Loading,
	SCHEDULER,
	SeqRobotMotor,
	SeqRobotSimulation,
	SeqPM1DeveloperControl,
	SeqPM2ChamberControl,

	CM1Placed,
	LowerArmWaferStatus,
	UpperArmWaferStatus,
	CM1_C01_Wafer,
	PM1_Wafer_Status,
	PM2_Wafer_Status,

	TM_RB_SYNCH,

	HandShakeMode,
	UpperSendAble,
	UpperSendStart,
	UpperSendComplete,
	LowerRecvAble,
	LowerRecvStart,
	LowerRecvComplete,
	LowerRecvStart_PM1,
	LowerRecvComplete_PM1,
	UpperSendAble_PM1,
	UpperSendStart_PM1,

	LowerRecvStart_PM2,
	LowerRecvComplete_PM2,
	UpperSendAble_PM2,
	UpperSendStart_PM2,

} IOPointIndex;

#endif
