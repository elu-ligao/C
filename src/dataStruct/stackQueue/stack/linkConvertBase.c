#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "testStackLink.h"
#include "stackLink_dynamic.h"

void testConvertBase(int argc, char *argv[])
{
    int num = 0, seed = 0; 
    int base = 2;
    int opt;
    char params[4+1] = {0};

    while((opt = getopt(argc, argv, "s:n:b:")) != -1)
    {
        switch (opt)
        {
        case 's':
            params[0] = '1';
            seed = atoi(optarg);
            break;
        case 'n':
            params[1] = '1';
            num = atoi(optarg);
            break;
        case 'b':
            params[2] = '1';
            base = atoi(optarg);
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-s seed] [-n num] [-b base]\n", argv[0]);
            return;
        }
    }

    if('1' != params[0])
        seed = time(NULL);
    srand(seed);

    if('1' != params[1])
        num = rand()%10000;
        
    pStackLink S = initStackFunc();
    convertBase(&S, num, base);

    char *s = getConvert(&S);
    if(s && strlen(s))
    {
        printf("num %d convert with base %d to %s.\n", num, base, s);
        free(s);
        s = NULL;
    }
}

void convertBase(pStackLink *S, int num, int base)
{
    if(base > 9 && base != 16)
    {
        perror("check the base.\n");
        return;
    }

    int mode = 0;
    do
    {
        mode = num % base;
        pElemType e = newElement();
        if(!e)
        {
            perror("calloc for element error.\n");
            return;
        }
        e->iVal = mode > 9 ? (mode - 10) + 'A' : mode + '0';
        pushFunc(S, e);  
        num /= base;
    }while(num);
}


char *getConvert(pStackLink *S)
{
    char *s = (char *)calloc(32, sizeof(char));
    if(!s)  return NULL;
    pElemType e = NULL;
    int i = 0;
    while((e = popFunc(S)) != NULL)
    {
        s[i++] = e->iVal;
    }
    free(e);
    e = NULL;
    return s;
}