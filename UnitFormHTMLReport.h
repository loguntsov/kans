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
    static int NClass; // ������� ���������� ����������� ������� ������
    int NumberClass; // ����� ������
public:
    AnsiString FileTemp;// ��� ���������� �����
    AnsiString FileName;// ��� ����� ��� ����������
    //
    __fastcall TMDIhtml(TComponent* Owner);
    void OpenFile(AnsiString FileName);
    void CreateTEMP(void);
    void OpenTMP(void);
    void FileSave(void); // ������ ������
    bool SaveStrings(TStrings *Str);// 
    void FileSaveAs(void); // ������ ������ ��� ...
    void FileLoad(AnsiString FileName); // �������� ������
    void Print(void);
    bool Clear(void);
    bool Modified;

   
};
//---------------------------------------------------------------------------
extern PACKAGE TMDIhtml *MDIhtml;
//---------------------------------------------------------------------------
#endif
