/*
 * @Author       : elu
 * @Date         : 2022-01-13 21:59:49
 * @LastEditTime : 2022-01-14 23:58:19
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/expression.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Usage: %s expression\n", argv[0]);
        return -1;
    }
    int N = 0;
    Stack S = CreateStack(100);
    PtrExp ExpArr = AnalysisExp(argv[1], &N);

    float result = GetPostfixExpressionValue(S, ExpArr, N);
    printf("%s = %.2f\n", argv[1], result);

    char *sPost;
    PtrElem ExpTree = GetPostfixExpressionTree(S, ExpArr, N);
    sPost = TraversePostRoot(ExpTree);
    printf("sPost = %s\n", sPost);
    DisposeStack(S);

    //  初始化优先级
    char **sPriorityArr = initPriorityArr();
    if(!sPriorityArr)   return -1;

    char *MidSeq = GetMidSeq(ExpTree, sPriorityArr, 0);
    printf("MidSeq = %s\n", MidSeq);
    TraverseMidRoot(ExpTree);
    printf("\n");
    FreeExpTree(&ExpTree);

    N = 0;
    ExpArr = AnalysisExp(MidSeq, &N);
    ExpTree = GetMidfixExpressionTree(sPriorityArr, ExpArr, N);
    // TraversePostRoot2(ExpTree);
    // printf("\n");
    sPost = TraversePostRoot(ExpTree);
    printf("sPost = %s\n", sPost);
    
    char *sPre = TraversePreRoot(ExpTree);
    printf("sPret = %s\n", sPre);
    FreeExpTree(&ExpTree);
    
    int k = 0;
    N = 0;
    ExpArr = AnalysisExp(sPre, &N);
    ExpTree = GetPrefixExpressionTree(ExpArr, N, &k);
    TraversePostRoot2(ExpTree);
    printf("\n");
    printf("%.2f\n", EvaluateExpTree(ExpTree));
    FreeExpTree(&ExpTree);

    freePriority(sPriorityArr);

    return 0;
}