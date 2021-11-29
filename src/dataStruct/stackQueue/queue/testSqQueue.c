#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sqQueue_dynamic.h"

int getData(pSqQueue Q, char sex, char *prefix);

int main(int argc, char *argv[])
{

    int rc = initSqQueueHandle();
    if(rc)
    {
        return -1;
    }

    srand(time(NULL));

    pSqQueue FQ = initQueueFunc();
    if(!FQ) return -1;
    pSqQueue MQ = initQueueFunc();
    if(!MQ) return -1;

    getData(FQ, 'F', "Miss.");
    getData(MQ, 'M', "Mr.");

    queueTraverseFunc(MQ);
    printf("--------------------------\n");
    queueTraverseFunc(FQ);

    pElemType Mr, Miss;
    while(!queueIsEmptyFunc(FQ) && !queueIsEmptyFunc(MQ))
    {
        Mr = deQueueFunc(MQ);
        Miss = deQueueFunc(FQ);
        printf("%s and %s are partners now.\n", Mr->name, Miss->name);
    }

    int i = 1;
    while(!queueIsEmptyFunc(MQ))
    {
        Mr = deQueueFunc(MQ);
        printf("%s is the next %dth to get in.\n", Mr->name, i++);
    }
    while(!queueIsEmptyFunc(FQ))
    {
        Miss = deQueueFunc(FQ);
        printf("%s is the next %dth to get in.\n", Miss->name, i++);
    }

    destroyQueueFunc(&MQ);
    destroyQueueFunc(&FQ);

    closeSqQueueHandle();

    return 0;
}

int getData(pSqQueue Q, char sex, char *prefix)
{
    int num = rand()%MAX_QUEUE_SIZE;
    num = num < 3 ? 3 : num;
    int i = 0;
    pElemType e = NULL;
    for(; i<num; ++i)
    {
        e = calloc(1, sizeof(ElemType));
        e->age = rand()%60+10;
        e->sex[0] = sex;
        sprintf(e->name, "%s%d", prefix, rand()%1000);
        if(0 == enQueueFunc(Q, e))
        {
            free(e);
            e = NULL;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}