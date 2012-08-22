#ifndef  __MATHDLL_H
#define  __MATHDLL_H
// DLL математических функций ( 2001 ) Логунцов С.В. ХГТУ УИТС-01
//-------------------------------------------------------
// Возможности:
//  1. Вычисление интегралов функций и табличных значений
//  2. * Работа с матрицами
//  3. Работа с комплексными числами
//  4. Решение систем линейных уравнений
//  5. Решение уравнения с одной неизвестной
//  6. Апроксимация полиномом табличных значение
//  7. Распознавание выражений (полезная вещь)
//  8. * Работа с дробями, вычисление НОК и НОД.
//  9. * Решение систем нелинейных уравнений
//  10. Вычисление максимумов
//--------------------------------------------------------
// * - не тестировано
//---------------------------------------------------------------------------



//
//  Объект типа Матрица
//
class _export Matrix
{
    private:
    public:
        int X,Y; // Порядок матрицы
        double **Mx; // Указатель на таблицу чисел матрицы
        Matrix(int x,int y); // Создание объекта Матрица
        ~Matrix(); // Удаление объекта матрица
        Matrix operator + (Matrix& b); // Сложение двух матриц
        Matrix operator - (Matrix& b); // Сложение двух матриц
        Matrix operator = (Matrix &b); // Присвоение одной матрицы значения другой.
        Matrix operator * (double b); // Сложение двух матриц
};
//
//  Объект типа Комплексное число
//
class _export Complex
{
    public:
        double a; // Действительная часть
        double b; // Мнимая часть
        Complex(double A,double B); // Создание объекта типа Комплексное число
        ~Complex(); // Удаление объекта Комплексное число
        Complex operator = (Complex B); // Присваивание
        Complex operator + (Complex B); // Сложение
        Complex operator - (Complex B); // Вычитание
        Complex operator * (Complex B); // Умножение
        Complex operator / (Complex B); // Деление
        bool operator == (Complex B); // Сравнение
        Complex operator * (double f); // Умножение на число
        Complex operator / (double f); // Деление на число
};
//
//  Численные методы
//
//
//  Решение уравнения одной переменной методом деления отрезка
//  пополам.
//
int _export ReshUrDOP(
    double (*func)(double), // Указатель на функцию уравнение
        // которое нужно решить func(x)=0
    double a, // Левая граница поиска корней
    double b, // Правая граница поиска корней
    double e, // Абсолютная точность поиска корней
    double h, // Шаг перебора
    double *Buf, // Буфер для помещения корней.
    int SizeBuf // Размер буфера Buf в количестве корней.
    ); // Выдает количество найденных корней
//
//  Вычисление значения интерполяционного многочлена Лагранжа
//
double _export ZnLagrang(
    double x0, // аргумент интерполяционного многочлена
    double *x, // массив значений абсцисс точек
    double *y, // массив значений ординат точек
    int n // количество пар координат (кол-во точек)
    ); // Результат - значение Y интерполяционного многочлена при
        // аргументе x0
//
//  Вычисление интеграла методом трфпеций (интегрирование прямой)
//  по нескольким точкам, в x[0] - минимальное значение абсциссы,
//  в x[n-1] - максимальное
//
double _export IntgrTrap(
    double *x, // Абсциссы точек
    double *y, // Ординаты точек
    int n // Количество точек
    ); // Результат - значение интеграла
//
// Вычисление интеграла функции методом трапеций
//
double _export IntgrTrapFunc(
    double (*func)(double), // Функция
    double a, // Левая граница интегрирования
    double b, // Правая граница интегрирования
    double e // Точность интегрирования
    ); // Результат - значение интеграла
//
//  Решение систем линейных уравнений методом Гаусса
//
int _export SistLinUrGauss(
        double **a, // Матрица коэффициентов уравнений
        double *b, // Массив свободных членов
        double *x, // Массив для записи корней
        int n // Количество неизвестных
        ); // Возвращает 0 - Если все нормально, иначе код ошибки.
//
// Апроксимация полиномом по методу наименьших квадратов
//
double _export AprPolinom(
    double *x, // Массив значений абсцисс
    double *y, // Массив значений ординат
    int h, // Количество пар координат x и y
    double *a0, // Массив для занесения найденных коэффициентов
    int n // Количество коэффициентов в a0 = Степень полинома + 1
    ); // Возвращает сумму квадратов отклонений между заданными точками
      // и полиномом.
//
// Значение степенного полинома
//
double _export ZnPolinom(double X , // Значение аргумента
                 double *A , // Массив коэффициентов полинома
                 int N // Количество коэффициентов = Степень полинома +1
                 ); // Возвращает значение стпенного полинома

//
// Решение системы n нелинейных уравнений модифицированным методом Ньютона
//
int __export ReshSistNeLinUrMN(
    double (*func)(int n,double *x), // Указатель на функцию возвращающую значения
        // нелинейных функций указанных в системе
        // double (*func)(int n,double *x)
        // n - номер вычисляемой функции
        // *ч - указатель на массив аргументов функции
        // функция должна возвращать значение n-ой функции в системе
    int n, // Порядок системы - число уравнений
    int m, // Максимальное число итераций
    double e, // Точность решения
    double *x0 // Вектор начальных приближений, полученное решение
    ); // Возвращает количество выполненных итераций

//
// Решение системы дифференциальных уравнений методом Рунге-Кутта
//
int __export SistDifUr(
    double (*func)(int n,double x,double *y,double n0)
        // Указатель на функцию возвращающую значения
        // нелинейных функций указанных в системе
        // double (*func)(int n,double x,double *y)
        // n - номер вычисляемой функции
        // x - значения аргумента функций
        // *y - указатель на массив значений производных
        // функция не должна менять значения y
        // n0 - количество производных (порядок системы)
        // функция должна возвращать значение n-ой функции в системе
    ,int n // Число уравнений
    ,double x // Начальное значение X
    ,double h // Шаг изменения X
    ,double k // Количество получаемых точек.
    ,double *w // Начальные значения производных
    ,double **rez // Массив результатов (значения производных)
        // массив должен быть размерности [n][u]
    ); // Возвращает код ошибки
    // 0 - нет ошибок.

//
//  Вычисление максимума функции методом золотого сечения
//
double __export MaxFuncMZS(
    double (*func)(double), // Указатель на функция значение которой нужно
    // вычислить
    double a, // Левая граница поиска
    double b, // Правая граница поиска
    double e // Погрешность результата
    ); // Возвращает значение аргумента в котором наблюдается максимум

//
//  Работа с целыми числами и дробями
//
long _export NOD(long a,long b); // Наибольший общий делитель
long _export NOK (long a,long b); // Наименьшее общее кратное
class _export Fraction
{
    private:
        long numer; // Числитель дроби
        long denomir; // Знаменатель дроби
    public:
        Fraction();
        Fraction(long Numer,long Denomir);
        double ToDouble(void);
        void Shorten(void); // Сокращение дроби
        Fraction operator * (Fraction b);
        Fraction operator / (Fraction b);
        Fraction operator + (Fraction b);
        Fraction operator - (Fraction b);
        bool operator == (Fraction b);
        bool operator > (Fraction b);
        bool operator < (Fraction b);
        bool operator >= (Fraction b);
        bool operator <= (Fraction b);
        bool operator != (Fraction b);
        bool operator == (double b);
        bool operator > (double b);
        bool operator < (double b);
        bool operator <= (double b);
        bool operator >= (double b);
        Fraction operator = (Fraction b);
        Fraction operator + (long b);
        Fraction operator - (long b);
        Fraction operator * (long b);
        Fraction operator / (long b);
};
/*
//
// Работа с нормальными сплайнами (m1=0;mn=0)
//
class _export Spline
{
    private:
        double *M,*H;
    public:
        double *X;
        double *Y;
        int N;

        Spline(double *X, // Массив значений аргумента
               double *Y, // Массив значений функции
               int n // Количество значений аргумента и значений функции
               ); // Конструктор
        ~Spline(); // Деструктор
        double Eval(double X); // Вычисление значения сплайна при X;
        bool SolveSpline(void); // Вычисление коэффициентов сплайна
};
*/
//
//
// Разбор выражений: скачено с InterNet'а с дополнениями
//
//

struct ExpressionIerarchy
{
  int Function;
  int ArgumentType1;
  AnsiString Argument1;
  int ArgumentType2;
  AnsiString Argument2;
};

class _export Expression
{
protected:
        ExpressionIerarchy *EA;
        int OperationIndex;
        double *X;
        AnsiString __fastcall ReturnString(int StrNum);
        int N;
public:
        Expression(int n // максимальное количество переменных в выражении
        );
        ~Expression();
        void __fastcall Analize(AnsiString BaseString); // Начальный анализ функции
        void __fastcall PutInEA(AnsiString CurrentString, int ArgumentNumber); // Служебная функция
        void __fastcall SetX(double * XIn); // Пересылка занчений аргументов в массив Expression
        int __fastcall ReadOI(); // Выдает количество выражений в функции
        int __fastcall PrASCaC(AnsiString BaseString);// Если выражение содержит ошибку, выдает код ошибки
            // Коды ошибок: 1 - Неожиданный символ в выражении
            // 2 - Строка должна содержать по крайней мере один символ
            // 3 - ')' потеря
            // 4 - '(' потеря
        double __fastcall Evaluate(int StrNum); // Вычисление выражения под номером StrNum
        double __fastcall EvalExpress(void); // Вычисляет выражение полностью
        AnsiString __fastcall Derivative(int StrNum); // Вычисление производной выражения под номером StrNum
        AnsiString __fastcall DerivExpress(void); // Вычисление производной
};

//
//  Вспомогательные функции, операторы, типы
//
struct ParamFunc // Структура для передачи функциям дополнительных параметров.
{
    int NumDParam; // Количество числовых параметров
    double *DParam; // Возможные числовые параметры
    void * VoidParam; // Указатель на параметр неизвестного типа.
};
extern ParamFunc ParFunc; // Дополнительные параметры какой-либо функции
    // Если функция требует дополнительные параметры (кроме X)
    // ,то они должны передаваться через ParFunc, а функция выбирать
    // из ParFunc.

//---------------------------------------------------------------------------

#endif
