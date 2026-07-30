#ifndef C_MODULE_H
#define C_MODULE_H


typedef enum { LOADON, UNLOADON } LPLAMPCON;

void ALARM_MESSAGE_REMAPPING( int id, char *HWErrorNo );
int HW_Alarm_Notify( void );
Module_Status HW_AlarmClear( void );
int READ_Digital_Trigger( int IO, int *OldValue, int *ChangeFlag );

int CheckFailureWaferInFOUP( void );
void LoadUnloadLampControl( LPLAMPCON SetValue );
void VISION_DCOP_CLEAR( void );
void VISION_DCOP_WRITE( void );

Module_Status ReadyStatusCheck( void );
Module_Status HomeStatusCheck( void );

Module_Status Mapping_Data_Clear( void );
// Module_Status GetMappingData( void );
Module_Status Robot_Arm_Retract_Check( void );
Module_Status WRITE_DIGITAL_TRUE( int io, int Data );
Module_Status READ_DIGITAL_TRUE( int io, int *Data );

#endif