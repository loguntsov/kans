//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Kans.h"
#include "MathMethod.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//
// CGeoSegment
//

CGeoSegment::CGeoSegment(
            CSortamentItem *SORTAMENTITEM, // Ссылка на сортаметы
            CGeoPoint *PBEG, // точка начала участка
            CGeoPoint *PEND, // точка конца участка
            double LEN, // длина участка
            double HBEG, // Начальное заглубление трубы
            double Qpop, // Попутный расход на участке
            AnsiString NAME, // Название участка
            int NBeg, // Номер начальной точки
            int NEnd // Номер конечной точки

        )
{
    NumBeg=NBeg;
    NumEnd=NEnd;
    Truba=NULL;
    PBeg=PBEG;PEnd=PEND;
    Len=LEN;HBeg=HBEG;
    SortamentItem=SORTAMENTITEM;
    Name=NAME;
    PEnd->AddBefore(this);
    //double x=GetKnerawn(Qpop/86.4);

    Q=Qpop/86.4;
}
CGeoSegment::~CGeoSegment()
{
    PEnd->DeleteBefore(this);
    delete Truba;
}
bool CGeoSegment::IsCalculated(void)
{
    return Truba!=NULL;
}
void CGeoSegment::Calculation(void)
{
    if (IsCalculated()) return;
    PBeg->Calculation();
    SInfoTruba a;
    a.SORTAMENTITEM=SortamentItem;
    a.LEN=Len;
    a.ZBEG=PBeg->z;
    a.ZEND=PEnd->z;
    a.HLBEG=HBeg;
    a.QCUR=PBeg->q;
    a.VVH=PBeg->v;
    a.QLIV=PBeg->qliv;
    a.TROFHIL=PBeg->GetTrubaOfMinHil();
    a.TROFLOT=PBeg->GetTrubaOfMinLot();
    a.Q0=Q;
    a.QS=PBeg->qs;
    a.NBeg=NumBeg;
    a.NEnd=NumEnd;
    Truba=new CTruba(a);
    //ShowMessage(AnsiString(PBeg->q+Q)+"  "+AnsiString(GetKnerawn(PBeg->q+Q)));
    Truba->ToGetSortament();
    if (Truba->HEnd>H0max) Truba->SetPumpStation();
    if (Truba->HEnd>Hmax && PBeg->h>Hmax)
    {
        delete Truba;
        for(int i=0;i<PBeg->CountBefore();i++)
        {
            if (PBeg->Items(i)->Truba->HEnd>Hmax)
                PBeg->Items(i)->Truba->SetPumpStation();
        }
        PBeg->Calculation();
        SInfoTruba a;
        a.SORTAMENTITEM=SortamentItem;
        a.LEN=Len;
        a.ZBEG=PBeg->z;
        a.ZEND=PEnd->z;
        a.HLBEG=HBeg;
        a.QCUR=PBeg->q;
        a.VVH=PBeg->v;
        a.QLIV=PBeg->qliv;
        a.TROFHIL=PBeg->GetTrubaOfMinHil();
        a.TROFLOT=PBeg->GetTrubaOfMinLot();
        a.Q0=Q;
        a.QS=PBeg->qs;
        a.NBeg=NumBeg;
        a.NEnd=NumEnd;
        Truba=new CTruba(a);
        Truba->ToGetSortament();
    }
}
void CGeoSegment::CreateReport(TStrings *Str)
{
    if (IsCalculated())
    {
        Str->Add("Участок: "+Name);
        Truba->CreateReport(Str);
        Str->Add("--------------------------------------------------");
    }
}
AnsiString CGeoSegment::CreateLineTable(void)
{
    if (IsCalculated()) return Format("%7s|",ARRAYOFCONST((Name.c_str())))+Truba->CreateLineTable();
    return "";
}
void CGeoSegment::CreateSpecialReport(TStrings *Str) // Создание отчета об инженерных конструкциях на участке
{
    if (IsCalculated())
    {
        Truba->CreateSpecialReport(Str);
        Str->Add("-----------------------------------------------------------------------------------------------------------------");
    }
}

bool CGeoSegment::CreateLineTableSmall(TStrings *Str)
{
    if (IsCalculated())
    {
        Str->Add("<tr>");
        Str->Add("<td><div align=\"center\">"+Name+"</td>");
        bool a=Truba->CreateLineTableSmall(Str);
        Str->Add("</tr>");
        if (Truba->PerepadWells!=NULL) Truba->PerepadWells->CreateReport(Str);
        if (Truba->PumpStation!=NULL) Truba->PumpStation->CreateReport(Str);
        return a;
    }
    return false;
}
bool CGeoSegment::CreateReportOtm(TStrings *Str) // Создание отчета отметок об участке
{
    if (IsCalculated())
    {
           Str->Add("<tr>");
           Str->Add("<td><div align=\"center\">"+Name+"</td>");
           bool a=Truba->CreateOtm(Str);
           bool b=Truba->CreateOtmSoor(Str);
           Str->Add("</tr>");
           return a && b;
    };
    return false;
}
//
// CGeoPoint
//
CGeoPoint::CGeoPoint(
        double Z, // Геометричекая высота точки
        double Q0 // Сосредоточенный расход м3/ч
        )
{
    z=Z;
    q0=Q0/3.6;
    SegmentBefore=new TList();
    TheEndPoint=false;
    v=0;q=0;
}
CGeoPoint::~CGeoPoint()
{
    for(int i=0;i<SegmentBefore->Count;i++)
        delete Items(i);
    delete SegmentBefore;
}
CGeoSegment * CGeoPoint::Items(int i)
{
    return (CGeoSegment *) SegmentBefore->Items[i];
}
void CGeoPoint::AddBefore(CGeoSegment *Seg)
{
    SegmentBefore->Add(Seg);
}
void CGeoPoint::DeleteBefore(CGeoSegment *Seg)
{
    for(int i=0;i<SegmentBefore->Count;i++)
        if(Items(i)==Seg)
        {
            SegmentBefore->Delete(i);
            break;
        }
}
void CGeoPoint::Calculation(void)
{
    q=0;v=0;h=0;qliv=0;qs=q0;
    double qmax=0;int item=-1;
    for(int i=0;i<SegmentBefore->Count;i++)
    {
        if (!Items(i)->IsCalculated())
        {
            Items(i)->Calculation();
            if (TheEndPoint)
                if (Items(i)->Truba->HEnd>Hmax)
                    Items(i)->Truba->SetPumpStation();
        }
        q=q+Items(i)->Truba->Info.Q0+Items(i)->Truba->Info.QCUR ;
        qs=qs+Items(i)->Truba->Info.QS; 
        if (Items(i)->Truba->Q>qmax)
        {
            qmax=Items(i)->Truba->Q;item=i;
        }
        qliv=qliv+Items(i)->Truba->Qliv;
        double vtr=Items(i)->Truba->V;
        double htr=Items(i)->Truba->HEnd;
        //if (v<vtr) v=vtr;
        if (h<htr) h=htr;
    }
    if (item>-1) v=Items(item)->Truba->V; else v=0;
    // Установка гасителей скорости
    for(int i=0;i<SegmentBefore->Count;i++)
    {
        if (int(Items(i)->Truba->V*1000)>int(v*1000)) Items(i)->Truba->GS=true;
    }
}
int CGeoPoint::CountBefore(void)
{
    return SegmentBefore->Count;
}
CTruba * CGeoPoint::GetTrubaOfMinHil(void)
{
    if (SegmentBefore->Count==0) return NULL;
    double x=100000000;CTruba *Tr;
    for(int i=0;i<SegmentBefore->Count;i++)
    {
        if (Items(i)->Truba->ZHEnd<x)
        {
            x=Items(i)->Truba->ZHEnd;Tr=Items(i)->Truba;
        }
    }
    return Tr;
}
CTruba * CGeoPoint::GetTrubaOfMinLot(void)
{
    if (SegmentBefore->Count==0) return NULL;
    double x=100000000;CTruba *Tr;
    for(int i=0;i<SegmentBefore->Count;i++)
    {
        if (Items(i)->Truba->ZLEnd<x)
        {
            x=Items(i)->Truba->ZLEnd;Tr=Items(i)->Truba;
        }
    }
    return Tr;

}

//
//
//

double GetKnerawn(double q) // Возвращает общий коэффициент неравномерности для данного расхода
{
    double x[]={5.0,10.0,20.0,50.0,100.0,300.0,500.0,1000.0,5000.0,100000000.0};
    double y[]={2.5,2.1,1.9,1.7,1.6,1.55,1.5,1.47,1.44,1.44};
    if (q<=5) return 2.5;
    if (q>=5000) return 1.44;
    LinIntr a(x,y,10);
    return a.GetY(q);
}

