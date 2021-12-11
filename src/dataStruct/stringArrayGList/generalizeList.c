#include <stdio.h>
#include <stdlib.h>
#include "generalizeList.h"

GList newGList(int tag)
{
    GList L = (GList)calloc(1, sizeof(GLNode));
    if(!L)
    {
        perror("calloc for GList Node error.\n");
        return NULL;
    }
    L->tag = tag;
    if(tag == ATOM)
        L->atom = (char *)calloc(20, sizeof(char));
    return L;
}

GList initGList()
{
    return NULL;
}


GList getHead(GList L)
{
    if(L->ptr.head)     // 表头存在
        return L->ptr.head;
    return NULL;        // 否则为空表，按定义无表头表尾
}


GList getTail(GList L)
{
    if(!L->ptr.head)    return NULL;    // 空表，按定义无表头表尾
    if(!L->ptr.tail)    return newGList(1);    // 表尾为空，返回空表
    else if(L->ptr.tail->tag == ATOM)   // 表尾为原子结点，套上外层表结点，取表尾返回的是一个广义表
    {
        GList p = newGList(1);
        if(!p)  return NULL;
        p->ptr.head = L->ptr.tail;
        return p;
    }
    return L->ptr.tail;     
}


GList insertGList(GList L, GList l)
{
    if(!L)
    {
        L = newGList(1);
    }
    if(L->tag == ATOM)  return NULL;
    GList p = L;
    while(p->ptr.tail)
        p = p->ptr.tail;
    p ->ptr.tail = l;
    return L;
}