#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "avl_dynamic.h"


// 声明调用函数指针变量
void *avlHandle = NULL;
newAvlElementHandle newAvlElementFunc = NULL;
newAvlNodeHandle newAvlNodeFunc = NULL;
createAVLTreeHandle createAVLTreeFunc = NULL;
searchAvlHandle searchAvlFunc = NULL;
minAvlHandle minAvlFunc = NULL;
maxAvlHandle maxAvlFunc = NULL;
predecessorAvlHandle predecessorAvlFunc = NULL;
successorAvlHandle successorAvlFunc = NULL;
// outputSortedAvlHandle outputSortedAvlFunc = NULL;
insertAvlHandle insertAvlFunc = NULL;
deleteAvlHandle deleteAvlFunc = NULL;
freeAvlHandle freeAvlFunc = NULL;
drawAvlTreeHandle drawAvlTreeFunc = NULL;
selectAvlHandle selectAvlFunc = NULL;
rankAvlHandle rankAvlFunc = NULL;
isBalanceTreeHandle isBalanceTreeFunc = NULL;
lrMirrorHandle lrMirrorFunc = NULL;

// 初始化avl动态库
int initAvlHandle()
{    
    avlHandle = dlopen(AVL_DLL_PATH, RTLD_LAZY);
    if(!avlHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    newAvlElementFunc = (newAvlElementHandle)dlsym(avlHandle, "newAvlElement");
    if(NULL == newAvlElementFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    newAvlNodeFunc = (newAvlNodeHandle)dlsym(avlHandle, "newAvlNode");
    if(NULL == newAvlNodeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    createAVLTreeFunc = (createAVLTreeHandle)dlsym(avlHandle, "createAVLTree");
    if(NULL == createAVLTreeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    searchAvlFunc = (searchAvlHandle)dlsym(avlHandle, "searchAvl");
    if(NULL == searchAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    minAvlFunc = (minAvlHandle)dlsym(avlHandle, "minAvl");
    if(NULL == minAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    maxAvlFunc = (maxAvlHandle)dlsym(avlHandle, "maxAvl");
    if(NULL == maxAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    predecessorAvlFunc = (predecessorAvlHandle)dlsym(avlHandle, "predecessorAvl");
    if(NULL == predecessorAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    successorAvlFunc = (successorAvlHandle)dlsym(avlHandle, "successorAvl");
    if(NULL == successorAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    // dlerror();
    // outputSortedAvlFunc = (outputSortedAvlHandle)dlsym(avlHandle, "outputSortedAvl");
    // if(NULL == outputSortedAvlFunc)
    // {
    //     fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
    //     return EXIT_FAILURE;
    // }

    dlerror();
    insertAvlFunc = (insertAvlHandle)dlsym(avlHandle, "insertAvl");
    if(NULL == insertAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    deleteAvlFunc = (deleteAvlHandle)dlsym(avlHandle, "deleteAvl");
    if(NULL == deleteAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    freeAvlFunc = (freeAvlHandle)dlsym(avlHandle, "freeAvl");
    if(NULL == freeAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    drawAvlTreeFunc = (drawAvlTreeHandle)dlsym(avlHandle, "drawAvlTree");
    if(NULL == drawAvlTreeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    isBalanceTreeFunc = (isBalanceTreeHandle)dlsym(avlHandle, "isBalanceTree");
    if(NULL == isBalanceTreeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    selectAvlFunc = (selectAvlHandle)dlsym(avlHandle, "selectAvl");
    if(NULL == selectAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    rankAvlFunc = (rankAvlHandle)dlsym(avlHandle, "rankAvl");
    if(NULL == rankAvlFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    lrMirrorFunc = (lrMirrorHandle)dlsym(avlHandle, "lrMirror");
    if(NULL == lrMirrorFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    return 0;
}


void closeAvlHandle()
{
    dlclose(avlHandle);
}