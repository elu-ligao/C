#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringList_dynamic.h"
#include "generalizeList.h"
#include "trace_dynamic.h"

void test_BF(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s S T [pos].\n", argv[0]);
        return;
    }

    char *S = argv[1];
    char *T = argv[2];
    int pos = 0;
    if(argc == 4)
    {
        pos = atoi(argv[3]);
    }

    int index = indexBFFunc(S, T, pos);
    if(index != -1)
    {
        printf("First metch index is %d.\n", index);
    }
    else
    {
        printf("\"%s\" not in \"%s\".\n", T, S);
    }
}


void test_KMP(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s S T [pos].\n", argv[0]);
        return;
    }

    char *S = argv[1];
    char *T = argv[2];
    int pos = 0;
    if(argc == 4)
    {
        pos = atoi(argv[3]);
    }

    int index = indexKMPFunc(S, T, pos);
    if(index != -1)
    {
        printf("First metch index is %d.\n", index);
    }
    else
    {
        printf("\"%s\" not in \"%s\".\n", T, S);
    }
}


void testGList()
{
    GList apple = newGList(0);
    strcpy(apple->atom, "apple");

    GList orange = newGList(0);
    strcpy(orange->atom, "orange");
    
    GList strawberry = newGList(0);
    strcpy(strawberry->atom, "strawberry");

    GList banana = newGList(0);
    strcpy(banana->atom, "banana");

    GList peach = newGList(0);
    strcpy(peach->atom, "peach");

    GList pear = newGList(0);
    strcpy(pear->atom, "pear");

    GList C = newGList(1);      // 联合体 只能对一种元素赋值，否则会被覆盖， 共用一片内存
    C->ptr.head = banana;

    GList listStrawberry = newGList(1);
    listStrawberry->ptr.head = strawberry;
    listStrawberry->ptr.tail = C;

    GList B = newGList(1);
    B->ptr.head = listStrawberry;
    B->ptr.tail = peach;

    GList listOrange = newGList(1);
    listOrange->ptr.head = orange;
    listOrange->ptr.tail = B;

    GList A = newGList(1);
    A->ptr.head = listOrange;
    A->ptr.tail = pear;

    GList L = newGList(1);
    L->ptr.head = apple;
    L->ptr.tail = A;


    InitTrace("test");
    GetTraceFile(NULL);


    GList b = getHead(getTail(getHead(getTail(getHead(getTail(L))))));
    printf("b.tag = %d, b.atom = %s, sizeof(int) = %ld, sizeof(b->ptr.head) = %ld, sizeof(b->ptr) = %ld\n", b->tag, b->atom, sizeof(int), sizeof(b->ptr.head), sizeof(b->ptr));
    LogDebugHex((unsigned char *)b, sizeof(GLNode), "banana");
    LogDebugHex((unsigned char *)&(b->atom), sizeof(b->atom), "banana.atom");
    LogDebugHex((unsigned char *)&(b->ptr.head), sizeof(b->ptr.head), "banana.ptr.head");
    b = getTail(L);
    LogDebugHex((unsigned char *)b, sizeof(GLNode), "L.tail");
    printf("b.tag = %d, b.atom = %s\n", b->tag, b->atom);
    b = getTail(b);
    LogDebugHex((unsigned char *)b, sizeof(GLNode), "L.tail.tail");
    printf("b.tag = %d, b.atom = %s\n", b->tag, b->atom);
    b = getHead(b);
    LogDebugHex((unsigned char *)b, sizeof(GLNode), "L.tail.head");
    printf("b.tag = %d, b.atom = %s\n", b->tag, b->atom);
    
}

int main(int argc, char *argv[])
{
    // initStringListHandle();

    // test_BF(argc, argv);

    // test_KMP(argc, argv);

    testGList();

    // closeStringListHandle();

    return 0;
}
