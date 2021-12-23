#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "algHeap.h"
#include "algraph.h"
#include "graphQueue.h"

pALGraph initALGraph()
{
    pALGraph G = (pALGraph)calloc(1, sizeof(ALGraph));
    if(!G)
    {
        perror("calloc for pALGraph error.\n");
        return NULL;
    }
    G->maxvexs = MAX_VERTEX_NUM;
    G->vertices = (pAdjList*)calloc(MAX_VERTEX_NUM, sizeof(pAdjList));
    if(!G->vertices)
    {
        perror("calloc for vertices error.\n");
        return NULL;
    }
    for(int i=0; i<G->maxvexs; ++i)
    {
        G->vertices[i] = newVerTex0();
        if(!G->vertices[i]) return NULL;
    }
    return G;
}

pALGraph resizeVertices(pALGraph G)
{
    int i = G->maxvexs;
    G->maxvexs += MAX_VERTEX_NUM;
    G->vertices = (pAdjList *)realloc(G->vertices, G->maxvexs*sizeof(pAdjList));
    if(!G->vertices)
    {
        perror("realloc for vertices error.\n");
        return NULL;
    }
    
    for(; i<G->maxvexs; ++i)
    {
        G->vertices[i] = newVerTex0();
        if(!G->vertices[i]) return NULL;
    }
    return G;
}


pAdjList newVerTex0()
{
    pAdjList vex = (pAdjList)calloc(1, sizeof(VNode));
    if(!vex)
    {
        perror("calloc for pAdjList error.\n");
        return NULL;
    }
    // vex->firstarc = newArc();
    // if(!vex->firstarc)  return NULL;
    vex->data = (VerTexType)calloc(VEX_NAME_LEN+1, sizeof(char));
    return vex;
} 

/**
 * @descripttion:   申请1个 VNode 空间来存放顶点信息
 * @return {*}
 */
pAdjList newVerTex(VerTexType u)
{
    pAdjList vex = newVerTex0();
    vex->data = (VerTexType)calloc(strlen(u)+1, sizeof(char));
    strcpy(vex->data, u);
    
    return vex;
} 

/**
 * @descripttion:   申请一块 ArcNode 空间存放边的权重信息， 即邻接列表
 * @return {*}
 */
pArcNode newArc()
{
    pArcNode arc = (pArcNode)calloc(1, sizeof(ArcNode));
    if(!arc)
    {
        perror("calloc for firstArc error.\n");
        return NULL;
    }
    arc->info = (pOtherInfo)calloc(1, sizeof(OtherInfo));
    return arc;
}



/**
 * @descripttion:   新增弧信息 <u1, u2> w
 * @return {*}
 * @param {pALGraph} G
 * @param {VerTexType} u1
 * @param {VerTexType} u2
 * @param {OtherInfo} w
 */
int insertArc(pavlTree *vexAvl, pALGraph G, VerTexType u1, VerTexType u2, pOtherInfo w)
{
    int idx1 = insertVex(vexAvl, G, u1);
    int idx2 = insertVex(vexAvl, G, u2);
    // 邻接边
    putArc(G, idx1, idx2, w);
    return 0;
} 

/**
 * @descripttion:   在G中查找顶点u, 并返回u在顶点数组中的索引
 * @return {*}
 * @param {pALGraph} G
 * @param {VerTexType} u
 */
int locateVex(pavlTree vexAvl, pALGraph G, VerTexType u)
{
    if(!vexAvl) return -1;
    int idx = -1;
    pavlElement key = newAvlElement(strlen(u));
    strcpy(key->key, u);
    pavlTree find = searchAvl(vexAvl,  key);
    if(find)
        idx = find->data->index;
    return idx;
}



/**
 * @descripttion:   新增顶点u
 * @return {*}
 * @param {pALGraph} G
 * @param {VerTexType} u
 */
int insertVex(pavlTree *vexAvl, pALGraph G, VerTexType u)
{
    int idx  = locateVex(*vexAvl, G, u);
    // printf("idx = %d\n", idx);
    if(idx == -1)
    {
        idx = G->vexnum;
        if(idx > G->maxvexs)
        {
            G = resizeVertices(G);
        }
        pavlElement key = newAvlElement(strlen(u));
        strcpy(key->key, u);
        key->index = idx;
        
        *vexAvl = insertAvl(*vexAvl, key, 0, NULL);

        putVex(G, idx, u);        
        G->vexnum += 1;
    }
    // printf("G.vexnum = %d, G.vertices[%d].data = %s\n", G->vexnum, idx, G->vertices[idx]->data);
    // showArcs(G);

    return idx;
} 

bool checkVexIndex(pALGraph G, int v)
{
    return !(v < 0 || v > G->vexnum);
}


/**
 * @descripttion:   // 对索引v的顶点赋值val
 * @return {*}
 * @param {pALGraph} G
 * @param {int} v
 * @param {VerTexType} val
 */
int putVex(pALGraph G, int v, VerTexType u)
{
    if(!checkVexIndex(G, v)) return -1;
    strcpy(G->vertices[v]->data, u);  
    return 0;
}  

/**
 * @descripttion:   // 在G中返回索引为v的顶点的信息
 * @return {*}
 * @param {pALGraph} G
 * @param {int} v
 */
VerTexType getVex(pALGraph G, int v)
{
    if(!checkVexIndex(G, v)) return NULL;
    return G->vertices[v]->data;
}           


/**
 * @descripttion:   // 在G中返回邻接列表arc[i][j]的值
 * @return {*}
 * @param {pALGraph} G
 * @param {int} i
 * @param {int} j
 */
pOtherInfo getArc(pALGraph G, int i, int j)
{
    pArcNode p = G->vertices[i]->firstarc;
    while(p && p->adjvex != j)
        p = p->nextarc;
    if(p)   return p->info;
    return NULL;
}          

/**
 * @descripttion:   // 对邻接列表arc[i][j]赋值val
 * @return {*}
 * @param {pALGraph} G
 * @param {int} i
 * @param {int} j
 * @param {pOtherInfo} w
 */
int putArc(pALGraph G, int i, int j, pOtherInfo w)
{
    if(i==j)    return 0;
    pArcNode p = G->vertices[i]->firstarc;

    pArcNode n = newArc();
    n->adjvex = j;
    memcpy(n->info, w, sizeof(OtherInfo)); 
    n->nextarc = p;
    
    G->vertices[i]->firstarc = n;
    G->arcnum += 1;

    return 0;
} 


/**
 * @descripttion:   // 返回v的第一个邻接顶点索引
 * @return {*}
 * @param {pALGraph} G
 * @param {int} v
 */
int firstAdjVex(pALGraph G, int v)
{
    return G->vertices[v]->firstarc->adjvex;
}           

/**
 * @descripttion:   // 返回v的相对于w的下一个邻接顶点索引
 * @return {*}
 * @param {pALGraph} G
 * @param {int} v
 * @param {int} w
 */
int nextAdjVex(pALGraph G, int v, int w)
{
    pArcNode p = G->vertices[v]->firstarc;
    while(p && p->adjvex != w)
        p = p->nextarc;
    if(p && p->nextarc) 
        return p->nextarc->adjvex;
    return -1;
}       

void showArcs(pALGraph G)
{
    int n = G->vexnum;
    int i = 0;
    pArcNode p = NULL;
    for(i=0; i<n; ++i)
    {
        printf("%d[%s] ", i, getVex(G, i));
        p = G->vertices[i]->firstarc;
        while(p)
        {
            printf("-> %d[%s]<%d> ", p->adjvex, getVex(G, p->adjvex), p->info->weight);
            p = p->nextarc;
        }
        printf("-> <nil>\n");
    }
}

void dfsTraverse(pALGraph G, char *visited, int v)
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

void DFSALGraph(pALGraph G, int v)
{
    if(checkVexIndex(G, v)) return;
    
    char *visited = (char *)calloc(G->vexnum+1, sizeof(char));
    if(!visited)    
    {
        perror("calloc for visited array error.\n");
        return;
    }
    memset(visited, '0', G->vexnum);
    printf("DFS for vex %d[%s]:\n", v, getVex(G, v));

    dfsTraverse(G, visited, v);
    printf("\n");

    free(visited);
    visited = NULL;
}

void BFSALGraph(pALGraph G, int v)
{
    int n = G->vexnum;
    char *visited = (char *)calloc(n+1, sizeof(char));
    memset(visited, '0', n);

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

    free(visited);  visited = NULL;
    free(e);        e = NULL;
    destroyQueue(&Q);
}


/**
 * @descripttion:   // 销毁图
 * @return {*}
 * @param {pALGraph} *G
 */
void destroyALGraph(pALGraph *G)
{
    if(!*G) return ;
    int n = (*G)->vexnum;

    pArcNode p = NULL, r = NULL;;
    int i = 0;
    for(i=0; i<n; ++i)
    {
        free((*G)->vertices[i]->data);
        (*G)->vertices[i]->data = NULL;

        p = (*G)->vertices[i]->firstarc;
        while(p)
        {
            r = p;
            p = p->nextarc;
            free(r->info);
            r->info = NULL;
            free(r);
            r = NULL;
        }
    }

    free(*G);
    *G = NULL;
}   


int *getIndegree(pALGraph G)
{
    int n = G->vexnum;
    int *indegree = (int *)calloc(n, sizeof(int));
    int i = 0;
    pArcNode p = NULL;
    for(i=0; i<n; ++i)
    {
        p = G->vertices[i]->firstarc;
        while(p)
        {
            indegree[p->adjvex]++;
            p = p->nextarc;
        }
    }
    return indegree;
}

/**
 * @descripttion:   // 拓扑排序 以入度为0的顶点开始, 有向图中要有入度为0的顶点
 * @return {*}
 * @param {pALGraph} G
 * @param {int} inverted
 */
int *topoSortALGraph(pALGraph G)
{
    int n = G->vexnum;
    int *topo = (int *)calloc(n, sizeof(int));
    int *indegree = getIndegree(G);
    int i = 0, k = 0;
    pArcNode p = NULL;
    for(i=0,k=0; i<n; ++i)
    {
        if(indegree[i] == 0)
        {
            topo[k++] = i;
            p = G->vertices[i]->firstarc;
            while(p)
            {
                indegree[p->adjvex]--;
                p = p->nextarc;
            }
        }
    }
    free(indegree); indegree = NULL;
    if(k != n)  return NULL;    // i!=0表明存在环
    return topo;
}        


void DFS_TOPO(pALGraph G, int inverted, int i, char *vistied, int *topo, int *curTp)
{
    vistied[i] = '1';   // 标记已访问
    if(inverted == 0)
    {
        pArcNode p = G->vertices[i]->firstarc;  // 对每个<i, k> dfs 顶点k
        while(p)
        {
            if(vistied[p->adjvex] != '1')   // 未访问过，dfs k
                DFS_TOPO(G, inverted, p->adjvex, vistied, topo, curTp);
            p = p->nextarc;
        }
    }
    else if(inverted == 1)  // topo 逆序
    {
        int j = 0;
        pArcNode p = NULL;
        for(j=0; j!=i && j<G->vexnum; ++j)
        {
            if(vistied[j] == '1')   continue;   // 被访问过
            p = G->vertices[j]->firstarc;
            while(p)
            {
                if(p->adjvex == i)
                {
                    DFS_TOPO(G, inverted, j, vistied, topo, curTp);
                    break;
                }
                p = p->nextarc;
            }
        }
    }
    *curTp -= 1;    // topo序回溯1位
    topo[*curTp] = i;   // 设置topo序顶点
}

/**
 * @descripttion:   // DFS 拓扑排序 任意有向图
 * @return {*}
 * @param {pALGraph} G
 * @param {int} inverted    0 拓扑正序  1 拓扑逆序
 */
int *topoSortALG(pALGraph G, int inverted)
{
    int n = G->vexnum;
    int curTp = n;
    char *vistied = (char *)calloc(n+1, sizeof(char));
    int *topo = (int *)calloc(n, sizeof(int));
    memset(vistied, '0', n);

    int i;
    for(i=0; i<n; ++i)
    {
        if(vistied[i] != '1')
        {
            DFS_TOPO(G, inverted, i, vistied, topo, &curTp);
        }
    }
    return topo;
}

void showTopo(pALGraph G, int *topo)
{
    int i = 0;
    int n = G->vexnum;
    for(i=0; i<n; ++i)
        printf("%d[%s] ", topo[i], getVex(G, topo[i]));
    printf("\n");
}

int getMinCost(pClosedge closedge, int n)
{
    int k = 0;
    for(k=0; k<n; ++k)
        if(closedge[k].lowcost.weight != 0)    
            break;
    for(int i=k+1; i<n; ++i)
    {
        if(closedge[i].lowcost.weight > 0 && closedge[i].lowcost.weight < closedge[k].lowcost.weight)
        {
            k = i;
        }
    }
    return k;
}

void showClosedge(pClosedge closedge, int n)
{
    for(int i=0; i<n; ++i)
        printf("%d: adjvex = %d, lowcost = %d\n", i, closedge[i].adjvex, closedge[i].lowcost.weight);
}

/**
 * @descripttion:   // 最小生成树 普里姆算法
 * @return {*}
 * @param {pALGraph} G
 * @param {VerTexType} u
 */
void miniSpanTree_Prim(pALGraph G, int i)
{
    int j, k;
    if(!checkVexIndex(G, i))
    {
        printf("顶点i: %d[%s] 不存在图中.\n", i, getVex(G, i));
        return;
    }
    
    int n = G->vexnum;

    pClosedge closedge = (pClosedge)calloc(n+1, sizeof(Closedge));
    if(!closedge)   return ;

    // 初始化closedge数组，将u加入，并初始化其余顶点到u的lowcost为相应的权
    pOtherInfo pw = NULL;
    for(j=0; j<n; ++j)
    {
        pw = getArc(G, i, j);
        closedge[j].adjvex = i;
        if(i != j && pw)
            closedge[j].lowcost.weight = pw->weight;
        else
            closedge[j].lowcost.weight = INT_MAX;

    }
    closedge[i].adjvex = i;
    closedge[i].lowcost.weight = 0;
    
    for(i=1; i<n; ++i)      // 循环n-1次，已选一个顶点u，剩余n-1个顶点待选
    {
        // showClosedge(closedge, n);
        k = getMinCost(closedge, n);
        j = closedge[k].adjvex;
        printf("%d[%s] %d[%s]\n", j, getVex(G, j), k, getVex(G, k));
        closedge[k].lowcost.weight = 0;    // 第k个顶点已被选择

        for(j=0; j<n; ++j)     // 将新选择的顶点与剩余顶点组成边的权 与 未选择k之前的 closedge lowcost.weight 比较
        {
            pw = getArc(G, k, j);
            // printf("%s %s %d\n", getVex(G, i), getVex(G, j), pw?pw->weight:-1);
            if(pw && (pw->weight < closedge[j].lowcost.weight))  // 如果权值更低，则更新 closedge 的信息
            {
                closedge[j].lowcost.weight = pw->weight;
                closedge[j].adjvex = k;
            }
        }
    }    
}   



/**
 * @descripttion:   // 最小生成树 克鲁斯卡尔算法
 * @return {*}
 * @param {pALGraph} G
 */
void miniSpanTree_Kruskal(pALGraph G)
{
    int n = G->vexnum;
    pHeap H = initHeap(0, n*(n+1)/2);
    pOtherInfo pw = NULL;

    int i, j;
    pElemHeap e = (pElemHeap)calloc(1, sizeof(ElemHeap));
    for(i=0; i<n; ++i)
    {
        for(j=i+1; j<n; ++j)
        {
            pw = getArc(G, i, j);
            if(pw)
                e->lowcost = pw->weight;
            else
               continue;
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

    int *vexset = (int *)calloc(n, sizeof(int));  // 连通分量数组
    for(i=0; i<n; ++i) // 初始化， 表示每个顶点各成一个连通分量
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
            for(i=0; i<n; ++i)
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


void showShortestPath(pALGraph G, int v0, int *D, int *path)
{
    int i = 0, k = 0, n = G->vexnum;
    printf("%s to each vex's shortest:\n", getVex(G, v0));
    for(i=0; i<n; ++i)
    {
        printf("%d[%s] -> %d[%s] shortest distinct = %d\n", v0, getVex(G, v0), i, getVex(G, i), D[i] < INT_MAX ? D[i] : -1);
        k = i;
        // while(path[k] != -1)
        // {
        //     printf("%d[%s]<-%d[%s](%d)\t", k, getVex(G, k), path[k], getVex(G, path[k]), getArc(G, path[k], k)->weight);
        //     if(path[k] == v0)
        //     {
        //         printf("\n");
        //         break;
        //     }   
        //     k = path[k];
        // }
    }
}

/**
 * @descripttion:   // 最短路径 狄杰斯特拉算法
 *                      S[i]:   记录源点v0到终点vi是否被确定最短路径
 *                      path[i]:记录源点v0到终点vi的最短路径vi的前一个顶点下标
 *                      D[i]:   记录源点v0到终点vi的最短路径长度
 * @return {*}      1.  狄杰斯特拉算法返回 int 型的二维数组，第一维为长度2，第二维长度为G.vexnum+1， 
 *                      a[0]为u到各顶点的路径长度，a[1]为各顶点的前驱顶点
 * @param {pALGraph} G
 * @param {VerTexType} u
 */
int **shortestPath_Dijkstra(pALGraph G, int v0)
{
    int i, j, k, shortest;
    if(!checkVexIndex(G, v0))
    {
        printf("顶点u: %d[%s] 不存在图中.\n", v0, getVex(G, v0));
        return NULL;
    }
    
    int n = G->vexnum;
    char *S = (char*)calloc(n+1, sizeof(char));
    int *D = (int *)calloc(n, sizeof(int));
    int *path = (int *)calloc(n, sizeof(int));
    pOtherInfo pw = NULL;
    S[0] = '-';
    for(i=0; i<n; ++i)     // 初始化
    {
        S[i] = '0';     // 所有顶点初始化为未确定
        pw = getArc(G, v0, i);
        if(pw)  // 如果arc[v0][i] 之间有弧，指定顶点i的前驱为v0
        {
            D[i] = pw->weight;
            path[i] = v0;
        }
        else    // 否则初始化为-1
        {
            D[i] = INT_MAX;
            path[i] = -1;
        }
    }
    S[v0] = '1';    D[v0] = 0;  // 将v0标记已确定
    for(i=1; i<n; ++i)
    {
        k = -1;
        shortest = INT_MAX;
        for(j=0; j<n; ++j)
        {
            if(S[j] != '1' && D[j] < shortest)  // 第j个顶点未被确认且v0到vj的路径更短，则更新相应信息
            {
                k = j;
                shortest = D[j];
            }
        }
        if(k >= 0)  // 确定了顶点k
        {
            S[k] = '1';
            for(j=0; j<n; ++j)     // 如果未确定的顶点中有顶点j到顶点k的距离arc[k][j]+ v0到顶点k的距离D[k] < 顶点v0到顶点j的距离D[j]
            {
                if(S[j] != '1')
                {
                    pw = getArc(G, k, j);
                    if(pw && pw->weight < INT_MAX && D[k] + pw->weight < D[j])   // 重新更新顶点k到未确定顶点的距离信息
                    {
                        D[j] = D[k] + pw->weight;
                        path[j] = k;
                    }
                }
            }
        }
    }

    // showShortestPath(G, v0, path, D);

    // printf("%s to each vex's shortest:\n", getVex(G, v0));
    // for(i=0; i<n; ++i)
    // {
    //     printf("%d[%s] -> %d[%s] shortest distinct = %d\n", v0, getVex(G, v0), i, getVex(G, i), D[i] < INT_MAX ? D[i] : -1);
    //     k = i;
    //     while(path[k] != -1)
    //     {
    //         printf("%d[%s]<-%d[%s]\t", k, getVex(G, k), path[k], getVex(G, path[k]));
    //         if(path[k] == v0)
    //         {
    //             printf("\n");
    //             break;
    //         }   
    //         k = path[k];
    //     }
    // }

    int **R = (int **)calloc(2, sizeof(int *));
    R[0] = D;
    R[1] = path;

    return R;
}   


/**
 * @descripttion:   // 最短路径 弗洛伊德算法
 * @return {*}      弗洛伊德算法返回 int 型的二维数组，第一维长度为 2*G.vexnum+1，第二维长度为G.vexnum+1，
 *                  其中 a[i][j] (1<=i<=n) 为i-j的路径长度
 *                       a[i][j] (n+1<=i<=2*n) 为j的前驱顶点
 * @param {pALGraph} G
 */
int **shortestPath_Flyod(pALGraph G)
{
    int n = G->vexnum;
    int i, j, k;
    pOtherInfo pw = NULL;
    int **DP = (int**)calloc(2*n, sizeof(int *)); // DP[0]-DP[n-1] 存放Dist  DP[n]-DP[n+n-1] 存放path 即j的前驱
    for(i=0; i<2*n; ++i)   
    {
        DP[i] = (int *)calloc(n, sizeof(int));
    }
    for(i=0; i<n; ++i) // 初始化
    {
        for(j=0; j<n; ++j)
        {
            pw = getArc(G, i, j);

            // 初始化为 DP[i][i] = arc[i][j]
            if(pw && i != j)    // i-j 有弧，j 的前驱设为i
            {
                DP[i][j] = pw->weight;
                DP[i+n][j] = i;
            }
            else if(!pw)    // 否则为-1
            {
                DP[i][j] = INT_MAX;
                DP[i+n][j] = -1;
            }
            if(i == j)
            {
                DP[i][j] = 0;
            }
        }
    }
    
    for(i=0; i<n; ++i)
    {
        for(j=0; j<n; ++j)
        {
            for(k=0; k<n; ++k)
            {
                if(DP[i][k] < INT_MAX && DP[k][j] < INT_MAX && DP[i][k] + DP[k][j] < DP[i][j])  // 找到顶点k，是的i-k的距离 + k-j的距离 小于 i-j的距离
                {
                    DP[i][j] = DP[i][k] + DP[k][j]; // 更新i-j 的距离
                    DP[i+n][j] = DP[k+n][j];        // 更新j 的前驱为k
                }
            }
        }
    }

    k = 0;
    printf("%s to each vex's shortest:\n", getVex(G, k));
    for(i=0; i<n; ++i)
    {
        printf("%d[%s] -> %d[%s] shoretest distinct = %d\n", k, getVex(G, k), i, getVex(G, i), DP[k][i] < INT_MAX ? DP[k][i] : -1);
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
 * @param {pALGraph} G
 */
int criticalPath(pALGraph G)
{
    int *topo = topoSortALGraph(G);  // 关键路径必须用这个有入度为0 的拓扑算法
    // int *topo = topoSortAMG(G, 0);
    if(!topo)
    {
        printf("图中存在环.\n");
        return -1;
    }   

    int n = G->vexnum;
    int *ve = (int *)calloc(n, sizeof(int));      // 各事件的最早发生时间 early
    int *vl = (int *)calloc(n, sizeof(int));      // 各事件的最迟发生时间 last
    int e, l;
    int i, j, k;
    pOtherInfo pw = NULL;

    for(i=0; i<n; ++i)     // 初始化每个事件的最早发生时间为0
        ve[i] = 0;
    for(i=0; i<n; ++i)  // 按拓扑序求每个事件的最早发生时间
    {
        k = topo[i];    // 取得拓扑序中顶点序号
        for(j=0; j<n; ++j)
        {
            pw = getArc(G, k, j);    // w 为arc[k][j]
            if(pw && pw->weight < INT_MAX && ve[j] < ve[k] + pw->weight)    // k-j 之间有弧，更新j的最早发生时间
                ve[j] = ve[k] + pw->weight;
        }
    }

    // for(i=0; i<n; ++i)
    //     printf("ve[%d] = %d\n", i, ve[i]);

    for(i=0; i<n; ++i)     // 初始化每个事件的最迟发生时间为ve[n]
        vl[i] = ve[n-1];
    for(i=n-1; i>=0; --i)     // 按拓扑逆序依次求得每个事件的最迟发生时间
    {
        k = topo[i];
        for(j=0; j<n; ++j)
        {
            pw = getArc(G, k, j);
            if(pw && pw->weight < INT_MAX && vl[k] > vl[j] - pw->weight)
                vl[k] = vl[j] - pw->weight;
        }
    }
    
    // for(i=0; i<n; ++i)
    //     printf("vl[%d] = %d\n", i, vl[i]);

    for(i=0; i<n; ++i) // 判断各个活动是否为关键活动 e == l?
    {
        for(j=0; j<n; ++j)
        {
            pw = getArc(G, i, j);    
            if(pw && pw->weight < INT_MAX) // i-j 有弧
            {
                e = ve[i];  // 活动<i,j>的最早开始时间
                l = vl[j] - pw->weight;  // 活动<i,j>的最迟开始时间
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



void showCC(pALGraph G, int *cc)
{
    int n = G->vexnum;
    int numCC = cc[0];
    CCINFO *summary = detailCC(cc, n);
    printf("total numCC = %d\n", numCC);
    int i = 0, j = 0;
    for(i=0; i<numCC; ++i)
    {
        printf("numCC = %d, num = %d\n", summary[i].numCC, summary[i].num);
        if(i > numCC -5)
        {
        printf("\t");
        for(j=1; j<=n; ++j)
        {
            if(cc[j] == summary[i].numCC)
                printf("%d[%s] ", j-1, getVex(G, j-1));
        }
        printf("\n");
        }
    }
    free(summary);
    summary = NULL;
}
 
static int cmpintp(const void *p1, const void *p2)
{
    // printf("p1.num = %d, p2.num = %d\n", ((CCINFO *)p1)->num, ((CCINFO *)p2)->num);
    return ((CCINFO *)p2)->num - ((CCINFO *)p1)->num;
}


CCINFO *detailCC(int *cc, int n)
{

    int numCC = cc[0];
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


/**
 * @descripttion:   无向图连通分量
 * @return {*}
 * @param {pALGraph} G
 */
int *UCC(pALGraph G)
{
    int numCC = 0;
    int n = G->vexnum;
    int *cc = (int *)calloc(n+1, sizeof(int));
    char *s = (char *)calloc(n+1, sizeof(char));
    memset(s, '0', n);

    int i, j, v;
    pLinkQueue Q = initQueue();    // 初始化队列;
    pElemType e = (pElemType)calloc(1, sizeof(ElemType));
    pArcNode p = NULL;
    for(i=0; i<n; ++i)
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
                cc[v+1] = numCC; // 设置该顶点为第numCC个连通分量
                // printf("e->idx = %d, numCC = %d\n", e->idx, numCC);
                p = G->vertices[v]->firstarc;   // 遍历v的每个邻接点
                while(p)
                {
                    j = p->adjvex;
                    // printf("    arc[%d][%d] = %d, s[%d] = %c\n", v, j, p->info->weight, j, s[j]);
                    if(p->info->weight < INT_MAX && s[j] != '1')   // 如果邻接点j为未访问过
                    {
                        // printf("\t%d enQueue.\n", j);
                        s[j] = '1'; //标记为已访问
                        e->idx = j;
                        memset(e->vex, 0, sizeof(e->vex));
                        strcpy(e->vex, getVex(G, j));
                        enQueue(Q, e);  // 将j入队
                    }
                    p = p->nextarc;
                }
            }
        }
    }
    cc[0] = numCC;

    showCC(G, cc);

    free(s);    s = NULL;

    return cc;
} 



void DFS_SCC(pALGraph G, char *s, int v, int numCC, int *scc)
{
    // printf(" --------- v = %d, numCC = %d\n", v, numCC);
    s[v] = '1';
    scc[v+1] = numCC;
    int k = 0;
    pArcNode p = G->vertices[v]->firstarc;
    while(p)    // v 的每个未访问过的邻接顶点依次 DFS_SCC
    {      
        k = p->adjvex;
        if(s[k] != '1') // 且顶点k未访问过
            DFS_SCC(G, s, k, numCC, scc);
        p = p->nextarc;
    }
}


/**
 * @descripttion:   有向图强连通分量
 * @return {*}
 * @param {pALGraph} G
 */
int *SCCKosaraju(pALGraph G)
{
    int n = G->vexnum;
    int *scc = (int *)calloc(n+1, sizeof(int));
    int *rtopo = topoSortALG(G, 1);
    char *s = (char *)calloc(n+1+1, sizeof(char));
    memset(s, '0', n+1);
    int numCC = 0;

    int i = 0, v;
    for(i=0; i<n; ++i)
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



/**
 * @descripttion:   狄杰斯特拉算法(基于堆的算法)
 * @return {*}
 * @param {pALGraph} G
 * @param {int} v
 */
int **DijkstraHeap(pALGraph G, int v)
{
    int n = G->vexnum;
    int *Dist = (int *)calloc(n, sizeof(int));
    int *Path = (int *)calloc(n, sizeof(int));
    pHeap H = initHeap(0, n);
    char *X = (char *)calloc(n+1, sizeof(char));
    memset(X, '0', n);
    pElemHeap e = (pElemHeap)calloc(1, sizeof(ElemHeap));
    e->head = v;
    e->lowcost = 0;
    insertHeap(H, e);
    int i = 0;
    for(i=0; i<n; ++i)
    {   
        if(i == v) continue;
        e->head = i;
        e->lowcost = INT_MAX;
        insertHeap(H, e);
    }

    int w = -1, y = -1;
    pArcNode p = NULL;
    while(!heapEmpty(H))
    {
        e = extractFirst(H);
        w = e->head;
        // printf("extract w = %d, X[w] = %c\n", w, X[w]);

        X[w] = '1';
        Dist[w] = e->lowcost;

        p = G->vertices[w]->firstarc;
        while(p)    // 对每条边<w, y> 更新y
        {
            y = p->adjvex;
            if(X[y] != '1')
            {
                e = deleteHeap(H, H->vexs[y]);
                // printf("to be update y = %d, e.lowcost = %d, Dist[w] + p->info->weight = %d\n", y, e->lowcost, Dist[w] + p->info->weight);
                // 比较 y.lowcost 与 Dist[w]+arc[w][y] 的大小，更新 y.lowcost = min(y.lowcost, Dist[w]+arc[w][y])
                if(Dist[w] < INT_MAX && e->lowcost > Dist[w] + p->info->weight)
                {
                    e->lowcost = Dist[w] + p->info->weight;
                    Path[y] = w;    // 更新y的前驱顶点
                }
                insertHeap(H, e);
            }
            p = p->nextarc;
        }
    }

    destroyHeap(&H);

    int **R = (int **)calloc(2, sizeof(int *));
    R[0] = Dist;
    R[1] = Path;
    return R;
}


CCINFO *sortDist(int *cc, int n)
{
    CCINFO *summary = (CCINFO *)calloc(n, sizeof(CCINFO));
    int i = 0, idx = 0;
    for(i=0; i<n; ++i)
    {
        summary[i].numCC = i;
        summary[i].num = cc[i];
    }

    qsort(summary, n, sizeof(CCINFO), cmpintp);
    return summary;
}

void showSortedDist(pALGraph G, CCINFO *summary)
{
    int i = 0;
    int n = G->vexnum;
    for(i=n-1; i>=0; --i)
    {
        
        printf("#%d: name[%s]-idx[%d]: dist[%d]\n", i, G->vertices[summary[i].numCC]->data, summary[i].numCC, summary[i].num);
    }
}