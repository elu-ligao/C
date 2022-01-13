/*
 * @Author       : elu
 * @Date         : 2022-01-08 22:01:38
 * @LastEditTime : 2022-01-09 22:33:55
 * @LastEditors  : Please set LastEditors
 * @Description  : 栈的链表实现
 */
#ifndef _STACK_LINK_H
#define _STACK_LINK_H

#define ELEMENT_LEN    ( 3 )

struct ElementType  // 元素域结点
{
    union 
    {
        char sdata[ELEMENT_LEN+1];
        int  idata;
    };
    
};
// struct ElementType;    // 元素域结点
typedef struct ElementType ElemType;
typedef ElemType *PtrElem;
// struct Node;    // 结构体结点

struct Node     // 结构体结点
{
    ElemType Element;
    struct Node *next;
};
typedef struct Node *PtrNode;   // 结构体结点指针
typedef PtrNode Stack;  // 栈类型指针

int IsEmpty(Stack S);   // 判断栈是否为空
Stack CreateStack();    // 创建栈
void DisposeStack(Stack S);     // 销毁栈
void MakeEmpty(Stack S);        // 清空栈
void Push(PtrElem X, Stack S);  // 元素 X 入栈
PtrElem Top(Stack S);   // 返回栈顶元素 
void Pop(Stack S);   // 将栈顶元素出栈



#endif