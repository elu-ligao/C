#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "testSqStack.h"
#include "stackSequence_dynamic.h"

#define MAX_BRACKETS    32

void testBrackets(pSqStack S, int argc, char *argv[])
{
    int seed = 0; 
    int opt;
    char params[4] = {0};
    char *brackets = NULL;
    
    while((opt = getopt(argc, argv, "s:b:")) != -1)
    {
        switch (opt)
        {
        case 's':
            params[0] = '1';
            seed = atoi(optarg);
            break;
        case 'b':
            params[1] = '1';
            brackets = optarg;
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-s seed] [-b brackets]\n", argv[0]);
            return;
        }
    }
    
    if('1' != params[0])
        seed = time(NULL);
    srand(seed);

    if('1' != params[1])
        brackets = getBrackets();

    printf("To be check string is: \"%s\"\n", brackets);

    int ok = metchBrackets(S, brackets);   
    if(1 == ok)
        printf("Metched!\n");
    else
        printf("Not Metched.\n");

    return;
}


char *getBrackets()
{
    char brackets[8+1] = "()[]{}<>";
    int allType = strlen(brackets)-1;
    char *dest = (char *)calloc(MAX_BRACKETS+1, sizeof(char));
    if(!dest)   return NULL;
    int lenBrackets = rand()%MAX_BRACKETS;
    int i = 0;
    while(i < lenBrackets)
    {
        dest[i] = brackets[rand()%allType];
        ++i;
    }
    return dest;
}

int metchBrackets(pSqStack S, char *sBrackets)
{
    if(!sBrackets || !S)    return 0;
    char left[4+1] = "([{<";
    char right[4+1] = ")]}>";
    int sLen = strlen(sBrackets);
    int flag = 1;
    int i = 0;
    pElemType e = NULL;
    while (i<sLen && flag)
    {
        if(index(left, sBrackets[i]))   // 是左括号
        {
            e = newElement();
            if(!e)  
            {
                perror("new Element error");
                return -2;
            }
            e->sVal[0] = sBrackets[i];
            pushFunc(S, e);
            free(e);
            e = NULL;
        }
        else if(index(right, sBrackets[i])) // 右括号
        {
            e = popFunc(S);
            if(!e)
            {
                printf("get right but before not have left.\n");
                flag = 0;
            }
            else
                switch (e->sVal[0])
                {
                case '(':
                    if(sBrackets[i] != ')') flag = 0;
                    break;
                case '[':
                    if(sBrackets[i] != ']') flag = 0;
                    break;
                case '{':
                    if(sBrackets[i] != '}') flag = 0;
                    break;
                case '<':
                    if(sBrackets[i] != '>') flag = 0;
                    break;
                
                default:
                    break;
                }
        }
        else    // 非法字符
        {
            fprintf(stderr, "index of %d is \"%c\" which is not valid.\n", i, sBrackets[i]);
            return -1;
        }

        if(!flag)
            printf("index of %d \"%c\" is not metched.\n", i, sBrackets[i]);

        ++i;
    }
    if(i == sLen && flag)
        return 1;
    else
        return -22;
}