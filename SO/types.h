
#ifndef SO_TYPES_H
#define SO_TYPES_H



#define NAME_LENGTH_LIMIT 200
#include <stdbool.h>



typedef struct tItemL {//Define the item of the list
    char text[NAME_LENGTH_LIMIT];
    int df;
    int mode;
} tItemL;

#endif //SO_TYPES_H
