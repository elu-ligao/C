#ifndef _AM_GRAPH_H_
#define _AM_GRAPH_H_
#include "gcommon.h"

/**
 * @brief 图的邻接矩阵表示
 *      Adjacency Matrix
 * 
 *          locateVex: 待改进
 */

#define MAX_VEX_NUM 20
#define VEX_NAME_LEN 7

typedef char* VerTexType;

typedef int ArcType;

typedef struct AMGraph  // 邻接矩阵表示
{
    VerTexType  *vexs;    // 存放顶点数组
    ArcType     *arcs;     // 存放边权重数组
    int     vexnum, arcnum;     // 当前图的顶点数和边数
    int     direction;      // 0 无向  1 有向
} AMGraph, *pAMGraph;

typedef struct ArcInfo  // 边信息
{
    VerTexType  v1, v2; // 边顶点
    ArcType     w;      // 边上权重
} ArcInfo, *pArcInfo;

typedef struct Closedge // 普里姆算法辅助结构体
{
    int adjvex;         // 顶点在图中vexs数组中的序号
    ArcType lowcost;    // 以该顶点相连的边的最小权重
}Closedge, *pClosedge;


typedef struct CCINFO
{
    int numCC;  // 连通分量号
    int num;    // 包含的顶点数
} CCINFO;


VerTexType *newVerTex(int n);   // 申请n个VerTexType 类型的数组来存放顶点信息
ArcType *newArc(int vexnum, int direction);     // 申请一块空间存放边的权重信息， 即邻接矩阵
pAMGraph initAMGraph();
pAMGraph createAMGraph(VerTexType *vexs, pArcInfo arcinfo, int vexnum, int arcnum, int direction);   // 根据顶点和弧信息构造图
void destroyAMGraph(pAMGraph *G);   // 销毁图
int locateVex(pAMGraph G, VerTexType u);        // 在G中查找顶点u, 并返回u在顶点数组中的索引
VerTexType getVex(pAMGraph G, int v);           // 在G中返回索引为v的顶点的信息
int putVex(pAMGraph G, int v, VerTexType val);  // 对索引v的顶点赋值val
ArcType getArc(pAMGraph G, int i, int j);           // 在G中返回邻接矩阵arc[i][j]的值
int putArc(pAMGraph G, int i, int j, ArcType val);  // 对邻接矩阵arc[i][j]赋值val
int firstAdjVex(pAMGraph G, int v);             // 返回v的第一个邻接顶点索引
int nextAdjVex(pAMGraph G, int v, int w);       // 返回v的相对于w的下一个邻接顶点索引
// int insertVex(pAMGraph G, VerTexType u);        // 新增顶点u
// int deleteVex(pAMGraph G, VerTexType u);        // 删除顶点u
// int insertArc(pAMGraph G, VerTexType u1, VerTexType u2, ArcType w); // 新增弧信息
// int deleteArc(pAMGraph G, VerTexType u1, VerTexType u2);            // 删除弧信息
void showArcs(pAMGraph G);
void DFSAMGraph(pAMGraph G, int v);  
void BFSAMGraph(pAMGraph G, int v); 

/**
 * @descripttion:   拓扑排序
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} inverted    0 拓扑正序  1 拓扑逆序
 */
int *topoSortAMGraph(pAMGraph G, int inverted);        // 拓扑排序 以入度为0的顶点开始, 有向图中要有入度为0的顶点
int *topoSortAMG(pAMGraph, int inverted);   // 拓扑排序 任意有向图

/**
 * @brief   最小生成树： 联通网中所有生成树中各边的代价之和最小的那颗生成树
 *              针对的是无向图
 * @param G 
 * @param u 
 */
void miniSpanTree_Prim(pAMGraph G, VerTexType u);   // 最小生成树 普里姆算法
void miniSpanTree_Kruskal(pAMGraph G);      // 最小生成树 克鲁斯卡尔算法

/**
 * @brief   最短路径
 *              1. 从某个源点到其余各个顶点的最短路径   狄杰斯特拉算法 Dijkstra  
 *              2. 每一对顶点之间的最短路径     弗洛伊德算法 Floyd
 * @param G 
 * @param u 
 * @return int **   
 *              1.  狄杰斯特拉算法返回 int 型的二维数组，第一维为长度2，第二维长度为G.vexnum+1， 
 *                  a[0]为u到各顶点的路径长度，a[1]为各顶点的前驱顶点
 *              2.  弗洛伊德算法返回 int 型的二维数组，第一维长度为 2*G.vexnum+1，第二维长度为G.vexnum+1，
 *                  其中 a[i][j] (1<=i<=n) 为i-j的路径长度
 *                       a[i][j] (n+1<=i<=2*n) 为j的前驱顶点
 */
int **shortestPath_Dijkstra(pAMGraph G, VerTexType u);   // 最短路径 狄杰斯特拉算法
int **shortestPath_Flyod(pAMGraph G);     // 最短路径 弗洛伊德算法

/**
 * @brief   找到AOE-网的关键路径
 *          1. 拓扑排序，确定每个时间的最早发生时间
 *          2. 按拓扑逆序求出每个时间的最迟发生时间
 *          3. 求出每个活动的最早和最晚开始时间
 *          4. 找出e(i) = l(i) 的活动ai， 即为关键活动，由关键活动形成的由源点到汇点的每一天路径都是关键路径
 * 
 * @param G 
 * @return int 
 */
int criticalPath(pAMGraph G);   // 关键路径算法

/**
 * @brief   连通分量
 *          1. UCC 无向图连通分量
 * 
 * @param G 
 * @return int* 
 *          1. ucc无向图连通分量： a[0] 连通分量个数, a[1-n]各个顶点所在的连通分量号
 */
int *UCC(pAMGraph G);   // 无向图连通分量
int *SCCKosaraju(pAMGraph G);   // 有向图强连通分量
CCINFO *detailCC(int *cc, int n);   // 各连通分量顶点个数


void freeVexs(VerTexType *vexs, int n);

#endif