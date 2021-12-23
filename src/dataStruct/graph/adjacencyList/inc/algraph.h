#ifndef _AL_GRAPH_H_
#define _AL_GRAPH_H_
// #include "gcommon.h"
#include "vexavl.h"

/**
 * @brief 图的邻接列表表示
 *      Adjacency List
 * 
 *          locateVex: 待改进
 */

#define MAX_VERTEX_NUM  1000000
#define VEX_NAME_LEN    7

typedef char *VerTexType;

typedef struct OtherInfo
{
    int weight;
} OtherInfo, *pOtherInfo;

typedef struct ArcNode
{
    int adjvex;     // 该边指向顶点的位置
    struct ArcNode *nextarc;
    pOtherInfo info; // 其他信息
} ArcNode, *pArcNode;

typedef struct VNode
{
    VerTexType data;    // 顶点
    pArcNode firstarc;
} VNode, *pAdjList;

typedef struct ALGraph  // 图的邻接列表表示
{
    pAdjList *vertices;  // 顶点信息
    int vexnum, arcnum; // 顶点数，边数
    int maxvexs;   // 能容纳总的顶点数
} ALGraph, *pALGraph;  


typedef struct Closedge // 普里姆算法辅助结构体
{
    int adjvex;         // 顶点在图中vexs数组中的序号
    OtherInfo lowcost;    // 以该顶点相连的边的最小权重
}Closedge, *pClosedge;


typedef struct CCINFO
{
    int numCC;  // 连通分量号
    int num;    // 包含的顶点数
} CCINFO;


pALGraph initALGraph();
pAdjList newVerTex0();   // 申请1个 VNode 空间来存放顶点信息
pArcNode newArc();     // 申请一块 ArcNode 空间存放边的权重信息， 即邻接列表
int insertArc(pavlTree *vexAvl, pALGraph G, VerTexType u1, VerTexType u2, pOtherInfo w); // 新增弧信息
int insertVex(pavlTree *vexAvl, pALGraph G, VerTexType u);        // 新增顶点u
int locateVex(pavlTree vexAvl, pALGraph G, VerTexType u);        // 在G中查找顶点u, 并返回u在顶点数组中的索引
pALGraph resizeVertices(pALGraph G);    
void destroyALGraph(pALGraph *G);   // 销毁图
VerTexType getVex(pALGraph G, int v);           // 在G中返回索引为v的顶点的信息
int putVex(pALGraph G, int v, VerTexType val);  // 对索引v的顶点赋值val
pOtherInfo getArc(pALGraph G, int i, int j);           // 在G中返回邻接列表arc[i][j]的值
int putArc(pALGraph G, int i, int j, pOtherInfo w);  // 对邻接列表arc[i][j]赋值val
int firstAdjVex(pALGraph G, int v);             // 返回v的第一个邻接顶点索引
int nextAdjVex(pALGraph G, int v, int w);       // 返回v的相对于w的下一个邻接顶点索引

// int deleteVex(pALGraph G, VerTexType u);        // 删除顶点u
// int deleteArc(pALGraph G, VerTexType u1, VerTexType u2);            // 删除弧信息
void showArcs(pALGraph G);
void DFSALGraph(pALGraph G, int v);  
void BFSALGraph(pALGraph G, int v); 

/**
 * @descripttion:   拓扑排序
 * @return {*}
 * @param {pALGraph} G
 * @param {int} inverted    0 拓扑正序  1 拓扑逆序
 */
int *topoSortALGraph(pALGraph G);        // 拓扑排序 以入度为0的顶点开始, 有向图中要有入度为0的顶点
int *topoSortALG(pALGraph G, int inverted);   // 拓扑排序 任意有向图
void showTopo(pALGraph G, int *topo);
/**
 * @brief   最小生成树： 联通网中所有生成树中各边的代价之和最小的那颗生成树
 *              针对的是无向图
 * @param G 
 * @param u 
 */
void miniSpanTree_Prim(pALGraph G, int i);   // 最小生成树 普里姆算法
void miniSpanTree_Kruskal(pALGraph G);      // 最小生成树 克鲁斯卡尔算法

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
int **shortestPath_Dijkstra(pALGraph G, int i);   // 最短路径 狄杰斯特拉算法
int **shortestPath_Flyod(pALGraph G);     // 最短路径 弗洛伊德算法
void showShortestPath(pALGraph G, int v0, int *path, int *D);
int **DijkstraHeap(pALGraph G, int v);  // 狄杰斯特拉算法(基于堆)
CCINFO *sortDist(int *cc, int n);
void showSortedDist(pALGraph G, CCINFO *summary);
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
int criticalPath(pALGraph G);   // 关键路径算法

/**
 * @brief   连通分量
 *          1. UCC 无向图连通分量
 * 
 * @param G 
 * @return int* 
 *          1. ucc无向图连通分量： a[0] 连通分量个数, a[1-n]各个顶点所在的连通分量号
 */
int *UCC(pALGraph G);   // 无向图连通分量
int *SCCKosaraju(pALGraph G);   // 有向图强连通分量
CCINFO *detailCC(int *cc, int n);   // 各连通分量顶点个数
void showCC(pALGraph G, int *cc);


void freeVexs(VerTexType *vexs, int n);

#endif