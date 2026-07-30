#ifndef IODEFINE_H
#define IODEFINE_H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
typedef enum
{
	IO_Driver_Loding				,

	SCHEDULER						,

	AMHS_STATUS						,
	MAIN_CONTROL					,
	MAIN_STATUS						,
	RESERVE_STATUS					,
	PORT_SERVICE					,

	MappingData						,
	LastErrorMessage				,
	LastErrorNo						,

	Comm_Status						,
	Ready_Status					,
	Home_Status						,
	Shuttle_Status					,
	Present							,
	Placed							,
	Door_Status						,
	Clamp_Status					,
	Run_Status						,

	WFR_INF01						,
	WFR_INF02						,
	WFR_INF03						,
	WFR_INF04						,
	WFR_INF05						,
	WFR_INF06						,
	WFR_INF07						,
	WFR_INF08						,
	WFR_INF09						,
	WFR_INF10						,
	WFR_INF11						,
	WFR_INF12						,
	WFR_INF13						,
	WFR_INF14						,
	WFR_INF15						,
	WFR_INF16						,
	WFR_INF17						,
	WFR_INF18						,
	WFR_INF19						,
	WFR_INF20						,
	WFR_INF21						,
	WFR_INF22						,
	WFR_INF23						,
	WFR_INF24						,
	WFR_INF25						,
	WFR_INF26						,

	Docking							,
	AlarmReset						,
	Homing							,
	Load							,
	Unload							,
	Clamp							,
	UnClamp							,
	Door_Ctrl						,

	Scan_Down						,
	TOPEN							,
	TCLOSE							,
	UPMappingEnable					,
	AUTO_LAMP						,
	MANUAL_LAMP						,
	LOAD_LAMP						,
	UNLOAD_LAMP						,
	RESERVE_LAMP					,
	OpMode							,
	LastAlarmFind					,

	C01_Wafer						,
	C02_Wafer						,
	C03_Wafer						,
	C04_Wafer						,
	C05_Wafer						,
	C06_Wafer						,
	C07_Wafer						,
	C08_Wafer						,
	C09_Wafer						,
	C10_Wafer						,
	C11_Wafer						,
	C12_Wafer						,
	C13_Wafer						,
	C14_Wafer						,
	C15_Wafer						,
	C16_Wafer						,
	C17_Wafer						,
	C18_Wafer						,
	C19_Wafer						,
	C20_Wafer						,
	C21_Wafer						,
	C22_Wafer						,
	C23_Wafer						,
	C24_Wafer						,
	C25_Wafer						,
	C26_Wafer						,
	MAPWAFER_COUNT					,
	MAPWAFER_INFO					,

	LP_Control						,
	Unload_Clamp_Mode				,
	Unload_Upmapping				,
	Move_Timeout					,

} IOPointIndex;
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif

