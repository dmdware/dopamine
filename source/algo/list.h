

#ifndef LIST_H
#define LIST_H

#include "../sys/inc.h"
#include "bool.h"

struct lnode
{
    struct lnode *next;
    struct lnode *prev;
	char data[0];
};

typedef struct lnode lnode;

struct list
{
	int size;
	lnode *head;
	lnode *tail;
};

typedef struct list list;

void linit(list *l);
void lfree(list *l);
void lpub(list *l, int size);
void lpub2(list *l, int size, void *data);
void lerase(list *l, lnode *link);
void lunlink(list *l, lnode *link);
void llinkb(list *l, lnode *link);
void lswap(list *l, lnode *a, lnode *b);
void lsort(list *l, dbool (*comparefunc)(void *a, void *b));

#endif