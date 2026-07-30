#ifndef IODEFINE_H
#define IODEFINE_H


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum {
	IO_Driver_Loading,

	SeqSputterDeviceControl,
	//	OBJ_Developer_Flow		,
	SeqPM2ChamberControl,

	dvRCP_MaxStep,
	dvRCP_StepNumber,

	dvRCP_Process_DataLog,
	dvRCP_Process_Status,
	dvRCP_Process_Control,
	dvRCP_Process_Run,

	avRCP_Total_RunTime,
	avRCP_Total_SetTime,
	avRCP_Step_RunTime,
	avRCP_Step_SetTime,

	avRCP_Progress,

	svRCP_Name,
	svRCP_StepName,
	svRCP_Process_Message,

	PROCESS_LOG,

	dvPM_Wafer_Status,
	dvPM_Wafer_Source,
	dvPM_Wafer_Result,

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

	avDcTargetPowerSP,

} IOPointIndex;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef struct {

	int		STEP_USE;
	int		AR_FLOW;

	double	PROC_PRESS;

	int		DC_POWER;
	int		RAMP_UP_TIME;


	int		PROC_TIME;
	int		RAMP_DOWN_TIME;

} RecipeStepTemplateM;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#endif
