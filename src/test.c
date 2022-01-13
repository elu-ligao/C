/*
 * @Author       : elu
 * @Date         : 2022-01-07 10:58:43
 * @LastEditTime : 2022-01-07 10:59:46
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include "trace.h"


void initLog()
{
    InitTrace("test");
    GetTraceFile(NULL);
}

int main()
{
    // char s[20] = "12345678";
    // char *a = "abcd";
    // printf("%s\n", strcpy(s, a));

    int a = 5;
    char *p = NULL;
    printf("%d\n", p && a == 5);
    return 0;

    initLog();

    LogDebug("--------------------------------------------------------");
    float f = 332767.672e5f;
    float F = 3.32e10f;
    printf("f = %f\n", f);
    LogDebug("f = %f", f);
    LogDebugHex((unsigned char *)&f, sizeof(float), "f = ");
    int z = 2048;
    f += z;
    LogDebug("f + z[%d] = %f", f, z);
    LogDebugHex((unsigned char *)&f, sizeof(float), "f + z = ");

    int x = 0x123456;
    LogDebug("x = %d, sizeof(x) = %d", x, sizeof(x));
    LogDebugHex((unsigned char *)&x, sizeof(int), "x = ");
    LogDebug("F = %f, sizeof(F) = %d", F, sizeof(F));
    LogDebugHex((unsigned char *)&F, sizeof(int), "F = ");


    return 0;
}