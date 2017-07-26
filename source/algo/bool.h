

#ifndef BOOL_H
#define BOOL_H

typedef unsigned char dbool;

/*
 Don't change these as these depend on
 statements like:
 
 return (dbool)(heap->total > 0);
 if(found)
 */
#define dtrue	1
#define dfalse	0

#endif