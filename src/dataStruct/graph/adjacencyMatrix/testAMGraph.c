#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "amgraph.h"


void freeArcinfo(pArcInfo arcinfo, int n);
int readData(VerTexType **vexs, pArcInfo *arcinfo, int *vexnum, int *arcnum);
int readFile(VerTexType **vexs, pArcInfo *arcinfo, int *vexnum, int *arcnum);

int main(int argc, char *argv[])
{
    int opt;
    int direction = 0;
    int V = 1;
    char sv[VEX_NAME_LEN+1] = {0};
    char file[32] = {0};


    while((opt = getopt(argc, argv, "s:d:v:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            strcpy(file, optarg);
            break;

        case 'd':
            direction = atoi(optarg);
            break;

        case 'v':
            // V = atoi(optarg);
            strcpy(sv, optarg);
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-f file] [-d 0|1] [-v VexIdx]\n", argv[0]);
            return -1;
        }
    }

    int vexnum, arcnum;
    VerTexType *vexs = NULL;
    pArcInfo arcinfo = NULL;
    if(strlen(file))    // 从文件读取数据
    {

    }
    else if(readData(&vexs, &arcinfo, &vexnum, &arcnum))   // 从键盘输入
    {
        return -11;
    }
    
    pAMGraph G = createAMGraph(vexs, arcinfo, vexnum, arcnum, direction);
    if(!G)  return -3;


    if(strlen(sv))
        V = locateVex(G, sv);

    freeVexs(vexs, vexnum);
    freeArcinfo(arcinfo, arcnum);

    printf("DFSAMGraph idx[%d][%s]:\n", locateVex(G, G->vexs[V]), G->vexs[V]);
    DFSAMGraph(G, locateVex(G, G->vexs[V]));

    printf("BFSAMGraph idx[%d][%s]:\n", locateVex(G, G->vexs[V]), G->vexs[V]);
    BFSAMGraph(G, locateVex(G, G->vexs[V]));

    if(G->direction == 1)
    {
        printf("TopoSort AMGraph:\n");
        topoSortAMGraph(G, 0);  
        // topoSortAMG(G, 0);

        printf("关键路径:\n");
        criticalPath(G);
    }

    printf("普里姆算法生成最小生成树: 起点 %s\n", G->vexs[V]);
    miniSpanTree_Prim(G, G->vexs[V]);
    
    printf("克鲁斯卡尔算法生成最小生成树:\n");
    miniSpanTree_Kruskal(G);

    printf("狄杰斯特拉算法: 起点 %s\n", G->vexs[V]);
    shortestPath_Dijkstra(G, G->vexs[V]);
    
    printf("弗洛伊德算法: 起点 %s\n", G->vexs[V]);
    shortestPath_Flyod(G);


    printf("连通分量:\n");
    if(G->direction == 0)
    {
        UCC(G);
    }
    else if (G->direction == 1)
    {
        SCCKosaraju(G);
    }
    

    return 0;
}


void freeArcinfo(pArcInfo arcinfo, int n)
{
    for(int i=0; i<n; ++i)
    {
        free(arcinfo[i].v1);    arcinfo[i].v1 = NULL;
        free(arcinfo[i].v2);    arcinfo[i].v2 = NULL;
        arcinfo[i].w = 0;
    }
    free(arcinfo);
    arcinfo = NULL;
}

int readData(VerTexType **vexs, pArcInfo *arcinfo, int *vexnum, int *arcnum)
{
    int i;
    printf("输入顶点个数: ");
    scanf("%d", vexnum);
    (*vexs) = newVerTex(*vexnum);
    if(!(*vexs))   return -1;

    printf("依次输入顶点数: ");
    for(i=1; i<=*vexnum; ++i)
    {
        scanf("%s", (*vexs)[i]);
    }

    printf("输入图中的边数: ");
    scanf("%d", arcnum);
    (*arcinfo) = (pArcInfo)calloc(*arcnum, sizeof(ArcInfo));
    if(!(*arcinfo))    return -2;
    for(i=0; i<*arcnum; ++i)
    {
        (*arcinfo)[i].v1 = (VerTexType)calloc(VEX_NAME_LEN+1, sizeof(char));
        if(!(*arcinfo)[i].v1) return -3;
        
        (*arcinfo)[i].v2 = (VerTexType)calloc(VEX_NAME_LEN+1, sizeof(char));
        if(!(*arcinfo)[i].v2) return -4;
    }

    printf("依次输入每条边的顶点v1 顶点v2 权w: \n");
    for(i=0; i<*arcnum; ++i)
    {
        scanf("%s %s %d", (*arcinfo)[i].v1, (*arcinfo)[i].v2, &(*arcinfo)[i].w);
    }
    
    return 0;
}