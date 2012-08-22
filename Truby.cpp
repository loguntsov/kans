//---------------------------------------------------------------------------

#include <vcl.h>
#include <Math.hpp>
#include <Math.h>
#include <Sysutils.hpp>

#pragma hdrstop

#include "Truby.h"
#include "Kans.h"
#include <algorithm.h>
#include "ChildWin.h"
#include "Main.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)


double const PI=M_PI;
double Hmin=2.0; // Минимальное заглубление
double Hmax=8.0; // Максимальное заглубление
double H0max=8.5; // Максимальное заглубление для насосной станции
double hPer=5.0; // Максимальная глубина перепадного колодца
double Md=99; // Максимальное суточное количество осадков по СНиП 2.01.01-82
double MinDim=0;
//
//  CMaterial
//
CMaterial::CMaterial(
        char *NamE, // наименование материала
        char *Gost, // гост на материал
        double N, // коэффициент шероховатости
        double VMAX // Максимальная скорость для данного материала
)
{
    Name=NamE;GOST=Gost;n=N;
    Vmax=VMAX;
}

// Возможные материалы труб.

CMaterial MAsbest1("асбестоцементные безнапорные","ГОСТ 1839",0.012,4);
CMaterial MAsbest2("асбестоцементные напорные","ГОСТ 539",0.012,4);
CMaterial MKeramika("керамические","ГОСТ 286",0.014,4);
CMaterial MBeton("железобетонные безнапорные","ГОСТ 22000",0.014,4);
CMaterial MStal("стальные электросварные","ГОСТ 10704-81",0.012,8);


//
// CDiametr
//


CDiametr::CDiametr(int Usl,double Fakt)
{
    UslDiametr=Usl;
    FaktDiametr=Fakt;
}
CDiametr::CDiametr()
{
   UslDiametr=FaktDiametr=0;
}
double CDiametr::GetMaxNapoln(void) // Возвращает максимальное наполнение для данного диаметра
{
    return MaxNapoln(UslDiametr);
}
double CDiametr::GetVtoNap100(double Q) // Возвращает скорость идеальной жидкости при 100% наполнении, Q - л/c
{
    return 4.0e+3*Q/PI/(FaktDiametr*FaktDiametr);
}

//
//  CTruba
//
CTruba::CTruba(SInfoTruba INFO)
{
    Info=INFO;
    Q=Info.QCUR;
    Rash=true;
    Vvh=Info.VVH;
    if (Vvh>1.5) Vvh=0.8*Vvh; else Vvh=Vvh;
    DH=0;HLBegMin=Info.HLBEG;
    HBeg=Info.HLBEG;
    PumpStation=NULL;
    PerepadWells=NULL;
    Q=(Info.Q0+Info.QCUR)*GetKnerawn(Info.Q0+Info.QCUR)+Info.QS;
    Qliv=Info.QLIV+0.15*Info.LEN/1000*sqrt(Md);
    GS=false;
    NapWodowod=NULL;
}
CTruba::~CTruba()
{
    delete PumpStation,PerepadWells,NapWodowod;
}
CSortament * CTruba::GetSortament(void)
{
    return Info.SORTAMENTITEM->Sortament[NSortament];
}
void CTruba::ToMaxSortament(void)
{
    //if (Info.NBeg ==30)
    //{
    //    double y=0;
    //}
    if (!FindDiametrAndi1000(Hmin,Hmax,true))
    {
        for(int i=0;Info.SORTAMENTITEM->Sortament[i]!=NULL;i++)
        {
            NSortament=i;
            if (IsDiametrGood()) break;
        }
        double y=Info.SORTAMENTITEM->Sortament[NSortament]->GetQmin();
        if (Q<Info.SORTAMENTITEM->Sortament[NSortament]->GetQmin())
        {
            // Не расчетные участки
            Rash=false;
            //NSortament=0;
            HBeg=GetHBeg();
            i1000=Geti1000ToHmin();
            if (HEnd>2*Hmax-Hmin) SetNapWodowod(); else
                if (HEnd>H0max) SetPumpStation();
        } else
        if (FindDiametrAndi1000(Hmax,Hmax+(Hmax-Hmin)*5,true))
        {
            if (HEnd>H0max+(Hmax-Hmin)*4) SetPumpStation();
        } else
        if (FindDiametrAndi1000(Info.ZEND -Info.ZBEG ,Hmin,true))
        {
            SetWells(Hmin+GetDiametr());
        } else
        {
            SetNapWodowod(); // Напорные водоводы
        }
    }
}

void CTruba::ToGetSortament(void)
{
    ToMaxSortament();
    ToNapolnenie();
}

double CTruba::func(double x)
{
    double d=Sortament->Diametr.FaktDiametr/1000;
    //double a0=Sortament->Material->a0;
    //double b0=Sortament->Material->b0;
    double a=2*ArcCos(2*x-1);
    double n=Sortament->Material->n;
    double A0=2*PI-a;
    double B0=sin(a);
    double R=(d/4)*(A0+B0)/A0;
    double w=(d*d/8)*(A0+B0);
    double y=2.5*sqrt(n)-0.13-0.75*sqrt(R)*(sqrt(n)-0.1);
    return w*pow(R,y)/n*sqrt(R*i1000/1000)-Q/1000;
}
void CTruba::ToNapolnenie(void)
{
    if (NapWodowod!=NULL) { hd=1;return;}
    const double b=0.001;
    const double h=-0.1;
    double x1=1,x2=1+h,y1=func(x1),y2;
    const double e=0.000001;
    while (x2>b)
    {
        y2=func(x2);
        if (y1*y2<=0)
        {
            double xx1=x1,xx2=x2;
            while (!(fabs(xx1-xx2)<e || fabs(func(xx1)-func(xx2))<=e))
            {
                double c=(xx1+xx2)/2;
                if(func(xx1)*func(c)<0) xx2=c; else xx1=c;
            }
            hd=(xx1+xx2)/2;return;
        }
        x1=x2;x2=x1+h;y1=y2;
    }
    hd=1;
}
double CTruba::GetHEnd(void)
{
    return (double(int((Info.ZEND -Info.ZBEG +HBeg+i1000/1000*Info.LEN -DH)*100+0.5)))/100;
}
double CTruba::GetHHEnd(void)
{
    return HEnd-GetDiametr();
}
void CTruba::SetHEnd(double HEND)
{
    i1000=(Info.ZBEG -Info.ZEND -(HBeg-HEND))/Info.LEN *1000;
}
double CTruba::GetW(double hd)
{
    double a=2*ArcCos(2*hd-1);
    double d=GetDiametr();
    return (d*d/8)*(2*PI-a+sin(a));
}
double CTruba::GetW(void)
{
    return GetW(hd);
}
double CTruba::GetV(void)
{
    if (NapWodowod==NULL) return (Q/1000)/GetW();
    return NapWodowod->GetV();
}
double CTruba::GetZLBeg(void)
{
    return Info.ZBEG -HBeg;
}
double CTruba::GetZLEnd(void)
{
    return Info.ZEND -HEnd;
}
double CTruba::GetZHBeg(void)
{
    return ZLBeg+GetDiametr();
}
double CTruba::GetZHEnd(void)
{
    return ZLEnd+GetDiametr();
}
double CTruba::GetDiametr(void)
{
    return Sortament->Diametr.FaktDiametr/1000;
}
double CTruba::Geti1000mest(void)
{
    return ((Info.ZBEG -Info.ZEND )/Info.LEN )*1000;
}
double CTruba::Geti1000ToHmin(void)
{
    return max((Info.ZBEG -Info.ZEND-(HBeg-(Hmin+GetDiametr())))/Info.LEN *1000,1/GetDiametr());
}
double CTruba::Geti1000ToHmax(void)
{
    return (Info.ZBEG -Info.ZEND-(HBeg-Hmax))/Info.LEN*1000;
}
double CTruba::Geti1000(void)
{
    try
    {
        return fi1000;
    }
    catch (...)
    {
        return -10000;
    }

}

void CTruba::Seti1000(double i)
{
    i=int(double(i/0.5)+0.999999)*0.5;
    //if (i>Geti1000ToHmax()) i=i-0.5;
    if (i<Geti1000ToHmin()) i=i+0.5;
    if (Sortament->Diametr.UslDiametr==150)
    {
        if (i<8)
        {
            fi1000=8;return;
        }
        fi1000=i;return;
    }
    if (Sortament->Diametr.UslDiametr==200)
    {
        if (i<5)
        {
            fi1000=5;return;
        }
        fi1000=i;return;
    }
    fi1000=i;
}
double CTruba::GetVmin(void)
{
    return max(Sortament->GetVmin(),Vvh);
}
void CTruba::CreateReport(TStrings *Str)
{
    Str->Add("Сведения о трубопроводе: ");
    Str->Add("      Расход: Q="+AnsiString(Q));
    Str->Add("      Диаметр: d="+AnsiString(Sortament->Diametr.UslDiametr)+" мм.");
    Str->Add("      Материал труб: Трубы "+Sortament->Material->Name+" "+Sortament->Material->GOST);
    Str->Add("      Уклон: i="+AnsiString(i1000));
    if (GS) Str->Add("На участке применяется гаситель скорости");
    if (!Rash)
    {
        Str->Add("Участок не расчетный ");
    } else
    {
        Str->Add("      Скорость: V="+AnsiString(V));
        Str->Add("      Наполнение: h/d="+AnsiString(hd));
        Str->Add("      Наполнение при пропуске ливневого расхода: h/d="+AnsiString(GetNapolnenieToQPlusQliv()));
    }
    if (PumpStation!=NULL)
    {
        Str->Add("На участке присутствует насосная станция");
        double l=PumpStation->GetL();
        Str->Add("  на расстоянии: "+AnsiString(l)+" метров от начала участка");
        Str->Add("  c перепадом уровней: "+AnsiString(DH)+" м.");
    } else
    if (PerepadWells!=NULL)
    {
        Str->Add("На участке присутсвуют перепадные колодцы.");
        PerepadWells->CreateReport(Str);
    } else
    if (NapWodowod!=NULL)
    {
        Str->Add("На участке присутсвуют напорные водоводы.");
        NapWodowod->CreateReport(Str);
    }
    Str->Add("Отметки :");
    Str->Add("В начале трубопровода: ");
    Str->Add("      Заглубление: h="+AnsiString(HBeg));
    Str->Add("      Лотка: Zл="+AnsiString(ZLBeg));
    Str->Add("      Шелыги: Zш="+AnsiString(ZHBeg));
    Str->Add("В конце трубопровода: ");
    Str->Add("      Заглубление: h="+AnsiString(HEnd));
    Str->Add("      Лотка: Zл="+AnsiString(ZLEnd));
    Str->Add("      Шелыги: Zш="+AnsiString(ZHEnd));
}
void CTruba::SetPumpStation(void)
{
    if (PumpStation!=NULL) delete PumpStation;
    PumpStation=new CPumpStation(this);
    DH=HEnd-PumpStation->HEnd[PumpStation->NPumpStation]; 
}
int CTruba::GetNumSortamentFrom(double Diam)
{
    int N=NSortament-Info.SORTAMENTITEM->GetNumSortament(Diam);
    return N;
}
double CTruba::GetHBeg(void) // Возвращает начальное заглубление соответсвующее правила конструирования сети.
{
    double H;
    if (Info.TROFHIL!=NULL)
    {
        if (Info.TROFHIL ->Sortament->Diametr.FaktDiametr<=Sortament->Diametr.FaktDiametr)
        {
            double x=Info.TROFHIL ->HHEnd+GetDiametr();
            H=max(x,HLBegMin);
        } else
        if (Info.TROFLOT ->Sortament->Diametr.FaktDiametr>Sortament->Diametr.FaktDiametr)
        if (Info.TROFLOT->GetNumSortamentFrom(Sortament->Diametr.FaktDiametr)<=1)
        {
            double x=Info.TROFLOT->HEnd;
            H=max(x,HLBegMin);
        }
    } else H=Info.HLBEG+GetDiametr();
    return H;
}
bool CTruba::IsDiametrGood(void)
{
    bool f0=true;
    double y=Sortament->Diametr.UslDiametr;
    if (Sortament->Diametr.UslDiametr < MinDim) return false;
    if (Info.TROFHIL!=NULL)
    {
        f0=false;
        if (Info.TROFHIL ->Sortament->Diametr.FaktDiametr<=Sortament->Diametr.FaktDiametr)
        {
            f0=true;
            double x=Info.TROFHIL ->HHEnd+GetDiametr();
            HBeg=max(x,HLBegMin);
        } else
        if (Info.TROFLOT ->Sortament->Diametr.FaktDiametr>Sortament->Diametr.FaktDiametr)
        if (Info.TROFLOT->GetNumSortamentFrom(Sortament->Diametr.FaktDiametr)<=1)
        {
            f0=true;
            double x=Info.TROFLOT->HEnd;
            HBeg=max(x,HLBegMin);
        }
    } else HBeg=Info.HLBEG+GetDiametr();
    return f0;
}

bool CTruba::FindDiametrAndi1000( // ищет подходящий уклон для текущего диаметра трубы, результаты записывает в NSortament и i1000
        double H1min, // Минимальное заглубление
        double H1max, // Максимальное заглубление
        bool f // если true - то ищем по минимальному заглублению, иначе по максимальному);
        )// возвращает true если все хорошо
{
    int i;
    double H=100000;
    double j=i1000;
    if (!f) H=-100000;
    int Diam=-1;
    double Uklon=10000000;
    double H0Beg=0;
    for(i=0;Info.SORTAMENTITEM ->Sortament[i]!=NULL;i++)
    {
        NSortament=i;
        if (IsDiametrGood())
        {
            HBeg=GetHBeg();
            // Вычисляем минимальный уклон
            HEnd=H1min+GetDiametr();
            double pi1000min=i1000;
            i1000=Sortament->Geti1000(Q);
            pi1000min=max(i1000,pi1000min);
            i1000=pi1000min;
            double H1=HEnd;
            // Вычисляем максимальный уклон
            HEnd=H1max;
            double pi1000max=i1000;
            for(j=pi1000min;j<=pi1000max;j=j+0.5)
            {
                i1000=j;
                double H0=HEnd;
                ToNapolnenie();
                if (V>Sortament->Material->Vmax) break;
                if (hd<=Sortament->Diametr.GetMaxNapoln() && hd>=0.2)
                {
                    double y=GetVmin();
                    //if (Info.NBeg==26)
                    //{
                      //  y=GetVmin();
                    //}
                    if (V>=GetVmin())
                    {
                        double Q0=Q;
                        Q=Q+Qliv;
                        ToNapolnenie();
                        Q=Q0;
                        if (hd<=0.95) // Проверка на доболнительный ливневой расход
                        {
                            int l1=int(H0*1000+0.5);
                            int l2=int(H*1000+0.5);
                            int lmin=int((H1min+GetDiametr())*1000+0.5);
                            int lmin0=int((H1min+Info.SORTAMENTITEM ->Sortament[max(0,Diam)]->Diametr.FaktDiametr/1000)*1000+0.5);
                            if ((f && l1<l2 && l1>=lmin) ||(!f && l1>l2))
                            {
                                H=H0;
                                H0Beg=HBeg;
                                Uklon=i1000;
                                Diam=i;
                            }
                        }
                    }
                }
            }
        }
    }
    if (Diam>-1)
    {
        NSortament=Diam;
        i1000=Uklon;HBeg=H0Beg;
        //ToNapolnenie();
    } //else HBeg=Info.HLBEG;
    return (Diam>-1);
}

void CTruba::SetWells(double H) // Установка перепадных колодцев с падением заглубления по лотку до H м
{
    if (PerepadWells!=NULL) delete PerepadWells;
    DH=HEnd-H;
    PerepadWells=new CPerepadWells(this);
}
void CTruba::SetNapWodowod(void) // Установка напорных водоводов.
{
    if (NapWodowod!=NULL) delete NapWodowod;
    DH=0;
    NapWodowod=new CNapWodowod(this);
}
double CTruba::GetQfromHD(double hd)
{
    double a=2.0*ArcCos(2.0*MaxNapoln(Sortament->Diametr.UslDiametr)-1);
    double w=GetW(hd);
    double r=(Sortament->Diametr.FaktDiametr/1000)/4*((PI*2-a+sin(a))/(PI*2-a));
    double n=Sortament->Material->n;
    double y=2.5*sqrt(n)-0.13-0.75*sqrt(r)*(sqrt(n)-0.1);
    double v=pow(r,y)/n*sqrt(r*i1000/1000);
    return 1000.0*w*v;
}
double CTruba::GetZmest(double Len) // Возвращает отметку земли на расстоянии Len от начала участка
{
    return Info.ZBEG -Geti1000mest()/1000*Len;
}
double CTruba::GetNapolnenieToQPlusQliv(void)
{
    double Q0=Q,HD=hd;
    Q=Q+Qliv;
    ToNapolnenie();
    double ret=hd;
    hd=HD;Q=Q0;
    return ret;
}
AnsiString CTruba::CreateLineTable(void) // Создание отчеста в виде строки таблицы
{
    AnsiString answ="";
    answ=Format("%7.1f|",ARRAYOFCONST((Info.LEN)))+
           Format("%7.1f|",ARRAYOFCONST((Q)))+
           Format("%6.0f |",ARRAYOFCONST(((double)Sortament->Diametr.UslDiametr)))+
           Format("%6.1f|",ARRAYOFCONST((i1000)));
           if (Rash) answ=answ+Format("%8.2f|",ARRAYOFCONST((V))); else answ=answ+"   -    |";
           if (Rash) answ=answ+Format("  %5.2f|",ARRAYOFCONST((hd))); else answ=answ+"   -   |";
           answ=answ+Format("%6.2f|",ARRAYOFCONST((Info.ZBEG)))+
           Format("%6.2f|",ARRAYOFCONST((ZHBeg)))+
           Format("%6.2f|",ARRAYOFCONST((ZLBeg)))+
           Format("%6.2f||",ARRAYOFCONST((HBeg)))+
           Format("%6.2f|",ARRAYOFCONST((Info.ZEND)))+
           Format("%6.2f|",ARRAYOFCONST((ZHEnd)))+
           Format("%6.2f|",ARRAYOFCONST((ZLEnd)))+
           Format("%6.2f|",ARRAYOFCONST((HEnd)));

    return answ;

}
void CTruba::CreateSpecialReport(TStrings *Str) // Создание отчета об инженерных конструкциях на трубе
{
    if (GS)  Str->Add("       |На участке применяется гаситель скорости       |      |      |      |      ||      |      |      |      |");
    if (PumpStation!=NULL) PumpStation->CreateSpecialReport(Str);
    else
    if (PerepadWells!=NULL) PerepadWells->CreateSpecialReport(Str);
    else
    if (NapWodowod!=NULL) NapWodowod->CreateSpecialReport(Str);
}
const AnsiString htmltag="  <td><div align=\"center\">";
bool CTruba::CreateLineTableSmall(TStrings *Str) // Создание отчета об параметрах трубопровода без отметок
{
    Str->Add(htmltag+Format("%1.1f",ARRAYOFCONST((Info.LEN)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((Q)))+"</td>");
    Str->Add(htmltag+Format("%1.0f",ARRAYOFCONST(((double)Sortament->Diametr.UslDiametr)))+"</td>");
    AnsiString answ=htmltag;
    if (NapWodowod==NULL)
    {
        answ+=Format("%1.1f",ARRAYOFCONST((i1000)));
    } else
    {
        answ+="-";
    };
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Rash)
        answ+=Format("%1.2f",ARRAYOFCONST((V)));
        else
        answ+="-";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Rash)
        answ+=Format("%1.2f",ARRAYOFCONST((hd)));
        else
        answ+=" - ";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    answ+=Format("%1.2f",ARRAYOFCONST((Qliv)));
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Rash && (NapWodowod==NULL))
        answ+=Format("%1.2f",ARRAYOFCONST((GetQfromHD(0.95))));
        else
        answ+=" - ";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (GS) answ+="ГС ";
    if (PumpStation!=NULL)
        answ+=PumpStation->CreateLineTableSmall();
    else
        if (PerepadWells!=NULL)
            answ+=PerepadWells->CreateLineTableSmall();
        else
            if (NapWodowod!=NULL)
                answ+=NapWodowod->CreateLineTableSmall();
                else
                if (!GS) answ+=" - ";
    answ+="</td>";
    Str->Add(answ);
    Str->Add("</tr>");
    return true;
}
bool CTruba::CreateOtm(TStrings *Str) // Создание строки таблицы отметок
{
    AnsiString a=htmltag;
    //if (PumpStation!=NULL || PerepadWells!=NULL) return true;
    if (i1000<0) a+="-"; else a+=Format("%1.2f",ARRAYOFCONST((i1000/1000*Info.LEN)));
    a+="</td>";
    Str->Add(a);
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((Info.ZBEG)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((Info.ZEND)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLBeg)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLEnd)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHBeg)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHEnd)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((HBeg)))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((HEnd)))+"</td>");
    Str->Add(htmltag+Sortament->Material->GOST+"</td>");
    return true;
}
bool CTruba::CreateOtmSoor(TStrings *Str) //Создание отметок земли технических сооружений
{
    if (PumpStation!=NULL) return PumpStation->CreateOtm(Str);
    else
        if (PerepadWells!=NULL) return PerepadWells->CreateOtm(Str);
    return true;
}
//---------------------------------------------------------------------------
//
// CSortament
//
CSortament::CSortament(CDiametr Diam,CMaterial *Mat)
{
    Diametr=Diam;
    Material=Mat;
}
double CSortament::GetVmin(void) // Возвращает минимальную скорость для данного сортамента
{
    double R=GetR();
    return 1.57*pow(R,1/(3.5+0.5*R));
}
double CSortament::GetQmin(void) // Возвращает минимальный расход
{
    return 1000*GetW()*GetVmin();
}
double CSortament::Geti1000min(void) // Возвращает минимальный уклон
{
    double R=GetR();
    double n=Material->n;
    double y=2.5*sqrt(n)-0.13-0.75*sqrt(R)*(sqrt(n)-0.1);
    double n0=3.5+0.5*R;
    return 2465*sqr(n)*pow(R,(2/n0)-2*y-1);
}
double CSortament::Geti1000(double Q) // Возвращает уклон трубы для данного диаметра при расходе Q и максимальном наполнении
{
    return sqr(Q/GetQmin())*Geti1000min();
}
double CSortament::GetW(void) // Возвращает площадь живого сечения трубы
{
    double arad=GetAlfa();
    double adeg=arad*180/M_PI;
    double d=Diametr.FaktDiametr/1000;
    return sqr(d)/4*(M_PI*(1-adeg/360)+sin(arad)/2);
}
double CSortament::GetAlfa(void) // Возвращает угол
{
    return 2*ArcCos(2*MaxNapoln(Diametr.FaktDiametr)-1);
}
double CSortament::GetR(void) // Возвращает гидравлический радиус
{
    double arad=GetAlfa();
    double adeg=arad*180/M_PI;
    double d=Diametr.FaktDiametr/1000;
    return d/4*(1+sin(arad)/(2*M_PI*(1-adeg/360)));
}
//
// CSortamentItem
//
CSortamentItem::CSortamentItem(AnsiString Nam,CSortament **Sort)
{
    Name=Nam;Sortament=Sort;
}
int CSortamentItem::GetNumSortament(double Diam) // Возвращает номер наиболее близкого сортамента к диаметру Diam
{
    for(int i=0;Sortament[i]==NULL;i++)
    {
        if (Diam<Sortament[i]->Diametr.FaktDiametr) return i;
    }
    return 0;
}

//
// CPumpStation // Насосная станция
//

CPumpStation::CPumpStation(CTruba *TRUBA)
{
    DH=0;
    Truba=TRUBA;
    NumPumpStation=0;
    Truba0=NULL;
    double a,b;
    do
    {
        do
        {
            delete Truba0;
            Truba0=NULL;
            NumPumpStation++;
            SInfoTruba a=Truba->Info;
            a.LEN=a.LEN-Len[0];
            a.ZBEG=Truba->GetZmest(Len[0]);
            a.HLBEG = Hmin;
            a.VVH =0;
            a.TROFHIL = NULL;
            a.TROFLOT = NULL;
            Truba0=new CTruba(a);
            if (Truba0->FindDiametrAndi1000(Hmin,Hmax+(Hmax-Hmin)*(NumPumpStation-1),true)) break;
        } while (true);
        a=HEnd[NumPumpStation];
    } while (!(a<Hmax));
    Truba0->ToNapolnenie();
    Truba->ToNapolnenie();    
}
CPumpStation::~CPumpStation()
{
    delete Truba0;
}
double CPumpStation::GetLen(int Index)
{
    if (Index<1)
    {
        double Len=(Hmax-Truba->GetHBeg())/((i1000[0]-Truba->Geti1000mest())/1000);
        if (Len<0) Len=0;
        return Len;
    }
    if (Index==NumPumpStation)
    {
        double Len0=Truba0->Info.LEN-(NumPumpStation-1)*Len[NumPumpStation-1];
        return Len0;
    }
    double Len0=(Hmax-(Hmin+Diam[Index]))/((i1000[Index]-Truba->Geti1000mest())/1000);
    return Len0;
}
double CPumpStation::GetDiam(int Index)
{
    if (Index==0)
    {
        return Truba->GetDiametr();
    } else return Truba0->GetDiametr();
}

double CPumpStation::Geti1000(int Index)
{
    if (Index<1) return Truba->i1000; else return Truba0->i1000;
}
double CPumpStation::GetZLBeg(int Index)
{
    return ZZBeg[Index]-HBeg[Index];
}
double CPumpStation::GetZLEnd(int Index)
{
    return ZZEnd[Index]-HEnd[Index];
}
double CPumpStation::GetHHBeg(int Index)
{
    return HBeg[Index]-Diam[Index];
}
double CPumpStation::GetHHEnd(int Index)
{
    return HEnd[Index]-Diam[Index];    
}
double CPumpStation::GetHBeg(int Index)
{
    if (Index==0) return Truba->GetHBeg(); else return Hmin+Diam[Index];
}
double CPumpStation::GetHEnd(int Index)
{
    double HEnd;
    HEnd=HBeg[Index]+Len[Index]*((i1000[Index]-Truba->Geti1000mest())/1000);
    return HEnd;
}
double CPumpStation::GetZZBeg(int Index)
{
    return Truba->GetZmest(GetLenBeg(Index-1));
}
double CPumpStation::GetZZEnd(int Index)
{
    return Truba->GetZmest(GetLenBeg(Index));
}
double CPumpStation::GetZHBeg(int Index)
{
    return ZZBeg[Index]-HHBeg[Index];
}
double CPumpStation::GetZHEnd(int Index)
{
    return ZZEnd[Index]-HHEnd[Index];
}
double CPumpStation::GetLenBeg(int Index)
{
    if (Index<-1) return 0;
    return GetLenBeg(Index-1)+Len[Index];
}
double CPumpStation::GetNumPumpStation(void)
{
    return NumPumpStation;
}
double CPumpStation::GetL(void)
{
    return GetLen(0);
}
void CPumpStation::CreateReport(TStrings *Str)
{
    AnsiString a;
    for (int i=0;i<=NPumpStation;i++)
    {
        CTruba * Trub=(i==0)? Truba:Truba0;
        Str->Add("<tr>");
        AnsiString BegPoint,EndPoint;
        if (i==0) BegPoint=IntToStr(Trub->Info.NBeg); else BegPoint="НС "+IntToStr(i);
        if (i==NPumpStation) EndPoint=IntToStr(Trub->Info.NEnd); else EndPoint="НС "+IntToStr(i+1);
        a=htmltag+"<em>"+BegPoint+" - "+EndPoint+"</em></td>";
        Str->Add(a);
        Str->Add(htmltag+Format("%1.1f",ARRAYOFCONST((Len[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((Trub->Q)))+"</td>");
        Str->Add(htmltag+Format("%1.0f",ARRAYOFCONST(((double)Trub->Sortament->Diametr.UslDiametr)))+"</td>");
        AnsiString answ=htmltag;
        answ+=Format("%1.1f",ARRAYOFCONST((i1000[i])));
        answ+="</td>";
        Str->Add(answ);
        answ=htmltag;
        if (Truba->Rash)
            answ+=Format("%1.2f",ARRAYOFCONST((Trub->V)));
            else
            answ+=" - ";
        answ+="</td>";
        Str->Add(answ);
        answ=htmltag;
        if (Truba->Rash)
            answ+=Format("%1.2f",ARRAYOFCONST((Trub->hd)));
            else
            answ+=" - ";
        answ+="</td>";
        Str->Add(answ);
        answ=htmltag;
        answ+=Format("%1.2f",ARRAYOFCONST((Trub->Qliv)));
        answ+="</td>";
        Str->Add(answ);
        answ=htmltag;
        if (Truba->Rash)
            answ+=Format("%1.2f",ARRAYOFCONST((Trub->GetQfromHD(0.95))));
            else
            answ+="-";
        answ+="</td>";
        Str->Add(answ);
        answ=htmltag+" - ";
        //if (Truba->GS) answ+="ГС "; else answ+=" - ";
        answ+="</td>";
        Str->Add(answ);
        Str->Add("</tr>");
    }
}
void CPumpStation::CreateSpecialReport(TStrings *String)
{
}
AnsiString CPumpStation::CreateLineTableSmall(void)
{
    return "НСП "+Format("%1.0f",ARRAYOFCONST((NPumpStation)))+" шт";
}
bool CPumpStation::CreateOtm(TStrings *Str)
{
    // Создание строк в отчете отметок для насосной станции.
    AnsiString a;
    for (int i=0;i<=NPumpStation;i++)
    {
        CTruba * Trub=(i==0)? Truba:Truba0;
        Str->Add("<tr>");
        AnsiString BegPoint,EndPoint;
        if (i==0) BegPoint=IntToStr(Trub->Info.NBeg); else BegPoint="НС "+IntToStr(i);
        if (i==NPumpStation) EndPoint=IntToStr(Trub->Info.NEnd); else EndPoint="НС "+IntToStr(i+1);
        a=htmltag+"<em>"+BegPoint+" - "+EndPoint+"</em></td>";
        Str->Add(a);
        a=htmltag;
        if (Truba->i1000<0) a+="-"; else a+=Format("%1.2f",ARRAYOFCONST((Trub->i1000/1000*Len[i])));
        a+="</td>";
        Str->Add(a);
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZZBeg[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZZEnd[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLBeg[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLEnd[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHBeg[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHEnd[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((HBeg[i])))+"</td>");
        Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((HEnd[i])))+"</td>");
        Str->Add(htmltag+Trub->Sortament->Material->GOST+"</td>");
        Str->Add("</tr>");
    }
    return true;



    /*AnsiString a;
    if (Truba->i1000<0) a="   -  |"; else a=Format("%5.2f |",ARRAYOFCONST());
    a=a+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%5.2f|",ARRAYOFCONST())+
      Format("%5.2f|",ARRAYOFCONST())+
      Format(" Len = %-9s  |",ARRAYOFCONST((Len[0]))
      );
    Str->Add(a);*/
}

//
// CNapWodowod
//
CNapWodowod::CNapWodowod(CTruba *TRUBA)
{
    Truba=TRUBA;
    Truba->Info.SORTAMENTITEM = GetSortamentItem(SortamentName,SStal);
    bool f;
    NNap=1;
    do
    {
        f=true;
        NNap++;
        for(int i=0;Truba->Info.SORTAMENTITEM ->Sortament[i]!=NULL;i++)
        {
            Truba->NSortament=i;
            double v=GetV();
            if (i==0 && v<1.5) { f=false;break;}
            //double vmin=Truba->Sortament->GetVmin();
            f=!(v>=1.5 && v<=2.0);
            if (!f) break;
        }
    } while (f);
}
void CNapWodowod::CreateReport(TStrings *Str)
{
}
double CNapWodowod::GetV(void) // Возвращает скорость в напоных водоводах
{
    return Truba->Sortament->Diametr.GetVtoNap100(Truba->Q)/NNap;
}
void CNapWodowod::CreateSpecialReport(TStrings *String)
{
}
AnsiString CNapWodowod::CreateLineTableSmall(void)
{
    return "НВ "+Format("%3d",ARRAYOFCONST((NNap)));
}

//
// CPerepadWells
//
CPerepadWells::CPerepadWells(CTruba *TRUBA)
{
    Truba=TRUBA;
    NWells=1;
    double x;
    do
    {
        NWells++;
        h=((Truba->Geti1000mest()-Truba->i1000)/1000)*(Truba->Info.LEN-Len[0])/(NWells);
        //(ZHEnd[0]-Truba->Info.ZEND-Hmin-Truba->i1000/1000*(Truba->Info.LEN-Len[0]))/(NWells);
    } while (h>3);// || h+Hmin>Hmax);
    //NWells--;
    //h=(Truba->Geti1000mest()-Truba->i1000)/1000/NWells*(Truba->Info.LEN-Len[0]);
    //ShowMessage(h);
    //ShowMessage((ZHEnd[0]-(Truba->Info.ZEND-Hmin)));
}
CPerepadWells::~CPerepadWells()
{
    Truba->DH=0;
    Truba->PerepadWells=NULL;
}

double CPerepadWells::GetZLBeg(int Index)
{
    if (Index<=0) return Truba->ZLBeg;
    return ZLEnd[Index-1]-h;
}
double CPerepadWells::GetZLEnd(int Index)
{
    if (Index<0) Index=0;
    //return ZLBeg[0]-Truba->i1000/1000*Len[0];
    return ZLBeg[Index]-Truba->i1000/1000*Len[Index];
}
double CPerepadWells::GetHHEnd(int Index)
{
    return ZEnd[Index]-ZHEnd[Index];     
}
double CPerepadWells::GetLen(int Index)
{
    if (Index==0) return ((ZBeg[Index]-ZHBeg[Index])-Hmin)/((Truba->Geti1000mest()-Truba->i1000)/1000);
    return (Truba->Info.LEN-Len[0])/(NWells);
    /*if (Index==NWells)
    {
        double s=0;
        for(int i=0;i<NWells;i++)
        {
            s=s+Len[i];
        }
        return Truba->Info.LEN-s;
    }*/
    //return ((ZBeg[Index]-ZHBeg[Index])-Hmin)/((Truba->Geti1000mest()-Truba->i1000)/1000);
}
double CPerepadWells::GetZBeg(int Index) // Отметка земли в начале для Index подучастка
{
    if (Index==0) return Truba->Info.ZBEG;
    return ZEnd[Index-1];
}
double CPerepadWells::GetZEnd(int Index) // Отметка земли в конце для Index подучастка
{
    return ZBeg[Index]-(Truba->Geti1000mest()/1000)*(Len[Index]);
}
double CPerepadWells::GetZHBeg(int Index)
{
    return ZLBeg[Index]+Truba->GetDiametr();
}
double CPerepadWells::GetZHEnd(int Index)
{
    return ZLEnd[Index]+Truba->GetDiametr();
}
void CPerepadWells::CreateReport(TStrings *Str)
{
    AnsiString a;
    for(int i=0;i<NWells+1;i++)
    {
    Str->Add("<tr>");
    a=htmltag+"<em>";
    if (i==0) a+=IntToStr(Truba->Info.NBeg); else a+="ПК "+IntToStr(i);
    a+=" - ";
    if (i==NWells) a+=IntToStr(Truba->Info.NEnd); else a+="ПК "+IntToStr(i+1);
    a+="</em></td>";
    Str->Add(a);
    //AnsiString a=htmltag;
    //if (Truba->i1000<0) a+="-"; else a+=Format("%1.2f",ARRAYOFCONST((Truba->i1000/1000*Len[i])));
    //a+="</td>";
    //Str->Add(a);
    Str->Add(htmltag+Format("%1.1f",ARRAYOFCONST((Len[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((Truba->Q)))+"</td>");
    Str->Add(htmltag+Format("%1.0f",ARRAYOFCONST(((double)Truba->Sortament->Diametr.UslDiametr)))+"</td>");
    AnsiString answ=htmltag;
    answ+=Format("%1.1f",ARRAYOFCONST((Truba->i1000)));
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Truba->Rash)
        answ+=Format("%1.2f",ARRAYOFCONST((Truba->V)));
        else
        answ+="-";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Truba->Rash)
        answ+=Format("%1.2f",ARRAYOFCONST((Truba->hd)));
        else
        answ="-";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    answ+=Format("%1.2f",ARRAYOFCONST((Truba->Qliv)));
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Truba->Rash)
        answ+=Format("%1.2f",ARRAYOFCONST((Truba->GetQfromHD(0.95))));
        else
        answ+="-";
    answ+="</td>";
    Str->Add(answ);
    answ=htmltag;
    if (Truba->GS) answ+="ГС "; else answ+=" - ";
    answ+="</td>";
    Str->Add(answ);
    Str->Add("</tr>");
    }
}
void CPerepadWells::CreateSpecialReport(TStrings *String)
{
//
}
AnsiString CPerepadWells::CreateLineTableSmall(void)
{
    return "ПК "+Format("%3d",ARRAYOFCONST((NWells)));
}
bool CPerepadWells::CreateOtm(TStrings *Str) // Создание списка отметок
{
    AnsiString a;
    for(int i=0;i<NWells+1;i++)
    {
    Str->Add("<tr>");
    a=htmltag+"<em>";
    if (i==0) a+=IntToStr(Truba->Info.NBeg); else a+="ПК "+IntToStr(i); 
    a+=" - ";
    if (i==NWells) a+=IntToStr(Truba->Info.NEnd); else a+="ПК "+IntToStr(i+1);
    a+="</em></td>";
    Str->Add(a);
    AnsiString a=htmltag;
    if (Truba->i1000<0) a+="-"; else a+=Format("%1.2f",ARRAYOFCONST((Truba->i1000/1000*Len[i])));
    a+="</td>";
    Str->Add(a);
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZBeg[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZEnd[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLBeg[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZLEnd[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHBeg[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZHEnd[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZBeg[i]-ZLBeg[i])))+"</td>");
    Str->Add(htmltag+Format("%1.2f",ARRAYOFCONST((ZEnd[i]-ZLEnd[i])))+"</td>");
    Str->Add(htmltag+Truba->Sortament->Material->GOST+"</td>");
    //Str->Add(htmltag+Format(" Len = %1.2f",ARRAYOFCONST((Len[i])))+"</td>");
    Str->Add("</tr>");    
    }

/*    a="";
    if (Truba->i1000<0) a="   -  |"; else a=Format("%5.2f |",ARRAYOFCONST((Truba->i1000/1000*Len[i])));
    a=a+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%6.2f|",ARRAYOFCONST())+
      Format("%5.2f|",ARRAYOFCONST())+
      Format("%5.2f|",ARRAYOFCONST())+
      Format(" Len = %-9s  |",ARRAYOFCONST((Len[i]))
      );
    Str->Add(a);
    }*/
    return true;
}

extern double MaxNapoln(
    double Diam // Условный диаметр трубы,мм
) // Возвращает наполнение для конкретного диаметра трубы.
{
    if (Diam>=1000) return 0.8;
    if (Diam>=450) return 0.75;
    if (Diam>=300) return 0.7;
    return 0.6;
}

double sqr(double X) // Возвращает x^2
{
    return X*X;
}


//
// Сортаменты
//
CSortamentItem *SortamentName[]={
    new CSortamentItem("Асбестоцементные трубы ГОСТ 1839 и ГОСТ 539",SAsbest),
    new CSortamentItem("Керамические трубы ГОСТ 286",SKeramika),
    new CSortamentItem("Железобетонные безнапорные трубы ГОСТ 22000-86",SBeton),
    new CSortamentItem("Керамические + Железобетонные",SKeramikaBeton),
    new CSortamentItem("Стальные электросварные ГОСТ 10704-81",SStal),
NULL
};
CSortamentItem * GetSortamentItem(
    CSortamentItem *SortArray[], // Указатель на массив сортаментов
    CSortament *Sortament[] // Указатель на нужный сортамент
    )// Поиск в массиве сортаментов элемента с нужным сортаментом,
// возвращает NULL - если не обнаружен, или адрес элемента массива.
{
    CSortamentItem * res=NULL;
    for(int i=0;SortArray[i]!=NULL;i++)
    {
        if (SortArray[i]->Sortament==Sortament)
        {
            res=SortArray[i];break;
        }
    }
    return res;
}


CSortament *SAsbest[]={
    new CSortament(CDiametr(100,100),&MAsbest1),
    new CSortament(CDiametr(150,141),&MAsbest1),
    new CSortament(CDiametr(200,189),&MAsbest1),
    new CSortament(CDiametr(250,244),&MAsbest2),
    new CSortament(CDiametr(300,279),&MAsbest1),
    new CSortament(CDiametr(350,334),&MAsbest2),
    new CSortament(CDiametr(400,368),&MAsbest1),
    new CSortament(CDiametr(500,473),&MAsbest2),
NULL
}; // Асбестоцементные трубы
CSortament *SKeramika[]={
    new CSortament(CDiametr(150,150),&MKeramika),
    new CSortament(CDiametr(200,200),&MKeramika),
    new CSortament(CDiametr(250,250),&MKeramika),
    new CSortament(CDiametr(300,300),&MKeramika),
    new CSortament(CDiametr(350,350),&MKeramika),
    new CSortament(CDiametr(400,400),&MKeramika),
    new CSortament(CDiametr(450,450),&MKeramika),
    new CSortament(CDiametr(500,500),&MKeramika),
    new CSortament(CDiametr(550,550),&MKeramika),
    new CSortament(CDiametr(600,600),&MKeramika),
NULL
}; // Керамические трубы
CSortament *SBeton[]={
    new CSortament(CDiametr(400,400),&MBeton),
    new CSortament(CDiametr(500,500),&MBeton),
    new CSortament(CDiametr(600,600),&MBeton),
    new CSortament(CDiametr(800,800),&MBeton),
    new CSortament(CDiametr(1000,1000),&MBeton),
    new CSortament(CDiametr(1200,1200),&MBeton),
    new CSortament(CDiametr(1400,1400),&MBeton),
    new CSortament(CDiametr(1600,1600),&MBeton),
    new CSortament(CDiametr(1800,1800),&MBeton),
    new CSortament(CDiametr(2200,2200),&MBeton),
    new CSortament(CDiametr(2000,2000),&MBeton),
    new CSortament(CDiametr(2400,2400),&MBeton),
NULL
}; // Железобетонные трубы
CSortament *SKeramikaBeton[]={
    new CSortament(CDiametr(150,150),&MKeramika),
    new CSortament(CDiametr(200,200),&MKeramika),
    new CSortament(CDiametr(250,250),&MKeramika),
    new CSortament(CDiametr(300,300),&MKeramika),
    new CSortament(CDiametr(350,350),&MKeramika),
    new CSortament(CDiametr(400,400),&MKeramika),
    new CSortament(CDiametr(500,500),&MBeton),
    new CSortament(CDiametr(600,600),&MBeton),
    new CSortament(CDiametr(800,800),&MBeton),
    new CSortament(CDiametr(1000,1000),&MBeton),
    new CSortament(CDiametr(1200,1200),&MBeton),
    new CSortament(CDiametr(1400,1400),&MBeton),
    new CSortament(CDiametr(1600,1600),&MBeton),
    new CSortament(CDiametr(2000,2000),&MBeton),
    new CSortament(CDiametr(2400,2400),&MBeton),
NULL
}; // Керамика + Бетон
CSortament *SStal[]={
    new CSortament(CDiametr(65,67.6),&MStal),
    new CSortament(CDiametr(80,83.6),&MStal),
    new CSortament(CDiametr(100,103),&MStal),
    new CSortament(CDiametr(125,128.2),&MStal),
    new CSortament(CDiametr(150,153.4),&MStal),
    new CSortament(CDiametr(200,203.6),&MStal),
    new CSortament(CDiametr(250,254),&MStal),
    new CSortament(CDiametr(300,304.4),&MStal),
    new CSortament(CDiametr(350,354.6),&MStal),
    new CSortament(CDiametr(400,404),&MStal),
    new CSortament(CDiametr(500,503.6),&MStal),
    new CSortament(CDiametr(600,603.4),&MStal),
    new CSortament(CDiametr(700,703),&MStal),
    new CSortament(CDiametr(800,803.6),&MStal),
    new CSortament(CDiametr(900,903.4),&MStal),
    new CSortament(CDiametr(1000,1003),&MStal),
NULL
}; // Стальные электросварные


// Данные о максимальном количестве осадков
AnsiString NameMest[]={"Хабаровск",""};
double MD[]={99,0};



