#include "mtxmul.h" /*dolaczenie wlasnego pliku naglowkowego*/
int main(int argc, char** argv)	/*uruchamianie z lini polecen - liczba i tablica argum*/
{
	long rozmiar;
	iloscarg(argc); /*spr ilosci argumentow*/
	format(argc, argv); /*sprawdzenie formatu zadanych plikow*/
	FILE **pliki = otwarcieplikow(argc, argv); /*utworzenie tablicy plikow*/
	int *wierszeplik = (int*)malloc((argc - 1) * sizeof(int)); /*alokacja tablic z iloscia wierszy i kolumn dla kazdej macierzy wejsciowej*/
	int *kolumnyplik = (int*)malloc((argc - 1) * sizeof(int));
	poprawnoscdanychwej(argc, argv, wierszeplik, kolumnyplik, pliki); /*sprawdzenie poprawnosci i wykonalnosci dzialania programu*/
	wykonalnoscmnozenia(argc, argv, wierszeplik, kolumnyplik, pliki);
	rozmiar = powrotnapocz(argc, pliki); /*powrot na pocz wszystkich plikow i ustalenie dlugosci najdluzszego*/
	liczba** macierzW = operacjamnozenia(argc, wierszeplik, kolumnyplik, pliki, rozmiar); /*utworzenie macierzy wynikowej*/
	wynik(argc, wierszeplik, kolumnyplik, macierzW, pliki); /*wpis wyniku i zwolnienie zasobow*/
	return 0;
}