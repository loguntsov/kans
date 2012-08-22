//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "Kans.h"
#include "Main.h"
#include "DiskSystem.h"
#include "UnitExec.h"
#include "UnitFormHTMLReport.h";
#include "Define.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
const int SaveVer=1;
//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
    x=1;y=1;f=true;
    IsChange=false;
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormCreate(TObject *Sender)
{
    for(int i=0;SortamentName[i]!=NULL;i++)
    {
        SortamentBox->Items->Add(SortamentName[i]->Name);
        SegmentSortamentBox->Items->Add(SortamentName[i]->Name);
    }
    SegmentSortamentBox->ItemIndex=0;
    GeodezicCells->Cells[0][0]="№";
    GeodezicCells->Cells[1][0]="Отметка земли";
    GeodezicCells->Cells[2][0]="Сосредоточенный расход";
    GeodezicCells->Cells[0][1]="1";
    GeodezicCells->Cells[0][2]="2";
    SegmentCells->Cells[0][0]="№ уч-ка";
    SegmentCells->Cells[0][1]="1";
    SegmentCells->Cells[1][0]="Начальная точка,м";
    SegmentCells->Cells[2][0]="Конечная точка,м";
    SegmentCells->Cells[3][0]="Длина,м";
    SegmentCells->Cells[4][0]="Заглубление,м";
    SegmentCells->Cells[5][0]="Путевой расход,м3/сут";
    SegmentCells->Cells[6][0]="Сортамент";
    MDIReport=NULL;
    for(int i=0;NameMest[i]!="";i++)
    {
        MdComboBox->Items->Add(NameMest[i]);
    }
    ComboType->ItemIndex=0;
}
//---------------------------------------------------------------------------


void __fastcall TMDIChild::Calculate(void)
{
#ifndef __kansForOnlyDataFile__
    TFormExec *ExecForm=new TFormExec(this);
    if (ComboType->ItemIndex>0) MinDim=200; else MinDim=150;
    int nSegment=NumberSegment->Text.ToInt();
    ExecForm->ProgressBar1->Max=nSegment*3-1;
    ExecForm->Show();
    Hmin=HminEdit->Text.ToDouble();
    Hmax=HmaxEdit->Text.ToDouble();
    H0max=Hmax*(1.00+dHmaxEdit->Text.ToDouble()/100);
    if (SortamentBox->ItemIndex<0)
    {
        MessageBox(Handle, "Не заданы сортаменты трубопроводов","",MB_OK | MB_ICONERROR);
        PageControl1->ActivePageIndex =0;
        SortamentBox->SetFocus();
        return;                
    }
    CSortamentItem * DefaultSortamentItem=SortamentName[SortamentBox->ItemIndex];
    CGeoPoint **GeoPoints=new CGeoPoint * [nSegment+1];
    CGeoSegment **GeoSegments=new CGeoSegment * [nSegment];
    for(int i=0;i<nSegment+1;i++)
    {
        double Z=GeodezicCells->Cells[1][i+1].ToDouble();
        double q0=0;
        if (GeodezicCells->Cells[2][i+1]!="")
            q0=GeodezicCells->Cells[2][i+1].ToDouble();
        GeoPoints[i]=new CGeoPoint(Z,q0);
        ExecForm->Next();
    }
    CSortamentItem *SortamentItem;
    bool *IsBeginGeoPoint=new bool [nSegment+1];
    for(int i=0;i<nSegment+1;i++) IsBeginGeoPoint[i]=false;
    for(int i=0;i<nSegment;i++)
    {
        int BegGeoPoint=SegmentCells->Cells[1][i+1].ToInt()-1;
        IsBeginGeoPoint[BegGeoPoint]=true;
        int EndGeoPoint=SegmentCells->Cells[2][i+1].ToInt()-1;
        double Len=SegmentCells->Cells[3][i+1].ToDouble();
        double HBeg=0,Q=0;
        if (SegmentCells->Cells[5][i+1]!="") Q=SegmentCells->Cells[5][i+1].ToDouble(); else Q=0;
        if (SegmentCells->Cells[4][i+1]!="") HBeg=SegmentCells->Cells[4][i+1].ToDouble();
        for(int j=0;SortamentName[j]!=NULL;j++)
        {
            if (SegmentCells->Cells[6][i]==SortamentName[j]->Name)
            {
                SortamentItem=SortamentName[j];break;
            } else SortamentItem=DefaultSortamentItem;
        }
        GeoSegments[i]=new CGeoSegment(
            SortamentItem,
            GeoPoints[BegGeoPoint],
            GeoPoints[EndGeoPoint],
            Len,HBeg,Q,
            AnsiString(BegGeoPoint+1)+" x "+AnsiString(EndGeoPoint+1),
            BegGeoPoint+1,EndGeoPoint+1
            );
    }
    int n=0,NumSeg=0;
    for(int i=0;i<nSegment+1;i++)
    {
        ExecForm->Next();
        if (!IsBeginGeoPoint[i])
        {
            n++;NumSeg=i;
        }
    }
    if (n==1)
    {
        GeoPoints[NumSeg]->Calculation();
        TMDIhtml *report=new TMDIhtml(Application);
        report->CreateTEMP();
        //TRichEdit *Rich=Report->RichBox;
        //Rich->Paragraph->LeftIndent = 30;
        //Rich->Paragraph->RightIndent = 30;
        //Rich->Paragraph->FirstIndent = 30;
        TStrings *Str=report->Memo1->Lines;//new TStringList;//report->Memo1->Lines;
        Str->Clear();
        switch(1)
        {
            case 0 :
/*
            //TStrings *Str=new TStrings();
            //Str->Clear();
            //TMDIhtml *form=new TMDIhtml(Application);
    Str->Add("<table width=\"100%\" border=\"1\">");
    Str->Add("  <tr align=\"center\" valign=\"middle\">");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Наименование участка</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Длина, м</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Расход, л/c</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Диаметр, мм</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Уклон, х1000</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">Скорость, м/c</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">h/d</div></td>");
    Str->Add("    <td colspan=\"6\">");
    Str->Add("      <div align=\"center\">Отметки земли</div></td>");
    Str->Add("  </tr>");
    Str->Add("  <tr>");
    Str->Add("    <td colspan=\"3\">");
    Str->Add("      <div align=\"center\">в начале</div></td>");
    Str->Add("    <td colspan=\"3\">");
    Str->Add("      <div align=\"center\">в конце</div></td>");
    Str->Add("  </tr>");
    Str->Add("  <tr>");
    Str->Add("    <td >");
    Str->Add("      <div align=\"center\">земля</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">лоток</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">шелыга</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">земля</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">лоток</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">шелыга</div></td>");
    Str->Add("  </tr>");
    /*            Str->Add("------------------------------------------------------------------------------------------------------------------");
            Str->Add("Наиме- | Длина | Расход|Диаметр|Уклон |Скорость|  h/d  |                        Отметки земли                   |");
            Str->Add("нование|   м   |  л/с  |  мм   |x1000 |  м/с   |   %   |       в начале            ||    в конце                |");
            Str->Add("участка|       |       |       |      |        |       |земля |лоток |шелыга|  H,м ||земля |лоток |шелыга| H,м  |");
            Str->Add("------------------------------------------------------------------------------------------------------------------");
            for(int i=0;i<nSegment;i++)
            {
                Str->Add(GeoSegments[i]->CreateLineTable());
                GeoSegments[i]->CreateSpecialReport(Str);
                ExecForm->Next();
            }
            break;*/
            case 1 :
                Str->Add("<HTML>");
                Str->Add("<HEAD>");
                Str->Add("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\">");
                Str->Add("</HEAD>");
                Str->Add("<BODY>");
                Str->Add("<p><div align=\"center\"><font size=\"+2\">Гидравлический расчет канализационной сети</font></p>");
                Str->Add("<table width=\"100%\" border=\"1\" cellspacing=0 cellpadding=0>");
                Str->Add("  <tr>");
                Str->Add("    <td><div align=\"center\">Номер участка</div></td>");
                Str->Add("    <td><div align=\"center\">Длина, м</div></td>");
                Str->Add("    <td><div align=\"center\">Расход, л/c</div></td>");
                Str->Add("    <td><div align=\"center\">Диаметр, мм</div></td>");
                Str->Add("    <td><div align=\"center\">Уклон, х1000</div></td>");
                Str->Add("    <td><div align=\"center\">Скорость, м/c</div></td>");
                Str->Add("    <td><div align=\"center\">h/d</div></td>");
                Str->Add("    <td> <div align=\"center\">Расход инфил., л/с</div></td>");
                Str->Add("    <td><div align=\"center\">Расход при h/d=0.95</div></td>");
                Str->Add("    <td><div align=\"center\">Примечание</div></td>");
                Str->Add("  </tr>");

            /*Rich->Paragraph->Alignment=taCenter;
            Rich->SelAttributes->Height=12;
            Rich->Lines->Add("Гидравлический расчет канализационной сети");
            Rich->Paragraph->Alignment=taLeftJustify;
            Str->Add("----------------------------------------------------------------------------------------");
            Str->Add("| Номер | Длина | Расход|Диаметр|Уклон |Скорость|  h/d  |Расход| Расход |  Примечание  |");
            Str->Add("|участка|   м   |  л/с  |  мм   |x0.001|  м/с   |       |инфил.|  при   |              |");
            Str->Add("|       |       |       |       |      |        |       |  л/с |h/d=0.95|              |");
            Str->Add("|-------|-------|-------|-------|------|--------|-------|------|--------|--------------|");
            */
            for(int i=0;i<nSegment;i++)
            {
                GeoSegments[i]->CreateLineTableSmall(Str);
                ExecForm->Next();
            }
            Str->Add("</table>");
            /*
            Str->Add("----------------------------------------------------------------------------------------");
            */
            Str->Add("<p><div align=\"left\">Примечание:</p>");
            Str->Add("<p>ГС - гаситель скорости</p>");
            Str->Add("<p>ПК ## - перепадные колодцы в количестве ## штук</p>");
            Str->Add("<p>НСП ## - насосная станция подкачки, установленная на расстоянии ## от начала уч-ка</p>");
            Str->Add("<p>НВ ## - напорные водоводы, в количестве ## штук + НСП устанавливаемая в начале участка</p>");
            Str->Add("<p></p>");
            //Rich->SelAttributes->Height=12;
            //Rich->Paragraph->Alignment=taCenter;
            Str->Add("<p><div align=\"center\"><font size=\"+2\">Отметки по участкам</font></p>");
            Str->Add("  <table width=\"100%\" border=\"1\" cellspacing=0 cellpadding=0>");
            Str->Add("  <tr>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\">Номер участка</div></td>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\">h=i*l</div></td>");
            Str->Add("    <td colspan=\"8\"><div align=\"center\">Отметки земли</div></td>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\"></div>");
            Str->Add("      <div align=\"center\"></div>");
            Str->Add("      <div align=\"center\">Примечание</div></td>");
            Str->Add("  </tr>");
            Str->Add("  <tr>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">Земля</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">Лоток</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">Шелыга</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">Заглубление</div></td>");
            Str->Add("  </tr>");
            Str->Add("  <tr>");
            Str->Add("    <td><div align=\"center\">нач</div></td>");
            Str->Add("    <td><div align=\"center\">кон</div></td>");
            Str->Add("    <td><div align=\"center\">нач</div></td>");
            Str->Add("    <td><div align=\"center\">кон</div></td>");
            Str->Add("    <td><div align=\"center\">нач</div></td>");
            Str->Add("    <td><div align=\"center\">кон</div></td>");
            Str->Add("    <td><div align=\"center\">нач</div></td>");
            Str->Add("    <td><div align=\"center\">кон</div></td>");
            Str->Add("  </tr>");
            //Rich->Paragraph->Alignment=taLeftJustify;
            //Str->Add("-----------------------------------------------------------------------------------");
            //Str->Add("| Номер |      |                      Отметки                        |  Материал  |");
            //Str->Add("|участка| h=i*l|    земля         лоток         шелыга    заглубление| Примечание |");
            //Str->Add("|       |      | нач  |  кон | нач  | кон  | нач  | кон  | нач | кон |            |");
            //Str->Add("|-------|------|------|------|------|------|------|------|-----|-----|------------|");
            for(int i=0;i<nSegment;i++)
            {
                GeoSegments[i]->CreateReportOtm(Str);
                ExecForm->Next();
            }
            /*
            Str->Add("-----------------------------------------------------------------------------------");
            //Rich->Paragraph->Alignment=taRightJustify;
            //Report->Visible=false;
            */
            Str->Add("</Table>");
            Str->Add("</BODY>");
            Str->Add("</HTML>");

            break;
        }

        Str->SaveToFile(report->FileTemp);
        report->OpenTMP();
        //report->htmlLite1->LoadStrings(Str);
        report->Visible=true;
        //delete Str;
    }
    else
    {
       // Ошибка
        ShowMessage("Ошибка в топологии сети!!!");
    }
    for(int i=0;i<nSegment;i++) delete GeoSegments[i];
    for(int i=0;i<nSegment-1;i++) delete GeoPoints[i];
    delete GeoSegments;
    delete GeoPoints;
    delete ExecForm;
#endif __kansForOnlyDataFile__
}
void TMDIChild::LoadSegmentData(int ACol,int ARow)
{
    x=ACol;y=ARow;
    f=false;
    BegGeoPointBox->Text=SegmentCells->Cells[1][ARow];
    EndGeoPointBox->Text=SegmentCells->Cells[2][ARow];
    SegmentLen->Text=SegmentCells->Cells[3][ARow];
    SegmentHBeg->Text=SegmentCells->Cells[4][ARow];
    SegmentQ->Text=SegmentCells->Cells[5][ARow];
    for(int i=0;i<SegmentSortamentBox->Items->Count;i++)
    {
        if (SegmentSortamentBox->Items->Strings[i]==SegmentCells->Cells[6][ARow])
        {
            SegmentSortamentBox->ItemIndex=i;break;
        } else SegmentSortamentBox->ItemIndex=0;
    }
    f=true;
}
void TMDIChild::DownloadSegmentData(void)
{
    f=false;
    SegmentCells->Cells[1][y]=BegGeoPointBox->Text;
    SegmentCells->Cells[2][y]=EndGeoPointBox->Text;
    SegmentCells->Cells[3][y]=SegmentLen->Text;
    SegmentCells->Cells[4][y]=SegmentHBeg->Text;
    SegmentCells->Cells[5][y]=SegmentQ->Text;
    SegmentCells->Cells[6][y]=SegmentSortamentBox->Text;
    f=true;
}
//---------------------------------------------------------------------------


void __fastcall TMDIChild::SegmentCellsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    LoadSegmentData(ACol,ARow);
    //DownloadSegmentData();
}
//---------------------------------------------------------------------------



void __fastcall TMDIChild::SegmentCellsSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
    LoadSegmentData(ACol,ARow);
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::BegGeoPointBoxChange(TObject *Sender)
{
    if (f) DownloadSegmentData();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::SpeedButton1Click(TObject *Sender)
{
    PageControl1->ActivePage=TabSheet2;        
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::UpDownChanging(TObject *Sender,
      bool &AllowChange)
{
    GeodezicCells->RowCount=UpDown->Position+2;

    for(int i=0;i<UpDown->Position+1;i++)
    {
        GeodezicCells->Cells[0][i+1]=i+1;
    }
    SegmentCells->RowCount=UpDown->Position+1;
    for(int i=1;i<SegmentCells->RowCount;i++)
    {
        SegmentCells->Cells[0][i]=i;
    }
    BegGeoPointBox->Items->Clear();
    EndGeoPointBox->Items->Clear();
    for(int i=1;i<UpDown->Position+2;i++)
    {
        BegGeoPointBox->Items->Add(i);
        EndGeoPointBox->Items->Add(i);
    }

}
//---------------------------------------------------------------------------


void __fastcall TMDIChild::NumberSegmentChange(TObject *Sender)
{
    bool f=true;
    UpDownChanging(Sender,f);
}
//---------------------------------------------------------------------------
void TMDIChild::FileSave(void) // Запись исходных данных
{
    if (FileName=="") FileSaveAs();
    else
    {
        Caption=ExtractFileName(FileName);
        // Дальше идет код сохранения данных формы
        TFileStream *a=new TFileStream(FileName,fmCreate|fmOpenWrite);
        DiskStream *S=new DiskStream(a);
        S->Write("KANS");
        S->Write(SaveVer); // Номер версии сохранения, должен увеличиваться с добавлением новых данных
        int nSegment=UpDown->Position; // Количество участков
        S->Write(nSegment);
        S->Write(SortamentBox->ItemIndex);
        S->Write(HminEdit->Text.ToDouble());
        S->Write(HmaxEdit->Text.ToDouble());
        S->Write(dHmaxEdit->Text.ToDouble());
        for(int i=0;i<nSegment+1;i++)
        {
            S->Write(GeodezicCells->Cells[1][i+1]);
            S->Write(GeodezicCells->Cells[2][i+1]);
        }
        for(int i=0;i<nSegment;i++)
        {
            S->Write(SegmentCells->Cells[1][i+1]);
            S->Write(SegmentCells->Cells[2][i+1]);
            S->Write(SegmentCells->Cells[3][i+1]);
            S->Write(SegmentCells->Cells[4][i+1]);
            S->Write(SegmentCells->Cells[5][i+1]);
            S->Write(SegmentCells->Cells[6][i+1]);
        }
        S->Write(ComboType->ItemIndex);
        delete S;
        delete a;
    }
}
void TMDIChild::FileSaveAs(void) // Запись исходных данных как ...
{
    if (SaveDialog->Execute())
    {
        FileName=SaveDialog->FileName;
        FileSave();
    }
}
void TMDIChild::FileLoad(AnsiString File) // Открытие исходных данных
{
    FileName=File;
    Caption=ExtractFileName(FileName);
    // Здесь идет код загрузки данных из файла
    TFileStream *a=new TFileStream(FileName,fmOpenRead);
    DiskStream *S=new DiskStream(a);
    AnsiString Format=S->ReadAnsiString();
    if (Format!="KANS")
    {
        ShowMessage("Файл не является файлом данных канализационных сетей");
        Abort();
    }
    int LoadVer=S->ReadInt(); // Номер версии сохранения, должен увеличиваться с добавлением новых данных
    int nSegment=S->ReadInt();
    //UpDown->Position=nSegment;
    NumberSegment->Text=nSegment;
    UpDown->Position = nSegment;
    SortamentBox->ItemIndex=S->ReadInt();
    HminEdit->Text=S->ReadDouble();
    HmaxEdit->Text=S->ReadDouble();
    dHmaxEdit->Text=S->ReadDouble();
    for(int i=0;i<nSegment+1;i++)
    {
        GeodezicCells->Cells[1][i+1]=S->ReadAnsiString();
        GeodezicCells->Cells[2][i+1]=S->ReadAnsiString();
    }
    for(int i=0;i<nSegment;i++)
    {
        SegmentCells->Cells[1][i+1]=S->ReadAnsiString();
        SegmentCells->Cells[2][i+1]=S->ReadAnsiString();
        SegmentCells->Cells[3][i+1]=S->ReadAnsiString();
        SegmentCells->Cells[4][i+1]=S->ReadAnsiString();
        SegmentCells->Cells[5][i+1]=S->ReadAnsiString();
        SegmentCells->Cells[6][i+1]=S->ReadAnsiString();
    }
    if (LoadVer>0)
    {
        ComboType->ItemIndex=S->ReadInt();
    }
    delete S;
    delete a;
    NumberSegment->Text=nSegment;    
}
bool TMDIChild::Clear(void)
{
    if (IsChange)
    {
        switch(MessageBox(Handle, "Файл не сохранен. Сохранить ?","",MB_YESNOCANCEL | MB_ICONQUESTION))
        {
            case IDYES : FileSave();break;
            case IDCANCEL : Abort();break;
        }
    }
    // Дальше должен идти код очистки формы
    return true;
}

void __fastcall TMDIChild::MdComboBoxChange(TObject *Sender)
{
    int x=0;
    for(int i=0;NameMest[i]!="";i++)
    {
        if (MdComboBox->Text==NameMest[i]) { x=i;break;}
    }
    MdEdit->Text=MD[x];
}
//---------------------------------------------------------------------------
void TMDIChild::CreateReport(TMDIReport *Report,int NumReport)
{
}

void __fastcall TMDIChild::SpeedButton2Click(TObject *Sender)
{
    Calculate();    
}
//---------------------------------------------------------------------------

