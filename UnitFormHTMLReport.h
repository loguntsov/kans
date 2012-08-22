//---------------------------------------------------------------------------

#ifndef UnitFormHTMLReportH
#define UnitFormHTMLReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
#include <Dialogs.hpp>
//#include "HTMLLite.hpp"
//---------------------------------------------------------------------------
class TMDIhtml : public TForm
{
__published:	// IDE-managed Components
    TSaveDialog *SaveDialog;
    TCppWebBrowser *WebBrowser;
    TMemo *Memo1;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    static int NClass; // Счетчик количества экземпляров данного класса
    int NumberClass; // Номер класса
public:
    AnsiString FileTemp;// Имя временного файла
    AnsiString FileName;// Имя файла для сохранения
    //
    __fastcall TMDIhtml(TComponent* Owner);
    void OpenFile(AnsiString FileName);
    void CreateTEMP(void);
    void OpenTMP(void);
    void FileSave(void); // Запись отчета
    bool SaveStrings(TStrings *Str);// 
    void FileSaveAs(void); // Запись отчета как ...
    void FileLoad(AnsiString FileName); // Открытие отчета
    void Print(void);
    bool Clear(void);
    bool Modified;

   
};
//---------------------------------------------------------------------------
extern PACKAGE TMDIhtml *MDIhtml;
//---------------------------------------------------------------------------
#endif
