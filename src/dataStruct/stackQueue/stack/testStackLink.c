#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testStackLink.h"
#include "stackLink_dynamic.h"


int main(int argc, char *argv[])
{
    int rc = 0;
    rc = initStackLinkHandle();
    if(rc)
    {
        fprintf( stderr, "[%s](%d) initStackLinkHandle get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }
    
    // 进制转换
    // testConvertBase(argc, argv);

    // 括号匹配
    // testBrackets(argc, argv);

    // 表达式求值
    evaluateExpressionLink(argc, argv);
    
    // printf("S = [%p]\n", S);

    closeStackLinkHandle();

    return EXIT_SUCCESS;
}