#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "amgraph.h"
#include "graphQueue.h"
#include "sqHeap.h"

/**
 * @descripttion:   校验顶点v是否合法
 * @return {*}      0 合法  -1 非法
 * @param {pAMGraph} G
 * @param {int} v
 */
int checkVexIndex(pAMGraph G, int v)
{
    if(v <= 0 || v > G->vexnum)
        return -1;
    return 0;
}


/**
 * @descripttion:   申请n个VerTexType 类型的数组来存放顶点信息
 * @return {*}
 * @param {int} n
 */
VerTexType *newVerTex(int n)
{
    VerTexType *vexs = (VerTexType *)calloc(n+1, sizeof(VerTexType));
    if(!vexs)
    {
        perror("calloc for VerTexType * error.\n");
        return NULL;
    }
    int i = 0;
    for(; i<=n; ++i)
    {
        vexs[i] = (VerTexType)calloc(VEX_NAME_LEN+1, sizeof(char));
        if(!vexs[i])
        {
            perror("calloc for VerTexType error.\n");
            return NULL;
        }
    }
    return vexs;
} 

/**
 * @descripttion:   申请一块空间存放边的权重信息， 即邻接矩阵
 *              direction=1 有向， size = MAX_VEX_NUM * MAX_VEX_NUM
 *              direction=0 无向， size = MAX_VEX_NUM*(MAX_VEX_NUM+1)/2     采用对称矩阵
 *                  k = i*(i-1) + j - 1  (i>=j)  s[k] = a[i][j]
 *                  k = j*(j-1) + i - 1  (i<j)   s[k] = a[i][j]
 * 
 * @return {*}
 * @param {int} vexnum
 * @param {int} direction
 */
ArcType *newArc(int vexnum, int direction)
{
    ArcType *arcs = NULL;
    int n = 0;
    if(direction == 1)      // 有向图
        n = vexnum*vexnum;
    else    // 无向图
        n = vexnum * (vexnum+1) / 2;

    arcs = (ArcType*)calloc(n, sizeof(int));
    if(!arcs)
    {
        perror("calloc for Arc error.\n");
        return NULL;
    }

    for(int i=0; i<n; ++i)
        arcs[i] = INT_MAX;

    return arcs;
} 

pAMGraph initAMGraph()
{
    pAMGraph G = (pAMGraph)calloc(1, sizeof(AMGraph));
    if(!G)
    {
        perror("initAMGraph error.\n");
        return NULL;
    }
    return G;
}

/**
 * @descripttion:   根据顶点信息和弧信息创建图
 * @return {*}
 * @param {VerTexType} *vexs
 * @param {pArcInfo} arcinfo
 * @param {int} vexnum
 * @param {int} arcnum
 * @param {int} direction
 */
pAMGraph createAMGraph(VerTexType *vexs, pArcInfo arcinfo, int vexnum, int arcnum, int direction)
{
    if(!vexs || vexnum == 0)    return NULL;

    pAMGraph G = initAMGraph(); 
    if(!G)  return NULL;

    G->direction = direction;
    G->arcnum = arcnum;
    G->vexnum = vexnum;
    G->vexs = newVerTex(vexnum);
    if(!G->vexs)    return NULL;
    G->arcs = newArc(vexnum, direction);
    if(!G->arcs)    return NULL;

    int i = 0;
    for(i=1; i<=vexnum; ++i)
        putVex(G, i, vexs[i]);

    int v1, v2, w;
    for(i=0; i<arcnum; ++i)
    {
        // printf("arcinfo[i].v1 = %s, arcinfo[i].v2 = %s\n", arcinfo[i].v1, arcinfo[i].v2);
        v1 = locateVex(G, arcinfo[i].v1);
        v2 = locateVex(G, arcinfo[i].v2);
        if(checkVexIndex(G, v1) || checkVexIndex(G, v2))
        {
            printf("v1 = %d, v2 = %d out of index.\n", v1, v2);
            return NULL;
        }
        w = arcinfo[i].w;
        putArc(G, v1, v2, w);
    }

        
    return G;
}

/**
 * @descripttion:   销毁图
 * @return {*}
 * @param {pAMGraph} *G
 */
void destroyAMGraph(pAMGraph *G)
{
    if(!G)  return;

    int n = (*G)->vexnum;
    int i = 0;
    for(; i<=n; ++i)
    {
        free((*G)->vexs[i]);
        (*G)->vexs[i] = NULL;
    }

    free((*G)->arcs);
    (*G)->arcs = NULL;

    free(G);
    G = NULL;
} 

/**
 * @descripttion:   在G中查找顶点u, 并返回u在顶点数组中的索引
 * 
 * @return {*}
 * @param {pAMGraph} G
 * @param {VerTexType} u
 */
int locateVex(pAMGraph G, VerTexType u)
{
    int i, n = G->vexnum;

    for(i=1; i<=n; ++i)
    {
        // printf("G->vexs[%d] = %s, u = %s\n", i, G->vexs[i], u);
        if(memcmp(G->vexs[i], u, strlen(u)) == 0)
            return i;
    }
    return -1;
} 

/**
 * @descripttion:   在G中返回索引为v的顶点的信息
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 */
VerTexType getVex(pAMGraph G, int v)
{
    if(checkVexIndex(G, v)) return NULL;
    return G->vexs[v];
} 

/**
 * @descripttion:   对索引v的顶点赋值val
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 * @param {VerTexType} val
 */
int putVex(pAMGraph G, int v, VerTexType val)
{
    if(checkVexIndex(G, v)) return -1;
    val[VEX_NAME_LEN] = 0;
    memcpy(G->vexs[v], val, strlen(val));

    return 0;
} 

/**
ArcType getArc(pAMGraph G, int i, int j);           // 
 * @descripttion:   
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} i
 * @param {int} j
 */
ArcType getArc(pAMGraph G, int i, int j)
{    
    // printf("in getArc: i=%d, j=%d, ", i, j);
    int n = 0;
    if(checkVexIndex(G, i) || checkVexIndex(G, j))    
        return -1;

    if(G->direction)    // 有向图
        n = (i-1) * G->vexnum + j - 1;
    else    // 无向图
    {
        if(i >= j)  n = i * (i-1) / 2 + j - 1;  
        else        n = j * (j-1) / 2 + i - 1;
    }
    // printf("G->arcs[%d] = %d\n", n, G->arcs[n]);
    return G->arcs[n];
}

/**
 * @descripttion:   对邻接矩阵arc[i][j]赋值val
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} i
 * @param {int} j
 * @param {ArcType} val
 */
int putArc(pAMGraph G, int i, int j, ArcType val)
{
    // printf("in putArc: i=%d, j=%d, ", i, j);
    int n = 0;
    if(checkVexIndex(G, i) || checkVexIndex(G, j))    
        return -1;
    
    if(G->direction)    // 有向图
    {
        n = (i-1) * G->vexnum + j - 1;      // arc[i][j] = S[n]
    }
    else    // 无向图 -- 对称矩阵
    {
        if(i >= j)  n = i * (i-1) / 2 + j - 1;  
        else        n = j * (j-1) / 2 + i - 1;
    }

    G->arcs[n] = val;
    // printf("G->arcs[%d] = %d\n", n, G->arcs[n]);
    return 0;
}

/**
 * @descripttion:   返回v的第一个邻接顶点索引
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 */
int firstAdjVex(pAMGraph G, int v)
{
    if(checkVexIndex(G, v)) return -1;
    int i = 1, n = G->vexnum;
    for(; i<=n; ++i)
    {
        if(getArc(G, v, i) < INT_MAX && getArc(G, v, i) > 0)
            return i;
    }
    return -1;
} 

/**
 * @descripttion:   返回v的相对于w的下一个邻接顶点索引
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 * @param {int} w
 */
int nextAdjVex(pAMGraph G, int v, int w)
{
    if(checkVexIndex(G, v) || checkVexIndex(G, w)) return -1;
    int i = w, n = G->vexnum;
    for(i=w+1; i<=n; ++i)
        if(getArc(G, v, i) < INT_MAX && getArc(G, v, i) > 0)
            return i;
    return -1;
}

// int insertVex(pAMGraph G, VerTexType u);        // 新增顶点u
// int deleteVex(pAMGraph G, VerTexType u);        // 删除顶点u
// int insertArc(pAMGraph G, VerTexType u1, VerTexType u2, ArcType w); // 新增弧信息
// int deleteArc(pAMGraph G, VerTexType u1, VerTexType u2);            // 删除弧信息


void dfsTraverse(pAMGraph G, char *visited, int v)
{
    printf("%d[%s] ", v, getVex(G, v));
    visited[v] = '1';
    int w = 0;
    for(w = firstAdjVex(G, v); w > 0; w = nextAdjVex(G, v, w))
    {
        if(visited[w] != '1')   
            dfsTraverse(G, visited, w);
    }
} 

/**
 * @descripttion:   以v为源点，深度优先遍历图
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 */
void DFSAMGraph(pAMGraph G, int v)
{
    if(checkVexIndex(G, v)) return;
    
    char *visited = (char *)calloc(G->vexnum+1+1, sizeof(char));
    if(!visited)    
    {
        perror("calloc for visited array error.\n");
        return;
    }
    memset(visited, '0', G->vexnum+1);
    printf("DFS for vex %d[%s]:\n", v, getVex(G, v));

    dfsTraverse(G, visited, v);
    printf("\n");
}


/**
 * @descripttion:   以v为源点，广度优先遍历图
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} v
 */
void BFSAMGraph(pAMGraph G, int v)
{
    if(checkVexIndex(G, v)) return;
    
    char *visited = (char *)calloc(G->vexnum+1+1, sizeof(char));
    if(!visited)    
    {
        perror("calloc for visited array error.\n");
        return;
    }
    memset(visited, '0', G->vexnum+1);

    printf("BFS for vex %d[%s]:\n", v, getVex(G, v));
    printf("%d[%s]", v, getVex(G, v));
    visited[v] = '1';
    pLinkQueue Q = initQueue();
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    e->idx = v;
    strcpy(e->vex, getVex(G, v));
    enQueue(Q, e);

    int u, w;
    while(!queueIsEmpty(Q))
    {
        e = deQueue(Q);
        u = e->idx;
        for(w=firstAdjVex(G, u); w>0; w=nextAdjVex(G, u, w))
        {
            if(visited[w] != '1')
            {
                printf(" - %d[%s]", w, getVex(G, w));
                visited[w] = '1';
                e->idx = w;
                enQueue(Q, e);
            }
        }
    }
    printf("\n");

    free(e);
    e = NULL;
    destroyQueue(&Q);
}


int *getIndegrees(pAMGraph G)
{
    int n = G->vexnum;

    int *indegrees = (int *)calloc(n+1, sizeof(int));
    if(!indegrees)
    {
        perror("calloc for indegrees error.\n");
        return NULL;
    }

    int i, j, w;
    for(i=1; i<=n; ++i)
    {
        for(j=1; j<=n; ++j)
        {
            if(i!=j)
            {
                w = getArc(G, i, j);
                if(w < INT_MAX && w > 0)
                {
                    indegrees[j]++;
                }
            }
        }
    }

    return indegrees;
}

void showIndegrees(int *indegrees, int n)
{
    printf("indegrees: ");
    for(int i=1; i<=n; ++i)
        printf("%d ", indegrees[i]);
    printf("\n");
}

void topoAMG(pAMGraph G, int *indegrees, int v, int *records, int *k, int inverted)
{
    // printf("%d[%s], k=%d \n", v, getVex(G, v), *k);
    records[*k] = v;
    (*k)++;
    indegrees[v]--;
    int i = 1;
    int w = 0;
    for(i=1; i<=G->vexnum; ++i)
    {
        if(inverted == 0)      // 拓扑正序 <v, i>
            w = getArc(G, v, i);
        else if(inverted == 1) // 拓扑逆序 <i, v>
            w = getArc(G, i, v);
        if(w > 0 && w < INT_MAX)
        {
            indegrees[i]--;
            if(indegrees[i] == 0)
                topoAMG(G, indegrees, i, records, k, inverted);
        }
    }
}


/**
 * @descripttion:   拓扑排序 针对有向无环图(DGA)
 * @return {*}
 * @param {pAMGraph} G
 * @param {int} direction   0 拓扑正序  1 拓扑逆序
 */
int *topoSortAMGraph(pAMGraph G, int inverted)
{
    if(G->direction != 1)   return NULL;

    int *indegrees = getIndegrees(G);

    int n = G->vexnum;
    int i = 1;
    int *records = (int *)calloc(n, sizeof(int));
    int k = 0;
    for(i=1; i<=n; ++i)
    {
        if(indegrees[i] == 0)
        {
            topoAMG(G, indegrees, i, records, &k, inverted);
        }
    }
    
    if(k != n)
    {
        return NULL;
    }

    for(i=0; i<n; ++i)
        printf("%d[%s] ", records[i], getVex(G, records[i]));
    printf("\n");

    free(indegrees);
    indegrees = NULL;

    return records;
} 

/**
 * @descripttion:   打印邻接矩阵
 * @return {*}
 * @param {pAMGraph} G
 */
void showArcs(pAMGraph G)
{
    int i, j = 0;
    for(i=1; i<=G->vexnum; ++i)
    {
        for(j=1; j<=G->vexnum; ++j)
            printf("%-10d ", getArc(G, i, j));
        printf("\n");
    }
}

void freeVexs(VerTexType *vexs, int n)
{
    int i = 0;
    for(i=0; i<=n; ++i)
    {
        free(vexs[i]);
        vexs[i] = NULL;
    }
    free(vexs);
    vexs = NULL;
}


int getMinCost(pClosedge closedge, int n)
{
    int k = 1;
    for(k=1; k<=n; ++k)
        if(closedge[k].lowcost != 0)    
            break;
    for(int i=k+1; i<=n; ++i)
    {
        if(closedge[i].lowcost > 0 && closedge[i].lowcost < closedge[k].lowcost)
        {
            k = i;
        }
    }
    return k;
}

void showClosedge(pClosedge closedge, int n)
{
    for(int i=1; i<=n; ++i)
        printf("%d: adjvex = %d, lowcost = %d\n", i, closedge[i].adjvex, closedge[i].lowcost);
}


/**
 * @descripttion:   最小生成树 普里姆算法 加点法 
 * @return {*}
 * @param {pAMGraph} G
 * @param {VerTexType} u
 */
void miniSpanTree_Prim(pAMGraph G, VerTexType u)
{
    int i, j, k;
    i = locateVex(G, u);
    if(checkVexIndex(G, i))
    {
        printf("顶点u: %s 不存在图中.\n", u);
        return;
    }
    
    int n = G->vexnum;

    pClosedge closedge = (pClosedge)calloc(n+1, sizeof(Closedge));
    if(!closedge)   return ;

    // 初始化closedge数组，将u加入，并初始化其余顶点到u的lowcost为相应的权
    closedge[i].adjvex = i;
    closedge[i].lowcost = 0;
    for(j=1; j<=n; ++j)
    {
        if(i != j)
        {
            closedge[j].adjvex = i;
            closedge[j].lowcost = getArc(G, i, j);
        }
    }
    
    for(i=1; i<n; ++i)      // 循环n-1次，已选一个顶点u，剩余n-1个顶点待选
    {
        // showClosedge(closedge, n);
        k = getMinCost(closedge, n);
        j = closedge[k].adjvex;
        printf("%d[%s] %d[%s]\n", j, getVex(G, j), k, getVex(G, k));
        closedge[k].lowcost = 0;    // 第k个顶点已被选择

        for(j=1; j<=n; ++j)     // 将新选择的顶点与剩余顶点组成边的权 与 未选择k之前的 closedge lowcost 比较
        {
            if(getArc(G, k, j) < closedge[j].lowcost)   // 如果权值更低，则更新 closedge 的信息
            {
                closedge[j].lowcost = getArc(G, k, j);
                closedge[j].adjvex = k;
            }
        }
    }
}


/**
 * @descripttion:   最小生成树 克鲁斯卡尔算法 加边法
 * @return {*}
 * @param {pAMGraph} G
 */
void miniSpanTree_Kruskal(pAMGraph G)
{
    int n = G->vexnum;
    pHeap H = initHeap(0, n*(n+1)/2);

    int i, j;
    pElemHeap e = (pElemHeap)calloc(1, sizeof(ElemHeap));
    for(i=1; i<=n; ++i)
    {
        for(j=i+1; j<=n; ++j)
        {
            e->lowcost = getArc(G, i, j);
            if(e->lowcost == INT_MAX)   continue;
            e->head = i;
            e->tail = j;
            if(!insertHeap(H, e)) 
            {
                printf("insertHeap error.\n");
                return;
            }
        }
    }
    // printf("after insert, H.size = %d, H.total = %d\n", H->size, H->total);

    int *vexset = (int *)calloc(n+1, sizeof(int));  // 连通分量数组
    for(i=1; i<=n; ++i) // 初始化， 表示每个顶点各成一个连通分量
        vexset[i] = i;

    int v, u;
    while(!heapEmpty(H))
    {
        // showHeap(H);
        e = extractFirst(H);    // 依次取权值最小的边
        v = vexset[e->head];    // v为一个顶点的连通分量
        u = vexset[e->tail];    // u为另一个顶点的连通分量
        
        if(v != u)  // 两个顶点不属于同一个连通分量
        {
            printf("%d[%s] %d[%s]\n", e->head, getVex(G, e->head), e->tail, getVex(G, e->tail));    // 输出此边
            for(i=1; i<=n; ++i)
            {
                if(vexset[i] == u)  // 合并两个连通分量
                    vexset[i] = v;  // 将连通分量为u的顶点更新为连通分量v
            }
        }
    }

    free(vexset);
    vexset = NULL;
    free(e);
    e = NULL;
    destroyHeap(&H);
}


/**
 * @descripttion:   最短路径 狄杰斯特拉算法
 *                      S[i]:   记录源点v0到终点vi是否被确定最短路径
 *                      path[i]:记录源点v0到终点vi的最短路径vi的前一个顶点下标
 *                      D[i]:   记录源点v0到终点vi的最短路径长度
 * @return {*}      狄杰斯特拉算法返回 int 型的二维数组，第一维为长度2，第二维长度为G.vexnum+1， 
 *                  a[0]为u到各顶点的路径长度，a[1]为各顶点的前驱顶点
 * @param {pAMGraph} G
 * @param {VerTexType} u
 */
int **shortestPath_Dijkstra(pAMGraph G, VerTexType u)
{
    int i, j, k, v0, shortest;
    v0 = locateVex(G, u);
    if(checkVexIndex(G, v0))
    {
        printf("顶点u: %s 不存在图中.\n", u);
        return NULL;
    }
    
    int n = G->vexnum;
    char *S = (char*)calloc(n+1+1, sizeof(char));
    int *D = (int *)calloc(n+1, sizeof(int));
    int *path = (int *)calloc(n+1, sizeof(int));
    S[0] = '-';
    for(i=1; i<=n; ++i)     // 初始化
    {
        S[i] = '0';     // 所有顶点初始化为未确定
        D[i] = getArc(G, v0, i);
        if(D[i] < INT_MAX)  // 如果arc[v0][i] 之间有弧，指定顶点i的前驱为v0
            path[i] = v0;
        else    // 否则初始化为-1
            path[i] = -1;
    }
    S[v0] = '1';    D[v0] = 0;  // 将v0标记已确定
    for(i=1; i<n; ++i)
    {
        k = -1;
        shortest = INT_MAX;
        for(j=1; j<=n; ++j)
        {
            if(S[j] != '1' && D[j] < shortest)  // 第j个顶点未被确认且v0到vj的路径更短，则更新相应信息
            {
                k = j;
                shortest = D[j];
            }
        }
        if(k >= 1)  // 确定了顶点k
        {
            S[k] = '1';
            for(j=1; j<=n; ++j)     // 如果未确定的顶点中有顶点j到顶点k的距离arc[k][j]+ v0到顶点k的距离D[k] < 顶点v0到顶点j的距离D[j]
            {
                if(S[j]!='1' && getArc(G, k, j) < INT_MAX && D[k] + getArc(G, k, j) < D[j])   // 重新更新顶点k到未确定顶点的距离信息
                {
                    D[j] = D[k] + getArc(G, k, j);
                    path[j] = k;
                }
            }
        }
    }

    printf("%s to each vex's shortest:\n", u);
    for(i=1; i<=n; ++i)
    {
        printf("%d[%s] -> %d[%s] shortest distinct = %d\n", v0, u, i, getVex(G, i), D[i] < INT_MAX ? D[i] : -1);
        k = i;
        while(path[k] != -1)
        {
            printf("%d[%s]<-%d[%s]\t", k, getVex(G, k), path[k], getVex(G, path[k]));
            if(path[k] == v0)
            {
                printf("\n");
                break;
            }   
            k = path[k];
        }
    }

    int **R = (int **)calloc(2, sizeof(int *));
    D[0] = n; path[0] = n;
    R[0] = D;
    R[1] = path;

    return R;
} 

/**
 * @descripttion:   最短路径 弗洛伊德算法
 * @return {*}      弗洛伊德算法返回 int 型的二维数组，第一维长度为 2*G.vexnum+1，第二维长度为G.vexnum+1，
 *                  其中 a[i][j] (1<=i<=n) 为i-j的路径长度
 *                       a[i][j] (n+1<=i<=2*n) 为j的前驱顶点
 * @param {pAMGraph} G
 */
int **shortestPath_Flyod(pAMGraph G)
{
    int n = G->vexnum;
    int i, j, k;
    int **DP = (int**)calloc(2*n+1, sizeof(int *)); // DP[1]-DP[n] 存放Dist  DP[1+n]-DP[n+n] 存放path 即j的前驱
    for(i=1; i<=2*n; ++i)   
    {
        DP[i] = (int *)calloc(n+1, sizeof(int));
    }
    for(i=1; i<=n; ++i) // 初始化
    {
        for(j=1; j<=n; ++j)
        {
            DP[i][j] = getArc(G, i, j);     // 初始化为 DP[i][i] = arc[i][j]
            if(i == j)
                DP[i][j] = 0;
            if(DP[i][j] < INT_MAX && i != j)    // i-j 有弧，j 的前驱设为i
                DP[i+n][j] = i;
            else    // 否则为-1
                DP[i+n][j] = -1;
        }
    }
    
    for(i=1; i<=n; ++i)
    {
        for(j=1; j<=n; ++j)
        {
            for(k=1; k<=n; ++k)
            {
                if(DP[i][k] < INT_MAX && DP[k][j] < INT_MAX && DP[i][k] + DP[k][j] < DP[i][j])  // 找到顶点k，是的i-k的距离 + k-j的距离 小于 i-j的距离
                {
                    DP[i][j] = DP[i][k] + DP[k][j]; // 更新i-j 的距离
                    DP[i+n][j] = DP[k+n][j];        // 更新j 的前驱为k
                }
            }
        }
    }

    k = 1;
    printf("%s to each vex's shortest:\n", G->vexs[k]);
    for(i=1; i<=n; ++i)
    {
        printf("%d[%s] -> %d[%s] shoretest distinct = %d\n", k, G->vexs[k], i, G->vexs[i], DP[k][i] < INT_MAX ? DP[k][i] : -1);
        j = i;
        while(DP[n+k][j] != -1)
        {
            printf("%d[%s]<-%d[%s]\t", j, getVex(G, j), DP[n+k][j], getVex(G, DP[n+k][j]));
            if(DP[n+k][j] == k)
            {
                printf("\n");
                break;
            }
            j = DP[n+k][j];
        }
    }

    return DP;
} 


/**
 * @descripttion:   关键路径
 * @return {*}
 * @param {pAMGraph} G
 */
int criticalPath(pAMGraph G)
{
    int *topo = topoSortAMGraph(G, 0);  // 关键路径必须用这个有入度为0 的拓扑算法
    // int *topo = topoSortAMG(G, 0);
    if(!topo)   return -1;

    int n = G->vexnum;
    int *ve = (int *)calloc(n+1, sizeof(int));      // 各事件的最早发生时间 early
    int *vl = (int *)calloc(n+1, sizeof(int));      // 各事件的最迟发生时间 last
    int e, l;
    int i, j, k, w;

    for(i=1; i<=n; ++i)     // 初始化每个事件的最早发生时间为0
        ve[i] = 0;
    for(i=0; i<n; ++i)  // 按拓扑序求每个事件的最早发生时间
    {
        k = topo[i];    // 取得拓扑序中顶点序号
        for(j=1; j<=n; ++j)
        {
            w = getArc(G, k, j);    // w 为arc[k][j]
            if(w < INT_MAX && ve[j] < ve[k] + w)    // k-j 之间有弧，更新j的最早发生时间
                ve[j] = ve[k] + w;
        }
    }

    // for(i=1; i<=n; ++i)
    //     printf("ve[%d] = %d\n", i, ve[i]);

    for(i=1; i<=n; ++i)     // 初始化每个事件的最迟发生时间为ve[n]
        vl[i] = ve[n];
    for(i=n; i>=1; --i)     // 按拓扑逆序依次求得每个事件的最迟发生时间
    {
        k = topo[i];
        for(j=1; j<=n; ++j)
        {
            w = getArc(G, k, j);
            if(w < INT_MAX && vl[k] > vl[j] - w)
                vl[k] = vl[j] - w;
        }
    }
    
    // for(i=1; i<=n; ++i)
    //     printf("vl[%d] = %d\n", i, vl[i]);

    for(i=1; i<=n; ++i) // 判断各个活动是否为关键活动 e == l?
    {
        for(j=1; j<=n; ++j)
        {
            w = getArc(G, i, j);    
            if(w < INT_MAX) // i-j 有弧
            {
                e = ve[i];  // 活动<i,j>的最早开始时间
                l = vl[j] - w;  // 活动<i,j>的最迟开始时间
                if(e == l)
                {
                    printf("<%d[%s] %d[%s]>\n", i, getVex(G, i), j, getVex(G, j));
                }
            }
        }
    }

    free(ve);   ve = NULL;
    free(vl);   vl = NULL;
    return 0;
}

void showCC(pAMGraph G, int *cc)
{
    int n = G->vexnum;
    int numCC = cc[0];
    CCINFO *summary = detailCC(cc, n);
    
    int i = 0, j = 0;
    for(i=0; i<numCC; ++i)
    {
        printf("numCC = %d, num = %d\n\t", summary[i].numCC, summary[i].num);
        for(j=1; j<=n; ++j)
        {
            if(cc[j] == summary[i].numCC)
                printf("%d[%s] ", j, getVex(G, j));
        }
        printf("\n");
    }
    free(summary);
    summary = NULL;
}
 


/**
 * @descripttion:   无向图连通分量
 * @return {*}
 * @param {pAMGraph} G
 */
int *UCC(pAMGraph G)
{
    int numCC = 0;
    int n = G->vexnum;
    int *cc = (int *)calloc(n+1, sizeof(int));
    char *s = (char *)calloc(n+1+1, sizeof(char));
    memset(s, '0', n+1);

    int i, j, v;
    pLinkQueue Q = initQueue();    // 初始化队列;
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    for(i=1; i<=n; ++i)
    {
        if(s[i] != '1')
        {
            numCC += 1;     // 新的连通分量

            e->idx = i;
            memset(e->vex, 0, sizeof(e->vex));
            strcpy(e->vex, getVex(G, i));
            enQueue(Q, e);  // 将i入队

            s[i] = '1';     // 标记i为已访问
            while(!queueIsEmpty(Q)) // 队列非空
            {
                e = deQueue(Q);     // 队首元素出队
                v = e->idx;
                cc[v] = numCC; // 设置该顶点为第numCC个连通分量
                // printf("e->idx = %d, numCC = %d\n", e->idx, numCC);
                for(j=1; j<=n; ++j) // 遍历i的每个邻接点
                {
                    // printf("    arc[%d][%d] = %d, s[%d] = %c\n", v, j, getArc(G, v, j), j, s[j]);
                    if(getArc(G, v, j) < INT_MAX && s[j] != '1')   // 如果邻接点j为未访问过
                    {
                        // printf("\t%d enQueue.\n", j);
                        s[j] = '1'; //标记为已访问
                        e->idx = j;
                        memset(e->vex, 0, sizeof(e->vex));
                        strcpy(e->vex, getVex(G, j));
                        enQueue(Q, e);  // 将j入队
                    }
                }
            }
        }
    }
    cc[0] = numCC;
    showCC(G, cc);

    free(s);    s = NULL;

    return cc;
} 


static int cmpintp(const void *p1, const void *p2)
{
    // printf("p1.num = %d, p2.num = %d\n", ((CCINFO *)p1)->num, ((CCINFO *)p2)->num);
    return ((CCINFO *)p2)->num - ((CCINFO *)p1)->num;
}

CCINFO *detailCC(int *cc, int n)
{

    int numCC = cc[0];
    // int **detail = (int **)calloc(2, sizeof(int *));
    CCINFO *summary = (CCINFO *)calloc(numCC, sizeof(CCINFO));
    int i = 0, idx = 0;
    for(i=1; i<=n; ++i)
    {
        idx = cc[i]-1;
        summary[idx].numCC = cc[i];
        summary[idx].num++;
    }

    // for(i=0; i<numCC; ++i)
    //     printf("summary[%d].numCC = %d, summary[%d].num = %d\n", i, summary[i].numCC, i, summary[i].num);

    qsort(summary, numCC, sizeof(CCINFO), cmpintp);
    return summary;
}


void DFS_TOPO(pAMGraph G, int inverted, int k, int *curNum, char *s, int *topo)
{
    s[k] = '1';
    int i = 0, n = G->vexnum, w;
    for(i=1; i<=n; ++i)
    {
        if(s[i] != '1')
        {
            if(inverted == 0)   // 正序拓扑
                w = getArc(G, k, i);    // <k, i> 存在，即判断k的外射顶点
            else if(inverted == 1)  // 逆序拓扑
                w = getArc(G, i, k);    // <i, k> 存在，即判断k的入射顶点
            if(w > 0 && w < INT_MAX)    // arc[k][i] 存在
                DFS_TOPO(G, inverted, i, curNum, s, topo);
        }
    }
    topo[*curNum] = k;
    // printf("------- k = %d, topo[%d] = %d\n", k, *curNum, topo[*curNum]);
    *curNum -= 1;
}

int *topoSortAMG(pAMGraph G, int inverted)
{
    int n = G->vexnum;
    int *topo = (int *)calloc(n+1, sizeof(int));
    char *s = (char *)calloc(n+1+1, sizeof(char));
    memset(s, '0', n+1);
    int curNum = n;
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        if(s[i] != '1')     // 第i个顶点为访问过
        {
            DFS_TOPO(G, inverted, i, &curNum, s, topo);
        }
    }
    
    if(curNum != 0) return NULL;    // 存在环

    // for(i=1; i<=n; ++i)
    //     printf("%d[%s] ", topo[i], getVex(G, topo[i]));
    // printf("\n");

    free(s);    s = NULL;
    
    topo[0] = n;
    return topo;
}



void DFS_SCC(pAMGraph G, char *s, int v, int numCC, int *scc)
{
    // printf(" --------- v = %d, numCC = %d\n", v, numCC);
    s[v] = '1';
    scc[v] = numCC;
    int k = 0, w;
    for(k=1; k<=G->vexnum; ++k)
    {
        w = getArc(G, v, k);        
        if(w < INT_MAX)     // <v, k> 存在
        {
            if(s[k] != '1') // 且顶点k未访问过
                DFS_SCC(G, s, k, numCC, scc);
        }
    }
}

/**
 * @descripttion:   有向图强连通分量
 * @return {*}
 * @param {pAMGraph} G
 */
int *SCCKosaraju(pAMGraph G)
{
    int n = G->vexnum;
    int *scc = (int *)calloc(n+1, sizeof(int));
    int *rtopo = topoSortAMG(G, 1);
    char *s = (char *)calloc(n+1+1, sizeof(char));
    memset(s, '0', n+1);
    int numCC = 0;

    int i = 0, v;
    for(i=1; i<=n; ++i)
    {
        v = rtopo[i];     // 逆topo序的顶点
        if(s[v] != '1')
        {
            numCC += 1;
            // printf("v = %d, numCC = %d\n", v, numCC);
            DFS_SCC(G, s, v, numCC, scc);
        }
    }
    scc[0] = numCC;

    showCC(G, scc);

    free(rtopo);    rtopo = NULL;
    free(s);    s = NULL;

    return scc;
} 