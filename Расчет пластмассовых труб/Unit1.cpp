//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#include "mathdll.h"
#include <math.hpp>
#include <math.h>
#include <iostream.h>
#include <conio.h>


#pragma argsused
/*
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
*/
double ke=0.00001;
double D,R,uk,lam;
double GetW(double D , double h)
{
    double alfa = (2*h-1<1)? 2 * ArcCos(2 * h - 1): 0;
    return (D*D/8) * (2*M_PI-alfa+sin(alfa));
}

double GetR(double D , double h)
{
    double alfa = (2*h-1<1)? 2 * ArcCos(2 * h - 1): 0;
    double a0 = 2 * 3.1415926 - alfa;
    double b0 = sin(alfa);
    return D / 4* (a0 + b0) / a0;
}
double GetB(double D , double V, double R)
{
    double Rs=lam;
    double Rekv=(500.0*4.0*R)/ke;
    double Ref=(V*4.0*R)/(1.49e-6);
    double b=3-log10(Rekv)/log10(Ref);
    //cout << "\n" << Ref << " " << Rekv << " " << b << " " << lam <<"\n";    
    if (b > 2) b = 2;
    return b;
}


double func(double x)
{
    double x0=pow(x,GetB(D,x,R));
    return x0-8*9.81*R*uk/lam;
}
double GetV(double D0,double Uk,double h)
{
    D=D0;uk=Uk;
    R=GetR(D,h);
    lam = 0.2*pow(ke/(4.0*R),0.3124*pow(ke,0.0516)); //pow(10, (0.258 * log10(0.8911 / R) - 2.0667));
    double buf[2]={0,0};
    ReshUrDOP(func,0.1,7,0.005,1,buf,2);
    double v=buf[0];
    return v;

}
double GetQ(double D,double nap,double V)
{
    return V*GetW(D,nap)*1000;
}
int main(int argc, char* argv[])
{
    double uk0,D0;
    cout << "Dfakt=";cin >> D0;
    cout << "ke=";cin >> ke;
    D0=D0/1000;
    while(true)
    {
    cout << "uk=";cin >>uk0;
    uk0=uk0/1000;
    cout << "  h          Q          V \n";
    int i=0;
    for( double nap=0.05;nap<=1.05;nap=nap+0.05)
    {
        i++;
        double V=GetV(D0,uk0,nap);
        printf(" %-4.2f      %-6.3f      %-6.2f \n",nap,GetQ(D0,nap,V),V);
        if (i%5==0) printf("\n");
    }
    }
    return 0;
}
//---------------------------------------------------------------------------


