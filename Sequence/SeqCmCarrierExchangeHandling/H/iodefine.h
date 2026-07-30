#ifndef IODEFINE_H
#define IODEFINE_H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum {
	IO_DRIVER_LOADING				,

	SCHEDULER						,

	CM_MID_CONTROL					,

	dvLOAD_CM_Signal				,
	dvULOAD_CM_Signal				,
	dvAMHS_CM_Moving				,
	dvPRCS_CM_End					,
	dvOPER_CM_Cancel				,
	CTC_FA_STATUS_MDL_CM			,
	Unload_Clamp_Mode				,

	dvAMHS_CM_HostAvailable			,
	dvAMHS_CM_EStop					,
	dvAMHS_CM_L_Request				,
	dvAMHS_CM_U_Request				,
	dvAMHS_CM_Ready					,

	dvAMHS_CM_CS0					,
	dvAMHS_CM_CS1					,
	dvAMHS_CM_Valid					,
	dvAMHS_CM_TR_Request			,
	dvAMHS_CM_Busy					,
	dvAMHS_CM_Complete				,
	dvAMHS_CM_Cont					,

	CTC_AMHS_TP1					,
	CTC_AMHS_TP2					,
	CTC_AMHS_TP3					,
	CTC_AMHS_TP4					,
	CTC_AMHS_TP5					,
	CTC_AMHS_TP6					,

	diLP_CM_Present					,
	diLP_CM_Placed					,

	doLP_CM_UnClamp					,
	doLP_CM_Clamp					,

	diLP_Clamp_Status				,
//	UnClamp_Status					,

	Load_Button						,
	Unload_Button					,

	doLP_Load_Lamp					,
	doLP_Unload_Lamp				,

	CM_EVENT_SERVICE				,

	CARRIER_ID						,
	CARRIER_ID2						,
	CARRIER_ID3						,
	CARRIER_ID4						,

	CARREAD_ID						,
	CARREAD_ID2						,
	CARREAD_ID3						,
	CARREAD_ID4						,

	JOBNAME_ID						,
	JOBNAME_ID2						,
	JOBNAME_ID3						,
	JOBNAME_ID4						,

	JOBREAD_ID						,
	JOBREAD_ID2						,
	JOBREAD_ID3						,
	JOBREAD_ID4						,

	RECIPE_ID						,
	RECIPE_ID2						,
	RECIPE_ID3						,
	RECIPE_ID4						,

	LOTNAME_ID						,
	LOTNAME_ID2						,
	LOTNAME_ID3						,
	LOTNAME_ID4						,

	//TM_Light_Curtain				,

} IOPointIndex;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif
