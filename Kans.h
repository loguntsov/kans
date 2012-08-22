//---------------------------------------------------------------------------

#ifndef KansH
#define KansH

#include "Truby.h"

class CGeoPoint;
class CGeoSegment
{
    private:
        CGeoPoint *PBeg; // Указатель на точку начала участка
        CGeoPoint *PEnd; // Указатель на точку конца участка
        double Len; // Длина участка
        double HBeg; // Начальное заглубление трубы
        CSortamentItem *SortamentItem; // Список возможных сортаментов
        double Q;
        int NumBeg;
        int NumEnd;
    public:
        CGeoSegment(
            CSortamentItem *SORTAMENTITEM, // Ссылка на сортаметы
            CGeoPoint *PBEG, // точка начала участка
            CGeoPoint *PEND, // точка конца участка
            double LEN, // длина участка
            double HBeg, // Начальное заглубление трубы
            double Qpop, // Попутный расход на участке, м3/сут
            AnsiString NAME, // Название участка
            int NBeg, // Номер начальной точки
            int NEnd // Номер конечной точки
        );
        AnsiString Name;
        ~CGeoSegment();
        CTruba *Truba;
        void Calculation(void); // Вычисление характеристик участка
        bool IsCalculated(void); // Возвращает true если участок был расчитан.
        void CreateReport(TStrings *Strings); // Создание отчета по участку.
        AnsiString CreateLineTable(void);
        void CreateSpecialReport(TStrings *Strings); // Создание отчета об инженерных конструкциях на участке
        bool CreateLineTableSmall(TStrings *Str);
        bool CreateReportOtm(TStrings *Str); // Создание отчета отметок об участке
};
class CGeoPoint
{
private:
    TList *SegmentBefore; // Список труб втекающих в данную точку
public:
    CGeoPoint(
        double Z, // Геометричекая высота точки
        double Q0 // Сосредоточенный расход м3/ч
        );
    ~CGeoPoint();
    void AddBefore(CGeoSegment *Seg);
    void DeleteBefore(CGeoSegment *Seg);
    double z; // Геометрическая высота точки
    double q; // Расход проходящий через данную точку
    double qliv; // Ливневой расход проходящий через данную точку.
    double q0;
    double qs; // Сумма сосредоточенных расходов с предыдущих участков
    double v; // Максимальная скорость течения жидкости приходящей в точку
    double h; // Максимальное заглубление труб, приходящих в точку.
    void Calculation(void); // Расчет предыдущих участков и подведение итогов в Q и V;
    CGeoSegment * Items(int i);
    int CountBefore(void);
    bool TheEndPoint;
    CTruba * GetTrubaOfMinHil(void); // Возвращает трубу с минимальной отметкой шелыги
    CTruba * GetTrubaOfMinLot(void); // Возвращает трубу с минимальной отметкой лотка
};

extern double GetKnerawn(double q); // Возвращает общий коэффициент неравномерности для данного расхода
//---------------------------------------------------------------------------
#endif
