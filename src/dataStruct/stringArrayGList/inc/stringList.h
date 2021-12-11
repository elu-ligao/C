#ifndef _STRING_LIST_H
#define _STRING_LIST_H


int indexBF(char *S, char *T, int pos);

int indexKMP(char *S, char *T, int pos);
int *getNext(char *T);
int *getNextVal(char *T);

#endif