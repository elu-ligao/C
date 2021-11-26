#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#include "testSqStack.h"
#include "stackSequence_dynamic.h"

#define OPERATOR_TYPE    6



void evaluateExpressionSq(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s expression.\n", argv[0]);
        return;
    }
    char *sExpression = initExpression(argv[1]);

    // printf("The expression is: \"%s\"\n", sExpression);

    char **sPriorityArr = (char **)calloc(OPERATOR_TYPE, sizeof(char *));
    if(!sPriorityArr)   return;
    int i = 0;
    for(i=0; i<OPERATOR_TYPE; ++i)
    {
        sPriorityArr[i] = (char*)calloc(OPERATOR_TYPE+1, sizeof(char));
        if(!sPriorityArr[i])    return;
    }
    initPriorityArr(sPriorityArr);

    evaluateExpression(sPriorityArr, sExpression);

    return;
}


char *initExpression(char *sExpression)
{
    if(!sExpression)    return NULL;
    int i, j;
    int sLen = strlen(sExpression);

    // 先去空格
    for(i=0, j=0; i<sLen; ++i)
    {
        if(sExpression[i] != ' ' && sExpression[i] != '\t')
        {
            sExpression[j++] = sExpression[i];
        }
    }
    sExpression[j] = 0;
    sLen = strlen(sExpression);     // 重新计算sLen
    // 在分割开数字个字符
    char *format = (char*)calloc(2*sLen+1, sizeof(char));
    if(!format)
    {
        perror("calloc error for deal with format sExpression.");
        return NULL;
    }
    int predigit = isdigit(sExpression[0]);
    int postdigit = 0;    
    format[0] = sExpression[0];
    for(i=1, j=1; i<sLen; ++i)
    {
        postdigit = isdigit(sExpression[i]);
        if(!(predigit && postdigit))   // 不都为数字
        {
            format[j++] = ' ';
        }
        format[j++] = sExpression[i];
        predigit = postdigit;
    }

    return format;
}


#if 1

/**
 * @brief 
 *          a\b  +   -   *   /   (   )
 *          +    >   >   <   <   <   >    
 *          -    >   >   <   <   <   >
 *          *    >   >   >   >   <   >
 *          /    >   >   >   >   <   >
 *          (    <   <   <   <   <   =
 *          )    >   >   >   >   x   >
 * 
 * @param sPriorityArr 
 * @param rows 
 * @param cols 
 * @return ** void 
 */
void initPriorityArr(char *sPriorityArr[])
{
    strcpy(sPriorityArr[0], ">><<<>");
    strcpy(sPriorityArr[1], ">><<<>");
    strcpy(sPriorityArr[2], ">>>><>");
    strcpy(sPriorityArr[3], ">>>><>");
    strcpy(sPriorityArr[4], "<<<<<=");
    strcpy(sPriorityArr[5], ">><<x>");
}


void freePriority(char **sPriorityArr)
{
    int i = 0;
    for(i=0; i<OPERATOR_TYPE; ++i)
    {
        free(sPriorityArr[i]);
        sPriorityArr[i] = NULL;
    }
    sPriorityArr = NULL;
}

char checkPriority(char **sPriorityArr, char pre, char post)
{
    int row = getIndex(pre);
    int col = getIndex(post);
    return sPriorityArr[row][col];
}



int getIndex(char expression)
{
    int index = -1;
    switch (expression)
    {
    case '+':
        index = 0;
        break;
    case '-':
        index = 1;
        break;
    case '*':
        index = 2;
        break;
    case '/':
        index = 3;
        break;
    case '(':
        index = 4;
        break;
    case ')':
        index = 5;
        break;
    
    default:
        break;
    }
    return index;
}

void evaluateExpression(char **sPriorityArr, char *sExpression)
{
    pSqStack OPTR = initStack();
    if(!OPTR)
    {
        perror("initStack OPTR error.\n");
        return;
    }
    pSqStack OPND = initStack();
    if(!OPND)
    {
        perror("initStack OPND error.\n");
        return;
    }

    int result = 0;
    if(doExpression(OPTR, OPND, sExpression, sPriorityArr, &result))
    {
        printf("result = %d\n", result);
    }

    destroyStack(&OPTR);
    destroyStack(&OPND);
    freePriority(sPriorityArr);
}


bool doExpression(pSqStack OPTR, pSqStack OPND, char *sExpression, char **sPriorityArr, int *rst)
{
    pElemType e = NULL;
    
    char top = 0;
    int a, b, result = 0;
    char *token, *saveptr, *str;
    str=sExpression;
    token = strtok_r(str, " ", &saveptr);
    if(!token)  return false;
    int getnext = 0;
    int dataOver = 0;
    // while(token || dataOver)   
    while(1) 
    {
        if(dataOver)    // 数据读取完毕则不再读取数据
        {
            getnext = 0;
            token = ")";    // 有口号作为待比较对象，因为其优先级最低，用于处理读取数据完毕后栈内剩余运算
        }
        else    // 否则要读取数据
            getnext = 1;
        // printf("token == [%s]\n", token);
        e = newElement();
        if(!e)  return false;

        if(strlen(token)==1)    // 判断数字和运算符
        {
            if(!isdigit(token[0]))  // 运算符 分3种情况讨论
            {
                if(stackEmpty(OPTR))    // 运算符栈为空分两种情况，
                {
                    if(!dataOver)       // 首次处理时为空栈，将运算符压入栈
                    {
                        e->sVal[0] = token[0];
                        pushFunc(OPTR, e);
                    }
                    else    // 最后处理时是由于弹出栈后把运算符处理完毕了， 故退出循环
                        break;
                }
                else
                {
                    
                    top = getTopFunc(OPTR)->sVal[0];    // OPTR 栈顶元素top
               
                    switch (checkPriority(sPriorityArr, top, token[0]))
                    {
                    case '<':           // 优先级更低，将当前运算符继续压入OPTR栈
                        e->sVal[0] = token[0];
                        pushFunc(OPTR, e);
                        break;
                    case '>':           // 优先级更高，弹出
                        popFunc(OPTR);
                        b = popFunc(OPND)->iVal;
                        a = popFunc(OPND)->iVal;
                        result = operator(a, top, b);
                        e->iVal = result;
                        pushFunc(OPND, e);

                        getnext = 0;
                        break;
                    case '=':
                        popFunc(OPTR);  // 匹配了左右括号,弹出左括号
                        break;
                    default:
                        // x is not allowed
                        break;
                    }
                }
            }
            else
            {
                e->iVal = atoi(token);
                pushFunc(OPND, e);
            }
        }
        else    // 肯定是运算数
        {
            e->iVal = atoi(token);
            pushFunc(OPND, e);
        }

        if(getnext)
        {
            str = NULL;
            token = strtok_r(str, " ", &saveptr);
            if(!token)
            {
                dataOver = 1;
                // break;
            }

        }

        free(e);
        e = NULL;
        
        // stackTraverseFunc(OPTR);
        // printf("OPTR-------------OPND\n");
        // stackTraverseFunc(OPND);
    }

    // printf("result = %d\n", result);
    *rst = result;
    return true;
}

int operator(int a, char express, int b)
{
    switch (express)
    {
    case '+':
        return a+b;
        break;
    case '-':
        return a-b;
    case '*':
        return a*b;
    case '/':
        return a/b;
    default:
        return 0;
        break;
    }
}


#endif