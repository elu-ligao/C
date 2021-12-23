#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "algraph.h"

int readData();
int readFile();

int main(int argc, char *argv[])
{
    int opt;
    // int direction = 0;
    char file[32] = {0};
    char sv[VEX_NAME_LEN+1] = {0};


    while((opt = getopt(argc, argv, "s:f:v:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            strcpy(file, optarg);
            break;

        // case 'd':
        //     direction = atoi(optarg);
        //     break;

        case 'v':
            // V = atoi(optarg);
            strcpy(sv, optarg);
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-f file] [-d 0|1] [-v VexIdx]\n", argv[0]);
            return -1;
        }
    }

    if(strlen(file))    // 从文件读取数据
        return readFile(file);
    else    // 从键盘输入
        return readData();
}



int readData()
{
    pALGraph G = initALGraph();
    pavlTree vexAvl = initAVLTree();
    char u1[VEX_NAME_LEN+1], u2[VEX_NAME_LEN+1];
    pOtherInfo w = (pOtherInfo)calloc(1, sizeof(OtherInfo));
    int arcnum = 0;
    int i;
    

    printf("输入图中的边数: ");
    scanf("%d", &arcnum);
    printf("依次输入每条边的顶点v1 顶点v2 权w: \n");
    for(i=0; i<arcnum; ++i)
    {
        memset(u1, 0, sizeof(u1));
        memset(u2, 0, sizeof(u1));
        scanf("%s %s %d", u1, u2, &w->weight);
        // printf("u1 = %s, u2 = %s, w = %d\n", u1, u2, w->weight);
        insertArc(&vexAvl, G, u1, u2, w);
        // printf("%d arc insert over.\n", i);
    }

    // showArcs(G);
    // outputSortedAvl(vexAvl, 0, 1);

    int *topo = topoSortALGraph(G);
    if(!topo)   
        printf("G 中存在环.\n");
    else
        showTopo(G, topo);

    topo = topoSortALG(G, 0);
    printf("拓扑正序:\n");
    showTopo(G, topo);

    topo = topoSortALG(G, 1);
    printf("拓扑逆序:\n");
    showTopo(G, topo);
    
    i = locateVex(vexAvl, G, getVex(G, 0));
    printf("最小生成树，普里姆算法: i=%d[%s]\n", i, getVex(G, i));
    miniSpanTree_Prim(G, i);

    printf("最小生成树，克鲁斯卡尔算法:\n");
    miniSpanTree_Kruskal(G);

    printf("最短距离， 狄杰斯特拉算法: i=%d[%s]\n", i, getVex(G, i));
    shortestPath_Dijkstra(G, i);

    printf("最短距离， 弗洛伊德算法: \n");
    shortestPath_Flyod(G);

    printf("关键路径:\n");
    criticalPath(G);

    printf("连通分量:\n");
    UCC(G);

    printf("SCC Kosaraju:\n");
    SCCKosaraju(G);

    free(topo);     topo = NULL;
    freeAvl(&vexAvl);
    destroyALGraph(&G);

    return 0;    
}

void countSCC()
{
    pALGraph G = initALGraph();
    pavlTree vexAvl = initAVLTree();

    char *file = "../src/dataStruct/graph/data/Challenge_SCC.ADG.dist";
    FILE *fp = fopen(file, "r");
    if(!fp)
    {
        perror("open file error.\n");
        return;
    }
    char u1[VEX_NAME_LEN+1] = {0}, u2[VEX_NAME_LEN+1] = {0};
    pOtherInfo w = (pOtherInfo)calloc(1, sizeof(OtherInfo));
    w->weight = 1;
    while(EOF != fscanf(fp, "%s %s %d", u1, u2, &w->weight))
    {
        // printf("%s, %s\n", u1, u2);
        insertArc(&vexAvl, G, u1, u2, w);
        memset(u1, 0, sizeof(u1));
        memset(u2, 0, sizeof(u2));
    }
    printf("G.vexnum = %d, G.arcnum = %d\n", G->vexnum, G->arcnum);
    // printf("SCC Kosaraju:\n");
    // SCCKosaraju(G);
    printf("dist from vertex 549700:\n");
    int v = locateVex(vexAvl, G, "549700");
    // int **R = shortestPath_Dijkstra(G, v);
    int **R = DijkstraHeap(G, v);

    CCINFO *sortedDist = sortDist(R[0], G->vexnum);
    showSortedDist(G, sortedDist);
}

void countDijkstra()
{
    pALGraph G = initALGraph();
    pavlTree vexAvl = initAVLTree();

    char *file = "../src/dataStruct/graph/data/Challenge_Dist.txt";
    FILE *fp = fopen(file, "r");
    if(!fp)
    {
        perror("open file error.\n");
        return;
    }
    
    char u1[VEX_NAME_LEN+1] = {0}, u2[VEX_NAME_LEN+1] = {0};
    pOtherInfo w = (pOtherInfo)calloc(1, sizeof(OtherInfo));
    char line[1024] = {0};
    char *token, *saveptr, *src;
    int first = 1, V2 = 0;
    while(fgets(line, sizeof(line), fp))
    {
        first = 1;
        for(src=line; ; src=NULL)   
        {
            memset(u2, 0, sizeof(u2));
            token = strtok_r(src, "\t", &saveptr);
            if(NULL == token)
                break;
            if(first)
            {
                first = 0;
                strcpy(u1, token);
                continue;
            }
            if(strstr(token, ",") == NULL)  break;
            // printf("token = %s\n", token);
            sscanf(token, "%d,%d", &V2, &w->weight);
            sprintf(u2, "%d", V2);
            // printf("u1 = %s, u2 = %s, w.weight = %d\n", u1, u2, w->weight);
            insertArc(&vexAvl, G, u1, u2, w);
        }
        memset(line, 0, sizeof(line));
        memset(u1, 0, sizeof(u1));
    } 
    
    printf("G.vexnum = %d, G.arcnum = %d\n", G->vexnum, G->arcnum);
    printf("countDijkstra from vertex 1:\n");
    int v = locateVex(vexAvl, G, "1");
    // int **R = shortestPath_Dijkstra(G, v);
    int **R = DijkstraHeap(G, v);
    
    CCINFO *sortedDist = sortDist(R[0], G->vexnum);
    showSortedDist(G, sortedDist);
    
    // showShortestPath(G, v, R[0], R[1]);
    // int i = 0, k = 0, n = G->vexnum;
    // printf("%s to each vex's shortest:\n", getVex(G, v));
    // for(i=0; i<n; ++i)
    // {
    //     memset(u1, 0, sizeof(u1));
    //     sprintf(u1, ",%s,", getVex(G, i));
    //     // if(!strstr(",7,37,59,82,99,115,133,165,188,197,", u1))  continue;
        
    //     printf("%d[%s] -> %d[%s] shortest distinct = %d\n", v, getVex(G, v), i, getVex(G, i), R[0][i] < INT_MAX ? R[0][i] : -1);
    //     // k = i;
    //     // while(R[1][k] != -1)
    //     // {
    //     //     printf("%d[%s]<-%d[%s](%d)\t", k, getVex(G, k), R[1][k], getVex(G, R[1][k]), getArc(G, R[1][k], k)->weight);
    //     //     if(R[1][k] == v)
    //     //     {
    //     //         printf("\n");
    //     //         break;
    //     //     }   
    //     //     k = R[1][k];
    //     // }
    // }

}

int readFile(char *file)
{
    if(strcmp(file, "scc") == 0)
        countSCC();
    else if(strcmp(file, "dist") == 0)
        countDijkstra();

    return 0;
}