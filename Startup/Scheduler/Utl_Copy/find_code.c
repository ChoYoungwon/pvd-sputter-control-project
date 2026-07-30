#include <stdio.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <direct.h>
#include <shlobj.h>

#include <windows.h>
#include <wingdi.h>
#include <winbase.h>
#include <windowsx.h>

#include <commctrl.h>
#include <sys/timeb.h>
#include <math.h>

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
int main( int argc , char *argv[] ) {
	if ( argc == 3 ) {
		return CopyFile( argv[1] , argv[2] , FALSE );
	}
	return 0;
}
