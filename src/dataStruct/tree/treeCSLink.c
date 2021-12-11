#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treeCSLink.h"


/**
 * @descripttion:   新建CSNode的数据结点
 * @return {*}
 */
pElemCSTree newCSElem()
{
    pElemCSTree p = (pElemCSTree)calloc(1, sizeof(ElemCSTree));
    if(!p)
    {
        perror("calloc for ElemCSTree error.\n");
        return NULL;
    }
    return p;
}

/**
 * @descripttion:   新建CSTree 结点
 * @return {*}
 */
pCSTree newCSTree()
{
    pCSTree BT = (pCSTree)calloc(1, sizeof(CSNode));
    if(!BT)
    {
        perror("calloc for CSTree node error.\n");
        return NULL;
    }
    BT->data = newCSElem();
    if(!BT->data)   return NULL;
    return BT;
}

pForestLink newForestNode()
{
    pForestLink p = (pForestLink)calloc(1, sizeof(ForestNode));
    if(!p)
    {
        perror("calloc for ForestNode error.\n");
        return NULL;
    }
    p->tree = newCSTree();
    if(!p->tree)    return NULL;
    return p;
}

/**
 * @descripttion:   初始化一片森林, 作为头结点
 * @return {*}
 */
pForestLink initForest()
{
    pForestLink F = (pForestLink)calloc(1, sizeof(ForestNode));
    if(!F)
    {
        perror("calloc for ForestLink node error.\n");
        return NULL;
    }
    return F;
}


void copyCSTree(pCSTree BT, pCSTree *newBT)
{
    if(!BT)
    {
        newBT = NULL;
        return;
    }
    *newBT = newCSTree();
    // memcpy((*newBT)->data, BT->data, sizeof(ElemCSTree));
    (*newBT)->data->iVal = BT->data->iVal;
    (*newBT)->data->sVal = BT->data->sVal;
    copyCSTree(BT->firstchild, &(*newBT)->firstchild);
    copyCSTree(BT->nextsibling, &(*newBT)->nextsibling);
}

/**
 * @descripttion:   将T树添加到森林F
 * @return {*}
 * @param {pForestLink} F
 * @param {pCSTree} BT
 */
pForestLink addCSTree2Forest(pForestLink F, pCSTree BT)
{
    if(!F)
    {
        F = initForest();
        if(!F)  return NULL;
    }

    if(!BT) return F;
    
    pForestLink p = F;
    while(p->next)  // p移动到最后一颗树
    {
        p = p->next;
    }

    p->next = newForestNode();
    pCSTree tree = NULL;
    copyCSTree(BT, &tree);
    p->next->tree = tree;

    return F;
}

/**
 * @descripttion:   由汇总二叉树分解生成森林F的第一棵树
 * @return {*}
 * @param {pForestLink} BTF   ------  由多棵二叉树组成的森林生成的汇总二叉树
 */
pResolveCSTree getFirstTree(pCSTree BTF)
{
    if(!BTF)    return NULL;
    pResolveCSTree resolve = NULL;
    resolve->firstCSTree = BTF->firstchild;
    resolve->remainCSTree = BTF->nextsibling;
    return resolve;
}


/**
 * @descripttion:   二叉树转父子结点存储类型的树
 * @return {*}
 * @param {pCSTree} BT
 */
pTreeLink *pCSTree2pTreeLink(pCSTree BT)
{
    int n = BT->data->iVal;
    pTreeLink *bt = initTree(n);
    pElemCSTree *datas = traveseCSTreePreRoot(BT);      // 先根遍历次序
    if(!datas)  return NULL;
    bt = createTreePreRoot(datas, n<<1);
    return bt;
}

/**
 * @descripttion:   父子结点存储类型的树转二叉树的递归创建
 * @return {*}
 * @param {pCSTree} *cstRoot    为左子树或右兄弟创建的结点
 * @param {pTreeLink} *T        树结构
 * @param {int} root            创建结点的索引
 */
void createCSTreePreRoot(pCSTree *cstRoot, pTreeLink *T, int root)
{
    if(!T || root < 1 || root > T[0]->data->iVal)   return ;
    if(!*cstRoot)
        *cstRoot = newCSTree();
    if(!cstRoot)    return ;
    (*cstRoot)->data->sVal = T[root]->data->sVal;
    childLink child = T[root]->sibling;
    pCSTree sibling = NULL;
    if(child->next)
    {
        child = child->next;
        // printf("root = %d, child = %d\n", root, child->index);
        createCSTreePreRoot(&((*cstRoot)->firstchild), T, child->index);
        while(child->next)
        {
            sibling = *cstRoot;
            while(sibling->nextsibling)  sibling = sibling->nextsibling;
            // printf(" --- root = %d, child = %d, sibling = %d\n", root, child->index, child->next->index);
            createCSTreePreRoot(&sibling->nextsibling, T, child->next->index);
            child = child->next;
        }
    }  
}


/**
 * @descripttion:   父子结点存储类型的树转二叉树
 * @return {*}
 * @param {pTreeLink} *T
 */
pCSTree pTreeLink2CSTree(pTreeLink *T)
{
    if(!T)  return NULL;
    int n = T[0]->data->iVal;
    
    pCSTree BT = newCSTree();
    if(!BT) return NULL;
    BT->data->iVal = n;
    createCSTreePreRoot(&(BT->firstchild), T, 1);
    return BT;
}

/**
 * @descripttion:   由n棵二叉树组成的森林生成汇总二叉树
 * @return {*}
 * @param {pForestLink} F
 */
pCSTree pForestLink2CSTree(pForestLink F)
{
    if(!F || !F->next)  return NULL;
    pCSTree BT = newCSTree();
    pForestLink p = F->next;
    pCSTree bt = BT;
    copyCSTree(p->tree, &bt->firstchild);
    BT->data->iVal += 1;
    p = p->next;
    while(p)
    {
        copyCSTree(p->tree, &bt->nextsibling);
        p = p->next;
        bt = bt->nextsibling;
        BT->data->iVal += 1;
    }
    return BT;
}

// pCSTree pTreeLinks2CSTree(pTreeLink *TS, int n);    // 由n棵父子结点存储类型树的森林生成汇总二叉树

// pForestLink pCSTrees2Forest(pCSTree BTF);   // 由汇总二叉树分解生成n棵二叉树森林
// pTreeLink *pCSTrees2pTreeLinks(pCSTree BTF, int *n);   // 由汇总二叉树分解生成n棵父子结点存储类型的树


/**
 * @descripttion:   递归遍历给元素复制， 不采用#, 因为以父子存储结构的带#遍历顺序规则不一致
 * @return {*}
 * @param {pCSTree} BT
 * @param {pElemCSTree} *datas
 * @param {int} i
 */
// int cstreePreRoot(pCSTree BT, pElemCSTree *datas, int i)
// {
//     if(!BT)
//     {
//         // printf("#[%d]", i++);
//         // datas[i++]->sVal = '#';
//         return i;
//     } 
//     datas[i++]->sVal = BT->data->sVal;
//     // printf("%c[%d]", BT->data->sVal, i++);

//     // printf("root = %c, BT->firstchild = [%p]%c\n", BT->data->sVal, BT->firstchild, BT->firstchild ? BT->firstchild->data->sVal : ' ');
//     i = cstreePreRoot(BT->firstchild, datas, i);
//     // printf("root = %c, BT->nextsibling = [%p]%c\n", BT->data->sVal, BT->nextsibling, BT->nextsibling ? BT->nextsibling->data->sVal : ' ');
//     i = cstreePreRoot(BT->nextsibling, datas, i);
//     return i;
// }

int cstreePreRoot(pCSTree BT, pElemCSTree *datas, int i)
{    
    if(!BT)
    {
        // printf("#[%d] ", i++);
        datas[i++]->sVal = '#';
        return i;
    }
    datas[i++]->sVal = BT->data->sVal;
    // printf("%c[%d] ", BT->data->sVal, i++);
    i = cstreePreRoot(BT->firstchild, datas, i);
    if(BT->nextsibling)
    {
        i = cstreePreRoot(BT->nextsibling, datas, i);
        // printf("BT = %c, next = %c\n", BT->data->sVal, BT->nextsibling->data->sVal);
        if(BT->firstchild)
        {
            // printf("#[%d] ", i++);
            datas[i++]->sVal = '#';
            return i;
        }
    }
    return i;
}


/**
 * @descripttion:   先根次序遍历二叉树
 * @return {*}
 * @param {pCSTree} BT
 * @param {int} *n
 */
pElemCSTree *traveseCSTreePreRoot(pCSTree BT)
{
    int n = BT->data->iVal;
    printf("n = %d\n", n);
    n <<= 1;  // 输出带#则这里要放开 带#输出元素翻倍
    pElemCSTree *datas = (pElemCSTree *)calloc(n, sizeof(ElemCSTree));
    if(!datas)
    {
        perror("calloc for CSTree datas error.\n");
        return NULL;
    }
    int i = 0;
    for(i=0; i<n; ++i)
    {
        datas[i] = (pElemCSTree)calloc(1, sizeof(ElemCSTree));
        if(!datas[i])
        {
            perror("calloc for CSNode data error.\n");
            return NULL;
        }
    }
    i = cstreePreRoot(BT->firstchild, datas, 0);
    if(i%2)
    {
        datas[i]->sVal = '#';
    }
    
    return datas;
}

int cstforestPreRoot(pCSTForest BTF, pElemCSTree *datas, int i)
{    
    pCSTree p = BTF->firstchild;
    i = cstreePreRoot(p->firstchild, datas, i);
    if(i%2)
    {
        // printf("#[%d] ", i++);
        datas[i++]->sVal = '#';
    }
    
    p = BTF->nextsibling;
    
    while(p)
    {
        i = cstreePreRoot(p->firstchild, datas, i);
        if(i%2)
        {
            // printf("#[%d] ", i++);
            datas[i++]->sVal = '#';
        }
        p = p->nextsibling;
    }
    return i;
}

int getCSTForestNodes(pCSTForest BTF)
{
    pCSTForest p = BTF->firstchild;
    int n = p->data->iVal;
    p = BTF->nextsibling;
    while(p)
    {
        n += p->data->iVal;
        p = p->nextsibling;
    }
    return n;
}


pElemCSTree *traveseCSTForestPreRoot(pCSTForest BTF, int *num)
{
    int n =getCSTForestNodes(BTF);
    // printf("n = %d\n", n);
    n <<= 1;  // 输出带#则这里要放开 带#输出元素翻倍

    pElemCSTree *datas = (pElemCSTree *)calloc(n, sizeof(ElemCSTree));
    if(!datas)
    {
        perror("calloc for CSTree datas error.\n");
        return NULL;
    }
    int i = 0;
    for(i=0; i<n; ++i)
    {
        datas[i] = (pElemCSTree)calloc(1, sizeof(ElemCSTree));
        if(!datas[i])
        {
            perror("calloc for CSNode data error.\n");
            return NULL;
        }
    }
    i = cstforestPreRoot(BTF, datas, 0);
    *num = i;
    
    return datas;
}


/** 
 * @descripttion:   后根次序遍历二叉树
 * @return {*}
 * @param {pCSTree} BT
 */
char *traveseCSTreePostRoot(pCSTree BT)
{
/**
 * @brief 要获取带#号的，为每层结点创建栈 STR={A,B,C}, STA={D,E}, STB={}, STC={F}, STF={G,H,K}, STG={}, STH={}, STK={}
 *      stlink = R|STR->A|STA->D|STD->E|STE->B|STB->C|STC->F|STF->G|STG->H|SGH->K|STK->nil
 *          然后以根节点的栈依次出栈 work(R)
 *          work(e):
 *              st = findST(e); cout << e;
 *              while(e=pop(st)):   
 *                  work(e)
 *              cout << #; 
 * 
 * @brief 先转成父子存储结构的树T，后序遍历T
 **/

    pTreeLink *T = pCSTree2pTreeLink(BT);
    char *s = traverseTreePreRootPost(T);
    int i = 0, n = strlen(s);
    char *t = (char*)calloc(n>>1, sizeof(char));
    int k = 0;
    for(i=0; i<n; i+=2)
    {
        if(s[i] == '#') continue;
        // printf("%c ", s[i]);
        t[k++] = s[i];
    }
    // printf("\n");
    return t;
}

/**
 * @descripttion:   先序遍历森林
 * @return {*}
 * @param {pForestLink} F
 */
void traveseForestPreOrder(pForestLink F)
{
    if(!F)  return;
    // pResolveCSTree TF = getFirstTree(F);
    // pElemCSTree datas = NULL;
    // int i = 0, n = 0;
    // while (TF)
    // {
    //     datas = traveseCSTreePreRoot(TF->firstCSTree);
    //     i = 0, n = TF->firstCSTree->data->iVal;
    //     for(i=0; i<2*n; ++i)
    //         printf("%c ", datas[i]);
    //     printf("\n");
    //     TF = getFirstTree(TF->remainCSTree);
    // }
    pElemType *datas = NULL;
    int i, n;
    pForestLink p = F->next;
    while(p)
    {
        datas = traveseCSTreePreRoot(p->tree);
        i = 0, n = p->tree->data->iVal;
        for(i=0; i<2*n; ++i)
            printf("%c ", datas[i]->sVal);
        printf("\n");
        freeDatas(datas, n);
        p = p->next;
    }
}

/**
 * @descripttion:   后根遍历森林
 * @return {*}
 * @param {pForestLink} F
 */
void traveseForestPostOrder(pForestLink F)
{
    if(!F)  return;
    pForestLink p = F->next;
    char *s = NULL;
    char *t = (char *)calloc(1024, sizeof(char));
    while(p)
    {
        s = traveseCSTreePostRoot(p->tree);
        strcpy(t+strlen(t), s);
        p = p->next;
    }
    printf("%s\n", t);
    free(t);
    t = NULL;
}

/**
 * @descripttion:   销毁二叉树
 * @return {*}
 * @param {pCSTree} *BT
 */
void destroyCSTree(pCSTree *BT)
{
    if(!*BT) return;
    pCSTree p = *BT, child = (*BT)->firstchild, sibling = (*BT)->nextsibling;

    free(p->data);  p->data = NULL;
    free(p);    p = NULL;

    if(child)   destroyCSTree(&child);
    if(sibling) destroyCSTree(&sibling);
}


/**
 * @descripttion:   销毁森林
 * @return {*}
 * @param {pForestLink} *F
 */
void destroyForest(pForestLink *F)
{
    if(!*F) return;
    pForestLink p = *F, r;

    while(p)
    {
        r = p;
        p = p->next;
        destroyCSTree(&r->tree);
        free(r);
        r = NULL;
    }
}