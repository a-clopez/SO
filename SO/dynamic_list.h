
#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

#define LNULL NULL

typedef struct tNode *tPosL;
struct tNode{
    tItemL data;
    tPosL next;
};
typedef tPosL tList;


tList createEmptyList(tList *L);
bool isEmptyList(tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL pos,tList L);
tPosL previous(tPosL pos,tList L);
bool insertItem(tItemL item,tPosL pos,tList *pL);
void  deleteAtPosition(tPosL pos,tList *pL);
tItemL getItem(tPosL pos,tList L);
void updateItem(tItemL item,tPosL pos,tList *pL);
int last_int(tList L);
tPosL findItem_df(int df,tList L);
tPosL findItem(char name[FILENAME_MAX],tList L);

#endif
