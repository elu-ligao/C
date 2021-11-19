#ifndef _TRACE_DYNAMIC_H
#define _TRACE_DYNAMIC_H

#include "trace.h"

#define TRACE_DLL_PATH     "/mnt/e/lg/vscode/lib/libtrace.so"


// 定义函数指针类型的别名
typedef int (*GetTraceFileHandle)(char *filename);
typedef int (*InitTraceHandle)(char *filename);
typedef int (*LogMsgHandle)(char *logfile, int level, const char *filename, int line, const char *fmt, ...);
typedef int (*LogMsgHexHandle)(char *logfile, int level, const char *filename, int line, unsigned char *data, int len, char *info);

// 声明调用函数指针变量
extern GetTraceFileHandle GetTraceFileFunc;
extern InitTraceHandle InitTraceFunc;
extern LogMsgHandle LogMsgFunc;
extern LogMsgHexHandle LogMsgHexFunc;


// 初始化bst动态库
int initTraceDynamicHandle();
void closeTraceDynamicHandle();



#endif