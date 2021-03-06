#ifndef  __MATHDLL_H
#define  __MATHDLL_H
// DLL �������������� ������� ( 2001 ) �������� �.�. ���� ����-01
//-------------------------------------------------------
// �����������:
//  1. ���������� ���������� ������� � ��������� ��������
//  2. * ������ � ���������
//  3. ������ � ������������ �������
//  4. ������� ������ �������� ���������
//  5. ������� ��������� � ����� �����������
//  6. ������������ ��������� ��������� ��������
//  7. ������������� ��������� (�������� ����)
//  8. * ������ � �������, ���������� ��� � ���.
//  9. * ������� ������ ���������� ���������
//  10. ���������� ����������
//--------------------------------------------------------
// * - �� �����������
//---------------------------------------------------------------------------



//
//  ������ ���� �������
//
class _export Matrix
{
    private:
    public:
        int X,Y; // ������� �������
        double **Mx; // ��������� �� ������� ����� �������
        Matrix(int x,int y); // �������� ������� �������
        ~Matrix(); // �������� ������� �������
        Matrix operator + (Matrix& b); // �������� ���� ������
        Matrix operator - (Matrix& b); // �������� ���� ������
        Matrix operator = (Matrix &b); // ���������� ����� ������� �������� ������.
        Matrix operator * (double b); // �������� ���� ������
};
//
//  ������ ���� ����������� �����
//
class _export Complex
{
    public:
        double a; // �������������� �����
        double b; // ������ �����
        Complex(double A,double B); // �������� ������� ���� ����������� �����
        ~Complex(); // �������� ������� ����������� �����
        Complex operator = (Complex B); // ������������
        Complex operator + (Complex B); // ��������
        Complex operator - (Complex B); // ���������
        Complex operator * (Complex B); // ���������
        Complex operator / (Complex B); // �������
        bool operator == (Complex B); // ���������
        Complex operator * (double f); // ��������� �� �����
        Complex operator / (double f); // ������� �� �����
};
//
//  ��������� ������
//
//
//  ������� ��������� ����� ���������� ������� ������� �������
//  �������.
//
int _export ReshUrDOP(
    double (*func)(double), // ��������� �� ������� ���������
        // ������� ����� ������ func(x)=0
    double a, // ����� ������� ������ ������
    double b, // ������ ������� ������ ������
    double e, // ���������� �������� ������ ������
    double h, // ��� ��������
    double *Buf, // ����� ��� ��������� ������.
    int SizeBuf // ������ ������ Buf � ���������� ������.
    ); // ������ ���������� ��������� ������
//
//  ���������� �������� ����������������� ���������� ��������
//
double _export ZnLagrang(
    double x0, // �������� ����������������� ����������
    double *x, // ������ �������� ������� �����
    double *y, // ������ �������� ������� �����
    int n // ���������� ��� ��������� (���-�� �����)
    ); // ��������� - �������� Y ����������������� ���������� ���
        // ��������� x0
//
//  ���������� ��������� ������� �������� (�������������� ������)
//  �� ���������� ������, � x[0] - ����������� �������� ��������,
//  � x[n-1] - ������������
//
double _export IntgrTrap(
    double *x, // �������� �����
    double *y, // �������� �����
    int n // ���������� �����
    ); // ��������� - �������� ���������
//
// ���������� ��������� ������� ������� ��������
//
double _export IntgrTrapFunc(
    double (*func)(double), // �������
    double a, // ����� ������� ��������������
    double b, // ������ ������� ��������������
    double e // �������� ��������������
    ); // ��������� - �������� ���������
//
//  ������� ������ �������� ��������� ������� ������
//
int _export SistLinUrGauss(
        double **a, // ������� ������������� ���������
        double *b, // ������ ��������� ������
        double *x, // ������ ��� ������ ������
        int n // ���������� �����������
        ); // ���������� 0 - ���� ��� ���������, ����� ��� ������.
//
// ������������ ��������� �� ������ ���������� ���������
//
double _export AprPolinom(
    double *x, // ������ �������� �������
    double *y, // ������ �������� �������
    int h, // ���������� ��� ��������� x � y
    double *a0, // ������ ��� ��������� ��������� �������������
    int n // ���������� ������������� � a0 = ������� �������� + 1
    ); // ���������� ����� ��������� ���������� ����� ��������� �������
      // � ���������.
//
// �������� ���������� ��������
//
double _export ZnPolinom(double X , // �������� ���������
                 double *A , // ������ ������������� ��������
                 int N // ���������� ������������� = ������� �������� +1
                 ); // ���������� �������� ��������� ��������

//
// ������� ������� n ���������� ��������� ���������������� ������� �������
//
int __export ReshSistNeLinUrMN(
    double (*func)(int n,double *x), // ��������� �� ������� ������������ ��������
        // ���������� ������� ��������� � �������
        // double (*func)(int n,double *x)
        // n - ����� ����������� �������
        // *� - ��������� �� ������ ���������� �������
        // ������� ������ ���������� �������� n-�� ������� � �������
    int n, // ������� ������� - ����� ���������
    int m, // ������������ ����� ��������
    double e, // �������� �������
    double *x0 // ������ ��������� �����������, ���������� �������
    ); // ���������� ���������� ����������� ��������

//
// ������� ������� ���������������� ��������� ������� �����-�����
//
int __export SistDifUr(
    double (*func)(int n,double x,double *y,double n0)
        // ��������� �� ������� ������������ ��������
        // ���������� ������� ��������� � �������
        // double (*func)(int n,double x,double *y)
        // n - ����� ����������� �������
        // x - �������� ��������� �������
        // *y - ��������� �� ������ �������� �����������
        // ������� �� ������ ������ �������� y
        // n0 - ���������� ����������� (������� �������)
        // ������� ������ ���������� �������� n-�� ������� � �������
    ,int n // ����� ���������
    ,double x // ��������� �������� X
    ,double h // ��� ��������� X
    ,double k // ���������� ���������� �����.
    ,double *w // ��������� �������� �����������
    ,double **rez // ������ ����������� (�������� �����������)
        // ������ ������ ���� ����������� [n][u]
    ); // ���������� ��� ������
    // 0 - ��� ������.

//
//  ���������� ��������� ������� ������� �������� �������
//
double __export MaxFuncMZS(
    double (*func)(double), // ��������� �� ������� �������� ������� �����
    // ���������
    double a, // ����� ������� ������
    double b, // ������ ������� ������
    double e // ����������� ����������
    ); // ���������� �������� ��������� � ������� ����������� ��������

//
//  ������ � ������ ������� � �������
//
long _export NOD(long a,long b); // ���������� ����� ��������
long _export NOK (long a,long b); // ���������� ����� �������
class _export Fraction
{
    private:
        long numer; // ��������� �����
        long denomir; // ����������� �����
    public:
        Fraction();
        Fraction(long Numer,long Denomir);
        double ToDouble(void);
        void Shorten(void); // ���������� �����
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
// ������ � ����������� ��������� (m1=0;mn=0)
//
class _export Spline
{
    private:
        double *M,*H;
    public:
        double *X;
        double *Y;
        int N;

        Spline(double *X, // ������ �������� ���������
               double *Y, // ������ �������� �������
               int n // ���������� �������� ��������� � �������� �������
               ); // �����������
        ~Spline(); // ����������
        double Eval(double X); // ���������� �������� ������� ��� X;
        bool SolveSpline(void); // ���������� ������������� �������
};
*/
//
//
// ������ ���������: ������� � InterNet'� � ������������
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
        Expression(int n // ������������ ���������� ���������� � ���������
        );
        ~Expression();
        void __fastcall Analize(AnsiString BaseString); // ��������� ������ �������
        void __fastcall PutInEA(AnsiString CurrentString, int ArgumentNumber); // ��������� �������
        void __fastcall SetX(double * XIn); // ��������� �������� ���������� � ������ Expression
        int __fastcall ReadOI(); // ������ ���������� ��������� � �������
        int __fastcall PrASCaC(AnsiString BaseString);// ���� ��������� �������� ������, ������ ��� ������
            // ���� ������: 1 - ����������� ������ � ���������
            // 2 - ������ ������ ��������� �� ������� ���� ���� ������
            // 3 - ')' ������
            // 4 - '(' ������
        double __fastcall Evaluate(int StrNum); // ���������� ��������� ��� ������� StrNum
        double __fastcall EvalExpress(void); // ��������� ��������� ���������
        AnsiString __fastcall Derivative(int StrNum); // ���������� ����������� ��������� ��� ������� StrNum
        AnsiString __fastcall DerivExpress(void); // ���������� �����������
};

//
//  ��������������� �������, ���������, ����
//
struct ParamFunc // ��������� ��� �������� �������� �������������� ����������.
{
    int NumDParam; // ���������� �������� ����������
    double *DParam; // ��������� �������� ���������
    void * VoidParam; // ��������� �� �������� ������������ ����.
};
extern ParamFunc ParFunc; // �������������� ��������� �����-���� �������
    // ���� ������� ������� �������������� ��������� (����� X)
    // ,�� ��� ������ ������������ ����� ParFunc, � ������� ��������
    // �� ParFunc.

//---------------------------------------------------------------------------

#endif
