#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "stackSequence_dynamic.h"

// 声明调用函数指针变量
void *stackSequenceHandle = NULL;
initStackHandle initStackFunc = NULL;
destroyStackHandle destroyStackFunc = NULL;
clearStackHandle clearStackFunc = NULL;
stackEmptyHandle stackEmptyFunc = NULL;
stackLengthHandle stackLengthFunc = NULL;
getTopHandle getTopFunc = NULL;
pushHandle pushFunc = NULL;
popHandle popFunc = NULL;
stackTraverseHandle stackTraverseFunc = NULL;
newElementHandle newElementFunc = NULL;


// 初始化bst动态库
int initStackSequenceHandle()
{    
    stackSequenceHandle = dlopen(STACK_SEQUENCE_DLL_PATH, RTLD_LAZY);
    if(!stackSequenceHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    initStackFunc = (initStackHandle)dlsym(stackSequenceHandle, "initStack");
    if(NULL == initStackFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }


    dlerror();
    destroyStackFunc = (destroyStackHandle)dlsym(stackSequenceHandle, "destroyStack");
    if(NULL == destroyStackFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    clearStackFunc = (clearStackHandle)dlsym(stackSequenceHandle, "clearStack");
    if(NULL == clearStackFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    stackEmptyFunc = (stackEmptyHandle)dlsym(stackSequenceHandle, "stackEmpty");
    if(NULL == stackEmptyFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    stackLengthFunc = (stackLengthHandle)dlsym(stackSequenceHandle, "stackLength");
    if(NULL == stackLengthFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    getTopFunc = (getTopHandle)dlsym(stackSequenceHandle, "getTop");
    if(NULL == getTopFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    pushFunc = (pushHandle)dlsym(stackSequenceHandle, "push");
    if(NULL == pushFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    popFunc = (popHandle)dlsym(stackSequenceHandle, "pop");
    if(NULL == popFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    stackTraverseFunc = (stackTraverseHandle)dlsym(stackSequenceHandle, "stackTraverse");
    if(NULL == stackTraverseFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    
    dlerror();
    newElementFunc = (newElementHandle)dlsym(stackSequenceHandle, "newElement");
    if(NULL == newElementFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



void closeStackSequenceHandle()
{
    dlclose(stackSequenceHandle);
}
