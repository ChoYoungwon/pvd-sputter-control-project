#include "FaRecipeAnal.h"
#include "CimsInterface.h"

#include "kutlfile.h"

#define	MAX_BM_CHAMBER_DEPTH	4
#define MAX_SLOT				10


int  ipIOSlot[10]	= { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int	 ipIOSCH = -1;


enum { eUse, 	eNotUse };

BOOL Get_DIO_Address( char *name , int *ioaddr ) {
	if ( *ioaddr == -2 ) {
		*ioaddr = _dll_FIND_FROM_STRING( _K_D_IO , name );
	}
	if ( *ioaddr == -1 ) return FALSE;
	return TRUE;
}

//=========================================================================================
__declspec(dllexport) void USER_RECIPE_DATA_INIT() {
	//
}
//=========================================================================================
__declspec(dllexport) int USER_RECIPE_DATA_MODE() {
	return TYPE_LOT_ALL_CLUSTER_ALL;
}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_DEFAULT_DATA_READ( int REGID , char *InfoStr , int *SC , int *EC , char *JobStr , char *LotStr , char *MidStr , int *SS , int *ES , int *LC ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_DEFAULT_DATA_REGIST( char *InfoStr , int *SC , int *EC , char *JobStr , char *LotStr , char *MidStr , int *SS , int *ES , int *LC , char *DispStr ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_PM_MODE( int REGID , int side ) {
//	return FALSE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_INOUT_DATA_READ( int REGID , int side , OUTCASSETTETemplate *OUT_CASSETTE ) {
//	return TRUE;
//}
//=========================================================================================
__declspec(dllexport) BOOL USER_RECIPE_LOT_DATA_READ( int REGID , int side , LOTStepTemplate *LOT_RECIPE , char *RunName ) {
	
	return TRUE;
}
//=========================================================================================
__declspec(dllexport) BOOL USER_RECIPE_LOT_DUMMY_DATA_READ( int REGID , int side , LOTStepTemplate2 *LOT_RECIPE_DUMMY , char *RunName ) {

	return TRUE;
}
//=========================================================================================
__declspec(dllexport) BOOL USER_RECIPE_CLUSTER_DATA_READ( int REGID , int side , CLUSTERStepTemplate *CLUSTER_RECIPE , int Slot , char *SlotName ) {
/*	
	int  iCommStaus;
	int  iSlotUse;
	int  i, j;

	int  iModuleSts;
	char szRecipeName[65];
	
	//---------------------------------------------------------------------------------------------------------
	// [2016.05.18] PM3, PM5, PM6 Recipe Copy for Dummy Process
	for ( i =0 ; i < MAX_CLUSTER_DEPTH; i++ ) {
		// PM1
		if ( strlen ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][0] ) > 0 ) {
			memset ( szRecipeName, 0, sizeof ( szRecipeName ) );
			strncpy ( szRecipeName, CLUSTER_RECIPE->MODULE_IN_RECIPE[i][0], 64 );
			iModuleSts = CLUSTER_RECIPE->MODULE[i][0];

			for ( j = 0;  j < 7; j++ ) {
				
				if ( ipIOSlot[j] != -1 ) { 
					iSlotUse = _dll_dREAD_DIGITAL ( ipIOSlot[j], &iCommStaus );
				}
				else {
					iSlotUse = eUse;
				}
				if ( j == 0 ) {
					if ( iSlotUse == eUse ) {
						strncpy ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][j], szRecipeName, 64 );
						CLUSTER_RECIPE->MODULE[i][j] = iModuleSts;
					}
					else {
						strncpy ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][j], "", 64 );
						CLUSTER_RECIPE->MODULE[i][j] = 0;
					}
				}
				else {
					if ( iSlotUse == eUse ) {
						strncpy ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][5+j], szRecipeName, 64 );
						CLUSTER_RECIPE->MODULE[i][5+j] = iModuleSts;
					}
					else {
						strncpy ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][5+j], "", 64 );
						if ( i ==0 ) { printf ( "--->%d\n", CLUSTER_RECIPE->MODULE[i][6+j] ); }
						CLUSTER_RECIPE->MODULE[i][5+j] = 0;
					}
				}
				
			}
		}
		// PM3
		if ( strlen ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][2] ) > 0 ) {
			strncpy ( CLUSTER_RECIPE->MODULE_OUT_RECIPE[i][2], CLUSTER_RECIPE->MODULE_IN_RECIPE[i][2], 64 );
			//printf( "[Recipe AnalDll] PM3 IN[%s] OUT[%s]\n", CLUSTER_RECIPE->MODULE_IN_RECIPE[i][2], CLUSTER_RECIPE->MODULE_OUT_RECIPE[i][2] );
		}
		//PM5
		if ( strlen ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][4] ) > 0 ) {
			strncpy ( CLUSTER_RECIPE->MODULE_OUT_RECIPE[i][4], CLUSTER_RECIPE->MODULE_IN_RECIPE[i][4], 64 ); 
		}
		//PM6
		if ( strlen ( CLUSTER_RECIPE->MODULE_IN_RECIPE[i][5] ) > 0 ) {
			strncpy ( CLUSTER_RECIPE->MODULE_OUT_RECIPE[i][5], CLUSTER_RECIPE->MODULE_IN_RECIPE[i][5], 64 ); 
		}
	}
	*/
	//--------------------------------------------------------------------------------------------------------

	return TRUE;
}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MAPPING_VERIFICATION( int REGID , int side , int position , CASSETTEVerTemplate MAPPING_INFO ) {
//	return TRUE;
//}

//=========================================================================================
//=========================================================================================
__declspec(dllexport) int USER_RECIPE_MANUAL_DATA_MODE() {
	return TYPE_LOT_ALL_CLUSTER_ALL;
}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_DEFAULT_DATA_READ( int REGID , char *InfoStr , int *SC , int *EC , char *JobStr , char *LotStr , char *MidStr , int *SS , int *ES , int *LC ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_PM_MODE( int REGID , int side ) {
//	return FALSE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_INOUT_DATA_READ( int REGID , int side , OUTCASSETTETemplate *OUT_CASSETTE ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_LOT_DATA_READ( int REGID , int side , LOTStepTemplate *LOT_RECIPE , char *RunName ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_LOT_DUMMY_DATA_READ( int REGID , int side , LOTStepTemplate2 *LOT_RECIPE_DUMMY , char *RunName ) {
//	return TRUE;
//}
//=========================================================================================
__declspec(dllexport) BOOL USER_RECIPE_MANUAL_CLUSTER_DATA_READ( int REGID , int side , CLUSTERStepTemplate *CLUSTER_RECIPE , int Slot , char *SlotName ) {
	return TRUE;
}
__declspec(dllexport) BOOL USER_RECIPE_INFO_VERIFICATION_EX( int REGID , 
															int side , 
															int pos , 
															int loopcount , 
															int orderindex , 
															int id ,
															CLUSTERStepTemplate CLUSTER_INFO ,
															int LotSpecialData , 
															char *LotUserData , 
															char *ClusterUserData ) {
	
	
	//	int  CommStatus, UseData, UserOP;
	int i=0, j=0, k=0, idata=0;
	double ddata;
	char cdata;
	char sdata[1024];

	char szPath1[128];
	char szPath2[1024];
	char szPath3[128];
	char szPath4[1024];

	char szIOName[65];

	int  iCommStaus;
	int  iModuleSts1, iModuleSts2 ;
	int Result;
	double dbChuck1Temp=0, dbChuck2Temp=0;


	//pos mapping Au EA 
	//orderindex = slot no.	

	STR_SEPERATE_CHAR(CLUSTER_INFO.MODULE_IN_RECIPE[0][0], '\\', szPath1, szPath2, 1024);

	printf("[USER_RECIPE_INFO_VERIFICATION_EX] ==> path1 : [%s], path2 : [%s]\n", szPath1, szPath2);

	return TRUE;
	

	if ( orderindex > 0 && pos == 0 ) {
		sprintf(szIOName, "PM1.avRecipe_Chuck_Temp" );
		iModuleSts1 = _dll_FIND_FROM_STRING( _K_A_IO , szIOName );
		sprintf(szIOName, "PM2.avRecipe_Chuck_Temp" );
		iModuleSts2 = _dll_FIND_FROM_STRING( _K_A_IO , szIOName );
		
		for ( i =0 ; i < MAX_CLUSTER_DEPTH; i++ ) {		//MAX_CLUSTER_DEPTH
			for ( k =0 ; k < MAX_CHAMBER_DEPTH; k++ ) { //MAX_CHAMBER_DEPTH
				//printf("[FARecipeAnal] Param=[%d][%d][%d][%d][%d][%d][%s]\n",side,pos,orderindex,id,k, CLUSTER_INFO.MODULE[i][k], CLUSTER_INFO.MODULE_IN_RECIPE[i][k]);
				for ( j = 1 ; j <= 200 ; j++ ) {				
					//-----------------------------------------------------------------------------------------------------------------------------------------------
					if ( CLUSTER_INFO.MODULE[i][k] != 0 ) {
						STR_SEPERATE_CHAR( CLUSTER_INFO.MODULE_IN_RECIPE[i][k], '\\', szPath1, szPath2, 1024 );
						STR_SEPERATE_CHAR( szPath2, '/', szPath3, szPath4, 1024 );
						if ( strcmp( szPath3, "PM1" ) == 0 ) {
							Result = K_RECIPE_STEP_ITEM_READ( CLUSTER_INFO.MODULE_IN_RECIPE[i][k] , j , "CHK_SET1" , FLOAT_FORMAT , &cdata , &idata , &ddata , sdata );
							if ( Result == 0 ) {
								dbChuck1Temp = ddata;
								printf( "[FARecipeAnal] Chuck Temp1[%.0lf] id[%d]\n", dbChuck1Temp, id );
								_dll_dWRITE_ANALOG( iModuleSts1, dbChuck1Temp, &iCommStaus );
							}
							else{
								if( Result == 12 ) {	//
									printf ( "[FARecipeAnal] file open error\n");
									return FALSE;
								}
								else if ( Result == 9 ) { //step end
									break;
								}
							}
						}
						if ( strcmp( szPath3, "PM2" ) == 0 ) {
							Result = K_RECIPE_STEP_ITEM_READ( CLUSTER_INFO.MODULE_IN_RECIPE[i][k] , j , "CHK_SET1" , FLOAT_FORMAT , &cdata , &idata , &ddata , sdata );
							
							if ( Result == 0 ) {
								dbChuck2Temp = ddata;
								printf( "[FARecipeAnal] Chuck Temp2[%.0lf] id[%d]\n", dbChuck2Temp, id );
								_dll_dWRITE_ANALOG( iModuleSts2, dbChuck2Temp, &iCommStaus );
							}
							else{
								if( Result == 12 ) {	//
									printf ( "[FARecipeAnal] file open error\n");
									return FALSE;
								}
								else if ( Result == 9 ) { //step end
									break;
								}
							}
						}
					}
				}
			}
		}
		
		
		

	}
	
	return TRUE;
}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_MANUAL_MAPPING_VERIFICATION( int REGID , int side , int position , CASSETTEVerTemplate MAPPING_INFO ) {
//	return TRUE;
//}
//=========================================================================================
//__declspec(dllexport) BOOL USER_RECIPE_INFO_VERIFICATION( int REGID , int side , int loopcount , int id , CLUSTERStepTemplate CLUSTER_INFO ) {
//	return TRUE;
//}
//=========================================================================================

__declspec(dllexport) BOOL USER_RECIPE_START_MODE_CHANGE( int REGID , int side , int loopcount , int firstmode , int *startmode ) {
	return TRUE;
}


//=============================================================================================================

int address = -2;

__declspec(dllexport) BOOL USER_RECIPE_SP_N101_SEPSUPPLY( int side , int *count ) {
	int data , CommStatus;
	if ( !Get_DIO_Address( "FM.BATCH_SEPARATE_COUNT" , &address ) ) return FALSE;
	data = _dll_dREAD_DIGITAL( address , &CommStatus );
	if ( data <= 0 ) return FALSE;
	*count = data;
	return TRUE;
}
