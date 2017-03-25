#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
*   Sifra projekta: #160/p20
*   Autor: Dzunja Dejan
*   Indeks: RA52-2014
*   Projekat: Staticka indeks-sekvencijalna
*               - propagacija najvecih vrednosti kljuca
*               - indirektno povezivanje prekoracilaca
*/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "datetime.h"
#include "unistd.h"
#include "math.h"

#define regMAX 10
#define oznMAX 4
#define fNameMAX 15
#define faktBlok 5
#define maxDin 100000
#define ibf 0.6
#define maxKljuc 99999999

typedef struct {
   FILE *f;
   char name[fNameMAX + 4 + 1];

}DATOTEKA;

typedef struct{

    int indikatorAktiv;
    unsigned int evidBroj;
    char regOzn[regMAX + 1];
    char oznRampa[oznMAX + 1];
    unsigned int racun;
    DATE datum;
    TIME vreme;

}SLOG;



/// jedan blok primarne zone, sa pokazivacem na prekoracioc
typedef struct{

    SLOG slog[faktBlok];
    int adresaPrek;

}B_PRIMZONA;

typedef struct{

    SLOG slog;
    int adresaSled;

}PREKORACIOC;

typedef struct stablo{

    int leviKljuc;
    int relativnaLevo;
    int desniKljuc;
    int relativnaDesno;


}INDEKS;

typedef struct{

    SLOG slog[faktBlok];

}BLOK;

typedef struct lista{

    SLOG val;
    struct lista* next;

}LIST;

typedef struct{

    int adresa;
    int redniBrojSloga; /// ukoliko se slog nalazi u primarnoj zoni, inace -1

}NADJEN_SLOG;

/// Definicija u functions.c ///
FILE* napraviDat(); /// Zadatak 1
void otvoriAktivnuDat(DATOTEKA*); /// Zadatak 2
void trenutnoAktivna(DATOTEKA*); /// Zadatak 3
void kreiranjeSerijskeDat();  /// Zadatak 4
void kreiranjeSekvencijalnteDat(); /// Zadatak 5
void kreiranjeIndeksSekvDat(DATOTEKA*); /// Zadatak 6
void upisNovogSlogaIndeksSek(DATOTEKA*); /// Zadatak 7
void unesiNadjiSlog(DATOTEKA*); /// Zadatak 8
void reorganizacijaAktivneDat(DATOTEKA*); /// Zadatak 9
void ispisAktivneDatoteke(DATOTEKA*); /// dodatak

/// Pomocne funkcije ///
/// Definicija u pomocneFun.c ///
void unosSloga(SLOG*);
void ispisSloga(SLOG);
void ispisIzFajl(FILE*);
FILE* otvoriPostojecuDat(char*);
void dodajElemSortListu(LIST**, SLOG);
void ispisListe(LIST*);
void obrisiListu(LIST*);
SLOG formiranjePraznogSloga();
B_PRIMZONA formiranjePraznogBlokaPrimarneZone();
int brojAktivnihSlogova(FILE*);
B_PRIMZONA* formiranjePrimarneZone(int, FILE*);
int visinaStFormula(int);
int brojCvorovaStFormula(int, int);
int maxBloka(B_PRIMZONA);
int najveciKljuc(INDEKS);
int brojCvorovaNaVisini(int, int);
void writeIndeksSekv(DATOTEKA*, B_PRIMZONA*, int);
void ispisIndeks(INDEKS, long);
NADJEN_SLOG traziSlog(FILE*, unsigned);
PREKORACIOC nadjiPrekoracioca(B_PRIMZONA*, SLOG);
void shiftUbaci(B_PRIMZONA*, int, SLOG);
void ispisBloka(int, FILE*);


#endif // FUNCTIONS_H_INCLUDED
