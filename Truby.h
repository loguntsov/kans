//---------------------------------------------------------------------------

#ifndef TrubyH
#define TrubyH

extern double Hmin; // ����������� �����������
extern double Hmax; // ������������ �����������
extern double H0max; // ������������ ����������� ��� �������� �������
extern double hPer; // ������������ ������� ����������� �������
extern double Md; // ������������ �������� ���������� ������� �� ���� 2.01.01-82
extern double MinDim; // ����������� ������� � ����  
class CMaterial
{
    public:
    CMaterial(
        char *NamE, // ������������ ���������
        char *Gost, // ���� �� ��������
        double N, // ����������� �������������
        double VMAX // ������������ �������� ��� ������� ���������
        );
    AnsiString Name; // �������� ���������;
// �������������� ���������
    double n; // ����������� �������������
    double Vmax; // ������������ ��������
    AnsiString GOST;
};
class CDiametr
{
public:
    CDiametr(int Usl,double Fakt);
    CDiametr();
    int UslDiametr; // �������� �������.
    double FaktDiametr; // ����������� �������.
    double GetMaxNapoln(void); // ���������� ������������ ���������� ��� ������� ��������
    double GetVtoNap100(double Q); // ���������� �������� ��������� �������� ��� 100% ����������
};
class CSortament
{
private:
    double GetW(void); // ���������� ������� ������ ������� �����
    double GetAlfa(void); // ���������� ����
    double GetR(void); // ���������� �������������� ������
public:
    CSortament(CDiametr Diam,CMaterial *Mat);
    CDiametr Diametr;
    CMaterial *Material;
    double GetVmin(void); // ���������� ����������� �������� ��� ������� ����������
    double GetQmin(void); // ���������� ����������� ������
    double Geti1000min(void); // ���������� ����������� �����
    double Geti1000(double Q); // ���������� ����� ����� ��� ������� �������� ��� ������� Q � ������������ ����������
};
class CSortamentItem
{
public:
    AnsiString Name;
    CSortament **Sortament;
    CSortamentItem(AnsiString Name,CSortament **Sortament);
    int GetNumSortament(double Diam); // ���������� ����� �������� �������� ���������� � �������� Diam
};

class CPumpStation;
class CPerepadWells;
class CNapWodowod;
class CTruba;
struct SInfoTruba
{
    CSortamentItem *SORTAMENTITEM; // ������ �� ��������� ����
    double LEN; // ����� �����
    double ZBEG; // ��������� ������� �����
    double ZEND; // �������� ������� �����
    double HLBEG; // ����������� ��������� ����������� ����� ����� � ������
    double QCUR; // ������ �������� � �����.
    double VVH; // �������� ����� �������� � �����.
    double QLIV; // �������������� ���������������� ������ � ���������� ��������
    CTruba *TROFHIL; // ����� ����������� � ������ ������ � ����������� �������� ������
    CTruba *TROFLOT; // ����� ����������� � ������ ������ � ����������� �������� �����
    double Q0; // �������� ������ (�� �������� ������) + � ���������� ��������
    double QS; // ��������������� ������
    int NBeg; // ������������ ����� � ������ �������
    int NEnd; // ������������ ����� � ����� �������
    //AnsiString NBeg; // ������������ ����� � ������ �������
    //AnsiString NEnd; // ������������ ����� � ����� �������
};
class CGeoSegment;
class CTruba
{
friend class CPumpStation;
private:
    CSortament * GetSortament(void);
    double func(double hd);
    double GetHEnd(void);
    void SetHEnd(double HEND);
    double GetW(void);
    double GetW(double hd);
    double GetV(void);
    double GetZLBeg(void);
    double GetZLEnd(void);
    double GetZHBeg(void);
    double GetZHEnd(void);
    double fi1000;
    double Geti1000(void);
    double Vvh;
    void Seti1000(double i1000);
    double GetHHEnd(void);
    double HLBegMin;
    bool IsDiametrGood(void); // ���������� true ���� ������� ������� ������������� �������� ������ ���������
    // ������� ��� ���� ��������� ����������� ����� ����� (������������ �� ����� ��� ������)
    bool FindDiametrAndi1000( // ���� ���������� ����� ��� �������� �������� �����, ���������� ���������� � NSortament � i1000
        double Hmin, // ����������� �����������
        double Hmax, // ������������ �����������
        bool f // ���� true - �� ���� �� ������������ �����������, ����� �� �������������);
        );// ���������� true ���� ��� ������
    CPumpStation *PumpStation;
    CPerepadWells *PerepadWells;
    CNapWodowod *NapWodowod;
    friend CPerepadWells;
    friend CGeoSegment;//::CreateLineTableSmall;
public:
    CTruba(SInfoTruba Info);
    ~CTruba();
    SInfoTruba Info;
    __property CSortament * Sortament={read=GetSortament}; // ��������� (������� � ��������) �����
    //double ZEnd; // ������� ����� � ����� �����
    __property double HEnd={read=GetHEnd,write=SetHEnd}; // ����������� ������ ����� � �����
    __property double V={read=GetV}; // �������� � �����.
    __property double ZLBeg={read=GetZLBeg}; // ������� ����� � ������
    __property double ZLEnd={read=GetZLEnd}; // ������� ����� � �����
    __property double ZHBeg={read=GetZHBeg}; // ������� ������ � ������
    __property double ZHEnd={read=GetZHEnd}; // ������� ������ � �����
    __property double i1000={read=Geti1000,write=Seti1000}; // ����� ��� ������� ������� �����.
    __property double HHEnd={read=GetHHEnd}; // ����������� ������
    //CSortamentItem *SortamentItem;
    int NSortament;
    double HBeg; // ����������� ����� � ������
    double hd; // ���������� �����
    double Q;
    double DH; // ���������� ��������� ����������� ��� ����� �������� ������� � ���������� ��������
     // DH>0 - �������� �������
     // DH=0 - ������� �������
     // DH<0 - ���������� �������
    double Qliv; // �������������� ���������������� ������ � ���������� �������� + �� �������
    void ToMaxSortament(void); // ���������� �������� �����, ��� ����� �������
    void ToNapolnenie(void); // ������ ���������� �����, ��� �������� ����������.
    void ToGetSortament(void); // ���������� �������� ����� � ������ �������.
    double GetVmin(void);
    bool Rash; // ������� �� ���������� �������
    void CreateReport(TStrings *Strings); // �������� ������ �� �����
    void SetPumpStation(void); // ��������� ������ ������� �������� � ������� �������� �� H �
    void SetWells(double H); // ��������� ���������� �������� � �������� ����������� �� ����� �� H �
    void SetNapWodowod(void); // ��������� �������� ���������.
    int GetNumSortamentFrom(double Diam); // ���������� ����� ����������� ����������� ����� ��������� ������ ����� � Diam (����������� �������,��)
    double GetDiametr(void); // ���������� ������� � ������
    double Geti1000mest(void); // ���������� ����� ���������
    double GetNapolnenieToQPlusQliv(void);
    bool GS; // ���������� ������� ��������.
    double Geti1000ToHmin(void); // ���������� ����� �����, ����� ��� ����� �� ����������� �����������
    double Geti1000ToHmax(void); // ���������� ����� �����, ����� ��� ����� �� ������������ �����������
    double GetZmest(double Len); // ���������� ������� ����� �� ���������� Len �� ������ �������
    AnsiString CreateLineTable(void); // �������� ������ � ���� ������ �������
    void CreateSpecialReport(TStrings *Str); // �������� ������ �� ���������� ������������ �� �����
    bool CreateLineTableSmall(TStrings *Str); // �������� ������ �� ���������� ������������ ��� �������
    bool CreateOtm(TStrings *Str); // �������� ������ ������� �������
    bool CreateOtmSoor(TStrings *Str); // �������� ������� ����� ����������� ����������
    double GetQfromHD(double hd); // ���������� ������ ��� ���������� HD.
    double GetHBeg(void); // ���������� ��������� ����������� �������������� ������� ��������������� ����.
};
class CPumpStation // �������� �������
{
private:
    CTruba *Truba;
    CTruba *Truba0;
    //CTruba *Truba; // ����� �� �������� �������
    //CTruba *Truba1; // ����� ����� �������� �������
    double DH;
    double GetZLBeg(int Index);
    double GetZLEnd(int Index);
    double GetHHBeg(int Index);
    double GetHHEnd(int Index);
    double GetHBeg(int Index);
    double GetHEnd(int Index);
    double GetZZBeg(int Index);
    double GetZZEnd(int Index);
    double GetZHBeg(int Index);
    double GetZHEnd(int Index);
    double GetLen(int Index);
    double GetLenBeg(int Index);
    double Geti1000(int Index);
    double GetDiam(int Index);
    double GetNumPumpStation(void); 
    int NumPumpStation;
public:
    CPumpStation(CTruba *TRUBA);
    ~CPumpStation();
    double GetL(void);
    void CreateReport(TStrings *Strings);
    void CreateSpecialReport(TStrings *String);
    AnsiString CreateLineTableSmall(void);
    bool CreateOtm(TStrings *Str);
    __property double NPumpStation={read=GetNumPumpStation};
    __property double ZLBeg[int Index]={read=GetZLBeg}; // ����� ������� ��� ���
    __property double ZLEnd[int Index]={read=GetZLEnd};
    __property double ZZBeg[int Index]={read=GetZZBeg};
    __property double ZZEnd[int Index]={read=GetZZEnd};
    __property double HHBeg[int Index]={read=GetHHBeg};
    __property double HHEnd[int Index]={read=GetHHEnd};
    __property double HBeg[int Index]={read=GetHBeg};
    __property double HEnd[int Index]={read=GetHEnd};
    __property double ZHBeg[int Index]={read=GetZHBeg};
    __property double ZHEnd[int Index]={read=GetZHEnd};
    __property double i1000[int Index]={read=Geti1000};
    __property double Diam[int Index]={read=GetDiam};
    __property double Len[int Index]={read=GetLen}; // ��������� ����� Index �������
};
class CNapWodowod
{
    private:
        CTruba *Truba; // ����� � ������� ��������� �������� �������.
    public:
    int NNap; // ���������� �������� ���������.
    CNapWodowod(CTruba *TRUBA);
    void CreateReport(TStrings *Str);
    double GetV(void); // ���������� �������� � ������� ���������
    void CreateSpecialReport(TStrings *String);
    AnsiString CreateLineTableSmall(void);    
};
class CPerepadWells
{
private:
    CTruba *Truba;
    double GetZLBeg(int Index);
    double GetZLEnd(int Index);
    double GetHHEnd(int Index);
    double GetLen(int Index);
    double GetZBeg(int Index); // ������� ����� � ������ ��� Index ����������
    double GetZEnd(int Index); // ������� ����� � �����
    double GetZHBeg(int Index); // ������� ����� � ������ ��� Index ����������
    double GetZHEnd(int Index); // ������� ����� � �����
    double h;
public:
    CPerepadWells(CTruba *TRUBA);
    void CreateReport(TStrings *Strings);
    void CreateSpecialReport(TStrings *String);
    AnsiString CreateLineTableSmall(void);
    bool CreateOtm(TStrings *Str); // �������� ������ �������    
    int NWells;
    ~CPerepadWells(); // ���������� ���������� ��������
    __property double ZLBeg[int Index]={read=GetZLBeg};
    __property double ZLEnd[int Index]={read=GetZLEnd};
    __property double ZHBeg[int Index]={read=GetZHBeg};
    __property double ZHEnd[int Index]={read=GetZHEnd};
    __property double HHEnd[int Index]={read=GetHHEnd};
    __property double Len[int Index]={read=GetLen};
    __property double ZBeg[int Index]={read=GetZBeg};
    __property double ZEnd[int Index]={read=GetZEnd};
};

//---------------------------------------------------------------------------

// ���������� ��������� ����.

extern CMaterial MAsbest1;//("���������������� �����������","���� 1839");
extern CMaterial MAsbest2;//("���������������� ��������","���� 539");
extern CMaterial MKeramika;//("������������","���� 286");
extern CMaterial MBeton;//("�������������� �����������","���� 6482");
extern CMaterial MStal; // ("�������� ��������������","���� 8696",0.012,8);
// ������ ���� �����������

extern CSortamentItem *SortamentName[];

// ���������� ���������� ����

extern CSortament *SAsbest[]; // ���������������� �����
extern CSortament *SKeramika[]; // ������������ �����
extern CSortament *SBeton[]; // �������������� �����
extern CSortament *SKeramikaBeton[]; // �������� + �����
extern CSortament *SStal[]; // �������� ��������������

// ������ � ������������ ����������� ������� �� ���������� ������

extern AnsiString NameMest[];
extern double MD[];


// �������

extern double MaxNapoln(
    double Diam // �������� ������� �����
); // ���������� ���������� ��� ����������� �������� �����.
extern double sqr(double X); // ���������� x^2
extern CSortamentItem * GetSortamentItem(
    CSortamentItem *SortArray[], // ��������� �� ������ �����������
    CSortament *Sortament[] // ��������� �� ������ ���������
    );// ����� � ������� ����������� �������� � ������ �����������,
// ���������� NULL - ���� �� ���������, ��� ����� �������� �������.

//extern AnsiString Fmt(double X); // ����������� X ��� "  00.00"



#endif
