//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------
USEFORM("Main.cpp", MainFormKansProgram);
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("About.cpp", AboutBox);
USEFORM("ReportUnit.cpp", MDIReport);
USEFORM("UNITEXEC.CPP", FormExec);
USEFORM("UnitFormHTMLReport.cpp", MDIhtml);
USEFORM("UnitMdiMap.cpp", MdiMap);
//---------------------------------------------------------------------------
#include "WinUser.h"
#include "Main.h"
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR Command, int)
{
    double x=time(NULL);
    //if (2*x>2*1067157422) return 0;
	Application->Initialize();
	Application->Title = " анализационные сети города";
    Application->CreateForm(__classid(TMainFormKansProgram), &MainFormKansProgram);
         Application->CreateForm(__classid(TMdiMap), &MdiMap);
         HANDLE hOpen=CreateEvent(NULL,TRUE,FALSE,"KansExeIsRuning");
    if (GetLastError() == ERROR_ALREADY_EXISTS) // ≈сли приложение уже запущено
    {
        SetLastError(0);
        ///HWND App=GetNextWindow(NULL,
        ///while (Get
        HWND App=FindWindow("TMainFormKansProgram",NULL);
        App=FindWindow("TMainFormKansProgram",NULL);
        ShowMessage(GetLastError());
        TMessageOpenFile a;
        a.Msg = WM_OpenFile;
        a.Str = "1234567890";
        SendMessage(App,WM_OpenFile,WPARAM(&a),long(&a));
        // Ёта возможность пока не используетс€
        return 0;
     }

    //if (x>1022028563) return 0;

    Application->Run();
    CloseHandle(hOpen);
	return 0;
}
//---------------------------------------------------------------------
