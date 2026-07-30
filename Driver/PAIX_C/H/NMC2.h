/**
  **************************************************************************************************
  * @file       NMC2.h
  * @author     PAIX Lab.
  * @version
  * @date       2022-02-25
  * @brief      본 파일은 NMC2, NMC2E Controller를 사용하기 위한 함수파일입니다.
  * @note       C, C++
  * @copyright  2018. PAIX Co., Ltd. All rights reserved.
  * @see        http://www.paix.co.kr/
  **************************************************************************************************
  * @note       2018-11-22    update release
  *             2019-03-25    4축보간 추가
  *             2019-04-25    NMF 기능 추가
  *             2019-04-30    nmc_GetCmdEncListPos 함수 추가
  *             2019-05-23    NMF 기능 분리
  *             2019-06-05    nmc_EMpgCounterSet, nmc_EMpgCounterGet 함수 추가
  *             2019-07-15    nmc_VarAbsDecStop 함수 추가
  *             2019-08-23    NMC2E Near(Home)On시 감속정지 기능 업데이트
  *                           및 nmc_SetAlarmStopMode 함수 추가
  *             2019-11-21    nmc_SetPulseOutMask 함수 추가
  *             2020-06-19    nmc_SetBacklashSlip, nmc_SetInSigFilter 함수 추가
  *             2021-06-17    nmf_GetCompo 함수 추가
  *				2021-08-20	  nmc_ContiStop 함수 추가(2019-07-15 누락분)
  *				2021-10-28	  nmc_nmfGetCompo, _TNMC_NMF_COMPO 함수, 구조체 이름 변경
  *				2022-02-25	  nmc_SetTriggerGroupAssign, nmc_GetTriggerGroupAssign 함수 추가
  **************************************************************************************************
  */
#ifndef _NMC2_H_
#define _NMC2_H_
  //#include <windef.h>
#ifdef __cplusplus
extern "C" {
#endif

#define WINAPI __stdcall
	/**
	  * @brief  장치 정의값
	  */
#define NMC2_220S                           0
#define NMC2_420S                           1
#define NMC2_620S                           2
#define NMC2_820S                           3

#define NMC2_220_DIO32                      4
#define NMC2_220_DIO64                      5
#define NMC2_420_DIO32                      6
#define NMC2_420_DIO64                      7
#define NMC2_820_DIO32                      8
#define NMC2_820_DIO64                      9

#define NMC2_DIO32                          10
#define NMC2_DIO64                          11
#define NMC2_DIO96                          12
#define NMC2_DIO128                         13

#define NMC2_220                            14
#define NMC2_420                            15
#define NMC2_620                            16
#define NMC2_820                            17
#define NMC2_620_DIO32                      18
#define NMC2_620_DIO64                      19
#define NMC2_UDIO                           100

	  /**
		* @brief  NMC2 장치타입 정의
		*/
#define EQUIP_TYPE_NMC_2_AXIS               0x0001
#define EQUIP_TYPE_NMC_4_AXIS               0x0003
#define EQUIP_TYPE_NMC_6_AXIS               0x0007
#define EQUIP_TYPE_NMC_8_AXIS               0x000F

#define EQUIP_TYPE_NMC_IO_32                0x0010      /*!< IN 16, OUT 16 */
#define EQUIP_TYPE_NMC_IO_64                0x0030      /*!< IN 32, OUT 32 */
#define EQUIP_TYPE_NMC_IO_96                0x0070      /*!< IN 48, OUT 48 */
#define EQUIP_TYPE_NMC_IO_128               0x00F0      /*!< IN 64, OUT 64 */
#define EQUIP_TYPE_NMC_IO_160               0x01F0      /*!< IN 80, OUT 80 */
#define EQUIP_TYPE_NMC_IO_192               0x03F0      /*!< IN 96, OUT 96 */
#define EQUIP_TYPE_NMC_IO_224               0x07F0      /*!< IN 112, OUT 112 */
#define EQUIP_TYPE_NMC_IO_256               0x0FF0      /*!< IN 128, OUT 128 */

#define EQUIP_TYPE_NMC_IO_IE                0x1000
#define EQUIP_TYPE_NMC_IO_OE                0x2000

#define EQUIP_TYPE_NMC_M_IO_8               0x4000

		/**
		  * @brief  함수 반환값
		  */
#ifndef PAIX_RETURN_VALUE
#define PAIX_RETURN_VALUE
#define NMC_TIME_OUT                    -19     /*!< 이동이 개시되거나 정지할때 까지 대기시 Time Out발생 */
#define NMC_CANNOT_APPLY                -18     /*!< 현재 진행 중인 모션에서 지원하지 않는 명령어를 보냈을 경우 */
#define NMC_NO_CONSTSPDDRV              -17     /*!< 정속 구간이 아닌 가속,감속 중 명령어가 입력된 경우 */
#define NMC_SET_1ST_SPDPPS              -16     /*!< 속도 프로파일을 먼저 입력하십시오 */
#define NMC_CONTI_BUF_FULL              -15     /*!< 무제한 연속보간의 버퍼가 모두 채워진 상태 */
#define NMC_CONTI_BUF_EMPTY             -14     /*!< 무제한 연속보간의 버퍼에 데이터가 없는 상태 */
#define NMC_INTERPOLATION               -13     /*!< 연속(일반) 보간 구동 중에 동작 명령어가 입력된 경우 */
#define NMC_FILE_LOAD_FAIL              -12     /*!< F/W file 로드 실패 */
#define NMC_ICMP_LOAD_FAIL              -11     /*!< ICMP.dLL 로드 실패, nmc_PingCheck 호출시 발생. PC에 DLL유무를 확인 */
#define NMC_NOT_EXISTS                  -10     /*!< 네트워크 장치가 식별되지 않는 경우, 방화벽이나 연결 상태를 확인 */
#define NMC_CMDNO_ERROR                 -9      /*!< 함수 호출 시, 식별코드에 오류 발생 */
#define NMC_NOTRESPONSE                 -8      /*!< 함수 호출 시, 응답이 없는 경우 */
#define NMC_BUSY                        -7      /*!< 현재 축이 구동 중인 경우 */
#define NMC_COMMERR                     -6      /*!< 함수 호출 시, 통신 오류 발생 */
#define NMC_SYNTAXERR                   -5      /*!< 함수 호출 시, 구문 오류 발생 */
#define NMC_INVALID                     -4      /*!< 함수 인자값에 오류발생 */
#define NMC_UNKOWN                      -3      /*!< 지원되지 않는 함수 호출 */
#define NMC_SOCKINITERR                 -2      /*!< 소켓 초기화 실패 */
#define NMC_NOTCONNECT                  -1      /*!< 장치와 연결이 끊어진 경우 */
#define NMC_OK                          0       /*!< 정상 */
#endif

		  /**
			* @brief  BIT 연산
			*/
#ifndef _PAIX_BITOPERATION_
#define _PAIX_BITOPERATION_
#define BITSET(val,bit)                 (val) |= (1<<(bit))
#define BITCLEAR(val,bit)               (val) &= (~(1<<(bit)))
#define BITTOGGLE(val,bit)              (val) ^= (1<<(bit))
#define BITVALUE(val, sdata, bit)       (val) = (((sdata)>> bit) & 0x01)
#define GETBIT(bytes, bit_no)           (((bytes)>>(bit_no))&0x01)
#define SETBIT(bytes, bit_no, val)      { (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif

			/**
			  * @brief  장치의 정지상태
			  */
#ifndef _NMC_STOP_MODE_
#define _NMC_STOP_MODE_
#define NMC_STOP_OK                     0
#define NMC_STOP_EMG                    1
#define NMC_STOP_MLIMIT                 2
#define NMC_STOP_PLIMIT                 3
#define NMC_STOP_ALARM                  4
#define NMC_STOP_NEARORG                5
#define NMC_STOP_ENCZ                   6
#endif

			  /**
				* @brief  장치의 원점 구동 상태
				*/
#ifndef _NMC_HOME_MODE_
#define _NMC_HOME_MODE_
#define NMC_HOME_LIMIT_P                0
#define NMC_HOME_LIMIT_M                1
#define NMC_HOME_NEAR_P                 2
#define NMC_HOME_NEAR_M                 3
#define NMC_HOME_Z_P                    4
#define NMC_HOME_Z_M                    5

#define NMC_HOME_USE_Z                  0x80

#define NMC_END_NONE                    0x00
#define NMC_END_CMD_CLEAR_A_OFFSET      0x01
#define NMC_END_ENC_CLEAR_A_OFFSET      0x02
#define NMC_END_CMD_CLEAR_B_OFFSET      0x04
#define NMC_END_ENC_CLEAR_B_OFFSET      0x08
#endif

				/**
				  * @brief  장치의 상태(Log)
				  */
#define NMC_LOG_NONE                        0
#define NMC_LOG_DEV                         0x01
#define NMC_LOG_MO_MOV                      0x02    /*!< 모션함수중 MOVE */
#define NMC_LOG_MO_SET                      0x04    /*!< 모션함수중 GET */
#define NMC_LOG_MO_GET                      0x08    /*!< 모션함수중 SET */
#define NMC_LOG_MO_EXPRESS                  0x10    /*!< 모션함수중 각종 상태값 읽는(빈번히 발생) */
#define NMC_LOG_IO_SET                      0x20
#define NMC_LOG_IO_GET                      0x40

				  /**************************************************************************************************/
#pragma pack(1)     /*!< Specifies packing alignment for structure */

/**
  * @brief  장치 로직 값 정의
  * @note   장치에 입력되는 입력 신호의 위상 및 설정값입니다.
  */
	typedef struct NMC_PARA_LOGIC
	{
		short   nEmg;                   /*!< Emergency 입력 신호 */
		short   nEncCount;              /*!< 엔코더 카운트 모드 */
		short   nEncDir;                /*!< 엔코더 카운트 방향 */
		short   nEncZ;                  /*!< 엔코더 Z 입력 신호 */
		short   nNear;                  /*!< 원점센서 입력 신호 */
		short   nMLimit;                /*!< - Limit 입력 신호 */
		short   nPLimit;                /*!< + Limit 입력 신호 */
		short   nAlarm;                 /*!< 알람 입력 신호 */
		short   nInp;                   /*!< Inposition 입력 신호 */
		short   nSReady;                /*!< Servo Ready 입력 신호 */
		short   nPulseMode;             /*!< 펄스 출력 모드 */
		short   nLimitStopMode;         /*!< ±Limit 입력 시, 정지모드 */
		short   nBusyOff;               /*!< Busy off 출력 시점 */
		short   nSWEnable;              /*!< SW limit 활성화 여부 */
		double  dSWMLimitPos;           /*!< - SW Limit 위치 */
		double  dSWPLimitPos;           /*!< + SW Limit 위치 */
	}PARALOGIC, *PPARALOGIC;

	/**
	  * @brief  장치 로직 값 정의 확장
	  * @note   장치에 입력되는 입력 신호의 위상 및 설정값입니다.
	  */
	typedef struct NMC_PARA_LOGIC_EX
	{
		short   nEmg;                   /*!< Emergency 입력 신호 */
		short   nEncCount;              /*!< 엔코더 카운트 모드 */
		short   nEncDir;                /*!< 엔코더 카운트 방향 */
		short   nEncZ;                  /*!< 엔코더 Z 입력 신호 */
		short   nNear;                  /*!< 원점센서 입력 신호 */
		short   nMLimit;                /*!< - Limit 입력 신호 */
		short   nPLimit;                /*!< + Limit 입력 신호*/
		short   nAlarm;                 /*!< 알람 입력 신호 */
		short   nInp;                   /*!< Inposition 입력 신호 */
		short   nSReady;                /*!< Servo Ready 입력 신호 */
		short   nPulseMode;             /*!< 펄스 출력 모드 */

		short   nLimitStopMode;         /*!< ±Limit 입력 시, 정지모드 */
		short   nBusyOff;               /*!< Busy off 출력 시점 */
		short   nSWEnable;              /*!< SW limit 활성화 여부 */
		double  dSWMLimitPos;           /*!< - SW Limit 위치 */
		double  dSWPLimitPos;           /*!< + SW Limit 위치 */

		/* 원점 완료상태 해지 사용여부 */
		short   nHDoneCancelAlarm;      /*!< Alarm 발생 시 사용여부 */
		short   nHDoneCancelServoOff;   /*!< Servo Off 시 사용여부 */
		short   nHDoneCancelCurrentOff; /*!< Current Off 시 사용여부 */
		short   nHDoneCancelServoReady; /*!< Servo Ready 해제 시 사용여부 */

		short   nDummy[10];             /*!< 예약 공간 */
	}PARALOGICEX, *PPARALOGICEX;

	/**
	  * @brief  각 축별로 설정된 속도 정보
	  */
	typedef struct NMC_PARA_SPEED
	{
		double  dStart;                 /*!< 시작속도 */
		double  dAcc;                   /*!< 가속도 */
		double  dDec;                   /*!< 감속도 */
		double  dDrive;                 /*!< 구동속도 */
		double  dJerkAcc;               /*!< 가속 Jerk, S-Curve 구동시 사용 */
		double  dJerkDec;               /*!< 감속 Jerk, S-Curve 구동시 사용 */
	}PARASPEED, *PPARASPEED;

	/**
	  * @brief  8축 모션전용 출력신호
	  */
	typedef struct NMC_AXES_MOTION_OUT
	{
		short   nCurrentOn[8];          /*!< 모터 전류 출력 상태(0=Off, 1=On) */
		short   nServoOn[8];            /*!< Servo On 출력 상태(0=Off, 1=On) */
		short   nDCCOn[8];              /*!< DCC 출력 상태(0=Off, 1=On) */
		short   nAlarmResetOn[8];       /*!< Alarm Reset 출력 상태(0=Off, 1=On) */
	}NMCAXESMOTIONOUT, *PNMCAXESMOTIONOUT;

	/**
	  * @brief  8축의 현재 센서입력, 위치, 보간 등의 상태정보
	  */
	typedef struct NMC_AXES_EXPR
	{
		short   nBusy[8];               /*!< 펄스 출력 상태(0=Idle, 1=Busy) */
		short   nError[8];              /*!< Error 발생 여부(0=None, 1=Error) */
		short   nNear[8];               /*!< 원점 센서 입력 상태(0=Off, 1=On) */
		short   nPLimit[8];             /*!< + Limit 센서 입력 상태(0=Off, 1=On) */
		short   nMLimit[8];             /*!< - Limit 센서 입력 상태(0=Off, 1=On) */
		short   nAlarm[8];              /*!< 알람 센서 입력 상태(0=Off, 1=On) */
		short   nEmer[2];               /*!< 그룹별 EMG 입력 상태(0=Off, 1=On) */
		short   nSwPLimit[8];           /*!< SW +Limit 입력 상태(0=Off, 1=On) */
		short   nInpo[8];               /*!< Inposition 입력 상태(0=Off, 1=On) */
		short   nHome[8];               /*!< Home Search 동작 상태(0=동작중, 1=None) */
		short   nEncZ[8];               /*!< 엔코더 Z상 입력 상태(0=Off, 1=On) */
		short   nOrg[8];                /*!< 원점 센서 입력 상태(0=Off, 1=On)(NMC-403S 에서만 지원) */
		short   nSReady[8];             /*!< Servo Ready 입력 상태(0=Off, 1=On) */
		short   nContStatus[2];         /*!< 연속보간 실행 상태(0=완료, 1=동작중) */
		short   nDummy[6];              /*!< 예약 공간 */
		short   nSwMLimit[8];           /*!< SW -Limit 입력 상태(0=Off, 1=On) */
		int     lEnc[8];                /*!< 엔코더 위치(UnitPerPulse 적용되지 않음) */
		int     lCmd[8];                /*!< 지령 위치(UnitPerPulse 적용되지 않음) */
		double  dEnc[8];                /*!< 엔코더 위치(UnitPerPulse 적용) */
		double  dCmd[8];                /*!< 지령 위치(UnitPerPulse 적용) */
		char    dummy[4];               /*!< 예약 공간 */
	}NMCAXESEXPR, *PNMCAXESEXPR;

	/**
	  * @brief  장치리스트 구조체
	  */
	typedef struct TNMC_EQUIP_LIST
	{
		/*  192.168.0.11
			BIT0~BIT7  : 192
			BIT8~BIT15  : 168
			BIT16~BIT23  : 0
			BIT24~BIT31  : 11 */
		int     lIP[200];               /*!< 장치의 IP번호 */
		int     lModelType[200];        /*!< 모델명 정보 */
		short   nMotionType[200];       /*!< 제어 축 수(0=None, 2, 4, 6, 8) */
		short   nDioType[200];          /*!< DIO 종류(0=None, 1=16/16, 2=32/32, 3=48/48, 4=64/64) */
		short   nEXDio[200];            /*!< EXDIO 종류(0=None, 1=In16, 2=Out16) */
		short   nMDio[200];             /*!< MDIO 유무(0=None, 1=8/8) */

	}NMCEQUIPLIST, *PNMCEQUIPLIST;

	/**
	  * @brief  보드 형태의 장치(NMF)구성 정보
	  */
	typedef struct _TNMC_NMF_COMPO
	{
		short   nIP[4];             /*!< IP주소 */
		short   nModelPart;         /*!< 모델 Part 정보 (0=NMF, 1=UDIO) 지원되는 신(NMF),구(UDIO)의 구분 */
		short   nType[8];			/*!< 보드 형태 (0=없음, 1=D016, 2=DI16, 3=AI8(해당 없음), 4=AO8(해당 없음), 5=AI8AO8(해당 없음), 6=DO8, 7=DI8, 8=DI8DO8, 9~49=예약) */
		short   nCntBrd[50];   		/*!< 형태별 보드 개수(Max.8) 배열 번호는 보드 형태 값 예)DI8=2개, DO16=3개 일 경우 nCntBrd[1]=3, nCntBrd[7]=2개가 됨. */
		short   nTotalCntBrd;       /*!< 연결된 전체 보드 개수 (Max.8) */
		short   nReserved;          /*!< 예약 */
	}TNMC_NMF_COMPO, *PTNMCNMFCOMPO;

#pragma pack()
	/**************************************************************************************************/

	/**
	  * @brief      장치와 연결을 수행합니다.
	  * @param[in]  nNmcNo          장치번호(로터리 스위치 번호이자 IP번호)
	  * @param[in]  lWaitTime       연결대기 시간
	  * @return     PAIX_RETURN_VALUE
	  * @see        nmc_CloseDevice
	  */
	short WINAPI nmc_OpenDevice(short nNmcNo);
	short WINAPI nmc_OpenDeviceEx(short nNmcNo, int lWaitTime);

	/**
	  * @brief      장치와의 연결을 해제합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @see        nmc_OpenDevice
	  */
	void WINAPI nmc_CloseDevice(short nNmcNo);

	/**
	  * @brief      장치와의 물리적 네트워크 연결을 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  lWaitTime       응답대기시간(ms)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_PingCheck(short nNmcNo, int lWaitTime);

	/**
	  * @brief      장치와 연결하기 위해 IP설정을 수행합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nField0         IP주소 첫번째 (ex. 192.xxx.xxx.xxx)
	  * @param[in]  nField1         IP주소 두번째 (ex. xxx.168.xxx.xxx)
	  * @param[in]  nField2         IP주소 세번째 (ex. xxx.xxx.100.xxx)
	  */
	void WINAPI nmc_SetIPAddress(short nNmcNo, short nField0, short nField1, short nField2);

	/**
	  * @brief      장치의 IP를 변경합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  pnIP            설정할 IP주소 배열 포인터(ex. 192.168.0)
	  * @param[in]  pnSubNet        설정할 SubnetMask 배열 포인터(ex. 255.255.255)
	  * @param[in]  nGateway        설정할 Gateway 배열 포인터(ex. 1)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_WriteIPAddress(short nNmcNo, short *pnIP, short *pnSubNet, short nGateway);

	/**
	  * @brief      장치의 통신방식을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMethod         신방식(0=TCP, 1=UDP)
	  */
	void WINAPI nmc_SetProtocolMethod(short nNmcNo, short nMethod);

	/**
	  * @brief      장치와의 통신 확인 시간을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  lTimeInterval   통신확인시간 (0=기능해제)(ms)
	  * @param[in]  nStopMode       정지모드(0=감속정지, 1=긴급정지)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    시간을 초과하여 통신이 재개되지 않으면 설정된 정지모드에 따라 모든 축은 정지합니다.
	  */
	short WINAPI nmc_SetDisconnectedStopMode(short nNmcNo, int lTimeInterval, short nStopMode);

	/**
	  * @brief      이동 단위를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dRatio          이동단위
	  * @return     PAIX_RETURN_VALUE
	  * @warning    설정값에 대한 상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_SetUnitPerPulse(short nNmcNo, short nAxisNo, double dRatio);

	/**
	  * @brief      지정 축의 현재 로직정보를 읽어 온다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         로직정보를 읽어올 축 번호
	  * @param[out] pLogic          로직정보를 가져올 PARALOGIC 구조체 포인터
	  * @param[out] pLogicEx        로직정보를 가져올 PARALOGICEX 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @see        NMC_PARA_LOGIC
	  */
	short WINAPI nmc_GetParaLogic(short nNmcNo, short nAxisNo, PPARALOGIC pLogic);
	short WINAPI nmc_GetParaLogicEx(short nNmcNo, short nAxisNo, PPARALOGICEX pLogicEx);

	/**
	  * @brief      축 그룹의 Emergency 신호 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        설정할 축 그룹(0그룹=0~3, 1그룹=1~7)
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEmgLogic(short nNmcNo, short nGroupNo, short nLogic);

	/**
	  * @brief      Emergency 입력을 활성화합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nEnable         활성상태(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEmgEnable(short nNmcNo, short nEnable);

	/**
	  * @brief      축의 +Limit 센서 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetPlusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 -Limit 센서 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMinusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 Inposition 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nUse            Software Limit 활성화여부(0=비활성화, 1=활성화)
	  * @param[in]  dSwMinusPos     -방향 제한위치
	  * @param[in]  dSwPlusPos      +방향 제한위치
	  * @param[in]  nOpt            위치 비교 기준(0=지령위치, 1=엔코더위치)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetSWLimitLogic(short nNmcNo, short nAxisNo, short nUse, double dSwMinusPos, double dSwPlusPos);
	short WINAPI nmc_SetSWLimitLogicEx(short nNmcNo, short nAxisNo, short nUse, double dSwMinusPos, double dSwPlusPos, short nOpt);

	/**
	  * @brief      축의 알람 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetAlarmLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 원점센서 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetNearLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 Inposition 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설정 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetInPoLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 Servo Ready 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          로직정보를 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetSReadyLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 엔코더 Z상 입력 로직을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          입력 로직(0=Low, 1=High)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEncoderZLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      축의 엔코더 카운트 방향을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nCountDir       카운터 방향(0=A|B(+), 1=B|A(-), 2=Up|Down, 3=Down|Up)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEncoderDir(short nNmcNo, short nAxisNo, short nCountDir);

	/**
	  * @brief      축의 엔코더 체배를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nCountMode      체배 설정값(0=4체배, 1=2체배, 2=1체배)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEncoderCount(short nNmcNo, short nAxisNo, short nCountMode);

	/**
	  * @brief      장치에서 축으로 출력되는 펄스 모드를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nLogic          설명서을 참고하십시오.(펄스 출력 모드)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetPulseLogic(short nNmcNo, short nAxisNo, short nLogic);

	/**
	  * @brief      원점이동 완료상태를 해제할 상태를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         값을 읽어올 축 번호
	  * @param[in]  nAlarm          알람 발생시 설정(0=비사용, 1=사용)
	  * @param[in]  nServoOff       Servo Off시 설정(0=비사용, 1=사용)
	  * @param[in]  nCurrentOff     Current Off시 설정(0=비사용, 1=사용)
	  * @param[in]  nServoReady     Servo Ready시 설정(0=비사용, 1=사용)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetHomeDoneAutoCancel(short nNmcNo, short nAxisNo, short nAlarm, short nServoOff, short nCurrentOff, short nServoReady);

	/**
	  * @brief      원점이동 완료상태 해제 설정상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[out] pnAlarm         알람 발생시 해제 포인터 변수
	  * @param[out] pnServoOff      Servo Off시 해제 포인터 변수
	  * @param[out] pnCurrentOff    Current Off시 해제 포인터 변수
	  * @param[out] pnServoReady    Servo Ready시 해제 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetHomeDoneAutoCancel(short nNmcNo, short nAxisNo, short *pnAlarm, short *pnServoOff, short *pnCurrentOff, short *pnServoReady);

	/**
	  * @brief      축의 로직 정보를 설정한다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         로직 정보를 설정할 축 번호
	  * @param[in]  pLogic          로직정보가 들어갈 PARALOGIC 구조체 포인터
	  * @param[in]  pLogicEx        로직정보가 들어갈 PARALOGICEX 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @see        NMC_PARA_LOGIC
	  */
	short WINAPI nmc_SetParaLogic(short nNmcNo, short nAxisNo, PPARALOGIC pLogic);
	short WINAPI nmc_SetParaLogicEx(short nNmcNo, short nAxisNo, PPARALOGICEX pLogicEx);

	/**
	  * @brief      8축의 로직 정보를 지정한 파일에서 설정한다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  szFileName      8축 로직정보가 들어있는 파일이름
	  * @return     PAIX_RETURN_VALUE
	  * @see        NMC_PARA_LOGIC
	  */
	short WINAPI nmc_SetParaLogicFile(short nNmcNo, char *szFileName);

	/**
	  * @brief      지정 축의 전류 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetCurrentOn(short nNmcNo, short nAxisNo, short nOut);

	/**
	  * @brief      지정 축의 Servo On을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetServoOn(short nNmcNo, short nAxisNo, short nOut);

	/**
	  * @brief      지정 축의 Alarm Reset을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetAlarmResetOn(short nNmcNo, short nAxisNo, short nOut);

	/**
	  * @brief      지정 축의 DCC 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetDccOn(short nNmcNo, short nAxisNo, short nOut);

	/**
	  * @brief      지정하는 다축의 전류 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisSelect    축 번호 배열 포인터
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMultiCurrentOn(short nNmcNo, short nAxisCount, short *pnAxisSelect, short nOut);

	/**
	  * @brief      지정하는 다축의 Servon On 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisSelect    축 번호 배열 포인터
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMultiServoOn(short nNmcNo, short nAxisCount, short *pnAxisSelect, short nOut);

	/**
	  * @brief      지정하는 다축의 Alarm Reset 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisSelect    축 번호 배열 포인터
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMultiAlarmResetOn(short nNmcNo, short nAxisCount, short *pnAxisSelect, short nOut);

	/**
	  * @brief      지정하는 다축의 DCC 출력을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisSelect    축 번호 배열 포인터
	  * @param[in]  nOut            출력값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMultiDccOn(short nNmcNo, short nAxisCount, short *pnAxisSelect, short nOut);

	/**
	  * @brief      사다리꼴 형태로 속도 프로파일을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dStartSpeed     시작속도(이동단위/초)
	  * @param[in]  dAcc            가속도(이동단위/초²)
	  * @param[in]  dDec            감속도(이동단위/초²)
	  * @param[in]  dDriveSpeed     구동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetSpeed(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);

	/**
	  * @brief      S-Curve 형태로 속도 프로파일을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dStartSpeed     시작속도(이동단위/초)
	  * @param[in]  dAcc            가속도(이동단위/초²)
	  * @param[in]  dDec            감속도(이동단위/초²)
	  * @param[in]  dDriveSpeed     구동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetSCurveSpeed(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);

	/**
	  * @brief      가속도를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dAcc            가속도(이동단위/초²)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetAccSpeed(short nNmcNo, short nAxisNo, double dAcc);

	/**
	  * @brief      감속도를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dDec            감속도(이동단위/초²)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetDecSpeed(short nNmcNo, short nAxisNo, double dDec);

	/**
	  * @brief      구동 중인 축의 속도를 오버라이드 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dAcc            가속도(이동단위/초²)
	  * @param[in]  dDec            감속도(이동단위/초²)
	  * @param[in]  dDriveSpeed     구동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetOverrideRunSpeed(short nNmcNo, short nAxisNo, double dAcc, double dDec, double dDriveSpeed);

	/**
	  * @brief      구동 중인 축의 구동속도를 오버라이드 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dDriveSpeed     구동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetOverrideDriveSpeed(short nNmcNo, short nAxisNo, double dDriveSpeed);

	/**
	  * @brief      축의 절대이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            이동할 절대위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_AbsMove(short nNmcNo, short nAxisNo, double dPos);

	/**
	  * @brief      축의 상대이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dAmount         이동할 상대위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_RelMove(short nNmcNo, short nAxisNo, double dAmount);

	/**
	  * @brief      입력되는 속도로 이동 모드에따라 축을 구동합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            이동할 위치
	  * @param[in]  dDrive          구동속도(이동단위/초)
	  * @param[in]  nMode           이동할 위치모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_VelMove(short nNmcNo, short nAxisNo, double dPos, double dDrive, short nMode);

	/**
	  * @brief      다축 절대이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisList      축 번호 배열 포인터
	  * @param[in]  pdPosList       이동할 절대위치 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_VarAbsMove(short nNmcNo, short nAxisCount, short *pnAxisList, double *pdPosList);

	/**
	  * @brief      다축 상대이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisList      축 번호 배열 포인터
	  * @param[in]  pdAmount        이동할 상대위치 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_VarRelMove(short nNmcNo, short nAxisCount, short *pnAxisList, double *pdAmount);

	/**
	  * @brief      구동 중인 축에 위치 오버라이드를 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            오버라이드할 절대위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_AbsOver(short nNmcNo, short nAxisNo, double dPos);

	/**
	  * @brief      구동 중인 다축의 위치 오버라이드를 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisList      축 번호 배열 포인터
	  * @param[in]  pdPosList       오버라이드할 절대위치 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_VarAbsOver(short nNmcNo, short nAxisCount, short *pnAxisList, double *pdPosList);

	/**
	  * @brief      지정한 다축을 절대 감속 정지를(이동량을 무시하고 지정한 감속만으로 정지) 수행합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisList      축 번호 배열 포인터
	  * @param[in]  pdPosList       감속값의 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_VarAbsDecStop(short nNmcNo, short nAxisCount, short *pnAxisList, double *pdDecList);

	/**
	  * @brief      방향을 지정하여 속도이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nDir            구동방향(0=CW(정방향), 1=CCW(역방향))
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_JogMove(short nNmcNo, short nAxisNo, short nDir);

	/**
	  * @brief      구동 중인 축을 긴급 정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SuddenStop(short nNmcNo, short nAxisNo);

	/**
	  * @brief      구동 중인 축을 감속 정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_DecStop(short nNmcNo, short nAxisNo);

	/**
	  * @brief      설정 모드에 따라 구동 중인 전체 축을 정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMode           정지모드(0=감속정지, 1=긴급정지)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_AllAxisStop(short nNmcNo, short nMode);

	/**
	  * @brief      설정 모드에 따라 구동 중인 여러 축을 선택적으로 정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisCount      정지할 축 수량
	  * @param[in]  pnAxisSelect    정지할 축 번호 배열 포인터
	  * @param[in]  nMode           정지모드(0=감속정지, 1=긴급정지)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_MultiAxisStop(short nNmcNo, short nAxisCount, short *pnAxisSelect, short nMode);

	/**
	  * @brief      축의 지령위치 값을 변경합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos 변경할 위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetCmdPos(short nNmcNo, short nAxisNo, double dPos);

	/**
	  * @brief      축의 엔코더위치 값을 변경합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            변경할 위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEncPos(short nNmcNo, short nAxisNo, double dPos);

	/**
	  * @brief      축의 지령위치와 엔코더 위치값을 변경합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            변경할 위치
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetCmdEncPos(short nNmcNo, short nAxisNo, double dPos);

	/**
	  * @brief      원점이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nHomeMode       원점이동 형태를 설정합니다.\n
									(0=+Limit, 1=-Limit, 2=+Near, 3=-Near, 4= -Z, 5= +Z)\n
									자세한 내용은 설명서를 참고하십시오.
	  * @param[in]  nHomeEndMode    원점이동 완료 후, 위치값 설정(설명서 참고)
	  * @param[in]  dOffset         원점이동 완료 후, Offset 이동 위치값
	  * @param[in]  nReserve        예약변수(0을 설정하십시오.)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_HomeMove(short nNmcNo, short nAxisNo, short nHomeMode, short nHomeEndMode, double dOffset, short nReserve);

	/**
	  * @brief      원점이동에 사용되는 구동속도를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dHomeSpeed0     1차 이동속도(이동단위/초)
	  * @param[in]  dHomeSpeed1     2차 이동속도(이동단위/초)
	  * @param[in]  dHomeSpeed2     3차 이동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetHomeSpeed(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2);

	/**
	  * @brief      원점이동에 사용되는 구동속도를 설정합니다.(Offset 이동 속도를 추가합니다.)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dHomeSpeed0     1차 이동속도(이동단위/초)
	  * @param[in]  dHomeSpeed1     2차 이동속도(이동단위/초)
	  * @param[in]  dHomeSpeed2     3차 이동속도(이동단위/초)
	  * @param[in]  dOffsetSpeed    Offset위치 이동속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetHomeSpeedEx(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);

	/**
	  * @brief      원점이동에 사용되는 가감속을 포함한 속도를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dHomeSpeed0     1차 이동속도(이동단위/초)
	  * @param[in]  dStart0         1차 시작속도(이동단위/초)
	  * @param[in]  dAcc0           1차 가속도(이동단위/초²)
	  * @param[in]  dDec0           1차 감속도(이동단위/초²)
	  * @param[in]  dHomeSpeed2     2차 이동속도(이동단위/초)
	  * @param[in]  dStart1         2차 시작속도(이동단위/초)
	  * @param[in]  dAcc1           2차 가속도(이동단위/초²)
	  * @param[in]  dDec1           2차 감속도(이동단위/초²)
	  * @param[in]  dHomeSpeed2     3차 이동속도(이동단위/초)
	  * @param[in]  dStart2         3차 시작속도(이동단위/초)
	  * @param[in]  dAcc2           3차 가속도(이동단위/초²)
	  * @param[in]  dDec2           3차 감속도(이동단위/초²)
	  * @param[in]  dOffsetSpeed    Offset위치 이동속도(이동단위/초)
	  * @param[in]  dOffsetStart    Offset 시작속도(이동단위/초)
	  * @param[in]  dOffsetAcc      Offset 가속도(이동단위/초²)
	  * @param[in]  dOffsetDec      Offset 감속도(이동단위/초²)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetHomeSpeedAccDec(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dStart0, double dAcc0, double dDec0,
		double dHomeSpeed1, double dStart1, double dAcc1, double dDec1,
		double dHomeSpeed2, double dStart2, double dAcc2, double dDec2,
		double dOffsetSpeed, double dOffsetStart, double dOffsetAcc, double dOffsetDec);

	/**
	  * @brief  원점이동 상태정보 구조체
	  */
#pragma pack(1)     /*!< Specifies packing alignment for structure */
	typedef struct NMC_HOME_FLAG
	{
		short   nSrchFlag[8];           /*!< 원점이동 동작여부(0=이동완료, 1=이동중) */
		short   nStatusFlag[8];         /*!< 원점이동 세부 동작 상태\n
											(0=이동완료, 1=이동중, 2= 사용자에 의한 정지, 3=원점이동 미실행\n
											4=비상정지, 5=알람정지, ... ) */
	}NMCHOMEFLAG, *PNMCHOMEFLAG;
#pragma pack()

	/**
	  * @brief      원점이동 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pHomeFlag       원점이동 상태를 받을 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetHomeStatus(short nNmcNo, PNMCHOMEFLAG pHomeFlag);

	/**
	  * @brief      원점이동 수행시, 단계별 안정화를 위한 지연시간을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nHomeDelay      설정할 시간(ms)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetHomeDelay(short nNmcNo, int nHomeDelay);

	/**
	  * @brief      원점이동 정상완료 상태를 해제합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_HomeDoneCancel(short nNmcNo, short nAxisNo);

	/**
	  * @brief      2축 직선보간을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dPos[x]         이동할 위치(X,Y)
	  * @param[in]  nOpt            이동모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_Interpolation2Axis(short nNmcNo, short nAxisNo0, double dPos0, short nAxisNo1, double dPos1, short nOpt);

	/**
	  * @brief      3축 직선보간을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dPos[x]         이동할 위치(X,Y,Z)
	  * @param[in]  nOpt            이동모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_Interpolation3Axis(short nNmcNo, short nAxisNo0, double dPos0, short nAxisNo1, double dPos1,
		short nAxisNo2, double dPos2, short nOpt);

	/**
	  * @brief      4축 직선보간을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dPos[x]         이동할 위치(X,Y,Z,U)
	  * @param[in]  nOpt            이동모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_Interpolation4Axis(short nNmcNo, short nAxisNo0, double dPos0, short nAxisNo1, double dPos1,
		short nAxisNo2, double dPos2, short nAxisNo3, double dPos3, short nOpt);

	/**
	  * @brief      원호보간을 명령합니다. 중심위치와 회전각도를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dCenter[x]      중심위치(X,Y)
	  * @param[in]  dAngle          회전각도(음수 = CW(정방향), 양수 = CCW(영방향))
	  * @param[in]  nOpt            중심위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationArc(short nNmcNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1, double dAngle, short nOpt);

	/**
	  * @brief      원호보간을 명령합니다. 중심위치와 종료위치, 회전방향을 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dCenter[x]      중심위치(X,Y)
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nDir            회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  nOpt            중심위치와 종료위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationArcCE(short nNmcNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1,
		double dEnd0, double dEnd1, short nDir, short nOpt);

	/**
	  * @brief      원호보간을 명령합니다. 통과위치와 종료위치를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dPass[x]        통과위치(X,Y)
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nOpt            통과위치와 종료위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationArcPE(short nNmcNo, short nAxisNo0, short nAxisNo1, double dPass0, double dPass1,
		double dEnd0, double dEnd1, short nOpt);

	/**
	  * @brief      원호보간을 명령합니다. 반지름과 종료위치, 회전방향, 이동거리를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dRadius         원의 반지름
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nLen            원호의 이동거리(0=짧은거리, 1=긴거리)
	  * @param[in]  nDir            회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  nOpt            종료위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationArcRE(short nNmcNo, short nAxisNo0, short nAxisNo1, double dRadius,
		double dEnd0, double dEnd1, short nLen, short nDir, short nOpt);

	/**
	  * @brief      헬리컬보간을 명령합니다. 중심위치와 회전각도를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dCenter[x]      중심위치(X,Y)
	  * @param[in]  dAngle          회전각도(음수 = CW(정방향), 양수 = CCW(영방향))
	  * @param[in]  nArcOpt         원호 중심위치 모드(0=상대, 1=절대)
	  * @param[in]  dZPos           Z축 이동위치값
	  * @param[in]  nZOpt           Z축 위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationHelical(short nNmcNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
		double dCenter0, double dCenter1, double dAngle, short nArcOpt, double dZPos, short nZOpt);

	/**
	  * @brief      헬리컬보간을 명령합니다. 중심위치와 종료위치, 회전방향을 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dCenter[x]      중심위치(X,Y)
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nDir            회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  nArcOpt         원호 중심위치와 종료위치 모드(0=상대, 1=절대)
	  * @param[in]  dZPos           Z축 이동위치값
	  * @param[in]  nZOpt           Z축 위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationHelicalCE(short nNmcNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
		double dCenter0, double dCenter1, double dEnd0, double dEnd1, short nDir, short nArcOpt, double dZPos, short nZOpt);

	/**
	  * @brief      헬리컬보간을 명령합니다. 통과위치와 종료위치를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dPass[x]        통과위치(X,Y)
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nArcOpt         원호 통과위치와 종료위치 모드(0=상대, 1=절대)
	  * @param[in]  dZPos           Z축 이동위치값
	  * @param[in]  nZOpt           Z축 위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationHelicalPE(short nNmcNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
		double dPass0, double dPass1, double dEnd0, double dEnd1, short nArcOpt, double dZPos, short nZOpt);

	/**
	  * @brief      헬리컬보간을 명령합니다. 반지름과 종료위치, 회전방향, 이동거리를 지정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo[x]      설정할 축 번호(0부터 오름차순으로 설정, nAxisNo0이 기준축)
	  * @param[in]  dRadius         원의 반지름
	  * @param[in]  dEnd[x]         종료위치(X,Y)
	  * @param[in]  nLen            원호의 이동거리(0=짧은거리, 1=긴거리)
	  * @param[in]  nDir            회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  nArcOpt         원호 종료위치 모드(0=상대, 1=절대)
	  * @param[in]  dZPos           Z축 이동위치값
	  * @param[in]  nZOpt           Z축 위치 모드(0=상대, 1=절대)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_InterpolationHelicalRE(short nNmcNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
		double dRadius, double dEnd0, double dEnd1, short nLen, short nDir, short nArcOpt, double dZPos, short nZOpt);

	/**
	  * @brief      링카운터 기능 사용 설정을 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  lMaxPulse       1회전을 하기위한 지령 펄스의 값
	  * @param[in]  lMaxEncoder     1회전시 엔코더 값
	  * @param[in]  nRingMode       링카운트 모드 사용여부(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetRingCountMode(short nNmcNo, short nAxisNo, int lMaxPulse, int lMaxEncoder, short nRingMode);

	/**
	  * @brief      축의 링카운트 정보를 읽어 온다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] plMaxPulse      링카운트 펄스 최대값을 읽어올 포인터 변수
	  * @param[out] plMaxEncoder    링카운트 엔코더 최대값을 읽어올 포인터 변수
	  * @param[out] pnRingMode      링카운트 활성화 여부를 읽어올 포인터 변수(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetRingCountMode(short nNmcNo, short nAxisNo, int *plMaxPulse, int *plMaxEncoder, short *pnRingMode);

	/**
	  * @brief      링카운터가 설정된 축의 이동명령입니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dPos            이동할 절대위치
	  * @param[in]  nMoveMode       구동방향(0=CW(정방향), 1=CCW(역방향))
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_MoveRing(short nNmcNo, short nAxisNo, double dPos, short nMoveMode);

	/**
	  * @brief      ±Limit 신호입력 시, 정지모드를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nStopMode       설정값(0=긴급정지, 1=감속정지)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetLimitStopMode(short nNmcNo, short nAxisNo, short nStopMode);

	/**
	  * @brief      Alarm 신호입력 시, 정지모드를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nStopMode       설정값(0=긴급정지, 1=감속정지)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetAlarmStopMode(short nNmcNo, short nAxisNo, short nStopMode);

	/**
	  * @brief      원점센서 신호입력 시, 감속정지의 수행여부를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nUse            설정값(0=비사용, 1=사용)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEnableNear(short nNmcNo, short nAxisNo, short nUse);

	/**
	  * @brief      엔코더 Z상 신호입력 시, 감속정지의 수행여부를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nMode           설정값(0=비사용, 1=사용)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetEnableEncZ(short nNmcNo, short nAxisNo, short nMode);

	/**
	  * @brief      Busy Off 신호의 출력 시점을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nMode           설정값(0=펄스 출력완료, 1=위치결정 완료)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetBusyOffMode(short nNmcNo, short nAxisNo, short nMode);

	/**
	  * @brief      MPG모드를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축번호(그룹별 0,1번째 축만 가능)
	  * @param[in]  nMode           MPG모드(0=사용안함, 1=연속펄스, 1=정량펄스, 2=수동펄스)
	  * @param[in]  lPulse          수동펄스 엔코더의 입력당 출력할 펄스의 수
	  * @return     PAIX_RETURN_VALUE
	  * @warning    제어내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_SetMpgMode(short nNmcNo, short nAxisNo, short nMode, int lPulse);

	/**
	  * @brief      MPG모드를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축번호(그룹별 0,1번째 축만 가능)
	  * @param[in]  nRunMode        MPG 구동모드(0=사용안함, 1=ByPass, 2=Step 실행)
	  * @param[in]  nInMode         입력모드(0=1체배, 1=2체배, 2=4체배, 3=2Pulse)
	  * @param[in]  nDir            A,B 입력의 Counter 방향(0=정방향, 1=역방향)
	  * @param[in]  nX              설정할 체배(1~32)
	  * @param[in]  nN              설정할 분주비(1=사용안함, 2~2048)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    E-Version 에서 사용됩니다. 제어내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_SetEMpg(short nNmcNo, short nAxisNo, short nRunMode, short nInMode, short nDir, short nX, short nN);

	/**
	  * @brief      E-MPG를 Count설정
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축번호(그룹별 0,1번째 축만 가능)
	  * @param[in]  nRunMode        MPG 구동모드(0=사용안함, 1=사용)
	  * @param[in]  nInMode         입력모드(0=1체배, 1=2체배, 2=4체배, 3=2Pulse)
	  * @param[in]  nDir            A,B 입력의 Counter 방향(0=정방향, 1=역방향)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    E-Version 에서 사용됩니다. 제어내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_EMpgCounterSet(short nNmcNo, short nAxisNo, short nRunMode, short nInMode, short nDir);

	/**
	  * @brief      E-MPG를 Count값 읽기
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] plRetCount      Count값을 받을 4개의 int형 배열 포인터(각 그룹의 0,1번 축만 MPG기능이 사용되므로 각 배열[0]=0축,[1]=1축,[2]=4축,[3]=5축
	  * @return     PAIX_RETURN_VALUE
	  * @warning    E-Version 에서 사용됩니다. 제어내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_EMpgCounterGet(short nNmcNo, int *plRetCount);

	//#define _PAIX_SERIAL_
#ifdef _PAIX_SERIAL_
#define B_9600              0
#define B_19200             1
#define B_38400             2

#define STOP_1              0
#define STOP_2              1

#define DATA_1              0
#define DATA_2              1
#define DATA_3              2
#define DATA_4              3
#define DATA_5              4
#define DATA_6              5
#define DATA_7              6
#define DATA_8              7

#define P_NONE              0
#define P_ODD               1
#define P_EVEN              2
#endif

/**
  * @brief      장치 제어를 위한 시리얼 통신 기능을 활성화합니다.
  * @param[in]  nNmcNo          장치번호
  * @param[in]  nMode           기능 활성화(0=비활성화, 1=활성화)
  * @return     PAIX_RETURN_VALUE
  */
	short WINAPI nmc_SetSerialMode(short nNmcNo, short nMode);

	/**
	  * @brief      시리얼 통신 환경을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nBaud           통신속도(0=9600, 1=19200, 2=38400 bps)
	  * @param[in]  nData           데이터 비트수 (설정값 0~7 = 1~8 bit)
	  * @param[in]  nStop           정지 비트 수 (0 = 1, 1 = 2)
	  * @param[in]  nParity         Parity 비트 (0=None, 1=Odd, 2=Even)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetSerialConfig(short nNmcNo, short nBaud, short nData, short nStop, short nParity);

	/**
	  * @brief      장치로 데이터를 전송합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nLen            전송 데이터의 바이트 수
	  * @param[in]  pStr            전송할 내용 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SerialWrite(short nNmcNo, short nLen, char *pStr);

	/**
	  * @brief      장치가 전송한 데이터를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnReadLen       수신 데이터의 바이트 수(최대 384bytes)
	  * @param[out] pReadStr        수신한 내용을 받을 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SerialRead(short nNmcNo, short *pnReadLen, char *pReadStr);

	/**
	  * @brief      삼각파형 방지기능을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nAVTRIMode      방지기능 설정(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_AVTRISetMode(short nNmcNo, short nAxisNo, short nAVTRIMode);

	/**
	  * @brief      삼각파형 방지기능을 설정상태를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[out] pnAVTRIMode     방지기능 상태를 읽어올 포인터 변수(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_AVTRIGetMode(short nNmcNo, short nAxisNo, short *pnAVTRIMode);

	/**
	  * @brief      Backlash, Slip보정 기능 설정
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nMode           모드 설정(0:None, 1:Backlash, 2:Slip)
	  * @param[in]  dAmount         이동량
	  * @param[in]  nCMask          카운터 동작여부(Bit0:Counter1(지령)동작, Bit1:Counter2(엔코더), Bit3:Counter3(편차), bit4:Counter4(범용))
	  * @param[in]  dSpeed          보정펄스의 출력 속도(이동단위/초)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetBacklashSlip(short nNmcNo, short nAxisNo, short nMode, double dAmount, short nCMask, double dSpeed);

	/**
	  * @brief      입력신호에 대하여 필터를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nEnc            [기본=0]엔코더 신호의 필터설정(0=50.862ns, 1=152ns)
	  * @param[in]  nMPG            [기본=0]MPG 신호의 필터설정(0=50.862ns, 1=152ns)
	  * @param[in]  nMSig           [기본=0]Limit, Near, Alarm, Inposition(0=101ns,  1=140us)
	  * @param[in]  nDR             [기본=0]DR입력신호(0=101ns, 1=32ms)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetInSigFilter(short nNmcNo, short nAxisNo, short nEnc, short nMPG, short nMSig, short nDR);

	/**
	  * @brief      통신 응답 대기시간을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  lWaitTime       설정할 응답대기시간(ms)
	  */
	void WINAPI nmc_SetWaitTime(short nNmcNo, int lWaitTime);

	/**
	  * @brief      포트 포워딩을 위한 포트 변경을 수행합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  lPortNum        변경할 포트번호
	  * @return     PAIX_RETURN_VALUE
	  * @warning    E-Version에서만 지원됩니다.
	  */
	short WINAPI nmc_SetNetPortNum(short nNmcNo, int lPortNum);

	/**
	  * @brief      Gantry 구동 설정을 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        설정할 그룹 번호(0=(0번축~3번축), 1=(4번축~7번축))
	  * @param[in]  nMain Gantry    메인 축 번호
	  * @param[in]  nSub Gantry     서브 축 번호
	  * @return     PAIX_RETURN_VALUE
	  * @warning    그룹에 따라 설정할 수있는 범위가 다르므로 주의하여 주십시오.
	  */
	short WINAPI nmc_SetGantryAxis(short nNmcNo, short nGroupNo, short nMain, short nSub);

	/**
	  * @brief      Gantry 구동 설정을 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnEnable        활성화 상태가 반환될 배열 포인터(2개)
	  * @param[out] pnMainAxes      메인 축 번호가 반환될 배열 포인터(2개)
	  * @param[out] pnSubAxes       Gantry 서브 축 번호가 반환될 배열 포인터(2개)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetGantryInfo(short nNmcNo, short *pnEnable, short *pnMainAxes, short *pnSubAxes);

	/**
	  * @brief      Gantry 구동 설정을 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        설정할 그룹 번호(0=(0번축~3번축), 1=(4번축~7번축))
	  * @param[in]  nEnable         Gantry 구동을 활성화 합니다.(0=비활성화, 1=활성화)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetGantryEnable(short nNmcNo, short nGroupNo, short nEnable);

	/**
	  * @brief      MDIO 입력상태를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnInStatus      입력pin 8개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetMDIOInput(short nNmcNo, short *pnInStatus);

	/**
	  * @brief      MDIO 출력상태를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnOutStatus     출력pin 8개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetMDIOOutput(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      MDIO 전체 출력상태를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  pnOutStatus     출력pin 8개의 출력설정 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMDIOOutput(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      MDIO 단일 Pin의 출력상태를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          출력 Pin 번호
	  * @param[in]  nOutStatus      출력 설정값(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);

	/**
	  * @brief      MDIO 단일 Pin의 출력상태를 반전 시킵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          Pin 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMDIOOutTogPin(short nNmcNo, short nPinNo);

	/**
	  * @brief      MDIO 지정하는 복수개의 Pin 출력상태를 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nCount          출력 Pin 수량
	  * @param[in]  pnPinNo         출력 Pin 번호 배열 포인터
	  * @param[in]  pnStatus        출력 설정값 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);

	/**
	  * @brief      MDIO 지정하는 복수개의 Pin 출력상태를 반전 시킵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nCount          Pin 수량
	  * @param[in]  pnPinNo         Pin 번호 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetMDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	/**
	  * @brief      DIO 모듈의 입력상태를 확인합니다.
	  * @param[in]  nNmcNo          DIO 장치번호
	  * @param[in]  pnInStatus      입력pin 64개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetDIOInput(short nNmcNo, short *pnInStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 입력상태를 확인합니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  pnInStatus      입력pin 128개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    DIO에서 64개가 넘어가는 값은 Reserved 처리됩니다.
	  */
	short WINAPI nmc_GetDIOInput128(short nNmcNo, short *pnInStatus);

	/**
	  * @brief      DIO 모듈의 출력상태를 확인합니다.
	  * @param[in]  nNmcNo          DIO 장치번호
	  * @param[in]  pnOutStatus     출력pin 64개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetDIOOutput(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 출력상태를 확인합니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  pnOutStatus     출력pin 128개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    DIO에서 64개가 넘어가는 값은 Reserved 처리됩니다.
	  */
	short WINAPI nmc_GetDIOOutput128(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      DIO 모듈의 출력 상태를 설정합니다.
	  * @param[in]  nNmcNo          DIO 장치번호
	  * @param[out] pnOutStatus     출력pin 64개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetDIOOutput(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 출력상태를 설정합니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[out] pnOutStatus     출력pin 128개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    DIO에서 64개가 넘어가는 값은 Reserved 처리됩니다.
	  */
	short WINAPI nmc_SetDIOOutput128(short nNmcNo, short *pnOutStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 Pin 하나의 출력상태를 설정합니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  nPinNo          Pin 번호
	  * @param[in]  nOutStatus      출력pin 128개의 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 Pin 하나의 출력상태를 반전시킵니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  nPinNo          Pin 번호
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_SetDIOOutTogPin(short nNmcNo, short nPinNo);

	/**
	  * @brief      DIO/UDIO 모듈의 지정하는 Pin들의 출력을 설정합니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  nCount          출력 Pin 수량
	  * @param[in]  pnPinNo         Pin 번호 배열 포인터
	  * @param[in]  pnStatus        출력 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_SetDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);

	/**
	  * @brief      DIO/UDIO 모듈의 지정하는 Pin 출력을 반전시킵니다.
	  * @param[in]  nNmcNo          DIO/UDIO 장치번호
	  * @param[in]  nCount          Pin 수량
	  * @param[in]  pnPinNo         Pin 번호 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_SetDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	/**
	  * @brief 확장형 DIO 제품의 사용 함수입니다.
	  */
	short WINAPI nmc_GetEXDIOInput(short nNmcNo, short *pnInStatus);
	short WINAPI nmc_GetEXDIOOutput(short nNmcNo, short *pnOutStatus);
	short WINAPI nmc_SetEXDIOOutput(short nNmcNo, short *pnOutStatus);
	short WINAPI nmc_SetEXDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
	short WINAPI nmc_SetEXDIOOutTogPin(short nNmcNo, short nPinNo);
	short WINAPI nmc_SetEXDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
	short WINAPI nmc_SetEXDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	/**
	  * @brief      출력 Pin에 유지시간 제한기능을 설정하여 출력 On/Off를 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nIoType         IO 종류(0=MDIO, 1=DIO, 2=EXDIO)
	  * @param[in]  nPinNo          IO종류별로 지정되는 Pin번호
	  * @param[in]  nOutStatus      설정할 출력값 (0=Off, 1=On)
	  * @param[in]  nTime           설정할 유지제한시간(ms)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_SetOutLimitTimePin(short nNmcNo, short nIoType, short nPinNo, short nOutStatus, int nTime);

	/**
	  * @brief      출력 Pin에 설정된 유지시간 제한기능을 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nIoType         IO 종류(0=MDIO, 1=DIO, 2=EXDIO)
	  * @param[in]  nPinNo          IO종류별로 지정되는 Pin번호
	  * @param[out] pnSet           설정 상태를 읽어올 포인터 변수(0=설정안됨, 1=On, 2=Off)
	  * @param[out] pnStatus        현재 제한시간의 상태를 읽어올 포인터 변수(0=설정안됨, 1=제한시간 진행중, 2=제한 시간 종료)
	  * @param[out] pnOutStatus     현재 Pin 출력상태를 읽어올 포인터 변수(0=Off, 1=On)
	  * @param[out] pnRemainTime    남은 시간을 읽어올 포인터 변수(ms)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    모델별 출력 Pin 개수를 확인하십시오.
	  */
	short WINAPI nmc_GetOutLimitTimePin(short nNmcNo, short nIoType, short nPinNo, short *pnSet, short *pnStatus,
		short *pnOutStatus, int *pnRemainTime);

	/**
	  * @brief      장치에 설정된 로직 및 모션환경값을 장치 내부 ROM에 저장합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMode           저장 범위(0=로직 및 모션설정, 1=로직설정, 2= 모션설정)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_MotCfgSaveToROM(short nNmcNo, short nMode);

	/**
	  * @brief      장치의 설정값을 초기화 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMode           초기화 범위(0=로직 및 모션설정, 1=로직설정, 2= 모션설정)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_MotCfgSetDefaultROM(short nNmcNo, short nMode);

	/**
	  * @brief      장치의 ROM에 저장된 설정값을 현재 장치에 반영합니다.(RAM->RAM)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMode           가져올 범위(0=로직 및 모션설정, 1=로직설정, 2= 모션설정)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_MotCfgLoadFromROM(short nNmcNo, short nMode);

	/**
	  * @brief      장치의 제품 종류를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] plDeviceType    내용은 설명서를 참고하십시오
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetDeviceType(short nNmcNo, int *plDeviceType);

	/**
	  * @brief      장치의 세부 구성정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnMotionType    모션제어 축 수(0=None, 2, 4, 6, 8)
	  * @param[out] pnDioType       DIO 종류(0=None, 1=16/16, 2=32/32, 3=48/48, 4=64/64)
	  * @param[out] pnEXDio         EXDIO 종류(0=None, 1=In16, 2=Out16)
	  * @param[out] pnMDio          MDIO 유무(0=None, 1=8/8)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetDeviceInfo(short nNmcNo, short *pnMotionType, short *pnDioType, short *pnEXDio, short *pnMDio);

	/**
	  * @brief      장치의 세부 구성정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pNmcList        세부정보를 읽어올 NMC_EQUIP_LIST 구조체 포인터
	  * @return     장치 리스트의 수량
	  */
	int WINAPI nmc_GetEnumList(short *pnIp, PNMCEQUIPLIST pNmcList);

	/**
	  * @brief      UDIO 장치의 IO 구성정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnInCount       입력 Pin의 수량
	  * @param[out] pnOutCount      출력 Pin의 수량
	  * @return     PAIX_RETURN_VALUE
	  * @warning    UDIO 제품에서만 수행됩니다.
	  */
	short WINAPI nmc_GetDIOInfo(short nNmcNo, short *pnInCount, short *pnOutCount);

#pragma pack(1)     /*!< Specifies packing alignment for structure */
	/**
	  * @brief  Mapping 데이터 구조체
	  */
	typedef struct TNMC_MAP_DATA
	{
		int     nMapCount;          /*!< Mapping 수량 */
		int     lMapData[52];       /*!< Mapping 데이터(단위:Pulse) */
		double  dMapData[52];       /*!< Mapping 데이터(단위:Unit/Pulse) */
	}NMCMAPDATA, *PNMCMAPDATA;
#pragma pack()
	/**
	  * @brief      Mapping 이동을 명령합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축번호
	  * @param[in]  dPos            이동할 위치
	  * @param[in]  nMapIndex       MDIO의 Search Pin(0,1)
	  * @param[in]  nOpt            이동모드(0=상대, 1=절대)
	  * @param[in]  nPosType        Mapping 이동으로 가져올 위치값(엔코더 위치, 지령위치)
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  * @warning    nmc_MapMove 함수는 위치값을 엔코더 위치만 반환합니다.
	  */
	short WINAPI nmc_MapMove(short nNmcNo, short nAxisNo, double dPos, short nMapIndex, short nOpt);
	short WINAPI nmc_MapMoveEx(short nNmcNo, short nAxisNo, double dPos, short nMapIndex, short nOpt, short nPosType);

	/**
	  * @brief      블록 0번의 Mapping 데이터를 가져옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMapIndex       MDIO의 Search Pin(0,1)
	  * @param[out] pNmcMapData     확인할 TNMC_MAP_DATA 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_GetMapData(short nNmcNo, short nMapIndex, PNMCMAPDATA pNmcMapData);

	/**
	  * @brief      Mapping 데이터를 블록별로 가져옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMapIndex       MDIO의 Search Pin(0,1)
	  * @param[in]  nDataIndex      데이터 블록 번호 (0~3)
	  * @param[out] pNmcMapData     확인할 TNMC_MAP_DATA 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @warning    상세 내용은 설명서를 참고하십시오.
	  */
	short WINAPI nmc_GetMapDataEx(short nNmcNo, short nMapIndex, short nDataIndex, PNMCMAPDATA pNmcMapData);

	/**
	  * @brief      MDIO의 Search 0, 1의 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnMapInStatus   Search의 입력 상태 배열 포인터(0=Off, 1=On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetMapIO(short nNmcNo, short *pnMapInStatus);

	/**
	  * @brief      카운트 기능을 시작합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          출력제어할 MDIO Pin번호(-1=출력제어 안함, 0~7)
	  * @param[in]  nEdge           카운트 기준(0=Rising, 1=Falling, 2=Rising+Falling)
	  * @param[in]  nOutStatus      지정된 MDIO Pin에 설정할 출력상태(0=Off, 1=On)
	  * @param[in]  lCount          카운트할 수량(1~65535)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchCountStart(short nNmcNo, short nPinNo, short nEdge, short nOutStatus, int lCount);

	/**
	  * @brief      카운트된 값을 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] plCount         카운트 값을 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchCountGet(short nNmcNo, int *plCount);

	/**
	  * @brief      카운트 기능을 해제 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchCountStop(short nNmcNo);


	/**
	  * @brief      Search Pin(0,1) 입력시 구동속도 Override 기능시작
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          MDIO의 Search Pin(0,1)
	  * @param[in]  nEdge           카운트 기준(0=Rising, 1=Falling, 2=Rising+Falling)
	  * @param[in]  nAxisCount      설정할 축 수량
	  * @param[in]  pnAxisList      축 번호 배열 포인터
	  * @param[in]  pdSpeedList     Override할 구동속도 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchOverDrvSpeedStart(short nNmcNo, short nPinNo, short nEdge, short nAxisCount, short *pnAxisList, double *pdSpeedList);

	/**
	  * @brief      Search Pin(0,1) 입력시 구동속도 Override 기능상태 읽기
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          MDIO의 Search Pin(0,1)
	  * @param[out] pnRetPinState   지정한 Search Pin의 입력 상태 (0=Off, 1=On)
	  * @param[out] pnRetCount		Pin에 입력된 횟수를 받을 포인터
	  * @param[out] pdLatchPosList  구동속도 Override될 시점의 Latch된 지령위치를 받을 8축 배열의 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchOverDrvSpeedGetStatus(short nNmcNo, short nPinNo, short *pnRetPinState, short *pnRetCount, double *pdLatchPosList);

	/**
	  * @brief      Search Pin(0,1) 입력시 구동속도 Override 기능정지
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nPinNo          MDIO의 Search Pin(0,1)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SearchOverDrvSpeedStop(short nNmcNo, short nPinNo);


#pragma pack(1)     /*!< Specifies packing alignment for structure */
	/**
	  * @brief      Trigger 설정정보 구조체
	  */
	typedef struct NMC_TRIG_STATUS
	{
		short   nRun[8];            /*<! Trigger 실행 여부(0=미실행, 1=실행중) */
		short   nOutLogic[8];       /*<! 설정된 출력 로직(0=Active High, 1=Active Low) */
		short   nMode[8];           /*<! 설정된 Trigger 형태(0=미실행, 1=Line Scan, 2=절대위치, 3=Range) */
		int     nCount[8];          /*<! Trigger 출력 개수 (0 ~ 65535) */
		short   nDummy[25];         /*<! 예비공간 */
	}NMCTRIGSTATUS, *PNMCTRIGSTATUS;
#pragma pack()

	/**
	  * @brief      Trigger 입력 및 출력사양을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nInType         Trigger에 사용할 펄스(0=지령펄스, 1=엔코더펄스)
	  * @param[in]  nOutLogic       출력로직(0=Active High, 1=Active Low)
	  * @param[in]  nOutDelay       출력시점에 지연시간, 설정시간만큼 대기후 출력(0~65535us)
	  * @param[in]  nOutWidth       Trigger 출력 펄스 폭(1~65536us)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetTriggerIO(short nNmcNo, short nAxisNo, short nInType, short nOutLogic, int nOutDelay, int nOutWidth);

	/**
	  * @brief      시작과 끝 범위에서 지정된 주기 Pulse 마다 Trigger를 출력합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dStartPos       Trigger 시작위치
	  * @param[in]  dEndPos         Trigger 종료위치
	  * @param[in]  dInterval       출력 간격(양수로만 설정)
	  * @param[in]  nDir            Trigger 출력 방향(0=양방향, 1=Counter Up, 2=Counter Down)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_TriggerOutLineScan(short nNmcNo, short nAxisNo, double dStartPos, double dEndPos, double dInterval, short nDir);

	/**
	  * @brief      설정된 복수 개의 절대위치에서 Trigger를 출력합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nCount          설정위치 수량
	  * @param[in]  pdPosList       설정위치값 배열
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_TriggerOutAbsPos(short nNmcNo, short nAxisNo, short nCount, double *pdPosList);

	/**
	  * @brief      설정된 복수 개의 구간에서 Trigger를 출력합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nCount          설정위치 수량(최대 64개)
	  * @param[in]  pdStartPosList  출력 시작위치값 배열
	  * @param[in]  pdEndPosList    출력 종료위치값 배열
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_TriggerOutRange(short nNmcNo, short nAxisNo, short nCount, double *pdStartPosList, double *pdEndPosList);

	/**
	  * @brief      Trigger를 출력합니다. nmc_SetTriggerIO 함수로 설정된 펄스폭 만큼 출력합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         출력할 축 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_TriggerOutOneShot(short nNmcNo, short nAxisNo);

	/**
	  * @brief      Trigger 출력을 정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_TriggerOutStop(short nNmcNo, short nAxisNo);

	/**
	  * @brief      그룹 Pin에 Axis를 Assign
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroup			그룹 번호
	  * @param[in]  nPin0			그룹 Pin에 Assign할 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[in]  nPin1			그룹 Pin에 Assign할 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[in]  nPin2			그룹 Pin에 Assign할 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[in]  nPin3			그룹 Pin에 Assign할 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetTriggerGroupAssign(short nNmcNo, short nGroup, short nPin0, short nPin1, short nPin2, short nPin3);

	/**
	  * @brief      그룹 Pin에 Assign된 Axis를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroup			그룹 번호
	  * @param[out] pnPin0			그룹 Pin에 Assign된 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[out] pnPin1			그룹 Pin에 Assign된 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[out] pnPin2			그룹 Pin에 Assign된 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @param[out] pnPin3			그룹 Pin에 Assign된 Axis (Group=0일 경우 0~3번 Axis, Group=1일 경우 4~7번 Axis)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetTriggerGroupAssign(short nNmcNo, short nGroup, short *pnPin0, short *pnPin1, short *pnPin2, short *pnPin3);

	/**
	  * @brief      전체 축의 Trigger 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pTriggerStatus  상태를 읽어올 NMC_TRIG_STATUS 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetTriggerStatus(short nNmcNo, PNMCTRIGSTATUS pTriggerStatus);

#pragma pack(1)     /*!< Specifies packing alignment for structure */
	/**
	  * @brief      List Motion 설정정보 구조체
	  */
	typedef struct NMC_LM_STATUS
	{
		short   nRun[8];                /*<! ListMotion 실행 여부(0=미실행, 1=실행중) */
		short   nWait[8];               /*<! Queue상태(0=실행할 노드 있음, 1=노드추가등록 대기) */
		short   nRemainNum[8];          /*<! 비어 있는 큐버퍼의 수 */
		unsigned int    uiExeNum[8];    /*<! 실행 중인 리스트 모션의 수(0 ~ 4,294,967,295) */
		short   nDummy[6];              /*<! 예비공간 */
	}NMCLMSTATUS, *PNMCLMSTATUS;
#pragma pack()

	/**
	  * @brief      ListMotion 환경설정을 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nEmptyMode      진행 중 등록된 노드가 모두 소진된 상태의 동작(0=강제종료, 1=노드추가등록 대기)
	  * @param[in]  nIoType         출력 사용될 IO 타입(0=MDIO, 1=DIO, 2=제어안함)
	  * @param[in]  nIoCtrlPinMask  IO PinMask 범위(MDIO=0~0xFF, DIO=0~0xFFFF)
	  * @param[in]  nIoCtrlEndVal   리스트모션 종료시, 출력 Pin값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ListMotSetMode(short nNmcNo, short nAxisNo, short nEmptyMode, short nIoType, int nIoCtrlPinMask, int nIoCtrlEndVal);

	/**
	  * @brief      ListMotion 노드를 등록합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nNodeCount      전송할 노드 수량(최대 25개)
	  * @param[in]  pnMode          노드마다 설정되는 제어모드(0=상대위치/사다리꼴, 1=절대위치/사다리꼴, 2=상대위치/초-Curve, 3=절대위치/초-Curve)
	  * @param[in]  pdPos           등록되는 노드마다 이동할 거리 배열
	  * @param[in]  pdStart         시작속도 값 배열
	  * @param[in]  pdAcc           가속도 값 배열
	  * @param[in]  pdDec           감속도 값 배열
	  * @param[in]  pdDrvSpeed      구동속도 값 배열
	  * @param[in]  pnIoCtrlVal     등록되는 노드마다 출력할 Pin 설정값
	  * @param[out] pnRetErrCode    반환되는 에러코드(0=정상등록, 1=버퍼초과)
	  * @param[out] pnRetRemainNum  총 512개의 버퍼에서 남은 공간 반환
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ListMotAddNodes(short nNmcNo, short nAxisNo, short nNodeCount, short *pnMode, double *pdPos,
		double *pdStart, double *pdAcc, double *pdDec, double *pdDrvSpeed, int *pnIoCtrlVal, short *pnRetErrCode, short *pnRetRemainNum);

	/**
	  * @brief      ListMotion 노드 등록을 종료합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  dEndSpeed       종료시 속도
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ListMotCloseNode(short nNmcNo, short nAxisNo, double dEndSpeed);

	/**
	  * @brief      ListMotion 기능을 시작/정지합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         설정할 축 번호
	  * @param[in]  nRunMode        기능 실행(0=정지, 1=실행)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ListMotRunStop(short nNmcNo, short nAxisNo, short nRunMode);

	/**
	  * @brief      전체 축의 ListMotion 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pListMotStatus  상태를 읽어올 NMC_LM_STATUS  구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ListMotGetStatus(short nNmcNo, PNMCLMSTATUS pListMotStatus);

#pragma pack(1)     /*!< Specifies packing alignment for structure */
	/**
	  * @brief      연속보간 상태정보 구조체
	  */
	typedef struct NMC_CONTI_STATUS
	{
		short   nContiRun[2];                       /*<! 연속 보간 실행 여부(0=Stop, 1=Run) */
		short   nContiWait[2];                      /*<! Queue상태(0=Queue에 실행 할 Data가 있음, \n
																   1=큐버퍼의 노드를 모두 소진하여 다음 노드 전송을 대기) */
		short   nContiRemainBuffNum[2];             /*<! 비어 있는 큐 버퍼의 수(0 ~ 1024) */
		unsigned int    uiContiExecutionNum[2];     /*<! 실행중인 큐 버퍼의 위치(0 ~ 4,294,967,295) */
		short   nContiStopReason[2];                /*<! 실행중인 연속 보간의 정지 원인 (E-Version only) */
		short   nDummy[4];                          /*<! 예비공간 */
	}NMCCONTISTATUS, *PNMCCONTISTATUS;
#pragma pack()

	/**
	  * @brief      연속보간을 설정합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        연속보간이 적용된 그룹 번호
	  * @param[in]  nAVTRIMode      NMC2:삼각파형 방지기능(0:비활성화, 1=활성화)\n
									E-Ver:헬리컬보간 사용(0:비활성화, 1=활성화)
	  * @param[in]  nEmptyMode      연속보간 Queue 버퍼 소진 시 동작(0=종료, 1=노드추가 대기)
	  * @param[in]  n1stAxis        그룹 내 보간이 적용될 축 번호
	  * @param[in]  n2ndAxis        그룹 내 보간이 적용될 축 번호
	  * @param[in]  n3rdAxis        그룹 내 보간이 적용될 축 번호
	  * @param[in]  dMaxDrvSpeed    최대 구동속도
	  * @param[in]  nIoType         사용 IO타입(0=MDIO, 1=DIO, 2=지정안함)
	  * @param[in]  nIoCtrlPinMask  출력제어 PinMask
	  * @param[in]  nIoCtrlEndVal   종료 시 최종 출력값
	  * @return     PAIX_RETURN_VALUE
	  * @warning    연속보간은 같은 그룹내의 축으로 동작됩니다.
	  */
	short WINAPI nmc_ContiSetMode(short nNmcNo, short nGroupNo, short nAVTRIMode, short nEmptyMode,
		short n1stAxis, short n2ndAxis, short n3rdAxis, double dMaxDrvSpeed, short nIoType, int nIoCtrlPinMask, int nIoCtrlEndVal);

	/**
	  * @brief      연속보간에 사용되는 Queue 버퍼를 초기화 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        연속보간이 적용된 그룹 번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiSetNodeClear(short nNmcNo, short nGroupNo);

	/**
	  * @brief      연속보간의 상태정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pContiStatus    연속보간 상태정보 NMC_CONTI_STATUS 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiGetStatus(short nNmcNo, PNMCCONTISTATUS pContiStatus);

	/**
	  * @brief      2축 직선 노드 등록
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dPos[x]         이동할 절대위치(X,Y)
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeLine2Axis(short nNmcNo, short nGroupNo, double dPos0, double dPos1,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      3축 직선 노드 등록
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dPos[x]         이동할 절대위치(X,Y,Z)
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeLine3Axis(short nNmcNo, short nGroupNo, double dPos0, double dPos1, double dPos2,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      4축 직선 노드 등록
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dPos[x]         이동할 절대위치(X,Y,Z,U)
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeLine4Axis(short nNmcNo, short nGroupNo, double dPos0, double dPos1, double dPos2, double dPos3,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      원호보간 노드등록 (중심위치와 회전각도)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dCenter[x]      원호 중심 절대위치(X,Y)
	  * @param[in]  dAngle          원호 회전각도(음수=CW(정방향), 양수=CCW(역방향))
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeArc(short nNmcNo, short nGroupNo, double dCenter0, double dCenter1, double dAngle,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      원호보간 노드등록 (중심위치와 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dCenter[x]      원호 중심 절대위치(X,Y)
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  nDir            원호 회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeArcCE(short nNmcNo, short nGroupNo, double dCenter0, double dCenter1, double dEnd0, double dEnd1,
		short nDir, double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      원호보간 노드등록 (통과위치와 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dPass[x]        원호 통과 절대위치(X,Y)
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeArcPE(short nNmcNo, short nGroupNo, double dPass0, double dPass1, double dEnd0, double dEnd1,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      원호보간 노드등록 (반지름과 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dRadius         원호의 반지름
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  nLen            원호 이동거리(0=짧은거리, 1=긴거리)
	  * @param[in]  nDir            원호 회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeArcRE(short nNmcNo, short nGroupNo, double dRadius, double dEnd0, double dEnd1,
		short nLen, short nDir, double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      헬리컬보간 노드등록 (중심위치와 회전각도)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dCenter[x]      원호 중심 절대위치(X,Y)
	  * @param[in]  dAngle          원호 회전각도(음수=CW(정방향), 양수=CCW(역방향))
	  * @param[in]  dZPos           Z축 이동 절대위치
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeHelical(short nNmcNo, short nGroupNo, double dCenter0, double dCenter1, double dAngle, double dZPos,
		double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      헬리컬보간 노드등록 (중심위치와 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dCenter[x]      원호 중심 절대위치(X,Y)
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  nDir            원호 회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  dZPos           Z축 이동 절대위치
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeHelicalCE(short nNmcNo, short nGroupNo, double dCenter0, double dCenter1, double dEnd0, double dEnd1,
		short nDir, double dZPos, double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      헬리컬보간 노드등록 (통과위치와 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dPass[x]        원호 통과 절대위치(X,Y)
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  dZPos           Z축 이동 절대위치
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeHelicalPE(short nNmcNo, short nGroupNo, double dPass0, double dPass1, double dEnd0, double dEnd1,
		double dZPos, double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      헬리컬보간 노드등록 (반지름과 종료위치)
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  dRadius         원호의 반지름
	  * @param[in]  dEnd[x]         원호 종료 절대위치(X,Y)
	  * @param[in]  nLen            원호 이동거리(0=짧은거리, 1=긴거리)
	  * @param[in]  nDir            원호 회전방향(0=CW(정방향), 1=CCW(역방향))
	  * @param[in]  dZPos           Z축 이동 절대위치
	  * @param[in]  dStart          시작속도
	  * @param[in]  dAcc            가속도
	  * @param[in]  dDec            감속도
	  * @param[in]  dDrvSpeed       구동속도
	  * @param[in]  nIoCtrlVal      출력Pin 값
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiAddNodeHelicalRE(short nNmcNo, short nGroupNo, double dRadius, double dEnd0, double dEnd1,
		short nLen, short nDir, double dZPos, double dStart, double dAcc, double dDec, double dDrvSpeed, int nIoCtrlVal);

	/**
	  * @brief      연속보간 노드 등록을 종료합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiSetCloseNode(short nNmcNo, short nGroupNo);

	/**
	  * @brief      연속보간을 실행/정지 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  nRunMode        실행모드(0=Stop, 1=Run)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiRunStop(short nNmcNo, short nGroupNo, short nRunMode);

	/**
	  * @brief      연속보간을 가/감속 정지를 합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nGroupNo        등록된 그룹번호
	  * @param[in]  nStopMode       정지모드(0=감속, 1=긴급)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_ContiStop(short nNmcNo, short nGroupNo, short nStopMode);

	/**
	  * @brief      현재 축의 펄스 출력 여부를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축 번호
	  * @param[out] pnBusyStatus 펄스 출력상태(0=출력 Off, 1=출력 On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetBusyStatus(short nNmcNo, short nAxisNo, short *pnBusyStatus);

	/**
	  * @brief      모든 축(8개)의 펄스 출력 여부를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnBusyStatus    펄스 출력상태 배열 포인터(0=출력 Off, 1=출력 On)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetBusyStatusAll(short nNmcNo, short *pnBusyStatus);

	/**
	  * @brief      축의 지령 위치를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축 번호
	  * @param[out] plCmdPos        값을 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetCmdPos(short nNmcNo, short nAxisNo, int *plCmdPos);

	/**
	  * @brief      축의 엔코더 위치를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         축 번호
	  * @param[out] plEncPos        값을 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetEncPos(short nNmcNo, short nAxisNo, int *plEncPos);

	/**
	  * @brief      축과 위치타입을 지정하여 위치정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nListCount      값을 획득할 위치값의 수량(pdPotList의 배열크기)
	  * @param[in]  pnAxisList      축 번호 배열 포인터 (범위: 0 ~ 7)
	  * @param[in]  pnPosMode       축에서 읽어올 위치 종류 (0=Cmd, 1=Enc)
	  * @param[out] pdPosList       위치값을 읽어올 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetCmdEncListPos(short nNmcNo, short nListCount, short *pnAxisList, short *pnPosMode, double *pdPosList);

	/**
	  * @brief      축의 현재 속도 정보를 읽어 온다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         속도 정보를 읽어올 축 번호
	  * @param[in]  pSpeed          속도 정보가 들어갈 PARASPEED 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @see        NMC_PARA_LOGIC
	  */
	short WINAPI nmc_GetParaSpeed(short nNmcNo, short nAxisNo, PPARASPEED pSpeed);

	/**
	  * @brief      지령위치 정보를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nAxisNo         위치를 읽어올 축 번호
	  * @param[out] pdTargetPos     위치를 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetParaTargetPos(short nNmcNo, short nAxisNo, double *pdTargetPos);

	/**
	  * @brief      모션 전용 출력신호의 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pOutStatus      출력상태를 읽어올 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @see        NMC_AXES_MOTION_OUT
	  */
	short WINAPI nmc_GetAxesMotionOut(short nNmcNo, PNMCAXESMOTIONOUT pOutStatus);

	/**
	  * @brief      현재 출력속도를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pDrvSpeed       출력속도를 읽어올 포인터 변수
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetDriveAxesSpeed(short nNmcNo, double *pDrvSpeed);

	/**
	  * @brief      함수 명령 시점의 8축 전체의 지령속도와 엔코더속도를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pdCmdSpeed      지령속도를 읽어올 배열 포인터
	  * @param[out] pdEncSpeed      엔코더속도를 읽어올 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetAxesCmdEncSpeed(short nNmcNo, double *pdCmdSpeed, double *pdEncSpeed);

	/**
	  * @brief      장치의 현재 축 정지상태를 확인합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pnStopInfo      정지상태를 확인할 배열 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetStopInfo(short nNmcNo, short *pnStopInfo);

	/**
	  * @brief      장치의 현재 축 상태를 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pNmcData        축 상태를 읽어올 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetAxesExpress(short nNmcNo, PNMCAXESEXPR pNmcData);

#pragma pack(1)     /*!< Specifies packing alignment for structure */
	/**
	  * @brief  한 축의 NMC 상태정보 구조체
	  */
	typedef struct TNMC_AXIS_INFO
	{
		short   nBusy;              /*!< 펄스 출력 상태(0=Idle, 1=Busy) */
		short   nError;             /*!< Error 발생 여부(0=None, 1=Error) */
		short   nNear;              /*!< 원점 센서 입력 상태(0=Off, 1=On) */
		short   nPLimit;            /*!< + Limit 센서 입력 상태(0=Off, 1=On) */
		short   nMLimit;            /*!< - Limit 센서 입력 상태(0=Off, 1=On) */
		short   nAlarm;             /*!< 알람 센서 입력 상태(0=Off, 1=On) */
		short   nEmer;              /*!< 그룹별 EMG 입력 상태(0=Off, 1=On) */
		short   nSwPLimit;          /*!< SW +Limit 입력 상태(0=Off, 1=On) */
		short   nInpo;              /*!< Inposition 입력 상태(0=Off, 1=On) */
		short   nHome;              /*!< Home Search 동작 상태(0=동작중, 1=None) */
		short   nEncZ;              /*!< 엔코더 Z상 입력 상태(0=Off, 1=On) */
		short   nOrg;               /*!< 원점 센서 입력 상태(0=Off, 1=On)(NMC-403S 에서만 지원) */
		short   nSReady;            /*!< Servo Ready 입력 상태(0=Off, 1=On) */
		short   nContStatus;        /*!< 연속보간 실행 상태(0=완료, 1=동작중) */
		short   nSwMLimit;          /*!< SW -Limit 입력 상태(0=Off, 1=On) */
		int     lEnc;               /*!< 엔코더 위치(UnitPerPulse 적용되지 않음) */
		int     lCmd;               /*!< 지령 위치(UnitPerPulse 적용되지 않음) */
		double  dEnc;               /*!< 엔코더 위치(UnitPerPulse 적용) */
		double  dCmd;               /*!< 지령 위치(UnitPerPulse 적용) */
		short   nCurrentOn;         /*!< 모터 전류 출력 상태(0=Off, 1=On) */
		short   nServoOn;           /*!< Servo On 출력 상태(0=Off, 1=On) */
		short   nDCCOn;             /*!< DCC 출력 상태(0=Off, 1=On) */
		short   nAlarmResetOn;      /*!< Alarm Reset 출력 상태(0=Off, 1=On) */
		short   nHomeSrchFlag;      /*!< 원점이동 동작여부(0=이동완료, 1=이동중) */
		short   nHomeStatusFlag;    /*!< 원점이동 세부 동작 상태 */
		short   nStopInfo;          /*!< 정지 원인 */
	}NMCAXISINFO, *PNMCAXISINFO;

	/**
	  * @brief  전체 축과 DIO를 포함한 NMC 상태정보 구조체
	  */
	typedef struct TNMC_STATE_INFO
	{
		NMCAXESEXPR         NmcAxesExpr;
		NMCAXESMOTIONOUT    NmcAxesMotOut;
		NMCHOMEFLAG         HomeFlag;
		short               nStopInfo[8];

		short               nInDio[64];
		short               nInExDio[16];
		short               nInMDio[8];

		short               nOutDio[64];
		short               nOutExDio[16];
		short               nOutMDio[8];
	}NMCSTATEINFO, *PNMCSTATEINFO;
#pragma pack()

	/**
	  * @brief      장치의 상태정보(축 상태,모션 전용 출력신호,원점상태, 정지원인,DIO, EXDIO, MDIO) 읽어옵니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[out] pNmcStateInfo   상태정보를 읽어올 구조체 포인터
	  * @param[in]  nStructSize     TNMC_STATE_INFO 구조체 크기
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_GetStateInfo(short nNmcNo, PNMCSTATEINFO pNmcStateInfo, int nStructSize);

	/**
	  * @brief      장치의 상태정보(축 상태,모션 전용 출력신호,원점상태, 정지원인,DIO, EXDIO, MDIO)를 한 축의 상태 정보(축 상태,모션 전용 출력신호,원점상태,정지원인)으로 추출
	  * @param[in]  nAxisNo         추출할 축 번호
	  * @param[in]  pState          상태정보를 읽어온 구조체 포인터
	  * @param[out] pAxis           추출한 상태정보를 받을 구조체 포인터
	  * @return     PAIX_RETURN_VALUE
	  * @details    DLL에서 전체 상태정보 구조체(PNMCSTATEINFO) 값으로부터 축 정보(PNMCAXISINFO)를 추출합니다.
	  */
	void WINAPI nmc_StateInfoToAxisInfo(short nAxisNo, PNMCSTATEINFO pState, PNMCAXISINFO pAxis);

	/**
	  * @brief      펄스 출력의 Mask(축별 Bit조합)를 설정하여 출력을 ON/OFF합니다.
	  * @param[in]  nNmcNo          장치번호
	  * @param[in]  nMask           축별(0~7)Bit조합(1=Mask설정하여 펄스 출력OFF, 0=Mask를 설정하지 않아 펄스 출력)
	  * @return     PAIX_RETURN_VALUE
	  */
	short WINAPI nmc_SetPulseOutMask(short nNmcNo, short nMask);

	/**
	  * @brief      장치의 구성(Composition) 정보를 확인합니다.
	  * @param[in]  nNmfNo          장치번호
	  * @param[out] ptCompo         구성 정보를 읽어올 TNMF_COMPO 구조체 포인터
	  * @return     Enmf_FUNC_RESULT
	  */
	short WINAPI nmc_nmfGetCompo(short nNmfNo, PTNMCNMFCOMPO ptCompo);

	/**************************************************************************************************/

	/**
	  * @brief      DLL 내부에서 사용되는 구조체 입니다.
	  * @warning    사용자 제공 구조체가 아닙니다.\n
	  *             무분별한 편집 및 사용은 제어에 문제가 될 수 있습니다.
	  */
#pragma pack(1)     /*!< Specifies packing alignment for structure */
	typedef struct TNMC_GANTRY
	{
		short   nEnable;
		short   nMainAxis;
		short   nSubAxis;
		short   nRsvd;
	}NMCGANTRYINFO, *PNMCGANTRYINFO;
	typedef struct TNMC_SETUP_INFO
	{
		NMCGANTRYINFO   NmcGantryInfo[2];
	}NMCSETUPINFO, *PNMCSETUPINFO;
#pragma pack()
	/**************************************************************************************************/
	/**
	  * @brief      지금은 사용되지 않으며, 과거 NMC 장치에서 지원되던 함수 및 구조체 입니다.\n
					신규 및 최근 제품의 사용자께서는 아래 함수를 사용하지 마십시오.
	  */
	  // 각 센서의 정지모드 (사용하지 않음)
	typedef struct NMC_STOPMODE
	{
		short   nEmg;
		short   nMLimit;
		short   nPLimit;
		short   nAlarm;
		short   nNear;
		short   nEncZ;
	}STOPMODE, *PSTOPMODE;

	// 연속보간 상태정보 (사용하지 않음)
	typedef struct NMC_CONT_STATUS
	{
		short   nStatus[2];
		short   nExeNodeNo[2];
	}NMCCONTSTATUS, *PNMCCONTSTATUS;

	//short WINAPI nmc_GetAxesCmdSpeed(short nNmcNo, double *pDrvSpeed); // 사용안함
	//short WINAPI nmc_GetAxesEncSpeed(short nNmcNo, double *pdEncSpeed); // 사용안함
	//short WINAPI nmc_SetEncoderMode(short nNmcNo, short nAxisNo, short nMode); // 사용안함
	//short WINAPI nmc_SetPulseMode(short nNmcNo, short nAxisNo, short nMode); // 사용안함
	//short WINAPI nmc_Set2PulseDir(short nNmcNo, short nAxisNo, short nDir);  // 사용안함
	//short WINAPI nmc_Set1PulseDir(short nNmcNo, short nAxisNo, short nDir); // 사용안함
	//short WINAPI nmc_SetPulseActive(short nNmcNo, short nAxisNo, short nPulseActive);   // 사용안함
	//short WINAPI nmc_InterpolationRelCircle(short nNmcNo, short nAxisNo0, double CenterPulse0, double EndPulse0, short nAxisNo1, double CenterPulse1, double EndPulse1, short nDir);    // 사용안함
	//short WINAPI nmc_InterpolationAbsCircle(short nNmcNo, short nAxisNo0, double CenterPulse0, double EndPulse0, short nAxisNo1, double CenterPulse1, double EndPulse1, short nDir);    // 사용안함
	//short WINAPI nmc_GetMDIOInPin(short nNmcNo, short nPinNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_SetMDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	//short WINAPI nmc_GetMDIOInputBit(short nNmcNo, short nBitNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_SetMDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus); //사용안함
	//short WINAPI nmc_SetMDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo); //사용안함
	//short WINAPI nmc_SetMDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	//short WINAPI nmc_GetMDIOInput32(short nNmcNo, int *plInStatus); //사용안함
	//short WINAPI nmc_GetMDIOOutput32(short nNmcNo, int *plOutStatus); //사용안함
	//short WINAPI nmc_SetMDIOOutput32(short nNmcNo, int lOutStatus); //사용안함
	//short WINAPI nmc_GetDIOInPin(short nNmcNo, short nPinNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_GetDIOInputBit(short nNmcNo, short nBitNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_SetDIOOutputBit(short nNmcNo, short nBitno, short nOutStatus); //사용안함
	//short WINAPI nmc_SetDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	//short WINAPI nmc_SetDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo); //사용안함
	//short WINAPI nmc_SetDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	//short WINAPI nmc_GetDIOInput64(short nNmcNo, LONG64 *plInStatus); //사용안함
	//short WINAPI nmc_GetDIOOutput64(short nNmcNo, LONG64 *plOutStatus); //사용안함
	//short WINAPI nmc_SetDIOOutput64(short nNmcNo, LONG64 lOutStatus); //사용안함
	//short WINAPI nmc_GetDIOInput32(short nNmcNo, short nIndex, int *plInStatus); //사용안함
	//short WINAPI nmc_GetDIOOutput32(short nNmcNo, short nIndex, int *plOutStatus); //사용안함
	//short WINAPI nmc_SetDIOOutput32(short nNmcNo, short nIndex, int lOutStatus); //사용안함
	//short WINAPI nmc_GetEXDIOInPin(short nNmcNo, short nPinNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_GetEXDIOInputBit(short nNmcNo, short nBitNo, short *pnInStatus); //사용안함
	//short WINAPI nmc_SetEXDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus); //사용안함
	//short WINAPI nmc_SetEXDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	//short WINAPI nmc_SetEXDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo); //사용안함
	//short WINAPI nmc_SetEXDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	//short WINAPI nmc_GetEXDIOInput32(short nNmcNo, int *plInStatus); //사용안함
	//short WINAPI nmc_GetEXDIOOutput32(short nNmcNo, int *plOutStatus); //사용안함
	//short WINAPI nmc_SetEXDIOOutput32(short nNmcNo, int lOutStatus); //사용안함
	//short WINAPI nmc_GetFirmVersion(short nNmcNo, char *pStr); //사용안함
	//double WINAPI nmc_GetUnitPerPulse(short nNmcNo, short nAxisNo); //사용안함
	//short WINAPI nmc_GetProtocolMethod(short nNmcNo); //사용안함
	//BOOL WINAPI nmc_GetIPAddress(short *pnField0, short *pnField1, short *pnField2, short *pnField3); //사용안함
	//short WINAPI nmc_SetDefaultIPAddress(short nNmcNo); //사용안함
	//short WINAPI nmc_AccOffsetCount(short nNmcNo, short nAxisNo, int lPulse); //사용안함
	//short WINAPI nmc_SetTriggerCfg(short nNmcNo, short nAxisNo, short nCompMode, int lCmpAmount, double dDioOnTime, short nPinNo, short nDioType, short nReserve); //사용안함
	//short WINAPI nmc_SetTriggerEnable(short nNmcNo, short nAxisNo, short nEnable); //사용안함
	//short WINAPI nmc_SetSyncSetup(short nNmcNo, short nMainAxisNo, short nSubAxisNoEnable0, short nSubAxisNoEnable1, short nSubAxisNoEnable2); // 사용안함
	//short WINAPI nmc_SetSync(short nNmcNo, short nGroupNo, short *pnSyncGrpList0, short *pnSyncGrpList1); // 사용안함
	//short WINAPI nmc_SyncFree(short nNmcNo, short nGroupNo); // 사용안함
	//short WINAPI nmc_SetDisconectedStopMode(short nNmcNo, int lTimeInterval, short nStopMode); // 사용안함
	//short WINAPI nmc_ContRun(short nNmcNo, short nGroupNo, short nRunMode); // 사용안함
	//short WINAPI nmc_GetContStatus(short nNmcNo, PNMCCONTSTATUS pContStatus); // 사용안함
	//short WINAPI nmc_SetContNodeLine(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1,
	//	double dPos0, double dPos1, double dStart, double dAcc, double dDec, double dDriveSpeed); // 사용안함
	//short WINAPI nmc_SetContNodeLineIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1,
	//	double dPos0, double dPos1, double dStart, double dAcc, double dDec, double dDriveSpeed, short nOnOff); // 사용안함
	//short WINAPI nmc_SetContNode3Line(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
	//	double dPos0, double dPos1, double dPos2, double dStart, double dAcc, double dDec, double dDriveSpeed); // 사용안함
	//short WINAPI nmc_SetContNode3LineIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, short nAxisNo2,
	//	double dPos0, double dPos1, double dPos2, double dStart, double dAcc, double dDec, double dDriveSpeed, short nOnOff); // 사용안함
	//short WINAPI nmc_SetContNodeArc(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1,
	//	double dCenter0, double dCenter1, double dAngle, double dStart, double dAcc, double dDec, double dDriveSpeed); // 사용안함
	//short WINAPI nmc_SetContNodeArcIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1,
	//	double dAngle, double dStart, double dAcc, double dDec, double dDriveSpeed, short nOnOff); // 사용안함
	//short WINAPI nmc_ContNodeClear(short nNmcNo, short nGroupNo); // 사용안함
	//short WINAPI nmc_ContSetIO(short nNmcNo, short nGroupNo, short nIoType, short nIoPinNo, short nEndNodeOnOff); // 사용안함
	short WINAPI nmc_SetSpecialFunction(short nNmcNo, short nData); //
	short WINAPI nmc_GetPowerResetCount(short nNmcNo, int *plResetCount); // 장치 전원 On/Off Counter
	short WINAPI nmc_GetGantryAxis(short nNmcNo, short *pnMainAxes, short *pnSubAxes); //
	short WINAPI nmc_GetSetupInfo(short nNmcNo, PNMCSETUPINFO pNmcSetupInfo, int nStructSize);  //

/**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  // _NMC2_H_
/* END OF FILE */
/**************************************************************************************************/