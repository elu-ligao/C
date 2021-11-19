#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "trace_dynamic.h"

void *traceHandle = NULL;

// 声明调用函数指针变量
GetTraceFileHandle GetTraceFileFunc = NULL;
InitTraceHandle InitTraceFunc = NULL;
LogMsgHandle LogMsgFunc = NULL;
LogMsgHexHandle LogMsgHexFunc = NULL;


// 初始化bst动态库
int initTraceDynamicHandle()
{
    traceHandle = dlopen(TRACE_DLL_PATH, RTLD_LAZY);
    if(!traceHandle)
    {
        fprintf( stderr, "[%s](%d) dlopen get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    InitTraceFunc = (InitTraceHandle)dlsym(traceHandle, "InitTrace");
    if(NULL == InitTraceFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    GetTraceFileFunc = (GetTraceFileHandle)dlsym(traceHandle, "GetTraceFile");
    if(NULL == GetTraceFileFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }

    dlerror();
    LogMsgFunc = (LogMsgHandle)dlsym(traceHandle, "LogMsg");
    if(NULL == LogMsgFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    dlerror();
    LogMsgHexFunc = (LogMsgHexHandle)dlsym(traceHandle, "LogMsgHex");
    if(NULL == LogMsgHexFunc)
    {
        fprintf( stderr, "[%s](%d) dlsym get error: %s\n", __FILE__, __LINE__, dlerror() );
        return EXIT_FAILURE;
    }
    
    return 0;
}


void closeTraceDynamicHandle()
{
    dlclose(traceHandle);
}




