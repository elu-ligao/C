/*
 * @Author       : elu
 * @Date         : 2021-12-06 21:54:39
 * @LastEditTime : 2021-12-07 16:46:56
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "sqBiTree.h"


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
        strcpy(line, "A B C D E F");
    n = (strlen(line)+1)>>1;
    pElemType *datas = getDatas(line, n);
    SqBiTree bt = createBiTreeLevelOrder(datas, n);
    freeDatas(datas, n);    
    printf("\ncreateBiTreeLevelOrder:\n");
    char *s = levelOrderTraverse(bt);
    free(s);    s = NULL;
    
    char *sPre = preOrderTraverse(bt);
    char *sIn = inOrderTraverse(bt);
    char *sPost = postOrderTraverse(bt);

    // 以层序遍历顺序创建的二叉树的前序遍历结果创建二叉树
    n = (strlen(sPre)+1)>>1;
    datas = getDatas(sPre, n);
    printf("\ncreateBiTreePreOrder:\n");
    SqBiTree preBt = createBiTreePreOrder(datas, n);
    freeDatas(datas, n);    
    levelOrderTraverse(preBt);
    preOrderTraverse(preBt);
    inOrderTraverse(preBt);
    postOrderTraverse(preBt);
    

    // 以层序遍历顺序创建的二叉树的后序遍历结果创建二叉树
    n = (strlen(sPost)+1)>>1;
    datas = getDatas(sPost, n);
    printf("\ncreateBiTreePostOrder:\n");
    SqBiTree postBt = createBiTreePostOrder(datas, n);
    freeDatas(datas, n);
    levelOrderTraverse(postBt);
    preOrderTraverse(postBt);
    inOrderTraverse(postBt);
    postOrderTraverse(postBt);

    destroyBiTree(bt);
    destroyBiTree(preBt);
    destroyBiTree(postBt);
    
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