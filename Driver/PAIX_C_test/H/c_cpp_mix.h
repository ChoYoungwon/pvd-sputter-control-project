#ifndef __C_CPP_MIX_H__
#define __C_CPP_MIX_H__

EXTERN_C short  fn_nmc_OpenDevice(short nNmcNo);
EXTERN_C void   fn_nmc_CloseDevice(short nNmcNo);
EXTERN_C short  fn_nmc_PingCheck(short nNmcNo, int lWaitTime);
EXTERN_C short  fn_nmc_GetDIOInput(short nNmcNo, short* pnInStatus);
EXTERN_C short  fn_nmc_GetAxesExpress(short nNmcNo, PNMCAXESEXPR pNmcData);
EXTERN_C short  fn_nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
EXTERN_C short  fn_nmc_SetServoOn(short nNmcNo, short nAxisNo, short nOut);
EXTERN_C short  fn_nmc_HomeMove(short nNmcNo, short nAxisNo, short nHomeMode, short nHomeEndMode, double dOffset, short nReserve);
EXTERN_C short  fn_nmc_SetCurrentOn(short nNmcNo, short nAxisNo, short nOut);
EXTERN_C short  fn_nmc_SuddenStop(short nNmcNo, short nAxisNo);
EXTERN_C short  fn_nmc_JogMove(short nNmcNo, short nAxisNo, short nDir);
//EXTERN_C short  fn_nmc_SetSCurveSpeedCh(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
//EXTERN_C short  fn_nmc_SetHomeSpeedCh(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
//EXTERN_C short  fn_nmc_SetSCurveSpeedNoz(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
//EXTERN_C short  fn_nmc_SetHomeSpeedNoz(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
//EXTERN_C short  fn_nmc_SetSCurveSpeedRb(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
//EXTERN_C short  fn_nmc_SetHomeSpeedRb(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
//EXTERN_C short  fn_nmc_SetSCurveSpeedCh2(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
//EXTERN_C short  fn_nmc_SetHomeSpeedCh2(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
//EXTERN_C short  fn_nmc_SetSCurveSpeedDev(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
//EXTERN_C short  fn_nmc_SetHomeSpeedDev(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
EXTERN_C short  fn_nmc_SetSpeed(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
EXTERN_C short  fn_nmc_SetHomeSpeed(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dHomeSpeed1, double dHomeSpeed2, double dOffsetSpeed);
EXTERN_C short  fn_nmc_SetCmdPos(short nNmcNo, short nAxisNo, double dPos);
EXTERN_C short  fn_nmc_SetEncPos(short nNmcNo, short nAxisNo, double dPos);
EXTERN_C short  fn_nmc_AbsMove(short nNmcNo, short nAxisNo, double dPos);
EXTERN_C short  fn_nmc_RelMove(short nNmcNo, short nAxisNo, double dAmount);
EXTERN_C short  fn_nmc_SetCmdEncPos(short nNmcNo, short nAxisNo, double dPos);
EXTERN_C short  fn_nmc_SetSCurveSpeedNoz(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);
EXTERN_C short  fn_nmc_SetSCurveSpeedCh(short nNmcNo, short nAxisNo, double dStartSpeed, double dAcc, double dDec, double dDriveSpeed);

#endif /* __C_CPP_MIX_H__  */