
#include "dynamic_list.h"
#include "string.h"

tList createEmptyList(tList *pL){
    tList L = LNULL;
    *pL = L;
    return L;
}

bool isEmptyList(tList L){
    if(L==LNULL)
        return true;
    else
        return false;
}
tPosL first(tList L){
    return L;
}
tPosL last(tList L){
    tPosL actpos = L;
    if (actpos==LNULL)return actpos;
    while(actpos->next!=NULL){
        actpos=actpos->next;
    }
    return actpos;
}

int last_int(tList L){
    tPosL actpos = L;
    int i=0;
    if (actpos==LNULL) return i;
    while(actpos->next!=LNULL){
        actpos=actpos->next;
        i++;
    }
    return i;
}

tPosL next(tPosL pos,tList L){
        return pos->next;

}

tPosL previous(tPosL pos,tList L){
    tPosL actpos = L;
    if (actpos->next==LNULL)
        return LNULL;
    while(actpos->next!=pos){
        actpos =actpos->next;
    }
    return actpos;
}
bool insertItem(tItemL item,tPosL pos,tList *pL){
    //Create tNode
    tPosL t,r,s;
    t = malloc(sizeof(struct tNode));
    if (t==LNULL)
        return false;
    else{
        t->data= item;
        t->next =LNULL;

    //List is empty
    if(*pL==LNULL){
        *pL=t;
    }
    else{
        //Pos is LNULL
        if (pos==LNULL){
            r=*pL;
            if(r->next!=LNULL){
                while (r->next!=LNULL) {
                    r = r->next;
                }
            }
            r->next=t;
        }
    }
}
    return true;
}


tPosL findItem(char name[FILENAME_MAX],tList L){
    tPosL pos = L;
    while(pos!=LNULL){
        tItemL item=getItem(pos,L);
        if(strcmp(item.text,name)==0) {
            return pos;
        }
        pos=pos->next;
    }
    return LNULL;
}

tPosL findItem_df(int df,tList L){
    tPosL pos = L;
    while(pos!=LNULL){
        tItemL item=getItem(pos,L);
        if(item.df==df){
            return pos;
        }
        pos=pos->next;
    }
    return LNULL;
}

void deleteAtPosition(tPosL pos,tList *pL){
    tPosL r=*pL;
    tPosL s,q;
    //Only 1 element
    if(r->next==LNULL){
            *pL=LNULL;
            free(pos);
    }
    else{
        //If pos is last element
        if(pos->next==LNULL){
            s=previous(pos,*pL);
            s->next=LNULL;
            free(pos);
        }
        else{
            //first element
            if(*pL==pos){
                *pL = pos->next;
                free(pos);
                return;
            }
            else{
                //default case
                s=pos->next;
                q = previous(pos,*pL);
                q->next = s;
                free(pos);
            }
        }
}}
tItemL getItem(tPosL pos,tList L){
        return pos->data;

}
void updateItem(tItemL item,tPosL pos,tList *pL){
    pos->data=item;
}

tList init_filelist(tList *pL) {
    tItemL archivo;
    for (int i = 0; i < 3; i++) {
        archivo.df = i;
        archivo.mode = 02;
        if (i == 0) strcpy(archivo.text, "entrada estandar");
        if (i == 1) strcpy(archivo.text, "salida estandar");
        if (i == 2) strcpy(archivo.text, "error estandar");
        insertItem(archivo, LNULL, pL);

    }return *pL;
}