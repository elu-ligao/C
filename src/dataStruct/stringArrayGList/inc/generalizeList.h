#ifndef _GRNERALIZE_LIST_H
#define _GRNERALIZE_LIST_H

typedef char* AtomType;

// 头尾链表的存储结构
typedef enum
{
    ATOM,   // ATOM = 0 原子结点
    LIST    // LIST = 1 广义表
} ElemTag;

typedef struct GLNode
{
    ElemTag tag;
    union 
    {
        AtomType atom;
        struct 
        {
            struct GLNode *head, *tail;
        }ptr;
    };
} GLNode, *GList;

GList newGList(int tag);
// 初始化，head=tail=NULL
GList initGList();
// 将l插入到L中接到最后一个tail后面
GList insertGList(GList L, GList l); 
// 取表头，取出的表头为非空广义表的第一个元素，可以是单元子，也可以是一个子表
GList getHead(GList L);
// 取表尾，取出的表尾为出去表头之外由其余元素构成的表，表尾一定是一个字表，即使为空表
GList getTail(GList L);

#endif