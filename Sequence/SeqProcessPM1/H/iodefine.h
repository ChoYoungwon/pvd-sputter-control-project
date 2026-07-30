#ifndef IODEFINE_H
#define IODEFINE_H


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum {
	IO_Driver_Loading,

	OBJ_Developer_Control,
	//	OBJ_Developer_Flow		,

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

	aiDevNoz_Cur_Pos,
	apDevNoz_Start_Pos,
	apDevNoz_Edge1_Pos,
	apDevNoz_Center_Pos,
	apDevNoz_Edge2_Pos,
	apDevNoz_End_Pos,


	dvPM_Wafer_Status,
	dvPM_Wafer_Source,
	dvPM_Wafer_Result,

} IOPointIndex;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef struct {

	char	STEP_NAME[256];
	int		STEP_TIME;

	double	DEVELOP_FLOW;
	double	DEVELOP_SPD;
	int		NOZ_MOVE;
	int		NOZ_POS;
	BOOL USE;

} RecipeStepTemplateM;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#endif
