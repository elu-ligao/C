#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "stringList_dynamic.h"


// 声明调用函数指针变量
void *stringListHandle = NULL;

indexBFHandle indexBFFunc = NULL;

indexKMPHandle indexKMPFunc = NULL;
// getNextHandle getNextFunc = NULL;
// getNextValHandle getNextValFunc = NULL;

// 初始化stringList动态库
int initStringListHandle()
{    
    stringListHandle = dlopen(STRINGLIST_DLL_PATH, RTLD_LAZY);
    if(!stringListHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    indexBFFunc = (indexBFHandle)dlsym(stringListHandle, "indexBF");
    if(NULL == indexBFFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    indexKMPFunc = (indexKMPHandle)dlsym(stringListHandle, "indexKMP");
    if(NULL == indexKMPFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    // dlerror();
    // getNextFunc = (getNextHandle)dlsym(stringListHandle, "getNext");
    // if(NULL == getNextFunc)
    // {
    //     fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
    //     return EXIT_FAILURE;
    // }
    
    // dlerror();
    // getNextValFunc = (getNextValHandle)dlsym(stringListHandle, "getNextVal");
    // if(NULL == getNextValFunc)
    // {
    //     fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
    //     return EXIT_FAILURE;
    // }

    return 0;
}


void closeStringListHandle()
{
    dlclose(stringListHandle);
}