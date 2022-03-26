/*
 * @Author       : elu
 * @Date         : 2021-12-23 13:17:03
 * @LastEditTime : 2022-03-03 13:29:22
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _EDGE_H
#define _EDGE_H

typedef struct Edge     // 克鲁斯卡尔算法辅助结构体
{
    int head, tail;     // 边的两个顶点
    int lowcost;        // 边上的权
} Edge, *pEdge;

#endif