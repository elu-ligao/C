/*
 * @Author       : elu
 * @Date         : 2021-12-08 12:31:12
 * @LastEditTime : 2021-12-09 23:11:34
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "treeCSLink.h"



pElemType *getDatas(char *s, int n);
void freeDatas(pElemType *datas, int n);


pForestLink createF()
{
    pForestLink F = initForest();
    
    char sbt1[20] = "A B # C # D # #";
    char sbt2[20] = "E F # #";
    char sbt3[20] = "G H # I J # # #";

    int n1 = (strlen(sbt1)+1)>>1;
    pElemType *datas1 = getDatas(sbt1, n1);
    pTreeLink *T1 = createTreePreRoot(datas1, n1);
    pCSTree BT1 = pTreeLink2CSTree(T1);

    int n2 = (strlen(sbt2)+1)>>1;
    pElemCSTree *datas2 = getDatas(sbt2, n2);
    pTreeLink *T2 = createTreePreRoot(datas2, n2);
    pCSTree BT2 = pTreeLink2CSTree(T2);
    

    
    int n3 = (strlen(sbt3)+1)>>1;
    pElemCSTree *datas3 = getDatas(sbt3, n3);
    pTreeLink *T3 = createTreePreRoot(datas3, n3);
    pCSTree BT3 = pTreeLink2CSTree(T3);


    F = addCSTree2Forest(F, BT1);
    F = addCSTree2Forest(F, BT2);
    F = addCSTree2Forest(F, BT3);


    freeDatas(datas1, n1);  datas1 = NULL;
    freeDatas(datas2, n2);  datas2 = NULL;
    freeDatas(datas3, n3);  datas3 = NULL;

    destroyTree(T1);
    destroyTree(T2);
    destroyTree(T3);

    destroyCSTree(&BT1);
    destroyCSTree(&BT2);
    destroyCSTree(&BT3);

    return F;
}

void testForest()
{
    pForestLink F = createF();
    printf("traveseForestPreOrder:\n");
    traveseForestPreOrder(F);

    
    printf("traveseForestPostOrder:\n");
    traveseForestPostOrder(F);

    int num = 0;
    pCSTForest BTF = pForestLink2CSTree(F);
    printf(" --- BTF = %d\n", BTF->data->iVal);
    printf("traveseCSTForestPreRoot:\n");
    pElemCSTree *datas = traveseCSTForestPreRoot(BTF, &num);
    printf("num = %d\n", num);
    int i = 0;
    for(i=0; i<num; ++i)
        printf("%c ", datas[i]->sVal);
    printf("\n");

    destroyCSTree(&BTF);
    destroyForest(&F);
}


int testCSTree(int argc, char *argv[])
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
    
    // 父子结构存储树转二叉树
    pCSTree BT = pTreeLink2CSTree(T);
    pElemCSTree *csdata = traveseCSTreePreRoot(BT);

    printf("traveseCSTreePreRoot: %d\n", BT->data->iVal);
    int i = 0;
    memset(line, 0, sizeof(line));
    for(i=0; i<2*BT->data->iVal; ++i)
    {
        printf("%c ", csdata[i]->sVal);
        line[i] = csdata[i]->sVal;
    }
    printf("\n");

    // 二叉树转父子结构存储树
    pTreeLink *bt2 = pCSTree2pTreeLink(BT);
    printf("traverseTree: \n");
    traverseTree(bt2);

    // 后根遍历二叉树 
    printf("traveseCSTreePostRoot: \n");
    traveseCSTreePostRoot(BT);


    destroyTree(bt2);
    destroyTree(T);
    destroyCSTree(&BT);
    freeDatas(datas, n);
    return 0;
}

int main(int argc, char *argv[])
{
    // testCSTree(argc, argv);

    testForest();
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
    printf("s = %s\n", s);
    for(i=0, str=s; ; str=NULL, ++i)
    {
        token = strtok_r(str, " ", &saveptr);
        if(!token)  break;
        datas[i]->sVal = token[0];
    }
    return datas;
}