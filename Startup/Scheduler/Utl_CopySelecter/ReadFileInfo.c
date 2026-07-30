#include <stdio.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

#include <windows.h>
#include <wingdi.h>
#include <winbase.h>
#include <windowsx.h>
#include <commctrl.h>

#include "kutlgui.h"

#include "resource.h"

//
//

BOOL APIENTRY InfoDisplay_Proc( HWND hdlg , UINT msg , WPARAM wParam , LPARAM lParam ) {
	static int select , result;
	switch ( msg ) {
	case WM_INITDIALOG:
		//
		select = -1;
		//
		MoveCenterWindow( hdlg );
		//
		KWIN_Item_String_Display( hdlg , IDOK , "Select" );
		KWIN_Item_Disable( hdlg , IDOK );
		//
		KWIN_Item_String_Display( hdlg , IDC_STRING , "Q.What kind of Scheduling style do you want?" );
		//
		ListBox_ResetContent( GetDlgItem( hdlg , IDC_LIST1 ) );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "1.Path thru Style(BM1->BM2)(2-Slot)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "2.Path thru Style(BM2->BM1)(2-Slot)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "3.Single Swapping Style(2-Slot)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "4.Full Swapping Style(2-Slot)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "5.Batch Style(2-Slot)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "6.Middle Swapping Style(4-Slot:2Slot-In/2Slot-Out)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "7.Batch All(25-Slot) Style" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "8.Batch All(25-Slot) Style(TM-DoubleFinger)" );
		ListBox_AddString( GetDlgItem( hdlg , IDC_LIST1 ) , "9.Batch All(25-Slot) Style(TM-DoubleFinger/FM-5Finger)" );
		//
		return TRUE;

	case WM_COMMAND :
		if ( GET_WM_COMMAND_CMD( wParam , lParam ) == LBN_SELCHANGE ) {
			if ( GET_WM_COMMAND_ID( wParam , lParam ) == IDC_LIST1 ) {
				select = (int) ListBox_GetCurSel( GetDlgItem( hdlg , IDC_LIST1 ) );
				switch( select ) {
				case 0 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 1.Path thru Style(BM1[In:2Slot]->BM2[Out:2Slot])" );		break;
				case 1 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 2.Path thru Style(BM2[In:2Slot]->BM1[Out:2Slot])" );		break;
				case 2 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 3.Single Swapping Style(2-Slot)" );						break;
				case 3 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 4.Full Swapping Style(2-Slot)" );							break;
				case 4 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 5.Batch Style(2-Slot)" );									break;
				case 5 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 6.Middle Swapping Style(4-Slot:2Slot-In/2Slot-Out)" );	break;
				case 6 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 7.Batch All(25-Slot) Style" );							break;
				case 7 :	KWIN_Item_String_Display( hdlg , IDOK , "Select - 8.Batch All(25-Slot) Style(TM-DoubleFinger)" );			break;
				case 8 :	KWIN_Item_String_Display( hdlg , IDOK , "Select-9.Batch All(25-Slot)Style(TM-DoubleFinger/FM-5Finger)" );	break;
				default :	KWIN_Item_Disable( hdlg , IDOK );	break;
				}
			}
			return TRUE;
		}
		switch( wParam ) {
		case IDOK :
			result = TRUE;
			switch( select ) {
			case 0 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(PathThru(12)).cfg"		, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 1 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(PathThru(21)).cfg"		, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 2 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(SingleSwap).cfg"		, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 3 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(FullSwap).cfg"			, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 4 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(Batch).cfg"				, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 5 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(MiddleSwap).cfg"		, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(Normal).cfg"				, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 6 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(BatchAll).cfg"			, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(BatchAll).cfg"			, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 7 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(BatchAll2).cfg"			, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(Normal).cfg"				, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(BatchAll-TMDual).cfg"	, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			case 8 :
				if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-System(BatchAll3).cfg"			, "Scheduler\\System.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\Scheduling-Robot(FM5Finger).cfg"			, "Scheduler\\Robot.cfg" , FALSE );
				//if ( result ) result = CopyFile( "Startup\\Scheduler\\GUI-Sub_Overview(BatchAll-TMDualF5).cfg"	, "gui\\ctc\\Sub_Overview.cfg" , FALSE );
				break;
			default :
				result = FALSE;
				break;
			}
			if ( !result ) {
				MessageBox( hdlg , "Definition File Copy Fail! Last Success Information will be loaded." , "ERROR" , MB_ICONHAND );
			}
			EndDialog( hdlg , 0 );
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow ) {
	GoModalDialogBoxParam( GetModuleHandle(NULL) , MAKEINTRESOURCE( IDD_DIALOG ) , NULL , InfoDisplay_Proc , (LPARAM) NULL );
	return 0;
}
