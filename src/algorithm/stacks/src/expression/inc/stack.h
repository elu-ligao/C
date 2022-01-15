/*
 * @Author       : elu
 * @Date         : 2022-01-13 15:54:58
 * @LastEditTime : 2022-01-13 22:18:59
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _STACK_H
#define _STACK_H

#define EmptyTOS    ( -1 )
#define MinStackSize    ( 20 )
#define DATA_LEN    ( 3 )

struct ElementType  // 元素域结点
{
    union
    {
        char sdata[DATA_LEN+1];
        float data;
    };
    struct ElementType *left;
    struct ElementType *right;
};    

typedef struct ElementType ElemType;
typedef ElemType *PtrElem;


struct StackRecord  // 结构体结点
{
    int Capacity;   // 容量
    int TopOfStack; // 栈顶元素在数组中的下标
    PtrElem Array;  // 元素域数组
};
typedef struct StackRecord *Stack;  // 栈指针

int IsEmpty(Stack S);   // 判断栈是否为空
int IsFull(Stack S);    // 判断栈是否已满
Stack CreateStack(int MaxElements);    // 创建栈
void DisposeStack(Stack S);     // 销毁栈
void MakeEmpty(Stack S);        // 清空栈
int Push(PtrElem X, Stack S);  // 元素 X 入栈
PtrElem Top(Stack S);   // 返回栈顶元素 
void Pop(Stack S);   // 将栈顶元素出栈
PtrElem TopAndPop(Stack S); // 将栈顶元素出栈并返回其指针 



#endif