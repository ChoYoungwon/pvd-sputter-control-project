#include "FaRecipeAnal.h"

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

BOOL read_form_NAME_FORMAT( HFILE hFilePointer , char *str_return ) {
	char  cc;
	int   ii;
	char  Reader[1025];
	int   wbyte;
	
	wbyte = _lread( hFilePointer , Reader , strlen(BEGIN_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , BEGIN_DELEMETER ) != 0 ) return( FALSE );
	wbyte = _lread( hFilePointer , &ii , sizeof(int) );
	if ( wbyte <= 0 ) return FALSE;
	wbyte = _lread( hFilePointer , Reader , ii );          Reader[wbyte] = 0x00;
	strcpy( str_return , Reader );
	wbyte = _lread( hFilePointer , &cc , sizeof(char) );
	if ( wbyte <= 0 ) return FALSE;
	if ( NAME_FORMAT != cc ) return ( FALSE );
	wbyte = _lread( hFilePointer , Reader , strlen(END_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE;
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , END_DELEMETER ) != 0 ) return( FALSE );
	return( TRUE );
}
//

BOOL read_return_form_MaxLimit( HFILE hFilePointer , char *ReturnNameIndicator , char *type_return , char *cc_return , int *ii_return , float *ff_return , double *dd_return , char *str_return ) {
	char  cc;
	int   ii;
	char  Reader[ 256 ];
	int   wbyte;
	
	wbyte = _lread( hFilePointer , Reader , strlen(BEGIN_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE; // 2002.05.03
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , BEGIN_DELEMETER ) != 0 ) return( FALSE );
	
	wbyte = _lread( hFilePointer , &ii , sizeof(int) );
	if ( wbyte <= 0 ) return FALSE; // 2002.05.03
	wbyte = _lread( hFilePointer , Reader , ii );          Reader[wbyte] = 0x00;
	strcpy( ReturnNameIndicator , Reader );
	
	wbyte = _lread( hFilePointer , Reader , sizeof(char) );
	if ( wbyte <= 0 ) return FALSE; // 2002.05.03
	memcpy( &cc , Reader , sizeof(char) );
	//	if ( cc < NAME_FORMAT || cc > FILENAME_FORMAT ) return ( FALSE ); // 2006.05.15
	//	if ( cc < NAME_FORMAT || cc > FILENAME_X_FORMAT ) return ( FALSE ); // 2006.05.15
	if ( cc < NAME_FORMAT || cc > DOUBLE_X_FORMAT ) return ( FALSE ); // 2008.05.30
	*type_return = cc;
	
	switch ( cc ) {
	case NAME_FORMAT   :
	case NAME_X_FORMAT   :
								break;
	case CHAR_FORMAT   :
	case CHAR_X_FORMAT   :
								wbyte = _lread( hFilePointer , cc_return , sizeof(char) );
								if ( wbyte <= 0 ) return FALSE; // 2002.05.03
								break;
	case INT_FORMAT    :
	case INT_X_FORMAT    :
								wbyte = _lread( hFilePointer , ii_return , sizeof(int) );
								if ( wbyte <= 0 ) return FALSE; // 2002.05.03
								break;
	case FLOAT_FORMAT  :
	case FLOAT_X_FORMAT  :
								wbyte = _lread( hFilePointer , ff_return , sizeof(float) );
								if ( wbyte <= 0 ) return FALSE; // 2002.05.03
								break;
	case DOUBLE_FORMAT  :
	case DOUBLE_X_FORMAT  :
								wbyte = _lread( hFilePointer , dd_return , sizeof(double) );
								if ( wbyte <= 0 ) return FALSE; // 2002.05.03
								break;
	case STRING_FORMAT :
	case FILENAME_FORMAT :
	case STRING_X_FORMAT :
	case FILENAME_X_FORMAT :
								wbyte = _lread( hFilePointer , &ii , sizeof(int) );
								if ( wbyte <= 0 ) return FALSE; // 2002.05.03
								wbyte = _lread( hFilePointer , str_return , ii );
								str_return[wbyte] = 0x00;
								break;
	}
	wbyte = _lread( hFilePointer , Reader , strlen(END_DELEMETER) );
	if ( wbyte <= 0 ) return FALSE; // 2002.05.03
	Reader[wbyte] = 0x00;
	if ( strcmp( Reader , END_DELEMETER ) != 0 ) return( FALSE );
	return( TRUE );
}
int RECIPE_OPEN( const char* RecipeFileName) {
	int mode;
	int  i , Step_Count = 0;
	HFILE hFile;
	BOOL StepEndFind;
	int  kill;
	
	char  cc_return;
	int   ii_return;
	float ff_return;
	double dd_return;
	char  str_return[4096];
	char  name_return[4096];
	char  type_return;

	if ( RecipeFileName == NULL ) return 1;
	hFile = _lopen( RecipeFileName , OF_READ );
	if ( hFile == -1 ) return 2;

	if ( !read_form_NAME_FORMAT( hFile , name_return ) ) {
		_lclose( hFile );
		return 11;
	}

	//
	if ( !read_form_NAME_FORMAT( hFile , name_return ) ) {
		_lclose( hFile );
		return 12;
	}

	if ( !read_form_NAME_FORMAT( hFile , name_return ) ) {
		_lclose( hFile );
		return 13;
	}

	if ( STRCMP_L( name_return , "__&&HEADER_BEGIN&&__" ) ) {
		mode = 0;
	}
	else {
		_lclose( hFile );
		return 14;
	}

	if ( !read_form( hFile , "NumberOfSteps" , INT_FORMAT , &cc_return , &Step_Count , &ff_return , str_return ) ) {
		_lclose( hFile );
		return 15;
	}
	while ( TRUE ) {

		if ( !read_return_form_MaxLimit( hFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) { _lclose( hFile ); return 15; } 
		if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&HEADER_END&&__" ) == 0 ) ) break;
	}
	
	if ( !read_form( hFile , "__&&STEP0_BEGIN&&__" , NAME_FORMAT , &cc_return , &ii_return , &ff_return , str_return ) ) {
		_lclose( hFile );
		return 17;
	}	
		StepEndFind = FALSE;
		while( !StepEndFind ) {

			if ( !read_return_form_MaxLimit( hFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) { 
				_lclose( hFile );
				return 18;
			}
			switch (type_return) {
			case NAME_FORMAT :	
				if ( strcmp( name_return , "__&&STEP0_END&&__" ) == 0 ) {
					StepEndFind = TRUE;
				}
				break;
				
			default :			kill++;
				break;
			}
			if ( kill >= 4096 )	{
				_lclose( hFile );
				return 99;
			}
		}


		for ( i = 0 ; ( i < Step_Count ) && ( i < 999 ) ; i++ ) {
			if ( !read_form( hFile , "__&&STEP_BEGIN&&__"   , NAME_FORMAT   , &cc_return , &ii_return , &ff_return , str_return        ) ) {
				_lclose( hFile );
				return 19;
			}
		//	printf( "STEP[%d]_BEGIN_ID = __&&STEP_BEGIN&&__\n" , i + 1 );
			//
			StepEndFind = FALSE;
			while( !StepEndFind ) {
				if ( !read_return_form_MaxLimit( hFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) { 
					_lclose( hFile );
					return 20;
				}
				switch (type_return) {
		/*		case NAME_FORMAT :	if ( strcmp( name_return , "__&&STEP_END&&__" ) == 0 ) {
					printf( "STEP[%d]_END_ID = __&&STEP_END&&__\n" , i + 1 );
					StepEndFind = TRUE;
									}
					else {
						printf( "\tDATA(n) : %s\n" , name_return );
					}
					break;
				case NAME_X_FORMAT :
					printf( "\t\tDATA-X(n) : %s\n" , name_return );
					break;
				case INT_FORMAT  :	
					printf( "\tDATA(i) : %s = %d\n" , name_return , ii_return );
					break;
					
				case INT_X_FORMAT  :	
					printf( "\t\tDATA-X(i) : %s = %d\n" , name_return , ii_return );
					break;
					*/
				case FLOAT_FORMAT :
					if ( !strcmp( name_return , "RF_POWER")) 
					{
						if ( ff_return > 0 ) return 50;
					}
					break;
					
			/*	case FLOAT_X_FORMAT :
					printf( "\t\tDATA-X(f) : %s = %f\n" , name_return , ff_return );
					break;
					
				case DOUBLE_FORMAT :
					printf( "\tDATA(d) : %s = %f\n" , name_return , dd_return );
					break;
					
				case DOUBLE_X_FORMAT :
					printf( "\t\tDATA-X(d) : %s = %f\n" , name_return , dd_return );
					break;
					
				case STRING_FORMAT :
					printf( "\tDATA(s) : %s = %s\n" , name_return , str_return );
					break;
					
				case STRING_X_FORMAT :
					printf( "\t\tDATA-X(s) : %s = %s\n" , name_return , str_return );
					break;
					
				case FILENAME_FORMAT:
					printf( "\tFILE    : %s = %s\n" , name_return , str_return );
					break;
					
				case FILENAME_X_FORMAT:
					printf( "\t\tFILE-X    : %s = %s\n" , name_return , str_return );
					break;*/
					
				default :			kill++;
					break;
				}
				if ( kill >= 4096 )	{
					_lclose( hFile );
					return 100;
				}
			}
		}
		
		_lclose( hFile );
		return 0;
}
BOOL WINAPI DllMain( HINSTANCE hinstDLL , DWORD fdwReason , LPVOID lpvReserved ) {
	switch ( fdwReason ) {
	case DLL_PROCESS_ATTACH:	return( TRUE );
	case DLL_PROCESS_DETACH:	break;
	case DLL_THREAD_ATTACH:		break;
	case DLL_THREAD_DETACH:		break;
	default:					break;
	}
	return( FALSE );
}

