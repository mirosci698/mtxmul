/*Program mnozy dowolna (od 1 do n) maciezy zespolonych zapisanych w pliku tekstowym.
W pliku *txt mamy liczby zespolone w formacie a+bi, gdzie a, b to rzeczywiste reprezentowane przez double.
Liczby rozdzielone sa bialymi znakami (tab i spacja) a znakiem konca wiersza jest enter.
W pliku nie moga znajdowac sie zadne inne znaki niz tab, spacja, enter, i +, - i cyfry od 0 do 9 oraz kropka, która jest separatorem.
Wynik jest zapisywany w pliku macierzwyn.txt w zamontowanym katalogu.*/
/*Program wymaga ustawienia _CRT_SECURE_NO_WARNINGS w Visual Studio.*/
#ifndef mtxmul_h /*definicja liku naglowkowego*/
#define mtxmul_h
#include <stdio.h> /*doloczenie biblioteki standardowej*/
typedef struct Liczba_ { /*struktura zawieraj¹ca wartoœci realis i imaginalis liczby zespolonej*/
	double re;
	double im;
} liczba;	/*deklaracje funkcji uzytych w programie*/
void iloscarg(int argumenty);
void format(int argumenty, char** parametry);
FILE ** otwarcieplikow(int argumenty, char** parametry);
void poprawnoscdanychwej(int argumenty, char** parametry, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow);
void wykonalnoscmnozenia(int argumenty, char** parametry, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow);
long powrotnapocz(int argumenty, FILE** tablicaplikow);
liczba** alokacjamacierzy(int nrpliku1, int nrpliku2, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow, int argumenty);
int odczyt(int* kolumnymacierzy, int nrpliku, liczba** macierz, FILE** tablicaplikow, long rozm);
void mnozenie(int *wierszemacierzy, int *kolumnymacierzy, int nriteracji, liczba** macierzA, liczba** macierzB, liczba** macierzwyn);
void zwolnijmiejsce(int *wierszemacierzy, int nriteracji, liczba** macierz);
void wynik(int argumenty, int *wierszemacierzy, int *kolumnymacierzy, liczba** macierzwyn, FILE** tablicaplikow);
liczba** operacjamnozenia(int argumenty, int* wierszemacierzy, int* kolumnymacierzy, FILE** tablicaplikow, long roz);
void zamykanieplikow(int argumenty, FILE** tablicaplikow);
#endif
