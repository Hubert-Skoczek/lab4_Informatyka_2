#define _CRT_SECURE_NO_WARNINGS
#include "math.h"
#include "stdio.h"
#include "winbgi2.h"
#include "stdlib.h"
#include "rk4.h"
double fun(double t,double y);
double y_analitycznie(double t);
double euler(double t,double y,double h,double (*fun)(double,double));
void main()
{
	double t0=0,y0=0;// warunek cauchy'ego y(0)=0 dany w poleceniu 6
	double wynik_eulera, wynik_rk4, blad_eulera, blad_rk4, wynik_analitycznie;
	double t,y,h,tk;
	int a;
	t=t0;
	y=y0;
	FILE* f;//otwarcie dwóch plików, f to plik z wynikami dla ostatniego kroku czasowego
	FILE* g;//g to plik z wynikami dla wszystkich kroków czasowych
	f=fopen("ostatniwynik.txt","w");
	g=fopen("bledywynik.txt","w");
	printf("podaj krawedz przedzialu calkowania\n");
	scanf("%lf",&tk);
	graphics(1000,1000);
	scale(0,-2,tk,2);
	printf("dla jakiego h chcesz aby wyswietlil sie wykres \n(wpisz liczbe a od 0 do 6 gdzie h=2^-a)\n");
	scanf("%d",&a);
	while(a>6 || a<0)//pętla zabezpiecza przed podaniem błędnego a
	{
		printf("a musi byc w przedziale <0;6>\n");
		scanf("%d",&a);
	}
	title("przykladowy wykres bledu od t dla h=2^-6 blad eulera-niebieski, blad rk4-zielony","","");//opis do wykresu
		for(int j=0;j<7;j++)
		{
			t=0;//resetowanie kroku czasowego po przejściu pierwszej pętli
			h=pow(2.,-j);//krok całkowania h=2^-a gdzie a jest od 0 do 6
			fprintf(f,"dla ostatniego kroku czasowego t gdzie krok calkowania h = %lf\n",h);
			fprintf(g,"\t\t\th = %lf\n",h);
			for(int i=0;i<tk/h;i++)
			{
				t+=h;//zwiększanie wartości t o krok czasowy
				fprintf(g,"\tt = %lf\n",t);
				wynik_analitycznie=y_analitycznie(t);//tu funkcje zwracają wartości do maina oraz obliczane są błedy
				wynik_eulera=euler(t,y,h,fun);
				blad_eulera=fabs(wynik_analitycznie-wynik_eulera)/wynik_eulera;
				fprintf(g,"blad euler = %lf\n",blad_eulera);
				wynik_rk4=rk4(t,y,h,fun);
				blad_rk4=fabs(wynik_analitycznie-wynik_rk4)/wynik_rk4;
				fprintf(g,"blad rk4 = %lf\n",blad_rk4);
				if(j==a)//ta pętla służy do narysowania wykresu dla takiego h jakie wybrał użytkownik
				{
					setcolor(0.0);//kolor niebieski
					circle(t,blad_eulera,1);
					setcolor(0.5);//kolor zielony
					circle(t,blad_rk4,1);	
				}
				if(i==tk/h-1)//ta pętla służy do zapisywania do pliku wyniku dla ostatniego kroku czasowego
				{
					fprintf(f,"\tt=%lf\n",t);
					fprintf(f,"\tblad euler: %lf\n",blad_eulera);
					fprintf(f,"\tblad rk4: %lf\n",blad_rk4);
				}
			}
		}
		fclose(f);//zamknięcie modyfikacji obu plików
		fclose(g);
	wait();
}
double euler(double t,double y,double h,double (*fun)(double,double))
{
		//obliczanie metodą Eulera zgodnie z algorytmem podanym w instrukcji
		y+=h*fun(y,t);//y=y+dy/dt
		return y;
}
double fun(double t,double y)
{
	return 100*(sin(t)-y);//funkcja podana w poleceniu 6, można ją zmienić ale, wtedy trzeba pamiętać o zmianie również wyliczenia analitycznego tego równania
}						 
double y_analitycznie(double t)
{
	return (sin(t)-0.01*cos(t)+0.01*exp(-100.*t))/1.0001;//wyliczone analitycznie równanie
}
