
#include "functions.h"

/*
*   Sifra projekta: #160/p20
*   Autor: Dzunja Dejan
*   Indeks: RA52-2014
*   Projekat: Staticka indeks-sekvencijalna
*               - propagacija najvecih vrednosti kljuca
*               - indirektno povezivanje prekoracilaca
*/

void ispisMeni();
void pozivFun(int, DATOTEKA*);

int main(){


    DATOTEKA aktivnaDat;
    aktivnaDat.f = NULL;

    int unos;

    do{

        ispisMeni();
        printf("\tUnos >> ");
        scanf("%d", &unos);
        fflush(stdin);
        pozivFun(unos, &aktivnaDat);


    }while(unos != 0);


    return 0;
}

void pozivFun(int unos, DATOTEKA* aktivnaDat){

    FILE* pom;

    switch(unos){

        case 1 : pom = napraviDat(); fclose(pom); break;
        case 2 : otvoriAktivnuDat(aktivnaDat); break;
        case 3 : trenutnoAktivna(aktivnaDat); break;
        case 4 : kreiranjeSerijskeDat(); break;
        case 5 : kreiranjeSekvencijalnteDat(); break;
        case 6 : kreiranjeIndeksSekvDat(aktivnaDat); break;
        case 7 : upisNovogSlogaIndeksSek(aktivnaDat); break;
        case 8 : unesiNadjiSlog(aktivnaDat); break;
        case 9 : reorganizacijaAktivneDat(aktivnaDat); break;
        case 10: ispisAktivneDatoteke(aktivnaDat); break;
        case 0 : fclose(aktivnaDat->f); printf("\tIzlaz iz programa! \n"); break;
        default: printf("\tNe postoji zadata komadna! \n"); break;
    }

}

void ispisMeni(){


    printf("\n\n\n\t ******************** MENI ****************** \n");
    printf("\t *  1.  Formiranje prazne datoteke \n");
    printf("\t *  2.  Otvori datoteku \n");
    printf("\t *  3.  Prikaz naziva aktivne datoteke \n");
    printf("\t *  4.  Formiranje serijske datoteke, unosom slogova iz tastature \n");
    printf("\t *  5.  Formiranje sekvencijalne (rastuce) datoteke, ucitavanjem iz serijske \n");
    printf("\t *  6.  Formiranje aktivne datoteke tako sto ce se popunjavati primarna zona datoteke\n");
    printf("\t *  7.  Upis novog sloga u aktivnu datoteku \n");
    printf("\t *  8.  Trazenje proizvoljnog sloga u aktivnoj datoteci \n");
    printf("\t *  9.  Reorganizacija aktivne datoteke \n");
    printf("\t *  0.  Izlaz \n");
    printf("\t ********************************************\n");
    printf("\t * 10. Ispis aktivne datoteke \n");
    printf("\t ******************************************** \n\n\n\n");

}


