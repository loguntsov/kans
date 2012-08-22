//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "About.h"
#include "ChildWin.h"
#include "UnitFormHTMLReport.h"
#include "Define.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainFormKansProgram *MainFormKansProgram;
//---------------------------------------------------------------------------

__fastcall TMainFormKansProgram::TMainFormKansProgram(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::CreateMDIChild(String Name)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->Caption = Name;
	/*if (FileExists (Name))
		Child->Memo1->Lines->LoadFromFile(Name);*/
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FileNew1Execute(TObject *Sender)
{
	CreateMDIChild("NONAME" + IntToStr(MDIChildCount + 1));
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FileOpen1Execute(TObject *Sender)
{
	if (OpenDialog->Execute())
    {
        AnsiString x=OpenDialog->FileName.SubString(OpenDialog->FileName.Length()-4,4).LowerCase();
        if (x!=".kan")
        {
            (new TMDIReport(this))->FileLoad(OpenDialog->FileName);
        } else
        {
            (new TMDIChild(this))->FileLoad(OpenDialog->FileName);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::HelpAbout1Execute(TObject *Sender)
{
    TAboutBox *Help=new TAboutBox(this);
	Help->ShowModal();
    delete Help;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TMainFormKansProgram::FileSave1Execute(TObject *Sender)
{
    switch(ActiveMDIChild->Tag)
    {
        case 1: ((TMDIChild *) ActiveMDIChild)->FileSave();break;
        case 2: ((TMDIReport *) ActiveMDIChild)->FileSave();break;
        case 3: ((TMDIhtml *) ActiveMDIChild)->FileSave();break;        
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FileSave1Update(TObject *Sender)
{
    if (ActiveMDIChild!=NULL)
    {
        switch(ActiveMDIChild->Tag)
        {
            case 2 : FileSave1->Enabled=true;break;
            case 1 : FileSave1->Enabled=true;break;
            case 3 : FileSave1->Enabled=true;break;            
            default: FileSave1->Enabled=false;
        }
    } else FileSave1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::CalculateCalculationExecute(TObject *Sender)
{
    switch(ActiveMDIChild->Tag)
    {
        case 1: ((TMDIChild *) ActiveMDIChild)->Calculate();break;
        default:
        ;
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainFormKansProgram::CalculateCalculationUpdate(TObject *Sender)
{
    if (ActiveMDIChild!=NULL)
    {
        switch(ActiveMDIChild->Tag)
        {
            case 1 : CalculateCalculation->Enabled=true;break;
            default: CalculateCalculation->Enabled=false;
        }
    } else CalculateCalculation->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::PrevPageUpdate(TObject *Sender)
{
    if (ActiveMDIChild!=NULL)
    {
        switch(ActiveMDIChild->Tag)
        {
            case 1 :
               if  (((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex==0) PrevPage->Enabled=false; else PrevPage->Enabled=true;
               break;
            default:
                PrevPage->Enabled=false;
        }
    } else PrevPage->Enabled=false;

}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::NextPageUpdate(TObject *Sender)
{
    if (ActiveMDIChild!=NULL)
    {
        switch(ActiveMDIChild->Tag)
        {
            case 1 :
               if  (((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex==3) NextPage->Enabled=false; else NextPage->Enabled=true;
               break;
            default:
                NextPage->Enabled=false;
        }
    } else NextPage->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::NextPageExecute(TObject *Sender)
{
    ((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex=((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex+1;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::PrevPageExecute(TObject *Sender)
{
    ((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex=((TMDIChild *) ActiveMDIChild)->PageControl1->ActivePageIndex-1;
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FileSaveAs1Execute(TObject *Sender)
{
    switch(ActiveMDIChild->Tag)
    {
        case 2: ((TMDIReport *) ActiveMDIChild)->FileSaveAs();
        case 3: ((TMDIhtml *) ActiveMDIChild)->FileSaveAs();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainFormKansProgram::FilePrintUpdate(TObject *Sender)
{
    if (ActiveMDIChild!=NULL)
    {
        switch(ActiveMDIChild->Tag)
        {
            //case 3 :
            case 2 :
                FilePrint->Enabled=true;break;
            default:
                FilePrint->Enabled=false;
        }
    } else FilePrint->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TMainFormKansProgram::FilePrintExecute(TObject *Sender)
{
    if (PrintDialog1->Execute())
    {
        switch(ActiveMDIChild->Tag)
        {
            case 2 :
                ((TMDIReport *) ActiveMDIChild)->Print();break;
            case 3 :
                ((TMDIhtml *) ActiveMDIChild)->Print();break;
            default:
                FilePrint->Enabled=false;
        }
    }
}
//---------------------------------------------------------------------------

void __stdcall TMainFormKansProgram::OnOpenFile(TMessageOpenFile &Msg)
{
    ShowMessage(Msg.Str);
    SetFocus();

}



void __fastcall TMainFormKansProgram::FormCreate(TObject *Sender)
{
#ifdef __kansForOnlyDataFile__
MessageDlg("Данная программа предназначена для только для ввода исходных данных",
               mtInformation, TMsgDlgButtons() << mbOK, 0);
#endif
}
//---------------------------------------------------------------------------

