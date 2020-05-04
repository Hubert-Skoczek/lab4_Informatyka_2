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
	double t0=0.1,y0=0.5;// warunek cauchy'ego y(0.1)=0.5
	double wynik_eulera, wynik_rk4, blad_eulera, blad_rk4, wynik_analitycznie;
	double h,t,y,N,tk;
	t=t0;
	y=y0;
	FILE* f;//otwarcie dwóch plików, f to plik z wynikami dla ostatniego kroku czasowego
	FILE* g;//g to plik z wynikami dla wszystkich kroków czasowych
	f=fopen("ostatniwynik.txt","w");
	g=fopen("bledywynik.txt","w");
	printf("podaj krawedz przedzialu calkowania wieksza od zagadnienia poczatkowego t0=0.1\n");
	scanf("%lf",&tk);//w przypadku wpisania zbyt dużej liczby tk(>5) metoda eulera ze względu na niską stabilność może wyliczyć bardzo duży błąd
	while(tk<=t0)
	{
		printf("podaj krawedz przedzialu calkowania wieksza od zagadnienia poczatkowego t0=0.1\n");
		scanf("%lf",&tk);//zabezpiecznie przed wpisanie błędnego tk
	}
	tk=tk-t0;
	graphics(1000,1000);
	scale(0,0,tk/2,10);//wyskalowanie, w przypadku zbyt duzego tk nie wszystkie punkty zmieszcza sie na wykresie
	
	title("blad eulera-niebieski, blad rk4-zielony","","");//opis do wykresu
	for(int j=0;j<7;j++)
	{
		N=pow(2.,j);//N=2^0,2^1,2^2...
		h=tk/N;//obliczanie kroku całkowania
		t=t0;
		fprintf(g,"\t\t\tN = %lf\n",N);
		for(int i=0;i<N;i++)
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
			if(i==N-1)//ta pętla służy do zapisywania do pliku wyniku dla ostatniego kroku czasowego i rysowania wykresu
			{
				setcolor(0.0);//kolor niebieski
				circle(h,blad_eulera,2);
				setcolor(0.5);//kolor zielony
				circle(h,blad_rk4,2);
				fprintf(f,"N=%lf\n",N);//zapisywanie odpowiednich wartosci do pliku z wynikami dla ostatiego kroku czasowego
				fprintf(f,"\th=%lf\n",h);
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
	return 0.5*exp(0.5*(t-0.1));//funkcja dla lambdy = 0.5 t0 = 0.1 i y0 = 0.5;
}						 
double y_analitycznie(double t)
{
	return 0.5*exp(0.5*(t-0.1));//wyliczone analitycznie równanie
}
