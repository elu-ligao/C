#ifndef _EDGE_H
#define _EDGE_H

typedef struct Edge     // 克鲁斯卡尔算法辅助结构体
{
    int head, tail;     // 边的两个顶点
    int lowcost;        // 边上的权
} Edge, *pEdge;

#endif