#ifndef DATETIME_H_INCLUDED
#define DATETIME_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{

    unsigned short dd;
    unsigned short mm;
    unsigned short yy;

}DATE;

typedef struct{

    unsigned short hh;
    unsigned short mm;
    unsigned short ss;

}TIME;

void createTime(TIME*);
void createDate(DATE*);

void printTime(TIME);
void printDate(DATE);


#endif // DATETIME_H_INCLUDED
