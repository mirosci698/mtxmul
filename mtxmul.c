#include "mtxmul.h" /*dolaczenie wlasnego pliku naglowkowego*/
#include <string.h> /*dolaczenie biblioteki do operacji na lancuchach znakowych*/
void iloscarg(int argumenty) /*funkcja badajaca poprawnosc ilosci argum. wywolania, otrzymuje ich liczbe, nie zwraca nic*/
{
	if (argumenty < 2) /*musi byc przyn. nazwa programu i jednej macierzy*/
	{
		printf("Niepoprawna ilosc argumentow.");	/*komunikat i opuszczenie programu*/
		exit(1);
	}
}
void format(int argumenty, char** parametry) /*funkcja sprawdzajaca rozszerzenia podanych argum. macierzy, przyjmuje ilosc argum. i ich tablice, nie zwraca nic*/
{
	int i;
	for (i = 1; i < argumenty; i++) /*sprawdzanie we wszystkich oprocz pierwszego argumentu koncowych 4 znakow, jezeli nie jest to .txt to komunikat i opuszczenie programu*/
		if (strcmp(parametry[i] + strlen(parametry[i]) - 4, ".txt"))
		{
			printf("Zly format pliku.");
			exit(1);
		}
}
FILE ** otwarcieplikow(int argumenty, char** parametry)
{ /*funkcja tworzaca tablice wskaznikow do plikow, otrzymuje ilosc i tablice argumentow programu, zwraca wskaznik na pierwszy element tej tablicy*/
	int i;
	FILE **tablica = (FILE**)malloc((argumenty - 1) * sizeof(FILE*)); /*alokacja odpowiedniej ilosci elementow w tablicy*/
	if (!tablica)
	{ /*w przypadku bledu alokacji komunikat i opuszcza program*/
		printf("Nieudana alokacja tablicy plikow.");
		exit(1);
	}
	for (i = 1; i < argumenty; i++)
	{	/*otwarcie podanych przez uzytkownika plikow*/
		tablica[i - 1] = fopen(parametry[i], "r");
		if (!tablica[i - 1])
		{	/*w przypadku bledu przy ktoryms otwarciu: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
			printf("Nieudana alokacja tablicy plikow.");
			int j;
			for (j = 0; j < i - 1; i++)
				fclose(tablica[j]);
			free(tablica);
			exit(1);
		}
	}
	return tablica;
}
void poprawnoscdanychwej(int argumenty, char** parametry, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow)
{ /*funkcja sprawdzajaca poprawnosc formatu pliku (odpowiednie znaki), otrzymuje ilosc i tablice argumentow, tablice z iloscia wierszy i kolumn w danej macierzy a takze tablice z plikami*/
	int i;
	int wiersze = 0; /*zmienne do zliczania wierszy i kontroli ilosci kolumn*/
	int kolumny = 0;
	int kolumnywcalym = 0;
	char pop = 0;	/*pomocnicze do sprawdzania poprawnosci znakow i ich ustawienia*/
	char buf;
	for (i = 0; i < argumenty - 1; i++)
	{
		while ((buf = getc(tablicaplikow[i])) != EOF) /*zczyt znak po znaku*/
		{
			if ((buf < 48 || buf > 57) && buf != 'i' && buf != '.' && buf != '\n' && buf != ' ' && buf != '	' && buf != '+' && buf != '-')
			{ /*w przypadku nieprawidlowego znaku: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
				printf("W %s wystepuja nieprawidlowe znaki.", parametry[i + 1]);
				zamykanieplikow(argumenty, tablicaplikow);
				free(wierszemacierzy);
				free(kolumnymacierzy);
				exit(1);
			}
			if (buf == '\n') /*zakonczenie wiersza macierzy*/
			{
				if (kolumnywcalym == 0)
					kolumnywcalym = kolumny; /*sprawdzenie ilosci el w pierwszym wierszu*/
				else
					if (kolumny != 0 && kolumny != kolumnywcalym) /*sprawdzenie czy w danym wierszu byl element i czy jest poprawna ilosc*/
					{ /*w przypadku roznej ilosci el. w wierszach: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
						printf("%s nie jest macierza.", parametry[i + 1]);
						zamykanieplikow(argumenty, tablicaplikow);
						free(wierszemacierzy);
						free(kolumnymacierzy);
						exit(1);
					}
				if (kolumny != 0)	/*nowy wiersz tylko jak byly jakies elementy odczytane w poprzednim*/
					wiersze++;
				kolumny = 0;
			}
			if ((((buf > 47) && (buf < 58)) || buf == '-' || buf == 'i') && (((pop < 48) || (pop > 57)) && pop != '-' && pop != 'i' && pop != '+' && pop != '.'))
				kolumny++; /*element wtedy  gdy poprzednio byl bialy znak a teraz znak lub cyfra*/
			pop = buf; /*zapamietanie poprzedniego*/
		}
		if (kolumnywcalym == 0) /*powtorzenie procedury z petli dla ostatniego wiersza, ktory moze nie byc zakonczony enterem*/
			kolumnywcalym = kolumny;
		else
			if (kolumny != 0 && kolumny != kolumnywcalym)
			{
				printf("%s nie jest macierza.", parametry[i + 1]);
				zamykanieplikow(argumenty, tablicaplikow);
				free(wierszemacierzy);
				free(kolumnymacierzy);
				exit(1);
			}
		if (kolumny != 0)
			wiersze++;
		kolumny = 0;
		wierszemacierzy[i] = wiersze; /*zapisanie zadanych wartosci w tablicach i wyzerowanie zmiennych kontrolnych*/
		kolumnymacierzy[i] = kolumnywcalym;
		wiersze = 0;
		kolumnywcalym = 0;
		pop = 0;
	}
}
void wykonalnoscmnozenia(int argumenty, char** parametry, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow)
{ /*funkcja sprawdzajaca czy da sie wykonac mnozenie - odp ilosc wierszy i kolumn w poszczegolnych macierzach, otrzymuje ilosc argum. i tablice z nimi, tablice z ilosciami kolumn i wierszy w poszczeg. plikach i tablice z nimi, nie zwraca nic*/
	int i;
	for (i = 0; i < argumenty - 2; i++) /*sprawdzenie dla wszystkich plikow*/
		if (kolumnymacierzy[i] != wierszemacierzy[i + 1])
		{ /*w przypadku nieodpowiedniej ilosci: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
			printf("Nie mozna wykonac mnozenia macierzy %s i %s.", parametry[i + 1], parametry[i + 2]);
			zamykanieplikow(argumenty, tablicaplikow);
			free(wierszemacierzy);
			free(kolumnymacierzy);
			exit(1);
		}
}
long powrotnapocz(int argumenty, FILE** tablicaplikow)
{ /*funkcja przesuwajaca kursor w plikach na poczatek i odczytujaca dlugosc najdluzszego w celu optymalizacji bufora, otrzymuje ilosc argumentow i tablice z plikami, zwraca dlugosc najdluzszego z plikow w bajtach*/
	int i;
	long max = 0;
	for (i = 0; i < argumenty - 1; i++)
	{ /*sprawdzenie który jest najdluzsz za pomoca max*/
		if (max < ftell(tablicaplikow[i]))
			max = ftell(tablicaplikow[i]);
		fseek(tablicaplikow[i], 0L, SEEK_SET); /*przesuniecie na poczatek pliku*/
	}
	return max;
}
liczba** alokacjamacierzy(int nrpliku1, int nrpliku2, int *wierszemacierzy, int *kolumnymacierzy, FILE** tablicaplikow, int argumenty)
{ /*funkcja tworzaca macierz (tablice 2-wymiarowa), zwraca wskaznik na jej pierwszy element, otrzymuje numery elementow w tablicy, ktore odpowiadaja ilosci kolumn i macierzy, tablice z plikami i ilosc argumentow programu*/
	int i;
	liczba **macierz = (liczba**)malloc(wierszemacierzy[nrpliku1] * sizeof(liczba*));
	if (!macierz)
	{ /*w przypadku bledu alokacji pamieci: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
		printf("Nieudana alokacja macierzy.");
		free(wierszemacierzy);
		free(kolumnymacierzy);
		zamykanieplikow(argumenty, tablicaplikow);
		exit(1);
	}
	for (i = 0; i < wierszemacierzy[nrpliku1]; i++) /*alokacja wszystkich potrzebnych wierszy*/
	{
		macierz[i] = (liczba*)malloc(kolumnymacierzy[nrpliku2] * sizeof(liczba));
		if (!macierz[i])
		{ /*w przypadku bledu przy alokacji ktoregos z wierszy: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
			int j;
			printf("Nieudana alokacja macierzy.");
			free(wierszemacierzy);
			free(kolumnymacierzy);
			zamykanieplikow(argumenty, tablicaplikow);
			for (j = 0; j < i; j++)
				free(macierz[j]);
			free(macierz);
			exit(1);
		}
	}
	return macierz;
}
int odczyt(int* kolumnymacierzy, int* wierszemacierzy, int nrpliku, liczba** macierz, FILE** tablicaplikow, long rozm)
{ /*funkcja odczytujaca liczby zespolone z pliku, otrzymuje tablice z ilosciami kolumn i wierszy w odp macierzach, macierz do wczytu, tablice z plikami i rozmiar bufora, zwraca 1 przy poprwanym odczycie lub 0 brzy bledach*/
	int i;
	int kolumny = 0;
	int wiersze = 0;
	char *lancuch = (char*)malloc(rozm * sizeof(char)); /*alokacja bufora*/
	if (!lancuch)
	{ /*w przypadku bledu przy alokacji miejsca na bufor: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
		int j;
		printf("Nieudana alokacja bufora.");
		free(kolumnymacierzy);
		for (j = 0; j < wierszemacierzy[nrpliku]; j++)
			free(macierz[j]);
		free(macierz);
		return 0;
	}
	for (i = 0; i < rozm; i++) /*wyzerowanie bufora*/
		lancuch[i] = '\0';
	char *lancuch2;
	while (fscanf(tablicaplikow[nrpliku], "%s", lancuch) != EOF) /*odczyt rozdzielonych bialymi znakami wartosci*/
	{
		int a;
		int lkropek = 0;
		for (a = 0; a < strlen(lancuch); a++) /*sprawdzenie wszystkich znakow we wczytanej liczbie*/
		{
			if (lancuch[a] == 'i' && a != strlen(lancuch) - 1)
			{ /*w przypadku wystapienia jedn. urojonej poza koncem liczby: komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
				int j;
				printf("Zly zapis w pliku.");
				free(kolumnymacierzy);
				for (j = 0; j < wierszemacierzy[nrpliku]; j++)
					free(macierz[j]);
				free(macierz);
				free(lancuch);
				return 0;
			}
			if (lancuch[a] == '+' || lancuch[a] == '-')
				lkropek = 0; /*osobna liczba kropek dla realis i imaginalis*/
			if (lancuch[a] == '.')
				lkropek++; /*inkrementacja ilosci kropek*/
			if (lkropek > 1) /*jezeli bylo wiecej kropek (separator ulamkowy)*/
			{ /*komunikat, zwolnienie pamieci wczesniej przydzielonej i wyjscie z programu*/
				int j;
				printf("Zly zapis w pliku.");
				free(kolumnymacierzy);
				for (j = 0; j < wierszemacierzy[nrpliku]; j++)
					free(macierz[j]);
				free(macierz);
				free(lancuch);
				return 0;
			}
		}
		if (lancuch[strlen(lancuch) - 1] == 'i') /*sprawdzenie czy istnieje czesc urojona*/
		{
			int offset = strlen(lancuch) - 1;
			lancuch2 = lancuch + offset;
			while (lancuch2[0] != '-' && lancuch2[0] != '+' && offset > 0)
			{ /*odnalezienie poczatku czesci urojonej*/
				lancuch2 = lancuch2 - 1;
				offset--;
			}
			if (!strcmp(lancuch2, "i") || !strcmp(lancuch2, "-i") || !strcmp(lancuch2, "+i"))
				lancuch2[strlen(lancuch2) - 1] = '1'; /*wpis odpowiedniej liczby*/
			else
				lancuch2[strlen(lancuch2) - 1] = '\0'; /*podmiana jednostki urojonej na NULL aby mozna bylo zczytac wartosc im*/
			sscanf(lancuch2, "%lf", &(macierz[wiersze][kolumny].im)); /*odczyt ze stringa wartosci jako double*/
			for (i = 0; i < strlen(lancuch2); i++)
				lancuch2[i] = '\0'; /*usuniecie czesci im aby mozna bylo odczytac czesc rzeczywista*/
			if (strlen(lancuch))
				sscanf(lancuch, "%lf", &(macierz[wiersze][kolumny].re));	/*odczyt ze stringa wartosci jako double*/
			else
				macierz[wiersze][kolumny].re = 0; /*przypadek samej wartosci urojonej*/
		}
		else
		{
			sscanf(lancuch, "%lf", &(macierz[wiersze][kolumny].re)); /*przypadek samej wartosci rzeczywistej*/
			macierz[wiersze][kolumny].im = 0;
		}
		if (kolumny != kolumnymacierzy[nrpliku] - 1) /*jezeli "niedoliczylismy" do odpowiedniej wartosci*/
			kolumny++;
		else
		{
			kolumny = 0; /*warunek odp. ilosci elem w wierszu*/
			wiersze++;
		}
	}
	free(lancuch);
	return 1;
}
void mnozenie(int *wierszemacierzy, int *kolumnymacierzy, int nriteracji, liczba** macierzA, liczba** macierzB, liczba** macierzwyn)
{ /* funkcja wykonujaca mnozenie 2 macierzy i umieszczajaca wyniki w 3, otzrymuje tablice z ilosciami wierszy i kolumn w odp plikach, 2 macierze do mnozenia i macierz do umieszczenia wyniku*/
	int a;
	int b;
	int p;
	double wynikre = 0;
	double wynikim = 0;
	for (a = 0; a < wierszemacierzy[0]; a++)
		for (b = 0; b < kolumnymacierzy[nriteracji + 1]; b++)
		{
			for (p = 0; p < kolumnymacierzy[nriteracji]; p++) /*mnozenie i dodawanie w petli wynikow mnozenia 2 liczb zespolonych*/
			{
				wynikre += macierzA[a][p].re*macierzB[p][b].re - macierzA[a][p].im*macierzB[p][b].im;
				wynikim += macierzA[a][p].im*macierzB[p][b].re + macierzA[a][p].re*macierzB[p][b].im;
			}
			macierzwyn[a][b].re = wynikre;
			macierzwyn[a][b].im = wynikim;
			wynikim = 0; /*wpis do macierzy wynikowej i wyzerowanie zmiennych*/
			wynikre = 0;
		}
}
void zwolnijmiejsce(int *wierszemacierzy, int nriteracji, liczba** macierz)
{ /*funkcja zwalniajaca miejsce zajete przez macierz, otrzymuje tablice z ilosciami wierszy w plikach, nr pliku z ilocia wierszy danej macierzy i wskaznik napierwszy el macierzy, nie zwraca nic*/
	int a;
	for (a = 0; a < wierszemacierzy[nriteracji]; a++)
		free(macierz[a]); /*zwolnienie miejsca kazdego wiersza*/
	free(macierz); /*wolnienie poczatkow wszystkich wierszy*/
	macierz = NULL;
}
void wynik(int argumenty, int *wierszemacierzy, int *kolumnymacierzy, liczba** macierzwyn, FILE** tablicaplikow)
{ /*funkcja wypisujaca macierz do pliku i zwalniajaca miejsce tablic, otrzymuje tablice z ilosciami wierszy i kolumn w plikach, ilosc argumentow programu, macierz do wypisania i tablice plikow*/
	int wiersze;
	int kolumny;
	zamykanieplikow(argumenty, tablicaplikow); /*zamkniecie plikow odzytywanych*/
	FILE* wynikowy = fopen("macierzwyn.txt", "w");
	if (!wynikowy)
	{ /*w przypadku bledu otwarcia pliku wyjsciowego: komunikat zwolnienie miejsca i opuszczenie programu*/
		printf("Blad otwarcia pliku zapisu.");
		zwolnijmiejsce(wierszemacierzy, 0, macierzwyn);
		free(wierszemacierzy);
		free(kolumnymacierzy);
		exit(1);
	}
	for (wiersze = 0; wiersze < wierszemacierzy[0]; wiersze++)
	{
		for (kolumny = 0; kolumny < kolumnymacierzy[argumenty - 2]; kolumny++)
		{
			if (macierzwyn[wiersze][kolumny].re != 0 && macierzwyn[wiersze][kolumny].im != 0) /*wypis wszystkich elementow w kol i wier*/
				fprintf(wynikowy, "%lf%+lfi	", macierzwyn[wiersze][kolumny].re, macierzwyn[wiersze][kolumny].im); /*gdy jest i re i im*/
			else
			{
				if (macierzwyn[wiersze][kolumny].im == 0)
					fprintf(wynikowy, "%lf	", macierzwyn[wiersze][kolumny].re); /*przypadek zwyklej rzeczywistej*/
				else
					fprintf(wynikowy, "%lfi	", macierzwyn[wiersze][kolumny].im); /*przypadek braku realis*/
			}
		}
		fprintf(wynikowy, "%c", '\n');
	}
	zwolnijmiejsce(wierszemacierzy, 0, macierzwyn); /*zwolnienie macierzy wynikowej*/
	free(wierszemacierzy); /*zwolnienie tablic z iloscia wierszy i kolumn dla plikow*/
	free(kolumnymacierzy);
	fclose(wynikowy); /*zamkniecie pliku wynikowego*/
}
liczba** operacjamnozenia(int argumenty, int* wierszemacierzy, int* kolumnymacierzy, FILE** tablicaplikow, long roz)
{ /*funkcja zwracajaca wynikowa, wymnozona macierz, otrzymuje ilosc argumentow programu, tablice z ilosciami kolumn i macierzy w poszczeg. plikach, tablice z plikami i rozmiar bufora do wczytu, zwraca wynikowa macierz*/
	int i;
	liczba **macierz1 = alokacjamacierzy(0, 0, wierszemacierzy, kolumnymacierzy, tablicaplikow, argumenty);
	if (odczyt(kolumnymacierzy, wierszemacierzy, 0, macierz1, tablicaplikow, roz) == 0)
	{ /*w przypadku odczytu: zwolnienie pamieci i opuszczenie programu*/
		zamykanieplikow(argumenty, tablicaplikow);
		free(wierszemacierzy);
		exit(1);
	}
	liczba **macierz2 = NULL;
	liczba **macierz3 = NULL;
	for (i = 0; i < argumenty - 2; i++)
	{ /*poszczegolne mnozenia w petli dla kazdych alokacja 2 macierzy - pomocniczej i wynikowej*/
		macierz2 = alokacjamacierzy(i + 1, i + 1, wierszemacierzy, kolumnymacierzy, tablicaplikow, argumenty);
		if (!macierz2)
		{ /*w przypadku bledu alokacji: zwolnienie pamieci i opuszczenie programu*/
			zwolnijmiejsce(wierszemacierzy, 0, macierz1);
			exit(1);
		}
		macierz3 = alokacjamacierzy(0, i + 1, wierszemacierzy, kolumnymacierzy, tablicaplikow, argumenty);
		if (!macierz3)
		{ /*w przypadku bledu alokacji: zwolnienie pamieci i opuszczenie programu*/
			zwolnijmiejsce(wierszemacierzy, 0, macierz1);
			zwolnijmiejsce(wierszemacierzy, i + 1, macierz2);
			exit(1);
		}
		if (odczyt(kolumnymacierzy, wierszemacierzy, i + 1, macierz2, tablicaplikow, roz) == 0)
		{ /*w przypadku bledu odczytu: zwolnienie pamieci i opuszczenie programu*/
			zamykanieplikow(argumenty, tablicaplikow);
			free(wierszemacierzy);
			zwolnijmiejsce(wierszemacierzy, 0, macierz1);
			zwolnijmiejsce(wierszemacierzy, 0, macierz3);
			exit(1);
		}
		mnozenie(wierszemacierzy, kolumnymacierzy, i, macierz1, macierz2, macierz3); /*wykonanie mnozeni 2 macierzy i kazdorazowe zwolnienie miejsca*/
		zwolnijmiejsce(wierszemacierzy, 0, macierz1);
		zwolnijmiejsce(wierszemacierzy, i + 1, macierz2);
		macierz1 = macierz3; /*przypisanie wynikowej jako lewa przy nast mnozeniu*/
	}
	return macierz1;
}
void zamykanieplikow(int argumenty, FILE** tablicaplikow)
{ /*funkcja zamykajaca pliki odczytywane, otrzymuje ilosc argumentow programu i tablice z plikami*/
	int i;
	for (i = 0; i < argumenty - 1; i++)
		fclose(tablicaplikow[i]); /*zamkniecie wszystkich plikow*/
	free(tablicaplikow); /*zwolnienie tablicy*/
}