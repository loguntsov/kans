//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFormHTMLReport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
//#pragma link "HTMLLite"
#pragma resource "*.dfm"
TMDIhtml *MDIhtml;

int TMDIhtml::NClass=0;
//---------------------------------------------------------------------------
__fastcall TMDIhtml::TMDIhtml(TComponent* Owner)
    : TForm(Owner)
{
 
}
//---------------------------------------------------------------------------

void TMDIhtml::OpenFile(AnsiString FileNa)
{
    FileName=FileNa;
    //WebBrowser->Navigate(WideString(FileName));
}

void TMDIhtml::CreateTEMP(void)
{
    FileTemp=GetCurrentDir()+"\\kans"+AnsiString(NumberClass)+".tmp";
    FileClose(FileCreate(FileTemp));
    FileName="";
}

void TMDIhtml::OpenTMP(void)
{
    WebBrowser->Navigate(WideString(FileTemp));
    FileName="";
}
void TMDIhtml::FileSave(void) // Запись отчета
{
    if (FileName=="") FileSaveAs();
    else
    {
        Memo1->Lines->SaveToFile(FileName);
        //RichBox->Lines->SaveToFile(FileName);
        Modified=false;
        //Caption=FileName;
    }
}
void TMDIhtml::FileSaveAs(void)
{
    if (SaveDialog->Execute())
    {
        FileName=SaveDialog->FileName;
        FileSave();
    }
}
void TMDIhtml::FileLoad(AnsiString File)
{
    OpenFile(File);
/*    Clear();
    FileName=File;
    RichBox->Lines->LoadFromFile(File);
    RichBox->Modified=false;
    Caption=File;*/

}
bool TMDIhtml::Clear(void)
{
    if (Modified)
    {
        switch(MessageBox(Handle, "Файл не сохранен. Сохранить ?","",MB_YESNOCANCEL | MB_ICONQUESTION))
        {
            case IDYES : FileSave();break;
            case IDCANCEL : Abort();break;
        }
    }
    if (FileTemp!="")
    {
        DeleteFile(FileTemp);
    }
    FileName="";FileTemp="";
    return True;
}
void TMDIhtml::Print(void)
{
    //Printer()->BeginDoc();
    //WebBrowser->PaintTo(Printer()->Handle, 0, 0);
    //Printer()->EndDoc();
}
bool TMDIhtml::SaveStrings(TStrings *Str)
{
    if (FileName!="")
    {
        Str->SaveToFile(FileName);
    } else
    if (FileTemp!="") Str->SaveToFile(FileTemp);
    else return false;
    return true;
}


void __fastcall TMDIhtml::FormClose(TObject *Sender,
        TCloseAction &Action)
{
    if (FileTemp!="")
    {
        DeleteFile(FileTemp);
    }
    //Memo1->Clear();
try
{
    Action = caFree;
}
catch (...)
{
}
    //delete htmlLite1;
}
//---------------------------------------------------------------------------

void __fastcall TMDIhtml::FormCreate(TObject *Sender)
{
   NClass++;
    NumberClass=NClass;
    FileTemp="";
    FileName="";
    Modified=false;    
}
//---------------------------------------------------------------------------

