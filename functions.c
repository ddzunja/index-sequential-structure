#include "functions.h"



FILE* napraviDat(){

    char fileName[fNameMAX + 4 + 1];
    int sizeN;
    FILE* f = NULL;



    printf("\n\n\t Unesite ime fajla (15 karaktera) >> ");


    do{
        scanf("%s", fileName);
        sizeN = strlen(fileName);

        if(sizeN > fNameMAX)
            printf("\t Ime fajla ne sme biti duze od 15 karaktera \n");

    }while(sizeN > fNameMAX);

    strcat(fileName, ".bin");

    f = fopen(fileName, "wb");

    if(f != NULL)
        printf("\t File %s je uspesno napravljena! \n", fileName);
    else printf("\t Doslo je do greske prilikom otvaranja fajla \n");

    return f;


}


void otvoriAktivnuDat(DATOTEKA* file){

    file->f = NULL;


    printf("\n\t Unesite puno ime datoteke bez ekstenzije >> ");
    scanf("%s", file->name);

    strcat(file->name, ".bin");

    file->f = otvoriPostojecuDat(file->name);

    if(file->f == NULL)
        strcpy(file->name, "");


}


void trenutnoAktivna(DATOTEKA* d){

    if(d->f == NULL)
        printf("\n\n\n\tNijedna datoteka nije trenutno aktivna \n");
    else printf("\tTrenutno aktivna datoteka: %s \n", d->name);

}

void kreiranjeSerijskeDat(){


    unsigned nizKljuceva[1000];
    int brojacKlj = 0;
    int postojiKlj = 0; /// 0 - Ne postoji isti kljuc // 1 - postoji duplikat


    char imeDat[fNameMAX + 4 + 4 + 1];
    strcpy(imeDat, "ser_");
    char ime[fNameMAX];

    printf("\tUnesite ime serijske >> ");
    gets(ime);
    strcat(imeDat, ime);
    strcat(imeDat, ".bin");

    FILE* f = NULL;
    f = fopen(imeDat, "wb");

    if(f == NULL){
        printf("\tERROR: Datoteka nije uspesno kreirana \n");
        return;
    }
    else printf("\tDatoteka '%s' je uspesno kreirana \n", imeDat);

    BLOK* blokUpisa = NULL;
    int brSlog = 0;
    int brBlok = 0;
    int brPopunjenih = 0;

    SLOG temp;
    char odg;


    do{

        if(blokUpisa == NULL){
            brPopunjenih = 0;
            blokUpisa = (BLOK*)malloc(sizeof(BLOK));
            int i;

            for(i = 0; i < faktBlok; i++)
                blokUpisa->slog[i] = formiranjePraznogSloga();



        }

        unosSloga(&temp);

        int i;
        for(i = 0; i < brojacKlj; i++){

            if(nizKljuceva[i] == temp.evidBroj){
                printf("\n\tPostoji vec isti kljuc! Slog nece biti unet! \n");
                postojiKlj = 1;
                break;
            }


        }


        if(postojiKlj != 1){
            blokUpisa->slog[brPopunjenih++] = temp;
            nizKljuceva[brojacKlj++] = temp.evidBroj;
            brSlog++;

        }

        postojiKlj = 0;



        if(brPopunjenih == faktBlok){
            fwrite(blokUpisa, sizeof(BLOK), 1, f);
            free(blokUpisa);
            blokUpisa = NULL;
            brBlok++;


        }

        printf("\n\tDa li zelite da upisete novi slog y/n >> ");
        odg = getchar();

    }while(odg == 'y');





    if(brPopunjenih == faktBlok){
        free(blokUpisa);
        brBlok++;
        blokUpisa = (BLOK*)malloc(sizeof(BLOK));
        int i;

        for(i = 0; i < faktBlok; i++){
                blokUpisa->slog[i] = formiranjePraznogSloga();
        }
        fwrite(blokUpisa, sizeof(BLOK), 1, f);
    }else if (brPopunjenih < faktBlok){

        fwrite(blokUpisa, sizeof(BLOK), 1, f);
        brBlok++;
        free(blokUpisa);
    }

    fclose(f);

    fflush(stdin);
    printf("\n\tUPISANO JE %d BLOKOVA I AKTIVNIH %d SLOGOVA \n\t", brBlok, brSlog);
    printf("\n\tDa li zelite da ispisemo zapisane podatke u datoteku y/n >> ");
    odg = getchar();

    if(odg == 'y'){

        FILE* fIn = NULL;
        fIn = fopen(imeDat, "rb");

        ispisIzFajl(fIn);

        fclose(fIn);

    }

}

void kreiranjeSekvencijalnteDat(){

    char imeDatSer[fNameMAX + 4 + 4 + 1];
    char imeDatSekv[fNameMAX + 4 + 4 + 1];
    strcpy(imeDatSer, "ser_");
    char ime[fNameMAX];

    printf("\tUnesite ime serijske na osnovu koje zelite da napravite sekvencijalnu >> ");
    gets(ime);
    strcat(imeDatSer, ime);
    strcat(imeDatSer, ".bin");

    FILE* f = otvoriPostojecuDat(imeDatSer);

    if(f == NULL)
        return;


    BLOK* blok = (BLOK*)malloc(sizeof(BLOK));
    LIST* head = NULL;


    while(fread(blok, sizeof(BLOK), 1, f) == 1){


        int i;
        for(i = 0; i < faktBlok; i++){
             if(blok->slog[i].indikatorAktiv != -1){
                dodajElemSortListu(&head, blok->slog[i]);

             }

        }
    }

    fclose(f);




    /// Stvaranje blokova

    LIST* headTemp = head;

    free(blok);
    blok = NULL;

    strcpy(imeDatSekv, "sekv_");
    strcat(imeDatSekv, ime);
    strcat(imeDatSekv, ".bin");

    f = fopen(imeDatSekv, "wb");
    int brPopunjenih = 0;


    while(headTemp != NULL){

        if(blok == NULL){

            blok = (BLOK*)malloc(sizeof(BLOK));
            int i;

            for(i = 0; i < faktBlok; i++)
                blok->slog[i] = formiranjePraznogSloga();

            brPopunjenih = 0;


        }

        blok->slog[brPopunjenih++] = headTemp->val;

        if(brPopunjenih == faktBlok){

            fwrite(blok, sizeof(BLOK), 1, f);
            free(blok);
            blok = NULL;
        }

        headTemp = headTemp->next;


    }

    if(blok != NULL){
        fwrite(blok, sizeof(BLOK), 1, f);
    }else if(blok == NULL){
        blok = (BLOK*)malloc(sizeof(BLOK));
        int i;

        for(i = 0; i < faktBlok; i++)
            blok->slog[i] = formiranjePraznogSloga();

        fwrite(blok, sizeof(BLOK), 1, f);
    }

    free(blok);
    fclose(f);

    printf("\tPodaci upisani u datoteku '%s' \n\n", imeDatSekv);
    printf("\tDa li zelite da ispisemo podatke iz datoteke y/n ? >> ");
    char odg;

    odg = getchar();

    if(odg == 'y'){

        f = fopen(imeDatSekv, "rb");
        ispisIzFajl(f);
        fclose(f);

    }

    obrisiListu(head);
    head = NULL;
    headTemp = NULL;


}

void kreiranjeIndeksSekvDat(DATOTEKA* aktivnaDat){

    if(aktivnaDat->f == NULL){
        printf("\n\tNijedna aktivna datoteka nije otvorena! \n");
        return;
    }



    char imeDatSekv[fNameMAX + 4 + 4 + 1];
    char ime[fNameMAX];

    printf("\n\tUnesite ime sekvencijalne za otvaranje >> ");
    gets(ime);

    strcpy(imeDatSekv, "sekv_");
    strcat(imeDatSekv, ime);
    strcat(imeDatSekv, ".bin");

    FILE* f = otvoriPostojecuDat(imeDatSekv);

    if(f == NULL){

        printf("\n\tDatoteka sa imenom '%s' ne postoji", imeDatSekv);
        return;

    }


    int brojSlogova = brojAktivnihSlogova(f);
    int brojPrimBlokova;

    if(brojSlogova % (int)(faktBlok*ibf) != 0)
        brojPrimBlokova = (int)ceil(brojSlogova / (double)(ibf*faktBlok));
    else brojPrimBlokova = brojSlogova / (ibf*faktBlok) + 1;

    printf("\tNa osnovu %d aktivnih slogova, stvaramo %d blokova primarne zone \n\n", brojSlogova, brojPrimBlokova);

    B_PRIMZONA* primarnaZona = formiranjePrimarneZone(brojPrimBlokova, f);

    writeIndeksSekv(aktivnaDat, primarnaZona, brojPrimBlokova);

    free(primarnaZona);


}

void upisNovogSlogaIndeksSek(DATOTEKA* aktivnaDat){

    if(aktivnaDat->f == NULL){
        printf("\n\tNijedna aktivna datoteka nije trenutno otvorena! \n");
        return;
    }

    fseek(aktivnaDat->f, 0, SEEK_SET);

    int root = -1;
    int prim = -1;
    int prekor = -1;

    if(fread(&root, sizeof(int), 1, aktivnaDat->f) != 1 && root <= 0){

        printf("\n\tU '%s' nije nadjeno stablo \n ", aktivnaDat->name);
        return;

    }

    fread(&prim, sizeof(int), 1, aktivnaDat->f);
    fread(&prekor, sizeof(int), 1, aktivnaDat->f);

    SLOG in;

    printf("\n\t***Unesite novi slog***\n");
    NADJEN_SLOG temp;

    do{
        unosSloga(&in);
        temp = traziSlog(aktivnaDat->f, in.evidBroj);

       if((temp.adresa < prekor && temp.redniBrojSloga != -1) || temp.adresa >= prekor) /// ukoliko je u bloku ili je prekoracioc, znaci da postoji i ne sme se uneti!
            printf("\n\tSLOG SA TIM KLJUCEM VEC POSTOJI! \n\tUNESITE PONOVO! %d \n", temp.adresa );
    /// funkcija "trazi" vraca adresu primarnog bloka bez obzira on nasao nju ili ne, jer ce nam trebati za unos, kako bi znali u kom bloku primarne zone taj slog pripada

    }while((temp.adresa < prekor && temp.redniBrojSloga != -1) || temp.adresa >= prekor);



    printf("\n\tSLOG IDE U BLOK %d", temp.adresa);

    fseek(aktivnaDat->f, temp.adresa, SEEK_SET);

    B_PRIMZONA blok;

    fread(&blok, sizeof(B_PRIMZONA), 1, aktivnaDat->f);
    PREKORACIOC prekIn = nadjiPrekoracioca(&blok, in);
    PREKORACIOC pomocni;
    int adresaPomocnog;
    PREKORACIOC pomocni1;

    /// ukoliko postoji prekoracioc udji
    if(prekIn.slog.evidBroj != 0){
            /// da li postoje prekoracioci na glavi bloka
        if(blok.adresaPrek == -1){
            /// ako ne postoje dodaj na glavu jedan i zavrsi
            fseek(aktivnaDat->f, 0, SEEK_END);
            blok.adresaPrek = (int)ftell(aktivnaDat->f);

        }else{
            /// ovo je specijalni slucaj za dodavanje prekoracioca izmedju glave i jos jednog prekoracioca
            fseek(aktivnaDat->f, blok.adresaPrek, SEEK_SET);
            adresaPomocnog = blok.adresaPrek;
            /// citam prekoracioca kod glave
            fread(&pomocni, sizeof(PREKORACIOC), 1, aktivnaDat->f);

           /// ukoliko je prekoracioc koji treba da se doda manji od prekoracioca na glavi, dodaj ga izmedju glave i tog prekoracioca
            if(prekIn.slog.evidBroj < pomocni.slog.evidBroj){

                fseek(aktivnaDat->f, 0, SEEK_END);
                int staraAdresa = blok.adresaPrek;
                blok.adresaPrek = (int)ftell(aktivnaDat->f);
                prekIn.adresaSled = staraAdresa;

            }else{

            /// ukoliko prokracioc ne ide na mesto ispred glave prolazi kroz listu
                while(1){
                    printf("\nUSAO U WHILE %d\n \n\t SLEDECI %d", pomocni.slog.evidBroj, pomocni.adresaSled);
                   /// ukoliko sam stigao na kraj liste prekoracioca
                    if(pomocni.adresaSled == -1){

                        fseek(aktivnaDat->f, 0, SEEK_END);
                        pomocni.adresaSled = (int)ftell(aktivnaDat->f);
                        fseek(aktivnaDat->f, adresaPomocnog, SEEK_SET);
                        fwrite(&pomocni, sizeof(PREKORACIOC), 1, aktivnaDat->f);
                        break;
                    }else{

                    /// ukoliko prekoracioc postoji negde u list

                        /// hocu da procitam jos jedan prekoracio koji je ispred pomocnog
                        fseek(aktivnaDat->f, pomocni.adresaSled, SEEK_SET);
                        int adresaPomocni1 = pomocni.adresaSled;
                        fread(&pomocni1, sizeof(PREKORACIOC), 1, aktivnaDat->f);
                        printf("EVID STO TREBA UBACITI %d < %d pomocni1 evid broj",prekIn.slog.evidBroj,pomocni1.slog.evidBroj );
                        if(prekIn.slog.evidBroj < pomocni1.slog.evidBroj){
                            prekIn.adresaSled = adresaPomocni1;
                            fseek(aktivnaDat->f, 0, SEEK_END);
                            pomocni.adresaSled = (int)ftell(aktivnaDat->f);
                            fseek(aktivnaDat->f, adresaPomocnog, SEEK_SET);
                            fwrite(&pomocni, sizeof(PREKORACIOC), 1, aktivnaDat->f);
                            break;
                        }

                        pomocni = pomocni1;

                    }


                }


            }





        }



        /// prekoracioc uvek ide na kraj

        fseek(aktivnaDat->f, 0, SEEK_END);
        fwrite(&prekIn, sizeof(PREKORACIOC), 1, aktivnaDat->f);

    }




    fseek(aktivnaDat->f, temp.adresa, SEEK_SET);
    fwrite(&blok, sizeof(B_PRIMZONA), 1, aktivnaDat->f);
    printf("\n\n\n\t***ISPIS POSLE DODAVANJA*** \n");
    ispisBloka(temp.adresa, aktivnaDat->f);

}





void unesiNadjiSlog(DATOTEKA* aktivnaDat){

    if(aktivnaDat->f == NULL){
        printf("\n\tNijedna aktivna datoteka nije trenutno otvorena! \n");
        return;
    }

    fseek(aktivnaDat->f, 0, SEEK_SET);

    int root;
    int primZona;
    int prekZona;

    if(fread(&root, sizeof(int), 1, aktivnaDat->f) != 1 && root <= 0){

        printf("\n\tU '%s' nije nadjeno stablo \n ", aktivnaDat->name);
        return;
    }

    fread(&primZona, sizeof(int), 1, aktivnaDat->f);
    fread(&prekZona, sizeof(int), 1, aktivnaDat->f);


    fseek(aktivnaDat->f, 0, SEEK_SET);

    int numDigits;
    unsigned unos;

     do{
        printf("\tUnesite evidencioni broj (8 cifara) >> ");
        scanf("%d", &unos);
        fflush(stdin);

        numDigits = (int)floor(log10((double)unos)) + 1;

        if(numDigits != 8)
            printf("\tEvidencioni broj mora imati 8 cifara! \n");


    }while(numDigits != 8);

    NADJEN_SLOG nadjenSlog = traziSlog(aktivnaDat->f, unos);
    printf("\n\t************** REZULTAT PRETRAGE *******************");

    if(nadjenSlog.adresa != -1){

        fseek(aktivnaDat->f, nadjenSlog.adresa, SEEK_SET);

        if(nadjenSlog.adresa < prekZona){

            BLOK b;
            fread(&b, sizeof(BLOK), 1, aktivnaDat->f);

            printf("\n\n\tAdresa bloka: %d \n", nadjenSlog.adresa);
            printf("\tRedni broj sloga: %d \n", nadjenSlog.redniBrojSloga);
            ispisSloga(b.slog[nadjenSlog.redniBrojSloga]);


        }else{

            PREKORACIOC p;
            fread(&p, sizeof(PREKORACIOC), 1, aktivnaDat->f);
            printf("\n\n\tAdresa prekoracioca %d \n", nadjenSlog.adresa);
            ispisSloga(p.slog);
        }

    }else{

        printf("\n\tSLOG NE POSTOJI! \n");

    }

    printf("\n\t*********************************\n\n");

    fseek(aktivnaDat->f, 0, SEEK_SET);

}

void ispisAktivneDatoteke(DATOTEKA* aktivnaDat){


    if(aktivnaDat->f == NULL){

        printf("\n\tAktivna datoteka nije otvorena \n");
        return;
    }

    fseek(aktivnaDat->f, 0, SEEK_SET);

    int root, primarna, prekor;

    if(fread(&root, sizeof(int), 1, aktivnaDat->f) != 1){
        printf("\n\tU aktivnoj datoteci stablo nije nadjeno \n");
        return;
    }

    fread(&primarna, sizeof(int), 1, aktivnaDat->f);
    fread(&prekor, sizeof(int), 1, aktivnaDat->f);

    fseek(aktivnaDat->f, 3*sizeof(int), SEEK_SET);

    int brojPrimarnihBlokova = (prekor - primarna)/sizeof(B_PRIMZONA);
    int visina = visinaStFormula(brojPrimarnihBlokova);
    int ukupanBrojCvorova = brojCvorovaStFormula(brojPrimarnihBlokova, visina);

    system("cls");

    printf("\n\n\n\t********* ISPIS DATOTEKE '%s' *********\n", aktivnaDat->name);

    printf("\n\t BROJ CVOROVA I LISTOVA %d ", ukupanBrojCvorova);
    printf("\n\t VISINA STABLA %d", visina);
    printf("\n\t BROJ PRIMARNIH BLOKOVA %d", brojPrimarnihBlokova);

    printf("\n\n\t********* INDEKSNA ZONA *********\n\n");
    while((int)ftell(aktivnaDat->f) < primarna){

        INDEKS temp;

        fread(&temp, sizeof(INDEKS), 1, aktivnaDat->f);
        ispisIndeks(temp, (int)ftell(aktivnaDat->f) - sizeof(INDEKS));

    }
    printf("\n\n\t********* KRAJ ZONE *********\n");


    fseek(aktivnaDat->f, primarna, SEEK_SET);
    printf("\n\n\t***** BLOKOVI PRIMARNE SA PREKORACIOCIMA *****\n\n ");
    int brojac = 0;
    while((int)ftell(aktivnaDat->f) < prekor){

        printf("\n\n\t**** PRIMARNI BLOK BROJ %d ****", brojac+1);

        B_PRIMZONA temp;

        fread(&temp, sizeof(B_PRIMZONA), 1, aktivnaDat->f);

        int adresaTemp = (int)ftell(aktivnaDat->f);

        int i;

        for(i = 0; i < faktBlok; i++){

           if(temp.slog[i].indikatorAktiv == 1)
                ispisSloga(temp.slog[i]);
        }

           if(temp.adresaPrek != -1)
                printf("\n\n\tPREKORACIOCI BLOKA %d \n", brojac + 1);

            int pointer = temp.adresaPrek;

            while(pointer != -1){
                PREKORACIOC tempPrek;

                fseek(aktivnaDat->f, pointer, SEEK_SET);
                fread(&tempPrek, sizeof(PREKORACIOC), 1, aktivnaDat->f);

                ispisSloga(tempPrek.slog);

                pointer = tempPrek.adresaSled;
            }

        fseek(aktivnaDat->f, adresaTemp, SEEK_SET);
         printf("\n\t**** KRAJ BLOKA BROJ %d ****\n\n\n", brojac+1);
        brojac++;



    }

    printf("\n\n\n\t********* KRAJ ISPISA '%s' *********\n", aktivnaDat->name);


}

void reorganizacijaAktivneDat(DATOTEKA* dat){

    LIST* head = NULL;

    if(dat->f == NULL){
        printf("\n\tNema otvorena aktivne datoteke \n");
        return;
    }

    int root, prim, prek;

    fseek(dat->f, 0, SEEK_SET);

    if(fread(&root, sizeof(int), 1, dat->f) != 1){
        printf("\n\tU datoteci '%s' se ne nalazi stablo \n", dat->name);
        return;
    }

    fread(&prim, sizeof(int), 1, dat->f);
    fread(&prek, sizeof(int), 1, dat->f);

    fseek(dat->f, 0, SEEK_END);


    if(((int)ftell(dat->f) - prek)/sizeof(PREKORACIOC) < 2){
        printf("\n\tUslov za reorganizaciju su bar 2 prekoracioca \n");
        return;
    }



    int brojSlogova = 0;

    fseek(dat->f, prim, SEEK_SET);

    while((int)ftell(dat->f) < prek){

        B_PRIMZONA p;

        fread(&p, sizeof(B_PRIMZONA), 1, dat->f);

        int i;

        for(i = 0; i < faktBlok; i++){

            if(p.slog[i].indikatorAktiv == 1){
                dodajElemSortListu(&head, p.slog[i]);
                brojSlogova++;
            }
        }
    }


    fseek(dat->f, 0, SEEK_END);

    int krajDat = (int) ftell(dat->f);

    fseek(dat->f, prek, SEEK_SET);



    while((int)ftell(dat->f) != krajDat){

        PREKORACIOC temp;

        fread(&temp, sizeof(PREKORACIOC), 1, dat->f);

        dodajElemSortListu(&head, temp.slog);
        brojSlogova++;

    }



  int size;

  if(brojSlogova % (int)(faktBlok*ibf) != 0)
        size = (int)ceil(brojSlogova / (double)(ibf*faktBlok));
    else size = brojSlogova / (ibf*faktBlok) + 1;

    printf("BROJ SLOGOVA: %d \n", brojSlogova);
    printf("BROJ BLOKOVA: %d \n", size);

    B_PRIMZONA* p = (B_PRIMZONA*)malloc(size*sizeof(B_PRIMZONA));

    int i, j;

    for(i = 0; i < size; i++)
        p[i] = formiranjePraznogBlokaPrimarneZone();

    int kraj = 0; /// 1 - kraj liste 0 - nije kraj liste

    LIST* tempHead = head;


    for(i = 0; i < size; i++){
        for(j = 0; j < faktBlok*ibf; j++){
           if(tempHead != NULL){
                p[i].slog[j] = tempHead->val;
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


    writeIndeksSekv(dat, p, size);

    free(p);


}







