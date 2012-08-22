//---------------------------------------------------------------------------

#ifndef TrubyH
#define TrubyH

extern double Hmin; // Минимальное заглубление
extern double Hmax; // Максимальное заглубление
extern double H0max; // Максимальное заглубление для насосной станции
extern double hPer; // Максимальная глубина перепадного колодца
extern double Md; // Максимальное суточное количество осадков по СНиП 2.01.01-82
extern double MinDim; // Минимальный диаметр в сети  
class CMaterial
{
    public:
    CMaterial(
        char *NamE, // наименование материала
        char *Gost, // гост на материал
        double N, // коэффициент шероховатости
        double VMAX // Максимальная скорость для данного материала
        );
    AnsiString Name; // Название материала;
// Характеристики материала
    double n; // Коэффициент шероховатости
    double Vmax; // Максимальная скорость
    AnsiString GOST;
};
class CDiametr
{
public:
    CDiametr(int Usl,double Fakt);
    CDiametr();
    int UslDiametr; // Условный диаметр.
    double FaktDiametr; // Фактический диаметр.
    double GetMaxNapoln(void); // Возвращает максимальное наполнение для данного диаметра
    double GetVtoNap100(double Q); // Возвращает скорость идеальной жидкости при 100% наполнении
};
class CSortament
{
private:
    double GetW(void); // Возвращает площадь живого сечения трубы
    double GetAlfa(void); // Возвращает угол
    double GetR(void); // Возвращает гидравлический радиус
public:
    CSortament(CDiametr Diam,CMaterial *Mat);
    CDiametr Diametr;
    CMaterial *Material;
    double GetVmin(void); // Возвращает минимальную скорость для данного сортамента
    double GetQmin(void); // Возвращает минимальный расход
    double Geti1000min(void); // Возвращает минимальный уклон
    double Geti1000(double Q); // Возвращает уклон трубы для данного диаметра при расходе Q и максимальном наполнении
};
class CSortamentItem
{
public:
    AnsiString Name;
    CSortament **Sortament;
    CSortamentItem(AnsiString Name,CSortament **Sortament);
    int GetNumSortament(double Diam); // Возвращает номер наиболее близкого сортамента к диаметру Diam
};

class CPumpStation;
class CPerepadWells;
class CNapWodowod;
class CTruba;
struct SInfoTruba
{
    CSortamentItem *SORTAMENTITEM; // Ссылка на сортамент труб
    double LEN; // Длина трубы
    double ZBEG; // Начальная отметка земли
    double ZEND; // Конечная отметка земли
    double HLBEG; // Минимальное начальное заглубление лотка трубы в начале
    double QCUR; // Расход входящий в трубу.
    double VVH; // Скорость входа жидкости в трубу.
    double QLIV; // Дополнительный инфильтрационный расход с предыдущих участков
    CTruba *TROFHIL; // Труба примыкающая к началу данной с минимальной отметкой шелыги
    CTruba *TROFLOT; // Труба примыкающая к началу данной с минимальной отметкой лотка
    double Q0; // Попутный расход (из исходных данных) + с предыдущих участков
    double QS; // Сосредоточенный расход
    int NBeg; // Наименование точки в начале участка
    int NEnd; // Наименование точки в конце участка
    //AnsiString NBeg; // Наименование точки в начале участка
    //AnsiString NEnd; // Наименование точки в конце участка
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
    bool IsDiametrGood(void); // Возвращает true если текущий диаметр удовлетворяет условиям выбора диаметров
    // выбирая при этом начальное заглубление лотка трубы (выравнивание по лотку или шелыги)
    bool FindDiametrAndi1000( // ищет подходящий уклон для текущего диаметра трубы, результаты записывает в NSortament и i1000
        double Hmin, // Минимальное заглубление
        double Hmax, // Максимальное заглубление
        bool f // если true - то ищем по минимальному заглублению, иначе по максимальному);
        );// возвращает true если все хорошо
    CPumpStation *PumpStation;
    CPerepadWells *PerepadWells;
    CNapWodowod *NapWodowod;
    friend CPerepadWells;
    friend CGeoSegment;//::CreateLineTableSmall;
public:
    CTruba(SInfoTruba Info);
    ~CTruba();
    SInfoTruba Info;
    __property CSortament * Sortament={read=GetSortament}; // Сортамент (диаметр и материал) трубы
    //double ZEnd; // Отметка земли в конце трубы
    __property double HEnd={read=GetHEnd,write=SetHEnd}; // Заглубление шелыги трубы в конце
    __property double V={read=GetV}; // Скорость в трубе.
    __property double ZLBeg={read=GetZLBeg}; // Отметка лотка в начале
    __property double ZLEnd={read=GetZLEnd}; // Отметка лотка в конце
    __property double ZHBeg={read=GetZHBeg}; // Отметка шилыги в начале
    __property double ZHEnd={read=GetZHEnd}; // Отметка шилыги в конце
    __property double i1000={read=Geti1000,write=Seti1000}; // Уклон под которым уложена труба.
    __property double HHEnd={read=GetHHEnd}; // Заглубление шелыги
    //CSortamentItem *SortamentItem;
    int NSortament;
    double HBeg; // Заглубление трубы в начале
    double hd; // Наполнение трубы
    double Q;
    double DH; // Приращение конечного заглубления для учета насосной станции и перепадных колодцев
     // DH>0 - насосная станция
     // DH=0 - обычный участок
     // DH<0 - перепадные колодцы
    double Qliv; // Дополнительный инфильтрационный расход с предыдущих участков + на текущем
    void ToMaxSortament(void); // Вычисление диаметра трубы, без учета рельефа
    void ToNapolnenie(void); // Расчет наполнения трубы, при заданных параметрах.
    void ToGetSortament(void); // Вычисление диаметра трубы с учетом рельефа.
    double GetVmin(void);
    bool Rash; // Признак не расчетного участка
    void CreateReport(TStrings *Strings); // Создание отчета по трубе
    void SetPumpStation(void); // Установка насной станции подкачки с высотой поднятия до H м
    void SetWells(double H); // Установка перепадных колодцев с падением заглубления по лотку до H м
    void SetNapWodowod(void); // Установка напорных водоводов.
    int GetNumSortamentFrom(double Diam); // Возвращает число сортаментов находящихся между диаметром данной трубы и Diam (фактический диаметр,мм)
    double GetDiametr(void); // Возвращает диаметр в метрах
    double Geti1000mest(void); // Возвращает уклон местности
    double GetNapolnenieToQPlusQliv(void);
    bool GS; // Применение гасител скорости.
    double Geti1000ToHmin(void); // Возвращает уклон трубы, чтобы она вышла на минимальное заглубление
    double Geti1000ToHmax(void); // Возвращвет уклон трубы, чтобы она вышла на максимальное заглубление
    double GetZmest(double Len); // Возвращает отметку земли на расстоянии Len от начала участка
    AnsiString CreateLineTable(void); // Создание отчета в виде строки таблицы
    void CreateSpecialReport(TStrings *Str); // Создание отчета об инженерных конструкциях на трубе
    bool CreateLineTableSmall(TStrings *Str); // Создание отчета об параметрах трубопровода без отметок
    bool CreateOtm(TStrings *Str); // Создание строки таблицы отметок
    bool CreateOtmSoor(TStrings *Str); // Создание отметок земли технических сооружений
    double GetQfromHD(double hd); // Возвращает расход при наполнении HD.
    double GetHBeg(void); // Возвращает начальное заглубление соответсвующее правила конструирования сети.
};
class CPumpStation // Насосная станция
{
private:
    CTruba *Truba;
    CTruba *Truba0;
    //CTruba *Truba; // Труба до насосной станции
    //CTruba *Truba1; // Труба после насосной станции
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
    __property double ZLBeg[int Index]={read=GetZLBeg}; // Думаю понятно что ЭТО
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
    __property double Len[int Index]={read=GetLen}; // Возращает длину Index участка
};
class CNapWodowod
{
    private:
        CTruba *Truba; // Труба к которой относится напорный водовод.
    public:
    int NNap; // Количество напорных водоводов.
    CNapWodowod(CTruba *TRUBA);
    void CreateReport(TStrings *Str);
    double GetV(void); // Возвращвет скорость в напоных водоводах
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
    double GetZBeg(int Index); // Отметка земли в начале для Index подучастка
    double GetZEnd(int Index); // Отметка земли в конце
    double GetZHBeg(int Index); // Отметка земли в начале для Index подучастка
    double GetZHEnd(int Index); // Отметка земли в конце
    double h;
public:
    CPerepadWells(CTruba *TRUBA);
    void CreateReport(TStrings *Strings);
    void CreateSpecialReport(TStrings *String);
    AnsiString CreateLineTableSmall(void);
    bool CreateOtm(TStrings *Str); // Создание списка отметок    
    int NWells;
    ~CPerepadWells(); // Количество перепадных колодцев
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

// Допустимые материалы труб.

extern CMaterial MAsbest1;//("асбестоцементные безнапорные","ГОСТ 1839");
extern CMaterial MAsbest2;//("асбестоцементные напорные","ГОСТ 539");
extern CMaterial MKeramika;//("керамические","ГОСТ 286");
extern CMaterial MBeton;//("железобетонные безнапорные","ГОСТ 6482");
extern CMaterial MStal; // ("стальные электросварные","ГОСТ 8696",0.012,8);
// Список всех сортаментов

extern CSortamentItem *SortamentName[];

// Допустимые сортаменты труб

extern CSortament *SAsbest[]; // Асбестоцементные трубы
extern CSortament *SKeramika[]; // Керамические трубы
extern CSortament *SBeton[]; // Железобетонные трубы
extern CSortament *SKeramikaBeton[]; // Керамика + Бетон
extern CSortament *SStal[]; // Стальные электросварные

// Данные о максимальном колличестве осадков на территории России

extern AnsiString NameMest[];
extern double MD[];


// Функции

extern double MaxNapoln(
    double Diam // Условный диаметр трубы
); // Возвращает наполнение для конкретного диаметра трубы.
extern double sqr(double X); // Возвращает x^2
extern CSortamentItem * GetSortamentItem(
    CSortamentItem *SortArray[], // Указатель на массив сортаментов
    CSortament *Sortament[] // Указатель на нужный сортамент
    );// Поиск в массиве сортаментов элемента с нужным сортаментом,
// возвращает NULL - если не обнаружен, или адрес элемента массива.

//extern AnsiString Fmt(double X); // Форматирует X как "  00.00"



#endif
