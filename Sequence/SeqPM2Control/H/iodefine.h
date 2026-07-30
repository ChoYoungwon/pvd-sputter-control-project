#ifndef IODEFINE_H
#define IODEFINE_H
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
typedef enum {
	IO_Driver_Loding			,
	SeqSputterDeviceControl,

	HandShakeMode,
	UpperSendAble,
	UpperSendStart,
	UpperSendComplete,
	LowerRecvAble,
	LowerRecvStart,
	LowerRecvComplete,

	PM2GateCloseSensor,
	PM2GateOpenSensor,
	PM2GateControl,

	GaugeForelinePressure,
	avCvgGaugePressure,
	doPVDTurboReady,

	avIonGaugePressure,
	avPN2GasFlowPV,
	avTurboRPM,


	dvVacuumStatus,
	ControlStatus,

	doPVDVacuum_LED,
	doPVDIdle_LED,
	doPVDProcess_LED,

} IOPointIndex;
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#endif
