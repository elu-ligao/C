#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "trace_dynamic.h"
#include "bst_dynamic.h"



void initLog()
{
    InitTraceFunc("test");
    GetTraceFileFunc(NULL);
}




int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        srand(atoi(argv[2]));
    }
    else if(argc == 2)
    {
        srand(time(NULL));
    }
    else
    {
        printf("Usage %s num [seed]\n", argv[0]);
        exit(-1);
    }

    int rc = 0;
    rc = initTraceDynamicHandle();
    if(rc)
    {
        fprintf( stderr, "[%s](%d) initTraceDynamicHandle get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }
    
    initLog();
    rc = initBstHandle();
    if(rc)
    {
        fprintf( stderr, "[%s](%d) initBstHandle get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }
    
    int i;
    int numElem = atoi(argv[1]);
    pbstElement elist = (pbstElement)calloc(numElem, sizeof(bstElement));
    for(i=0; i<numElem; ++i)
    {
        elist[i].key = rand()%100;
        printf("%d ", elist[i].key );
    }
    printf("\n");

    pbsTree root = createBSTree(elist, numElem);

    // free elist
    free(elist);
    elist = NULL;

    drawBsTreeFunc(root, 2);
    
    LogInfoHex((unsigned char *)root->data, sizeof(bstElement), "root->data");
    pbstElement find = (pbstElement)calloc(1, sizeof(bstElement));
    find->key = rand()%100;
    printf("find element = %d\n", find->key);
    pbsTree f = searchBstFunc(root, find);
    if(f)
    {
        printf("Find!\n");
        LogDebugHex((unsigned char*)f, sizeof(bstNode), "find node");
        LogDebug("f->data: %d", f->data->key);

        pbsTree minmax = minBstFunc(root);
        printf("minist element = %d\n", minmax->data->key);
        minmax = maxBstFunc(root);
        printf("maxist element = %d\n", minmax->data->key);

        minmax = predecessorBstFunc(root, find);
        if(minmax)
            printf("predecessor then %d is %d\n", find->key, minmax->data->key);
        else
            printf("find is the minist element.\n");

        minmax = successorBstFunc(root, find);
        if(minmax)
            printf("successor then %d is %d\n", find->key, minmax->data->key);
        else
            printf("The find %d is the maxist element.\n", find->key);
    }
    else
    {
        printf("Not found.\n");
    }

    printf("Input a num to delete: ");
    scanf("%d", &find->key);
    
    rc = deleteBstFunc(&root, find);
    if(0 == rc)
    {
        printf("delete %d success\n", find->key);
        LogDebugHex((unsigned char *)root, sizeof(bstNode)*numElem-1, "root");
        
        drawBsTreeFunc(root, 2);
    }
    else
    {
        printf("To be deleted element %d is not found\n", find->key);
    }

    freeBst(&root);

    closeBstHandle();
    closeTraceDynamicHandle();

    return 0;
}