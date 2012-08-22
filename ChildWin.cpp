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
    GeodezicCells->Cells[0][0]="�";
    GeodezicCells->Cells[1][0]="������� �����";
    GeodezicCells->Cells[2][0]="��������������� ������";
    GeodezicCells->Cells[0][1]="1";
    GeodezicCells->Cells[0][2]="2";
    SegmentCells->Cells[0][0]="� ��-��";
    SegmentCells->Cells[0][1]="1";
    SegmentCells->Cells[1][0]="��������� �����,�";
    SegmentCells->Cells[2][0]="�������� �����,�";
    SegmentCells->Cells[3][0]="�����,�";
    SegmentCells->Cells[4][0]="�����������,�";
    SegmentCells->Cells[5][0]="������� ������,�3/���";
    SegmentCells->Cells[6][0]="���������";
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
        MessageBox(Handle, "�� ������ ���������� �������������","",MB_OK | MB_ICONERROR);
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
    Str->Add("      <div align=\"center\">������������ �������</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">�����, �</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">������, �/c</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">�������, ��</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">�����, �1000</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">��������, �/c</div></td>");
    Str->Add("    <td rowspan=\"3\">");
    Str->Add("      <div align=\"center\">h/d</div></td>");
    Str->Add("    <td colspan=\"6\">");
    Str->Add("      <div align=\"center\">������� �����</div></td>");
    Str->Add("  </tr>");
    Str->Add("  <tr>");
    Str->Add("    <td colspan=\"3\">");
    Str->Add("      <div align=\"center\">� ������</div></td>");
    Str->Add("    <td colspan=\"3\">");
    Str->Add("      <div align=\"center\">� �����</div></td>");
    Str->Add("  </tr>");
    Str->Add("  <tr>");
    Str->Add("    <td >");
    Str->Add("      <div align=\"center\">�����</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">�����</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">������</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">�����</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">�����</div></td>");
    Str->Add("    <td>");
    Str->Add("      <div align=\"center\">������</div></td>");
    Str->Add("  </tr>");
    /*            Str->Add("------------------------------------------------------------------------------------------------------------------");
            Str->Add("�����- | ����� | ������|�������|����� |��������|  h/d  |                        ������� �����                   |");
            Str->Add("�������|   �   |  �/�  |  ��   |x1000 |  �/�   |   %   |       � ������            ||    � �����                |");
            Str->Add("�������|       |       |       |      |        |       |����� |����� |������|  H,� ||����� |����� |������| H,�  |");
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
                Str->Add("<p><div align=\"center\"><font size=\"+2\">�������������� ������ ��������������� ����</font></p>");
                Str->Add("<table width=\"100%\" border=\"1\" cellspacing=0 cellpadding=0>");
                Str->Add("  <tr>");
                Str->Add("    <td><div align=\"center\">����� �������</div></td>");
                Str->Add("    <td><div align=\"center\">�����, �</div></td>");
                Str->Add("    <td><div align=\"center\">������, �/c</div></td>");
                Str->Add("    <td><div align=\"center\">�������, ��</div></td>");
                Str->Add("    <td><div align=\"center\">�����, �1000</div></td>");
                Str->Add("    <td><div align=\"center\">��������, �/c</div></td>");
                Str->Add("    <td><div align=\"center\">h/d</div></td>");
                Str->Add("    <td> <div align=\"center\">������ �����., �/�</div></td>");
                Str->Add("    <td><div align=\"center\">������ ��� h/d=0.95</div></td>");
                Str->Add("    <td><div align=\"center\">����������</div></td>");
                Str->Add("  </tr>");

            /*Rich->Paragraph->Alignment=taCenter;
            Rich->SelAttributes->Height=12;
            Rich->Lines->Add("�������������� ������ ��������������� ����");
            Rich->Paragraph->Alignment=taLeftJustify;
            Str->Add("----------------------------------------------------------------------------------------");
            Str->Add("| ����� | ����� | ������|�������|����� |��������|  h/d  |������| ������ |  ����������  |");
            Str->Add("|�������|   �   |  �/�  |  ��   |x0.001|  �/�   |       |�����.|  ���   |              |");
            Str->Add("|       |       |       |       |      |        |       |  �/� |h/d=0.95|              |");
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
            Str->Add("<p><div align=\"left\">����������:</p>");
            Str->Add("<p>�� - �������� ��������</p>");
            Str->Add("<p>�� ## - ���������� ������� � ���������� ## ����</p>");
            Str->Add("<p>��� ## - �������� ������� ��������, ������������� �� ���������� ## �� ������ ��-��</p>");
            Str->Add("<p>�� ## - �������� ��������, � ���������� ## ���� + ��� ��������������� � ������ �������</p>");
            Str->Add("<p></p>");
            //Rich->SelAttributes->Height=12;
            //Rich->Paragraph->Alignment=taCenter;
            Str->Add("<p><div align=\"center\"><font size=\"+2\">������� �� ��������</font></p>");
            Str->Add("  <table width=\"100%\" border=\"1\" cellspacing=0 cellpadding=0>");
            Str->Add("  <tr>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\">����� �������</div></td>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\">h=i*l</div></td>");
            Str->Add("    <td colspan=\"8\"><div align=\"center\">������� �����</div></td>");
            Str->Add("    <td rowspan=\"3\"><div align=\"center\"></div>");
            Str->Add("      <div align=\"center\"></div>");
            Str->Add("      <div align=\"center\">����������</div></td>");
            Str->Add("  </tr>");
            Str->Add("  <tr>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">�����</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">�����</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">������</div></td>");
            Str->Add("    <td colspan=\"2\"><div align=\"center\">�����������</div></td>");
            Str->Add("  </tr>");
            Str->Add("  <tr>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("    <td><div align=\"center\">���</div></td>");
            Str->Add("  </tr>");
            //Rich->Paragraph->Alignment=taLeftJustify;
            //Str->Add("-----------------------------------------------------------------------------------");
            //Str->Add("| ����� |      |                      �������                        |  ��������  |");
            //Str->Add("|�������| h=i*l|    �����         �����         ������    �����������| ���������� |");
            //Str->Add("|       |      | ���  |  ��� | ���  | ���  | ���  | ���  | ��� | ��� |            |");
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
       // ������
        ShowMessage("������ � ��������� ����!!!");
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
void TMDIChild::FileSave(void) // ������ �������� ������
{
    if (FileName=="") FileSaveAs();
    else
    {
        Caption=ExtractFileName(FileName);
        // ������ ���� ��� ���������� ������ �����
        TFileStream *a=new TFileStream(FileName,fmCreate|fmOpenWrite);
        DiskStream *S=new DiskStream(a);
        S->Write("KANS");
        S->Write(SaveVer); // ����� ������ ����������, ������ ������������� � ����������� ����� ������
        int nSegment=UpDown->Position; // ���������� ��������
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
void TMDIChild::FileSaveAs(void) // ������ �������� ������ ��� ...
{
    if (SaveDialog->Execute())
    {
        FileName=SaveDialog->FileName;
        FileSave();
    }
}
void TMDIChild::FileLoad(AnsiString File) // �������� �������� ������
{
    FileName=File;
    Caption=ExtractFileName(FileName);
    // ����� ���� ��� �������� ������ �� �����
    TFileStream *a=new TFileStream(FileName,fmOpenRead);
    DiskStream *S=new DiskStream(a);
    AnsiString Format=S->ReadAnsiString();
    if (Format!="KANS")
    {
        ShowMessage("���� �� �������� ������ ������ ��������������� �����");
        Abort();
    }
    int LoadVer=S->ReadInt(); // ����� ������ ����������, ������ ������������� � ����������� ����� ������
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
        switch(MessageBox(Handle, "���� �� ��������. ��������� ?","",MB_YESNOCANCEL | MB_ICONQUESTION))
        {
            case IDYES : FileSave();break;
            case IDCANCEL : Abort();break;
        }
    }
    // ������ ������ ���� ��� ������� �����
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

