#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "treePCLink.h"

pElemType *getDatas(char *s, int n);
void freeDatas(pElemType *datas, int n);

int main(int argc, char *argv[])
{
    int n = 0;
    int opt;
    char params = '0';
    char line[80] = {0};

    while((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch (opt)
        {
        case 's':
            params = '1';
            strcpy(line, optarg);
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-s prerootseq] \n", argv[0]);
            return -1;
        }
    }

    if(params == '0')
        strcpy(line, "R A D # E # # B # C F G # H # K # # # #");
    n = (strlen(line)+1)>>1;
    pElemType *datas = getDatas(line, n);
    pTreeLink *T = createTreePreRoot(datas, n);

    traverseTree(T);
    char *s = traverseTreePreRootPre(T);
    free(s);
    s = traverseTreePreRootPost(T); 
    
    printf("deepth = %d\n", treeDepth(T));
    printf("all nodes are %d\n", T[0]->data->iVal);

    destroyTree(T);
    
    memset(line, 0, sizeof(line));
    // strcpy(line, "# # # D # E A # B # # # G # H # K F C R");
    strcpy(line, s);
    n = (strlen(line)+1)>>1;
    
    datas = getDatas(line, n);
    T = createTreePostRoot(datas, n);
    traverseTree(T);
    traverseTreePostRootPost(T);
    traverseTreePostRootPre(T);

    destroyTree(T);
    freeDatas(datas, n);
    return 0;
}

void freeDatas(pElemType *datas, int n)
{
    if(!datas)  return;
    int i = 0;
    for(i=0; i<n; ++i)
    {
        free(datas[i]);
        datas[i] = NULL;
    }
    free(datas);
    datas = NULL;
}

pElemType *getDatas(char *s, int n)
{
    pElemType *datas = (pElemType *)calloc(n, sizeof(pElemType));
    if(!datas)
    {
        perror("calloc for pElemType datas error.\n");
        return NULL;
    }
    int i = 0;
    for(i=0; i<n; ++i)
    {
        datas[i] = (pElemType)calloc(1, sizeof(ElemType));
        if(!datas[i])
        {
            perror("calloc for ElemType memory error.\n");
            return NULL;
        }
    }

    char *str, *token, *saveptr;
    for(i=0, str=s; ; str=NULL, ++i)
    {
        token = strtok_r(str, " ", &saveptr);
        if(!token)  break;
        datas[i]->sVal = token[0];
    }
    return datas;
}