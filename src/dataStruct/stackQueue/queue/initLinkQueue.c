#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "linkQueue_dynamic.h"

// 声明调用函数指针变量
void *LinkQueueHandle = NULL;
newQueueNodeHandle newQueueNodeFunc = NULL;
initQueueHandle initQueueFunc = NULL;
destroyQueueHandle destroyQueueFunc = NULL;
clearQueueHandle clearQueueFunc = NULL;
queueIsEmptyHandle queueIsEmptyFunc = NULL;
queueLengthHandle queueLengthFunc = NULL;
getHeadHandle getHeadFunc = NULL;
enQueueHandle enQueueFunc = NULL;
deQueueHandle deQueueFunc = NULL;
queueTraverseHandle queueTraverseFunc = NULL;


// 初始化bst动态库
int initLinkQueueHandle()
{    
    LinkQueueHandle = dlopen(LINK_QUEUE_DLL_PATH, RTLD_LAZY);
    if(!LinkQueueHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    newQueueNodeFunc = (newQueueNodeHandle)dlsym(LinkQueueHandle, "newQueueNode");
    if(NULL == newQueueNodeFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    initQueueFunc = (initQueueHandle)dlsym(LinkQueueHandle, "initQueue");
    if(NULL == initQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    destroyQueueFunc = (destroyQueueHandle)dlsym(LinkQueueHandle, "destroyQueue");
    if(NULL == destroyQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    clearQueueFunc = (clearQueueHandle)dlsym(LinkQueueHandle, "clearQueue");
    if(NULL == clearQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    queueIsEmptyFunc = (queueIsEmptyHandle)dlsym(LinkQueueHandle, "queueIsEmpty");
    if(NULL == queueIsEmptyFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    queueLengthFunc = (queueLengthHandle)dlsym(LinkQueueHandle, "queueLength");
    if(NULL == queueLengthFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    getHeadFunc = (getHeadHandle)dlsym(LinkQueueHandle, "getHead");
    if(NULL == getHeadFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    enQueueFunc = (enQueueHandle)dlsym(LinkQueueHandle, "enQueue");
    if(NULL == enQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    deQueueFunc = (deQueueHandle)dlsym(LinkQueueHandle, "deQueue");
    if(NULL == deQueueFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    queueTraverseFunc = (queueTraverseHandle)dlsym(LinkQueueHandle, "queueTraverse");
    if(NULL == queueTraverseFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



void closeLinkQueueHandle()
{
    dlclose(LinkQueueHandle);
}
