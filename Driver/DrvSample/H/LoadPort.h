#ifndef __LoadPort__H__
#define __LoadPort__H__

#include "Define.h"

int ControlLoadPort( int ID2, int Data );
int ControlLEDLamp( int ID2, int Data );

// int GetDuraPortStatus();
int UpdateLoadPortStatus(char *statusData, int count );
void UpdateIndicatorStatus( char *statusData );
int UpdateLampStatus( int ID2, int ID3, int ID4 );
int UpdateMappingData( char *mappingData, int count );
int GetMappingDataFromMemory( int ID2 );
int SetLoadPortMode( EPortMode mode );
int GetAlarmData( int ID2 );
int GetFirmwareVersion( );
void SetDefaultValue( );
void UpdateErrorInform( char *errorInform, int count );
void SaveLog( ELogMethod logMethod, char *logType, char *logMessage );
void Polling( void );
RESPONSETYPE AnalysisResponseData( char *strResponse, int count );
void EventDataReset();

#endif
