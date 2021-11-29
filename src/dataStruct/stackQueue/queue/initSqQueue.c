#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "sqQueue_dynamic.h"

// 声明调用函数指针变量
void *SqQueueHandle = NULL;
initQueueHandle initQueueFunc = NULL;
destroyQueueHandle destroyQueueFunc = NULL;
clearQueueHandle clearQueueFunc = NULL;
queueIsEmptyHandle queueIsEmptyFunc = NULL;
queueIsFullHandle queueIsFullFunc = NULL;
queueLengthHandle queueLengthFunc = NULL;
getHeadHandle getHeadFunc = NULL;
enQueueHandle enQueueFunc = NULL;
deQueueHandle deQueueFunc = NULL;
queueTraverseHandle queueTraverseFunc = NULL;


// 初始化bst动态库
int initSqQueueHandle()
{    
    SqQueueHandle = dlopen(SQ_QUEUE_DLL_PATH, RTLD_LAZY);
    if(!SqQueueHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    initQueueFunc = (initQueueHandle)dlsym(SqQueueHandle, "initQueue");
    if(NULL == initQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }


    dlerror();
    destroyQueueFunc = (destroyQueueHandle)dlsym(SqQueueHandle, "destroyQueue");
    if(NULL == destroyQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    clearQueueFunc = (clearQueueHandle)dlsym(SqQueueHandle, "clearQueue");
    if(NULL == clearQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    queueIsEmptyFunc = (queueIsEmptyHandle)dlsym(SqQueueHandle, "queueIsEmpty");
    if(NULL == queueIsEmptyFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    queueIsFullFunc = (queueIsFullHandle)dlsym(SqQueueHandle, "queueIsFull");
    if(NULL == queueIsFullFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    queueLengthFunc = (queueLengthHandle)dlsym(SqQueueHandle, "queueLength");
    if(NULL == queueLengthFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    getHeadFunc = (getHeadHandle)dlsym(SqQueueHandle, "getHead");
    if(NULL == getHeadFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    enQueueFunc = (enQueueHandle)dlsym(SqQueueHandle, "enQueue");
    if(NULL == enQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    deQueueFunc = (deQueueHandle)dlsym(SqQueueHandle, "deQueue");
    if(NULL == deQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    queueTraverseFunc = (queueTraverseHandle)dlsym(SqQueueHandle, "queueTraverse");
    if(NULL == queueTraverseFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



void closeSqQueueHandle()
{
    dlclose(SqQueueHandle);
}
