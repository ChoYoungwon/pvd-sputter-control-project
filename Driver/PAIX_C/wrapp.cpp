
#include "NMC2.h"


extern "C"  short   fn_nmc_OpenDevice(short nNmcNo) {
		
	return nmc_OpenDevice(nNmcNo);
}

extern "C"	void	fn_nmc_CloseDevice(short nNmcNo) {

	return nmc_CloseDevice(nNmcNo);
}

extern "C"	short  fn_nmc_PingCheck(short nNmcNo, int lWaitTime) {

	return nmc_PingCheck(nNmcNo, lWaitTime);
}

extern "C"	short  fn_nmc_GetDIOInput(short nNmcNo, short* pnInStatus) {

	return  nmc_GetDIOInput(nNmcNo, pnInStatus);
}

extern "C"	short   fn_nmc_GetAxesExpress(short nNmcNo, PNMCAXESEXPR pNmcData) {

	return nmc_GetAxesExpress(nNmcNo, pNmcData);
}

extern "C"	short  fn_nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus) {

	return nmc_SetDIOOutPin(nNmcNo, nPinNo, nOutStatus);
}

extern "C" short  fn_nmc_SetServoOn(short nNmcNo, short nAxisNo, short nOut) {

	return nmc_SetServoOn(nNmcNo, nAxisNo, nOut);
}

extern "C" short  fn_nmc_HomeMove(short nNmcNo, short nAxisNo, short nHomeMode, short nHomeEndMode, double dOffset, short nReserve) {

	return nmc_HomeMove(nNmcNo, nAxisNo, nHomeMode, nHomeEndMode, dOffset, nReserve);
}

extern "C" short  fn_nmc_SetCurrentOn(short nNmcNo, short nAxisNo, short nOut) {

	return nmc_SetCurrentOn(nNmcNo, nAxisNo, nOut);
}

extern "C" short  fn_nmc_SuddenStop(short nNmcNo, short nAxisNo) {

	return nmc_SuddenStop(nNmcNo, nAxisNo);
}

extern "C" short  fn_nmc_JogMove(short nNmcNo, short nAxisNo, short nDir) {

	return nmc_JogMove(nNmcNo, nAxisNo, nDir);
}

//extern "C" short  fn_nmc_SetSCurveSpeedCh(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {
//
//	return nmc_SetSCurveSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
//}
//
//extern "C" short   fn_nmc_SetHomeSpeedCh(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {
//
//	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
//}
//
//extern "C" short  fn_nmc_SetSCurveSpeedNoz(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {
//
//	return nmc_SetSCurveSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
//}
//
//extern "C" short   fn_nmc_SetHomeSpeedNoz(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {
//
//	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
//}
//
//extern "C" short  fn_nmc_SetSCurveSpeedRb(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {
//
//	return nmc_SetSCurveSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
//}
//
//extern "C" short   fn_nmc_SetHomeSpeedRb(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {
//
//	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
//}
//
//extern "C" short  fn_nmc_SetSCurveSpeedCh2(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {
//
//	return nmc_SetSCurveSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
//}
//
//extern "C" short   fn_nmc_SetHomeSpeedCh2(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {
//
//	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
//}
//
//extern "C" short  fn_nmc_SetSCurveSpeedDev(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {
//
//	return nmc_SetSCurveSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
//}
//
//extern "C" short   fn_nmc_SetHomeSpeedDev(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {
//
//	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
//}
//

extern "C" short  fn_nmc_SetSpeed(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed) {

	return nmc_SetSpeed(nNmcNo, nAxisNo, dStartSpeed, dAcc, dDec, dDriveSpeed);
}

extern "C" short   fn_nmc_SetHomeSpeed(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed) {

	return nmc_SetHomeSpeedEx(nNmcNo, nAxisNo, dHomeSpeed0, dHomeSpeed1, dHomeSpeed2, dOffsetSpeed);
}

extern "C" short  fn_nmc_SetCmdPos(short nNmcNo, short nAxisNo, double dPos) {

	return nmc_SetCmdPos(nNmcNo, nAxisNo, dPos);
}

extern "C" short  fn_nmc_SetEncPos(short nNmcNo, short nAxisNo, double dPos) {

	return nmc_SetEncPos(nNmcNo, nAxisNo, dPos);
}

extern "C" short  fn_nmc_AbsMove(short nNmcNo, short nAxisNo, double dPos) {

	return nmc_AbsMove(nNmcNo, nAxisNo, dPos);
}

extern "C" short   fn_nmc_RelMove(short nNmcNo, short nAxisNo, double dAmount) {

	return nmc_RelMove(nNmcNo, nAxisNo, dAmount);
}

extern "C" short   fn_nmc_SetCmdEncPos(short nNmcNo, short nAxisNo, double dPos) {

	return nmc_SetCmdEncPos(nNmcNo, nAxisNo, dPos);
}