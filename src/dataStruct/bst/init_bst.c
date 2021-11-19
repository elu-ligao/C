#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "bst_dynamic.h"


// 声明调用函数指针变量
void *bstHandle = NULL;
newBstElementHandle newBstElementFunc = NULL;
newBstNodeHandle newBstNodeFunc = NULL;
createBSTreeHandle createBSTreeFunc = NULL;
searchBstHandle searchBstFunc = NULL;
minBstHandle minBstFunc = NULL;
maxBstHandle maxBstFunc = NULL;
predecessorBstHandle predecessorBstFunc = NULL;
successorBstHandle successorBstFunc = NULL;
// outputSortedBstHandle outputSortedBstFunc = NULL;
insertBstHandle insertBstFunc = NULL;
deleteBstHandle deleteBstFunc = NULL;
freeBstHandle freeBstFunc = NULL;
drawBsTreeHandle drawBsTreeFunc = NULL;

// 初始化bst动态库
int initBstHandle()
{    
    bstHandle = dlopen(BST_DLL_PATH, RTLD_LAZY);
    if(!bstHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    newBstElementFunc = (newBstElementHandle)dlsym(bstHandle, "newBstElement");
    if(NULL == newBstElementFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    newBstNodeFunc = (newBstNodeHandle)dlsym(bstHandle, "newBstNode");
    if(NULL == newBstNodeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    createBSTreeFunc = (createBSTreeHandle)dlsym(bstHandle, "createBSTree");
    if(NULL == createBSTreeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    searchBstFunc = (searchBstHandle)dlsym(bstHandle, "searchBst");
    if(NULL == searchBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    minBstFunc = (minBstHandle)dlsym(bstHandle, "minBst");
    if(NULL == minBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    maxBstFunc = (maxBstHandle)dlsym(bstHandle, "maxBst");
    if(NULL == maxBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    predecessorBstFunc = (predecessorBstHandle)dlsym(bstHandle, "predecessorBst");
    if(NULL == predecessorBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    successorBstFunc = (successorBstHandle)dlsym(bstHandle, "successorBst");
    if(NULL == successorBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    // dlerror();
    // outputSortedBstFunc = (outputSortedBstHandle)dlsym(bstHandle, "outputSortedBst");
    // if(NULL == outputSortedBstFunc)
    // {
    //     fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
    //     return EXIT_FAILURE;
    // }

    dlerror();
    insertBstFunc = (insertBstHandle)dlsym(bstHandle, "insertBst");
    if(NULL == insertBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    deleteBstFunc = (deleteBstHandle)dlsym(bstHandle, "deleteBst");
    if(NULL == deleteBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    freeBstFunc = (freeBstHandle)dlsym(bstHandle, "freeBst");
    if(NULL == freeBstFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    drawBsTreeFunc = (drawBsTreeHandle)dlsym(bstHandle, "drawBsTree");
    if(NULL == drawBsTreeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    return 0;
}


void closeBstHandle()
{
    dlclose(bstHandle);
}