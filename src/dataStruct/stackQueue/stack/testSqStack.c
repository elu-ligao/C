#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testSqStack.h"
#include "stackSequence_dynamic.h"


int main(int argc, char *argv[])
{
    int rc = 0;
    rc = initStackSequenceHandle();
    if(rc)
    {
        fprintf( stderr, "[%s](%d) initStackSequenceHandle get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }

    pSqStack S = initStackFunc();
    if(!S)
    {
        fprintf( stderr, "[%s](%d) initStackFunc get error: rc = %d\n", __FILE__, __LINE__, rc);
        return rc;
    }
    
    // 进制转换
    // testConvertBase(&S, argc, argv);

    // 括号匹配
    // testBrackets(S, argc, argv);

    // 表达式求值
    evaluateExpressionSq(argc, argv);
    
    stackTraverseFunc(S);
    destroyStackFunc(&S);
    // printf("S = [%p]\n", S);

    closeStackSequenceHandle();

    return EXIT_SUCCESS;
}