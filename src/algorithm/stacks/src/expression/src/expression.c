/*
 * @Author       : elu
 * @Date         : 2022-01-13 16:15:14
 * @LastEditTime : 2022-01-14 23:59:59
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "../inc/expression.h"

/**
 * @descripttion:   申请 MaxExpSize 个空间的Express 结构体数组
 * @return {*}
 * @param {int} MaxExpSize
 */
PtrExp GenExpArray(int MaxExpSize)
{
    if(MaxExpSize < MIN_EXPRESS_LEN)    MaxExpSize = MIN_EXPRESS_LEN;
    PtrExp ExpArr = (PtrExp)calloc(MaxExpSize, sizeof(Express));
    if(NULL == ExpArr)
    {
        perror("callloc for ExpArr error.\n");
        return NULL;
    }
    return ExpArr;
}

/**
 * @descripttion:   将exp表达式进行格式化，以空格分开运算数与运算符
 * @return {*}
 * @param {char} *exp
 */
char *FormatExp(char *exp)
{
    int i = 0, n = strlen(exp);
    char *s = (char *)calloc(1024, sizeof(char));
    int inOperand = 0;  // 运算数
    int k = 0;
    for(i=0; i<n; ++i)
    {
        if(isblank(exp[i])) // 是空格
        {
            inOperand = 0;  // 表明是运算数标志结束
            continue;
        }
        else if(isalnum(exp[i]) || '.' == exp[i])    // 是运算数或者是小数点
        {
            if(!inOperand)  // 前面不是运算数，这个就是运算数的第一位，加空格与前面的分开
            {
                inOperand = 1;
                s[k++] = ' ';
            }
            s[k++] = exp[i];
        }
        else    // 是元算符 
        {
            inOperand = 0;
            s[k++] = ' ';   // 加空格与前面的分开
            s[k++] = exp[i];
        }
    }
    if(s[0] == ' ') return s+1; // 是返回的首位不是空格(exp第一个为运算数时是这种情况)
    return s;
}

/**
 * @descripttion:   对表达式exp进行解析，并存放于ExpArr数组
 * @return {*}      *N: ExpArr 元素个数
 * @param {char} *exp
 */
PtrExp AnalysisExp(char *exp, int *N)
{
    PtrExp ExpArr = GenExpArray(100);
    if(NULL == ExpArr)  return NULL;

    char *Sexp = FormatExp(exp);
    printf("Sexp = [%s]\n", Sexp);

    int k = 0;
    char *operators = "()+-*/^";    // 运算符
    char *str, *token, *saveptr;
    for(str=Sexp, k=0; ; str=NULL, ++k)
    {
        token = strtok_r(str, " ", &saveptr);
        if(NULL == token)   break;

        strcpy(ExpArr[k].data, token);
        if(strstr(operators, token))    // 是运算符
            ExpArr[k].type = '0';
        else    // 运算数 
            ExpArr[k].type = '1';
    }
    *N = k;
    return ExpArr;
}

/**
 * @descripttion:   以root为根，left,right分别为左右子树建立表达式二叉树
 * @return {*}
 * @param {PtrElem} root
 * @param {PtrElem} left
 * @param {PtrElem} right
 */
PtrElem CreateExpressionTree(PtrElem root, PtrElem left, PtrElem right)
{
    root->left = left;
    root->right = right;
    return root;
}


PtrExp GetNext(PtrExp ExpArr, int k)
{
    return &ExpArr[k];
}

float Operate(float a, float b, char opr)
{
    switch (opr)
    {
    case '+':
        return a+b;
        break;
    case '-':
        return a-b;
        break;
    case '*':
        return a*b;
        break;
    case '/':
        return a/b;
        break;
    case '^':
        return powf(a, b);
        break;
    
    default:
        break;
    }
    return 0;
}

/**
 * @descripttion:   获取后缀表达式exp的值
 * @return {*}
 * @param {Stack} S
 * @param {PtrExp} ExpArr
 * @param {int} N
 */
float GetPostfixExpressionValue(Stack S, PtrExp ExpArr, int N)
{
    PtrExp ExpElem;
    PtrElem PElement, a, b;
    int k = 0;
    while(k < N)
    {
        PElement = (PtrElem)calloc(1, sizeof(ElemType));
        ExpElem = GetNext(ExpArr, k++);
        if(ExpElem->type == '1')    // 是运算数
        {
            PElement->data = atof(ExpElem->data);
            Push(PElement, S);
        }
        else    // 是运算符
        {
            b = TopAndPop(S);
            a = TopAndPop(S);
            PElement->data = Operate(a->data, b->data, ExpElem->data[0]);
            Push(PElement, S);
        }
    }
    return TopAndPop(S)->data;
}

/**
 * @descripttion:   获取后缀表达式exp的表达式树
 * @return {*}
 * @param {Stack} S
 * @param {PtrExp} ExpArr   元素个数为N的Express 数组
 * @param {int} N
 */
PtrElem GetPostfixExpressionTree(Stack S, PtrExp ExpArr, int N)
{
    PtrExp ExpElem;
    PtrElem PElement, a, b;
    int k = 0;
    while(k < N)
    {
        PElement = (PtrElem)calloc(1, sizeof(ElemType));
        ExpElem = GetNext(ExpArr, k++);
        if(ExpElem->type == '1')    // 是运算数
        {
            PElement->data = atof(ExpElem->data);
            Push(PElement, S);
        }
        else    // 是运算符
        {
            b = TopAndPop(S);
            a = TopAndPop(S);
            PElement->sdata[0] = ExpElem->data[0];
            PElement = CreateExpressionTree(PElement, a, b);
            Push(PElement, S);
        }
    }
    return TopAndPop(S);
}

/**
 * @descripttion:   后根遍历表达式树
 * @return {*}
 * @param {PtrElem} S
 */
void TraversePostRoot2(PtrElem S)
{
    if(NULL == S)   return;
    TraversePostRoot2(S->left);
    TraversePostRoot2(S->right);
    if(S->left)    printf("%s ", S->sdata);     // 符号位是非叶子结点
    else    printf("%.2f ", S->data);           // 元算数是叶子结点
}

char *TraversePostRoot(PtrElem S)
{
    char *left, *right, *s;
    if(NULL == S)   return NULL;
    left = TraversePostRoot(S->left);
    right = TraversePostRoot(S->right);
    if(S->left)
    {
        s = (char *)calloc(strlen(left)+strlen(right)+2+1+1, sizeof(char));   // 2 for blank, 1 for opr, 1 for '\0'
        sprintf(s, "%s %s %s", left, right, S->sdata);     // 符号位是非叶子结点
    }
    else
    {    
        s = (char *)calloc(LEN_EXPRESS_DATA+1, sizeof(char));
        sprintf(s, "%.2f", S->data);           // 元算数是叶子结点
    }
    return s;
}

/**
 * @descripttion:   获取中序遍历的带括号的表达式序列
 * @return {*}
 * @param {pBiTree} T
 * @param {char} *          sPriorityArr 优先级数组
 * @param {int} brackets    是否要加括号
 */
char *GetMidSeq(PtrElem T, char **sPriorityArr, int brackets)
{
    
    if(NULL == T->left && NULL == T->right)  // 叶子结点必是运算数
    {
        char *s = (char *)calloc(LEN_EXPRESS_DATA+1, sizeof(char));
        sprintf(s, "%.2f", T->data);
        return s;
    }
    else    // 非叶子结点必为运算符 
    {
        // printf("T.sdata = %s, T->left->sdata = %s\n", T->sdata, T->left->sdata);
        char *left, *right;
        // 左子树根结点是运算符, 比较子树的运算符与当前根节点的运算符优先级，如果子树运算符优先级低，表明要加括号
        if(T->left && T->left->left && checkPriority(sPriorityArr, T->left->sdata[0], T->sdata[0]) == '<')
            left = GetMidSeq(T->left, sPriorityArr, 1);  // 递归左子树求值
        else
            left = GetMidSeq(T->left, sPriorityArr, 0);

        // 右子树根结点是运算符, 比较子树的运算符与当前根节点的运算符优先级，如果子树运算符优先级低，表明要加括号
        if(T->right && T->right->left && checkPriority(sPriorityArr, T->right->sdata[0], T->sdata[0]) == '<')
            right = GetMidSeq(T->right, sPriorityArr, 1); // 递归右子树求值
        else
            right = GetMidSeq(T->right, sPriorityArr, 0); 
            
        char *s = (char*)calloc(brackets*2+1+strlen(left)+strlen(right), sizeof(char));
        if(brackets)    // 要加括号
            sprintf(s, "(%s%c%s)", left, T->sdata[0], right);
        else    // 不加括号
            sprintf(s, "%s%c%s", left, T->sdata[0], right);
        // printf("s = %s\n", s);
        return s;
    }
}

/**
 * @descripttion:   比较运算符 pre 与 post 的优先级
 * @return {*}
 * @param {char} *
 * @param {char} pre
 * @param {char} post
 */
char checkPriority(char **sPriorityArr, char pre, char post)
{
    int row = getIndex(pre);
    int col = getIndex(post);
    // printf("checkPriority -->  %c\n", sPriorityArr[row][col]);
    return sPriorityArr[row][col];
}


/**
 * @descripttion:   初始化运算符的优先级数组 
            a\b  +   -   *   /   ^   (   )   #
 *          +    >   >   <   <   <   <   >   > 
 *          -    >   >   <   <   <   <   >   >
 *          *    >   >   >   >   <   <   >   >
 *          /    >   >   >   >   <   <   >   >
 *          ^    >   >   >   >   >   <   >   >
 *          (    <   <   <   <   <   <   =   x
 *          )    >   >   >   >   >   x   >   >
 *          #    <   <   <   <   <   <   x   =
 * @return {*}
 * @param {char} *sPriorityArr
 */
char **initPriorityArr()
{
    char **sPriorityArr = (char **)calloc(OPERATOR_TYPE, sizeof(char *));
    if(!sPriorityArr)   return NULL;

    int i = 0;
    for(i=0; i<OPERATOR_TYPE; ++i)
    {
        sPriorityArr[i] = (char*)calloc(OPERATOR_TYPE+1, sizeof(char));
        if(!sPriorityArr[i])    return NULL;
    }

    strcpy(sPriorityArr[0], ">><<<<>>");    // +
    strcpy(sPriorityArr[1], ">><<<<>>");    // -
    strcpy(sPriorityArr[2], ">>>><<>>");    // *
    strcpy(sPriorityArr[3], ">>>><<>>");    // /
    strcpy(sPriorityArr[4], ">>>>><>>");    // ^
    strcpy(sPriorityArr[5], "<<<<<<=x");    // (
    strcpy(sPriorityArr[6], ">>>>>x>>");    // )
    strcpy(sPriorityArr[7], "<<<<<<x=");    // #
    return sPriorityArr;
}

/**
 * @descripttion:   释放sPriorityArr
 * @return {*}
 * @param {char} *
 */
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


/**
 * @descripttion:   获取运算符在sPriorityArr数组中的行列索引
 * @return {*}
 * @param {char} expression
 */
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
    case '^':
        index = 4;
        break;
    case '(':
        index = 5;
        break;
    case ')':
        index = 6;
        break;
    case '#':
        index = 7;
        break;
    
    default:
        // x 理应不应该出现这种匹配
        break;
    }
    return index;
}

void FreeExpTree(PtrElem *T)
{
    if(!*T) return;

    FreeExpTree(&(*T)->left);
    FreeExpTree(&(*T)->right);

    free(*T);
    *T = NULL;
}

/**
 * @descripttion:   二叉树中序遍历
 * @return {*}
 * @param {PtrElem} T
 */
void TraverseMidRoot(PtrElem T)
{
    if(!T)  return;
    TraverseMidRoot(T->left);
    if(T->left)
        printf("%s", T->sdata);
    else 
        printf("%.2f", T->data);
    TraverseMidRoot(T->right);
}

/**
 * @descripttion:   获取中缀表达式exp的表达式树
 * @return {*}
 * @param {PtrExp} ExpArr
 * @param {int} N
 * @param {char} *exp
 */
PtrElem GetMidfixExpressionTree(char **sPriorityArr, PtrExp ExpArr, int N)
{
    Stack EXPT = CreateStack(10);   // 表达式树栈(存放已建立好的表达式树根结点)
    Stack OPTR = CreateStack(10);   // 运算符栈
    PtrExp ExpElem;
    PtrElem PElement, a, b;
    char opr;
    PElement = (PtrElem)calloc(1, sizeof(ElemType));
    PElement->sdata[0] = '#';   // 开始标志符
    Push(PElement, OPTR);   // 先将开始标志符压入栈
    ExpArr[N].data[0] = '#';    // 将结束标识符添加到ExpArr 最后
    ExpArr[N].type = '0';
    ++N;
    // int T = 0;
    int k = 0; 
    ExpElem = GetNext(ExpArr, k++);
    while(k < N || Top(OPTR)->sdata[0] != '#')
    {
        // printf("k = %d, N = %d, Top(OPTR)->sdata[0] = %s\n", k, N, Top(OPTR)->sdata);
        // printf("ExpElem->type = %c, ExpElem->data = %s\n", ExpElem->type, ExpElem->data);
        PElement = (PtrElem)calloc(1, sizeof(ElemType));
        if(ExpElem->type == '1')    // 是运算数
        {
            PElement->data = atof(ExpElem->data);
            Push(PElement, EXPT);
            ExpElem = GetNext(ExpArr, k++);         // 获取下一个元素
            // PElement->data = atof(ExpElem->data);
            // PElement = CreateExpressionTree(PElement, NULL, NULL);
            // Push(PElement, EXPT);
        }
        else    // 是运算符
        {
            switch (checkPriority(sPriorityArr, Top(OPTR)->sdata[0], ExpElem->data[0]))
            {
            case '<':
                PElement->sdata[0] = ExpElem->data[0];
                Push(PElement, OPTR);
                ExpElem = GetNext(ExpArr, k++);     // 获取下一个元素
                break;
            case '>':
                opr = TopAndPop(OPTR)->sdata[0];        // 弹出运算符栈顶的运算符opr
                b = TopAndPop(EXPT);                    // 弹出运算数栈顶的两个操作数
                a = TopAndPop(EXPT);
                PElement->sdata[0] = opr;
                PElement = CreateExpressionTree(PElement, a, b);
                Push(PElement, EXPT);   // 将 a opr b 构成的表达式树压入 EXPT 栈
                break;
            case '=':   // 左右括号匹配
                Pop(OPTR);  // 弹出栈顶的'('左括号
                ExpElem = GetNext(ExpArr, k++);     // 获取下一个元素
                break;            
            default:
                break;
            }
        }
        // printf("---- k = %d, Top(OPTR)->sdata[0] = %s\n", k, Top(OPTR)->sdata);
    }
    return TopAndPop(EXPT);
}

/**
 * @descripttion:   前根遍历
 * @return {*}
 * @param {PtrElem} S
 */
char *TraversePreRoot(PtrElem S)
{
    char *left, *right, s[LEN_EXPRESS_DATA+1+1] = {0};
    if(NULL == S)   return "";
    if(S->left)
        sprintf(s, "%s ", S->sdata);     // 符号位是非叶子结点
    else
        sprintf(s, "%.2f ", S->data);           // 元算数是叶子结点
        
    left = TraversePreRoot(S->left);
    right = TraversePreRoot(S->right);
    char *ss = (char *)calloc(strlen(left)+strlen(right)+2+strlen(s)+1, sizeof(char));   // 2 for blank, 1 for opr, 1 for '\0'
    sprintf(ss, "%s%s%s", s, left, right);  
    return ss;
}

/**
 * @descripttion:   计算表达式树T的值
 * @return {*}
 * @param {PtrElem} T
 */
float EvaluateExpTree(PtrElem T)
{
    float left, right;
    if(NULL == T->left && NULL == T->right)
        return T->data;
    
    left = EvaluateExpTree(T->left);
    right = EvaluateExpTree(T->right);
    return Operate(left, right, T->sdata[0]);
}

/**
 * @descripttion:   获取前缀表达式exp的表达式树
 * @return {*}
 * @param {char} *
 * @param {PtrExp} ExpArr
 * @param {int} N
 */
PtrElem GetPrefixExpressionTree(PtrExp ExpArr, int N, int *k)
{
    if(*k == N)  return NULL;
    PtrExp ExpElem;
    PtrElem PElement = (PtrElem)calloc(1, sizeof(ElemType));
    
    ExpElem = GetNext(ExpArr, (*k)++);
    if(ExpElem->type == '1')    // 
    {
        PElement->data = atof(ExpElem->data);
        // printf("%.2f\n", PElement->data);
    }
    else if(ExpElem->type == '0')    // 是运算符 才有子树
    {
        PElement->sdata[0] = ExpElem->data[0];
        // printf("%s\n", PElement->sdata);
        PElement->left = GetPrefixExpressionTree(ExpArr, N, k);
        PElement->right = GetPrefixExpressionTree(ExpArr, N, k);
    }    
    return PElement;
}