#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "trace.h"
#include "avl_dynamic.h"



void initLog()
{
    InitTrace("test");
    GetTraceFile(NULL);
}




int main(int argc, char *argv[])
{
    int rc = 0;
    initLog();

    rc = initAvlHandle();
    if(rc)
    {
        fprintf( stderr, "[%s](%d) initAvlHandle get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }

    srand(time(NULL));
    // srand(100);

    int i;
    int numElem = atoi(argv[1]);
    pavlElement elist = (pavlElement)calloc(numElem, sizeof(avlElement));
    for(i=0; i<numElem; ++i)
    {
        elist[i].key = rand()%100;
        printf("%d ", elist[i].key );
    }
    printf("\n");

    pavlTree root = createAVLTree(elist, numElem);

    // free elist
    free(elist);
    elist = NULL;

    drawAvlTreeFunc(root, 2);
    
    LogInfoHex((unsigned char *)root->data, sizeof(avlElement), "root->data");
    pavlElement find = (pavlElement)calloc(1, sizeof(avlElement));
    find->key = rand()%100;
    printf("find element = %d\n", find->key);
    pavlTree f = searchAvlFunc(root, find);
    if(f)
    {
        printf("Find!\n");
        LogDebugHex((unsigned char*)f, sizeof(avlNode), "find node");
        LogDebug("f->data: %d", f->data->key);

        pavlTree minmax = minAvlFunc(root);
        printf("minist element = %d\n", minmax->data->key);
        minmax = maxAvlFunc(root);
        printf("maxist element = %d\n", minmax->data->key);

        minmax = predecessorAvlFunc(root, find);
        if(minmax)
            printf("predecessor then %d is %d\n", find->key, minmax->data->key);
        else
            printf("find is the minist element.\n");

        minmax = successorAvlFunc(root, find);
        if(minmax)
            printf("successor then %d is %d\n", find->key, minmax->data->key);
        else
            printf("The find %d is the maxist element.\n", find->key);
    }
    else
    {
        printf("Not found.\n");
    }

    rc = rand()%numElem;
    f = selectAvlFunc(root, rc);
    if(f)
        printf("random index = %d, selectAvl of this index's Element key is %d, and it's rankAvl is %d\n", rc, f->data->key, rankAvlFunc(root, f->data));
    else
        printf("random index = %d, selectAvl of this index not found\n", rc);
    printf("BalanceTree = %d\n", isBalanceTreeFunc(root));

    lrMirrorFunc(root);
    drawAvlTree(root, 2);
    printf("After lrMirror, BalanceTree = %d\n", isBalanceTreeFunc(root));

    // printf("Input a num to delete: ");
    // scanf("%d", &find->key);
    // rc = deleteAvlFunc(&root, find);
    // if(0 == rc)
    // {
    //     printf("after delete root is %d[%p]\n", root->data->key, root);
    //     printf("delete %d success\n", find->key);
    //     LogDebugHex((unsigned char *)root, sizeof(avlNode)*numElem-1, "root");
    //     drawAvlTreeFunc(root, 12);
    // }
    // else
    // {
    //     printf("To be delete element %d is not found\n", find->key);
    // }

    freeAvl(&root);

    closeAvlHandle();

    return 0;
}