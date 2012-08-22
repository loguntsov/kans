//---------------------------------------------------------------------------

#ifndef KansH
#define KansH

#include "Truby.h"

class CGeoPoint;
class CGeoSegment
{
    private:
        CGeoPoint *PBeg; // ��������� �� ����� ������ �������
        CGeoPoint *PEnd; // ��������� �� ����� ����� �������
        double Len; // ����� �������
        double HBeg; // ��������� ����������� �����
        CSortamentItem *SortamentItem; // ������ ��������� �����������
        double Q;
        int NumBeg;
        int NumEnd;
    public:
        CGeoSegment(
            CSortamentItem *SORTAMENTITEM, // ������ �� ���������
            CGeoPoint *PBEG, // ����� ������ �������
            CGeoPoint *PEND, // ����� ����� �������
            double LEN, // ����� �������
            double HBeg, // ��������� ����������� �����
            double Qpop, // �������� ������ �� �������, �3/���
            AnsiString NAME, // �������� �������
            int NBeg, // ����� ��������� �����
            int NEnd // ����� �������� �����
        );
        AnsiString Name;
        ~CGeoSegment();
        CTruba *Truba;
        void Calculation(void); // ���������� ������������� �������
        bool IsCalculated(void); // ���������� true ���� ������� ��� ��������.
        void CreateReport(TStrings *Strings); // �������� ������ �� �������.
        AnsiString CreateLineTable(void);
        void CreateSpecialReport(TStrings *Strings); // �������� ������ �� ���������� ������������ �� �������
        bool CreateLineTableSmall(TStrings *Str);
        bool CreateReportOtm(TStrings *Str); // �������� ������ ������� �� �������
};
class CGeoPoint
{
private:
    TList *SegmentBefore; // ������ ���� ��������� � ������ �����
public:
    CGeoPoint(
        double Z, // ������������� ������ �����
        double Q0 // ��������������� ������ �3/�
        );
    ~CGeoPoint();
    void AddBefore(CGeoSegment *Seg);
    void DeleteBefore(CGeoSegment *Seg);
    double z; // �������������� ������ �����
    double q; // ������ ���������� ����� ������ �����
    double qliv; // �������� ������ ���������� ����� ������ �����.
    double q0;
    double qs; // ����� ��������������� �������� � ���������� ��������
    double v; // ������������ �������� ������� �������� ���������� � �����
    double h; // ������������ ����������� ����, ���������� � �����.
    void Calculation(void); // ������ ���������� �������� � ���������� ������ � Q � V;
    CGeoSegment * Items(int i);
    int CountBefore(void);
    bool TheEndPoint;
    CTruba * GetTrubaOfMinHil(void); // ���������� ����� � ����������� �������� ������
    CTruba * GetTrubaOfMinLot(void); // ���������� ����� � ����������� �������� �����
};

extern double GetKnerawn(double q); // ���������� ����� ����������� ��������������� ��� ������� �������
//---------------------------------------------------------------------------
#endif
