#include "functions.h"

void unosSloga(SLOG* s){

    s->indikatorAktiv = 1;

    int numDigits;

    do{
        printf("\tUnesite evidencioni broj (8 cifara) >> ");
        scanf("%d", &s->evidBroj);
        fflush(stdin);

        numDigits = (int)floor(log10((double)s->evidBroj)) + 1;

        if(numDigits != 8)
            printf("\tEvidencioni broj mora imati 8 cifara! \n");


    }while(numDigits != 8);

    do{
        printf("\tUnesite registarsku oznaku (10 karaktera) >> ");

        gets(s->regOzn);

        if(strlen(s->regOzn) != regMAX)
            printf("\tRegistarska oznaka mora imati 10 karakatera! \n");

    }while(strlen(s->regOzn) != regMAX);

    do{

         printf("\tUnesite oznaku rampe (4 karaktera) >> ");

        gets(s->oznRampa);

        if(strlen(s->oznRampa) != oznMAX)
            printf("\tOznaka rampe mora imati 4 karakatera! \n");

    }while(strlen(s->oznRampa) != oznMAX);

    do{

        printf("\tUnesite racun (max 100000) >> ");
        scanf("%d", &s->racun);

        if(s->racun > 100000 || s->racun < 0)
            printf("\tRacun moze iznositi maksimalno 100000 din \n");

    }while(s->racun > 100000 || s->racun < 0);

    createTime(&s->vreme);
    createDate(&s->datum);

}



void ispisSloga(SLOG s){

    printf("\n\t**********SLOG***********");
    printf("\n\t*Status sloga: ");
    if(s.indikatorAktiv == 1)
        printf("Aktivan");
    else if(s.indikatorAktiv == 0)
        printf("Neaktivan");
    else if(s.indikatorAktiv == -1)
        printf("Prazan");
    else printf("ERROR");
    printf("\n\t*Kljuc: %d", s.evidBroj);
    printf("\n\t*Registarska oznaka: %s", s.regOzn);
    printf("\n\t*Oznaka rampe: %s", s.oznRampa);
    printf("\n\t*Racun: %d RSD", s.racun);
    printf("\n\t*Vreme unosa: ");
    printDate(s.datum);
    printf(" u ");
    printTime(s.vreme);
    printf(" casova");
    printf("\n\t*************************\n\n");


}

void ispisIzFajl(FILE* f){

        BLOK* tempPom = (BLOK*)malloc(sizeof(BLOK));
        int brBlok = 0;
        int brAktivnih = 0;
        int brNeaktivnih = 0;
        int brPraznih = 0;
        int brGreska = 0;

        while(fread(tempPom, sizeof(BLOK), 1, f) == 1){

            brBlok++;
            int i;

            printf("\n\t********* BLOK %d ********* \n", brBlok);
            for(i = 0; i < faktBlok; i++){
               ispisSloga(tempPom->slog[i]);
               if(tempPom->slog[i].indikatorAktiv == 1)
                    brAktivnih++;
               else if(tempPom->slog[i].indikatorAktiv == 0)
                    brNeaktivnih++;
              else if(tempPom->slog[i].indikatorAktiv == -1)
                    brPraznih++;
              else brGreska++;
              }
            printf("\t***************************\n");

            free(tempPom);
            tempPom = (BLOK*)malloc(sizeof(BLOK));


        }

        free(tempPom);

        printf("\n\t UKUPAN BROJ BLOKOVA: %d", brBlok);
        printf("\n\t UKUPAN BROJ AKTIVNIH SLOGOVA: %d", brAktivnih);
        printf("\n\t UKUPAN BROJ NEAKTIVNIH SLOGOVA: %d", brNeaktivnih);
        printf("\n\t UKUPAN BROJ PRAZNIH SLOGOVA: %d\n\n", brPraznih);
        if(brGreska > 0)
            printf("\n\t ERROR U KODU \n\t UKUPAN BROJ LOSE UCITANIH SLOGOVA: %d", brGreska);


}


void dodajElemSortListu(LIST** head, SLOG val){

    if(*head == NULL){

        *head = (LIST*)malloc(sizeof(LIST));
        (*head)->next = NULL;
        (*head)->val = val;

    }else if(val.evidBroj < ((*head)->val).evidBroj){

        LIST* node = (LIST*)malloc(sizeof(LIST));
        node->val = val;
        node->next = *head;
        *head = node;

    }else if((*head)->next == NULL){

        dodajElemSortListu(&(*head)->next ,val);
    }else if(((*head)->val).evidBroj < val.evidBroj && (((*head)->next)->val).evidBroj > val.evidBroj){

        LIST* node = (LIST*)malloc(sizeof(LIST));
        node->val = val;
        LIST* temp =(*head)->next;
        (*head)->next = node;
        node->next = temp;
    }else dodajElemSortListu(&(*head)->next ,val);
}

void ispisListe(LIST* head){


    if(head != NULL){

        ispisSloga(head->val);
        ispisListe(head->next);

    }

}

/// ime fajla sa ekstenzijom
FILE* otvoriPostojecuDat(char* imeFajla){

    FILE* retVal = NULL;

     if(access(imeFajla, F_OK) != -1){

        retVal = fopen(imeFajla, "rb+"); /// otvaramo fajl za citanje i pisanje binarno

    }else{
        printf("\tFajl %s nepostoji! \n", imeFajla);
        imeFajla = NULL;
    }

    if(imeFajla != NULL)
        printf("\tFajl '%s' je uspesno otvoren! \n", imeFajla);
    else printf("\tERROR: Fajl '%s' nije otvoren! \n", imeFajla);

    return retVal;


}


void obrisiListu(LIST* head){

    if(head != NULL){
        obrisiListu(head->next);
        free(head);
    }

}


SLOG formiranjePraznogSloga(){

    SLOG* retVal = (SLOG*)malloc(sizeof(SLOG));
    retVal->evidBroj = 0;
    retVal->indikatorAktiv = -1;
    strcpy(retVal->oznRampa, "-");
    retVal->racun = 0;
    strcpy(retVal->regOzn, "-");
    retVal->datum.dd = 0;
    retVal->datum.mm = 0;
    retVal->datum.yy = 0;
    retVal->vreme.hh = 0;
    retVal->vreme.mm = 0;
    retVal->vreme.ss = 0;

    return *retVal;

}

B_PRIMZONA formiranjePraznogBlokaPrimarneZone(){

    B_PRIMZONA retVal;


    int i;

    for(i = 0; i < faktBlok; i++){

        retVal.slog[i] = formiranjePraznogSloga();
        retVal.adresaPrek = -1;


    }

    return retVal;


}

int brojAktivnihSlogova(FILE* f){


    BLOK* blok = (BLOK*)malloc(sizeof(BLOK));
    int aktivSlog = 0;

    fseek(f, 0, 0);
    int k;

    while( (k = fread(blok, sizeof(BLOK), 1, f)) == 1){
        int i;
        for(i = 0; i < faktBlok; i++){
            if(blok->slog[i].indikatorAktiv != -1){

                aktivSlog++;
            }
            else break;

        }

    }
    free(blok);
    fseek(f, 0, 0);
    return aktivSlog;



}


B_PRIMZONA* formiranjePrimarneZone(int size, FILE* f){

    fseek(f, 0, 0);

    B_PRIMZONA* primarnaZona = (B_PRIMZONA*)malloc(size*sizeof(B_PRIMZONA));
    BLOK* blok = (BLOK*)malloc(sizeof(BLOK));

    int i, j;
    for(i = 0; i < size; i++)
        primarnaZona[i] = formiranjePraznogBlokaPrimarneZone();


    LIST* head = NULL;

    while(fread(blok, sizeof(BLOK), 1, f) == 1){

        int i;

        for(i = 0; i < faktBlok; i++)
            if(blok->slog[i].indikatorAktiv == 1)
                dodajElemSortListu(&head, blok->slog[i]);

    }

    LIST* tempHead = head;
    int kraj = 0; /// 1 - kraj liste 0 - nije kraj liste


    for(i = 0; i < size; i++){
        for(j = 0; j < faktBlok*ibf; j++){
           if(tempHead != NULL){
                primarnaZona[i].slog[j] = tempHead->val;
                tempHead = tempHead->next;
           }else{

                kraj = 1;
                break;

           }
        }

        if(kraj == 1)
            break;
    }

    obrisiListu(head);

    head = NULL;
    tempHead = NULL;

    return primarnaZona;
}

int visinaStFormula(int brojPrimBlokova){

    return (int)ceil((log(brojPrimBlokova)/log(2)));

}

int brojCvorovaStFormula(int brojPrimBlokova, int visinaStab){

    int i;
    int brojCvorova = 0;
    for(i = 1; i <= visinaStab; i++){
        brojCvorova += (int)ceil(brojPrimBlokova/(double)pow(2, visinaStab - i + 1));
    }

    return brojCvorova;
}

int brojCvorovaNaVisini(int brojPrimBlokova, int trenutnaVisinaStab){

    int visina = visinaStFormula(brojPrimBlokova);

    return (int)ceil(brojPrimBlokova/(double)pow(2, visina - trenutnaVisinaStab + 1));
}

int maxBloka(B_PRIMZONA primBlok){

    int retVal = -1;
    int i;
    for(i = 0; i < faktBlok; i++)
        if(primBlok.slog[i].indikatorAktiv == 1)
            retVal = primBlok.slog[i].evidBroj;

    if(retVal == -1)
        printf("ERROR \n\n");

    return retVal;
}

int najveciKljuc(INDEKS i){

    if(i.desniKljuc > i.leviKljuc)
        return i.desniKljuc;
    else return i.leviKljuc;

}

void ispisIndeks(INDEKS i, long p){

    printf("\n\t****INDEKS****");
    if(p != -1)
        printf("\n\t Pozicija %ld \n", p);
    printf("\n\t Levi kljuc: %d ", i.leviKljuc);
    printf("\n\t Desni kljuc: %d \n\n" ,i.desniKljuc);
    printf("\n\t Leva relativna: %d ", i.relativnaLevo);
    printf("\n\t Desna relativna: %d \n", i.relativnaDesno);
    printf("\t**************\n");


}

void writeIndeksSekv(DATOTEKA* dat, B_PRIMZONA* prim, int sizePrim){


    int root = -1;
    int zonaPrimarna = -1;
    int zonaPrekoracioc = -1;
    int stvoreniIndexi = 0;

    int visinaStabla = visinaStFormula(sizePrim);
    int indexNum = brojCvorovaStFormula(sizePrim, visinaStabla);


    printf("\n\t VISINA STABLA: %d \n\t BROJ CVOROVA I LISTOVA STABLA: %d\n",visinaStabla,indexNum);

    fclose(dat->f);

    dat->f = fopen(dat->name, "wb+");

    fseek(dat->f, 0, SEEK_SET);

    fwrite(&root, sizeof(int), 1, dat->f);
    fwrite(&zonaPrimarna, sizeof(int), 1, dat->f);
    fwrite(&zonaPrekoracioc, sizeof(int), 1, dat->f);

    int i = 0;
    int trenutnaVisina = visinaStabla;

    printf("\n\t*****STVARANJE LISTOVA*****");

    for(i = 0; i < sizePrim; i += 2){

        INDEKS input;

        /// specijalni slucaj poslednji list, ima jednu primarnu zonu. leviKljuc lista uzima maksimalnu mogucu vrednost kljuca iz domena, desni uzima -1, jer nema vise primarnih blokova
        if(i == sizePrim - 1){

            input.leviKljuc = maxKljuc;
            input.desniKljuc = -1;
            input.relativnaLevo = 3*sizeof(int) + indexNum*sizeof(INDEKS) + i*sizeof(B_PRIMZONA);
            input.relativnaDesno = -1;


        /// specijalni slucaj za poslednji list, koji ima dva primarne zone. leviKljuc uzima max vrednost iz primanog bloka, a desniKljuc uzima max mogucu vrenost kljuca iz domena
        }else if(i == sizePrim - 2){
            input.leviKljuc = maxBloka(prim[i]);
            input.desniKljuc = maxKljuc;
            input.relativnaLevo = 3*sizeof(int) + indexNum*sizeof(INDEKS) + i*sizeof(B_PRIMZONA);
            input.relativnaDesno = 3*sizeof(int) + indexNum*sizeof(INDEKS) + (i+1)*sizeof(B_PRIMZONA);
        }else{

            input.leviKljuc = maxBloka(prim[i]);
            input.desniKljuc = maxBloka(prim[i+1]);
            input.relativnaLevo = 3*sizeof(int) + indexNum*sizeof(INDEKS) + i*sizeof(B_PRIMZONA);
            input.relativnaDesno = 3*sizeof(int) + indexNum*sizeof(INDEKS) + (i+1)*sizeof(B_PRIMZONA);

        }

        printf("\n\tStvoren je list\n");
        ispisIndeks(input, ftell(dat->f));

        fwrite(&input, sizeof(INDEKS), 1, dat->f);
        stvoreniIndexi++;
    }

    printf("\n\tBroj stvorenih listova %d \n\t potrebno je stvoriti %d \n ", stvoreniIndexi, brojCvorovaNaVisini(sizePrim, visinaStabla));
    printf("\tKRAJ STVARANJU LISTOVA");

    int brojacRelativne = 3*sizeof(int);
    trenutnaVisina--;

    while(stvoreniIndexi != indexNum && trenutnaVisina != 0){

        int brCvorovaNaProslojVisini = brojCvorovaNaVisini(sizePrim, trenutnaVisina + 1);
        int brStvorenihCvorovaNaVisini = 0;
        int brPotrebnihCvorovaNaVisini = brojCvorovaNaVisini(sizePrim, trenutnaVisina);
        INDEKS upis;
        int citacRead;

        printf("\n\tFORMIRANJE NIVOA %d \n", trenutnaVisina);
        for(i = 0; i < brCvorovaNaProslojVisini; i += 2){

             if(i == brCvorovaNaProslojVisini - 1){ /// ukoliko cvor koji treba da se stvori ima samo jednog naslednika

                fseek(dat->f, brojacRelativne, SEEK_SET);
                INDEKS temp;

                if( ( citacRead = fread(&temp, sizeof(INDEKS), 1, dat->f)) != 1){
                    printf("\n\t fread vraca %d \n", citacRead);
                    printf("\n\tERROR na RELATIVNOJ %d", brojacRelativne);
                    printf("\n\tStvarna pozicija fajla %ld \n", ftell(dat->f));
                }

                upis.leviKljuc = najveciKljuc(temp);
                upis.desniKljuc = -1;

                upis.relativnaLevo = brojacRelativne;
                upis.relativnaDesno = -1;

                brojacRelativne +=  sizeof(INDEKS);

                fseek(dat->f, 0, SEEK_END);


            }else{

                INDEKS temp;

                ///
                fseek(dat->f, brojacRelativne, SEEK_SET);

                if( ( citacRead = fread(&temp, sizeof(INDEKS), 1, dat->f)) != 1){
                    printf("\n\t fread vraca %d \n", citacRead);
                    printf("\n\tERROR na RELATIVNOJ %d", brojacRelativne);
                    printf("\n\tStvarna pozicija fajla %ld \n", ftell(dat->f));
                }

                upis.leviKljuc = najveciKljuc(temp);
                upis.relativnaLevo = brojacRelativne;
                brojacRelativne +=  sizeof(INDEKS);
                ///


                fseek(dat->f, brojacRelativne, SEEK_SET);
                if((citacRead = fread(&temp, sizeof(INDEKS), 1, dat->f)) != 1){
                    printf("\n\t fread vraca %d \n", citacRead);
                    printf("\n\tERROR na RELATIVNOJ %d", brojacRelativne);
                    printf("\n\tStvarna pozicija fajla %ld \n", ftell(dat->f));
                }


                upis.relativnaDesno = brojacRelativne;
                upis.desniKljuc = najveciKljuc(temp);
                brojacRelativne +=  sizeof(INDEKS);



                fseek(dat->f, 0, SEEK_END);

            }

            printf("\n\t********************************\n");
            brStvorenihCvorovaNaVisini++;
            ispisIndeks(upis, ftell(dat->f));
            fwrite(&upis, sizeof(INDEKS), 1, dat->f);
            stvoreniIndexi++;

            }

        printf("\n\tBROJ POTREBNIH CVOROVA NA VISINI %d JE %d",trenutnaVisina, brPotrebnihCvorovaNaVisini);
        printf("\n\t BROJ STVORENIH CVOROVA NA VISINI %d JE %d\n", trenutnaVisina, brStvorenihCvorovaNaVisini);
        printf("\n\t*****FORMIRAN JE NIVO %d*****", trenutnaVisina);

        trenutnaVisina--;



    }


        /// formiranje primarne zone!

    printf("\n\n\n\t*** FORMIRANJE PRIMARNE ZONE *** \n");

    fseek(dat->f, 0, SEEK_END);

    root = (int)ftell(dat->f) - sizeof(INDEKS);
    zonaPrimarna = (int)ftell(dat->f);



    for(i = 0; i < sizePrim; i++){
        printf("\n\tPisanje primarnog bloka na %ld", ftell(dat->f));
        int j;

        printf("\n\tUPIS BLOKA %d SA SLEDECIM BROJEVIMA", i+1);
        for(j = 0; j < faktBlok; j++)
            printf("\n\t%d", prim[i].slog[j].evidBroj);

        fwrite(prim + i, sizeof(B_PRIMZONA), 1, dat->f);
    }

    zonaPrekoracioc = (int)ftell(dat->f);

    printf("\n\n\tKOREN: %d ", root);
    printf("\n\tPRIMRANA: %d ", zonaPrimarna);
    printf("\n\tPREKORACIOCI: %d \n", zonaPrekoracioc);

    fseek(dat->f, 0, SEEK_SET);

    fwrite(&root, sizeof(int), 1, dat->f);
    fwrite(&zonaPrimarna, sizeof(int), 1, dat->f);
    fwrite(&zonaPrekoracioc, sizeof(int), 1, dat->f);

    fseek(dat->f, 0, SEEK_END);


    printf("\n\n\tAKTIVNA DATOTEKA JE FORMIRANA \n\n\n\n");


}

NADJEN_SLOG traziSlog(FILE* f, unsigned evidBroj){


    NADJEN_SLOG retVal;

    retVal.adresa = -1;
    retVal.redniBrojSloga = -1;


    if(f == NULL){
        printf("\n\t ERROR \n");
        return retVal;
    }


    fseek(f, 0, SEEK_SET);

    int root = -1, primarZone = -1, prekZone = -1;

    fread(&root, sizeof(int), 1, f);
    fread(&primarZone, sizeof(int), 1, f);
    fread(&prekZone, sizeof(int), 1, f);

    int pointer = root;

    INDEKS temp;

    while(pointer < primarZone){


        fseek(f, pointer, SEEK_SET);

        if(fread(&temp, sizeof(INDEKS), 1, f) != 1){
            printf("\n\tERROR KOD INDEKSA! \n");
            return retVal;
        }

        if(temp.desniKljuc == -1){
            pointer = temp.relativnaLevo;

        }
        else if(temp.leviKljuc < evidBroj && evidBroj <= temp.desniKljuc){
            pointer = temp.relativnaDesno;

        }else if(evidBroj <= temp.leviKljuc){
            pointer = temp.relativnaLevo;

        }


    }




    B_PRIMZONA blok;
    fseek(f, pointer, SEEK_SET);

    ispisBloka(pointer, f);

    fseek(f, pointer, SEEK_SET);

    if(fread(&blok, sizeof(B_PRIMZONA), 1, f) != 1){
        printf("\n\tERROR KOD BLOKA! \n");
        return retVal;
    }

    int i;


    for(i = 0; i < faktBlok; i++){

        retVal.adresa = pointer; /// ukoliko nije nadjen, u kom bloku nije nadjen. Vazno je zbog unosa u slog! Kako bi se odmah pozicionirali

        if(blok.slog[i].evidBroj != 0){
            if(blok.slog[i].evidBroj == evidBroj){


                retVal.redniBrojSloga = i;
                return retVal;

            }else if(evidBroj < blok.slog[i].evidBroj)
                return retVal;

        }else return retVal;

    }

    pointer = blok.adresaPrek;

    PREKORACIOC prek;

    while(pointer != -1){


        fseek(f, pointer, SEEK_SET);

        if(fread(&prek, sizeof(PREKORACIOC), 1, f) != 1){
            printf("\n\tERROR KOD BLOKA! \n");
            return retVal;
        }


        if(prek.slog.evidBroj == evidBroj){
            retVal.redniBrojSloga = -1;
            retVal.adresa = pointer;
            return retVal;

        }else if(prek.slog.evidBroj > evidBroj)
            return retVal;

        pointer = prek.adresaSled;

    }


    return retVal;


}

PREKORACIOC nadjiPrekoracioca(B_PRIMZONA* blok, SLOG s){

    PREKORACIOC retVal;
    retVal.adresaSled = -1;
    retVal.slog = formiranjePraznogSloga();

    int i;
    int brojAktiv = 0;

    for(i = 0; i < faktBlok; i++)
        if(blok->slog[i].indikatorAktiv == 1)
            brojAktiv++;
        else break;

    printf("\n\tPostoje %d aktivnih slogova \n", brojAktiv);

    if(brojAktiv < faktBlok){
        /// Ukoliko ima slobodnih mesta u bloku ubaci
        shiftUbaci(blok, brojAktiv, s);

    }else if(brojAktiv == faktBlok){

        /// Ukoliko je slog veci od poslednjeg u bloku, onda je on prekoracioc
        if(s.evidBroj > blok->slog[faktBlok - 1].evidBroj){

                retVal.adresaSled = -1;
                retVal.slog = s;

        }else{
        /// Ukoliko slog nije veci od poslednje u bloku, onda je poslednji u bloku prekoracioc
            retVal.adresaSled = -1;
            retVal.slog = blok->slog[faktBlok - 1];
            blok->slog[faktBlok - 1] = formiranjePraznogSloga();
            shiftUbaci(blok, brojAktiv - 1, s);

        }

    }

    return retVal;

}


void shiftUbaci(B_PRIMZONA* blok, int brojAktiv, SLOG unos){

    int i, save = -1;
    for(i = 0; i < faktBlok; i++)
        if (unos.evidBroj < blok->slog[i].evidBroj){
            save = i;
            break;
        }else if(blok->slog[i].evidBroj == 0){

            save = i;
            break;
        }

    for(i = faktBlok - 2; i >= save; i--){
        blok->slog[i + 1] = blok->slog[i];
    }

    if(save == -1)
        printf("\n\tERROR U SHITOVANJU \n");

    blok->slog[i + 1] = unos;

}

void ispisBloka(int adresa, FILE* f){

    if(f == NULL){
        printf("ERROR U ispisBloka \n");
        return;
    }

    B_PRIMZONA B;
    int staraAdresa = (int)ftell(f);
    fseek(f, adresa, SEEK_SET);
    fread(&B, sizeof(B_PRIMZONA), 1, f);

    int i;
    printf("\n\tIspis bloka na adresi %d", adresa);
    for(i = 0; i < faktBlok; i++)
        printf("\n\t %d", B.slog[i].evidBroj);


    printf("\n\tPREKORACIOCI \n");
    int sledecaAdresa = B.adresaPrek;
    while(sledecaAdresa != -1){

        fseek(f, sledecaAdresa, SEEK_SET);
        PREKORACIOC temp;

        fread(&temp, sizeof(PREKORACIOC), 1, f);
        printf("\n\t %d",temp.slog.evidBroj);
        sledecaAdresa = temp.adresaSled;
    }


    fseek(f, staraAdresa, SEEK_SET); //// vracamo pokazivac tam gde je bio pre ispisa

}






