#include "datetime.h"


void createTime(TIME* t){

    unsigned short hh, mm, ss;
    int provera = 1; /// vreme je validno uneto

    do{

        printf("\tUpisite vreme u formatu HH:MM:SS >> ");
        scanf("%hu:%hu:%hu", &hh, &mm, &ss);
        fflush(stdin);

        provera = hh < 24 && mm < 60 && ss < 60 && hh >= 0 && mm >= 0 && ss >=0;

        if(!provera)
            printf("\tVreme nije dobro upisano! \n");

    }while(!provera);

    t->hh = hh;
    t->mm = mm;
    t->ss = ss;


}

void createDate(DATE* d){

    unsigned short dd, mm, yy;
    int provera = 1; /// datum je validno uneto

    do{

        printf("\tUpisite datum u formatu DD.MM.YYYY >> ");
        scanf("%hu.%hu.%hu", &dd, &mm, &yy);
        fflush(stdin);

        provera = yy <= 2117 && yy >= 1917 && mm >= 1 && mm <= 12;

        if(mm == 1 && mm == 3 && mm ==5 && mm == 7 && mm == 8 && mm == 12)
            provera = provera && dd <= 31;
        else if(mm != 2){
            provera = provera && dd <= 30;
        }else if (mm == 2){

            if(yy % 400 == 0)
                provera = provera && dd <= 29;
            else if (yy % 100 == 0)
                provera = provera && dd <= 28;
            else if(yy % 4 == 0)
                provera = provera && dd <= 29;
            else provera = provera && dd <= 28;

    }




        if(!provera)
            printf("\tDatum nije dobro upisan! \n");

    }while(!provera);

    d->yy = yy;
    d->mm = mm;
    d->dd = dd;

}


void printDate(DATE d){

    if(d.dd < 10)
        printf("0%d.", d.dd);
    else printf("%d.", d.dd);

    if(d.mm < 10)
        printf("0%d.", d.mm);
    else printf("%d.", d.mm);

    printf("%d", d.yy);

}

void printTime(TIME t){

    if(t.hh < 10)
        printf("0%d:", t.hh);
    else printf("%d:", t.hh);

    if(t.mm < 10)
        printf("0%d:", t.mm);
    else printf("%d:", t.mm);

    if(t.ss < 10)
        printf("0%d", t.ss);
    else printf("%d", t.ss);

}
