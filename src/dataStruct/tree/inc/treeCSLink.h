
#ifndef _TREE_CHILD_SIBLING_LINK_H
#define _TREE_CHILD_SIBLING_LINK_H

#include "treePCLink.h"

/*  二叉树的
 *  孩子-兄弟法存储结构 
 *  firstchild  data    nextsibling    
**/

// typedef struct ElemCSTree
// {
//     int  iVal;
//     // char *sVal;
// } ElemCSTree, *pElemCSTree;

typedef ElemType ElemCSTree;
typedef pElemType pElemCSTree;

/**
 * @brief   pCSTree 二叉树和 pForestLink 森林均有头结点
 *              二叉树的头结点存放了结点数量信息
 *          pCSTForest 汇总二叉树也有头结点， 汇总二叉树的头结点存放了包含的二叉树的个数信息，
 *              其中包含的每棵二叉树均为带头结点的二叉树
 * 
 */
typedef struct CSNode   // 树的孩子-兄弟存储结构
{
    pElemCSTree data;
    struct CSNode *firstchild, *nextsibling;
} CSNode, *pCSTree;

typedef pCSTree pCSTForest;

typedef struct ForestNode   // 森林存储结构 
{
    pCSTree tree;   // 树结构
    struct ForestNode *next;   // 下一棵树根地址    
} ForestNode, *pForestLink;


typedef struct ResolveCSTree
{
    pCSTree firstCSTree;        // 分解二叉树为森林的第一棵树
    pCSTree remainCSTree;       // 除分解出去的第一棵树外余留下的二叉树
} ResolveCSTree, *pResolveCSTree;

pElemCSTree newCSElem();      // 新建CSNode的数据结点
pCSTree newCSTree();        // 新建CSTree 结点

pForestLink initForest();   // 初始化一片森林
pForestLink addCSTree2Forest(pForestLink F, pCSTree BT);     // 将T树添加到森林F
pResolveCSTree getFirstTree(pCSTree F);        // 由汇总二叉树分解生成森林F的第一棵树

void copyCSTree(pCSTree src, pCSTree *dst);   // 复制二叉树

pCSTree pTreeLink2CSTree(pTreeLink *T);     // 父子结点存储类型的树转二叉树
pTreeLink *pCSTree2pTreeLink(pCSTree BT);   // 二叉树转父子结点存储类型的树

pCSTForest pForestLink2CSTree(pForestLink F);   // 由n棵二叉树组成的森林生成汇总二叉树
pCSTForest pTreeLinks2CSTree(pTreeLink *TS, int n);    // 由n棵父子结点存储类型树的森林生成汇总二叉树

pForestLink pCSTrees2Forest(pCSTForest BTF);   // 由汇总二叉树分解生成n棵二叉树森林
pTreeLink *pCSTrees2pTreeLinks(pCSTForest BTF, int *n);   // 由汇总二叉树分解生成n棵父子结点存储类型的树

pElemCSTree *traveseCSTreePreRoot(pCSTree BT);      // 先根次序遍历二叉树
char *traveseCSTreePostRoot(pCSTree BT);    // 后根次序遍历二叉树


pElemCSTree *traveseCSTForestPreRoot(pCSTForest BTF, int *num);      // 先根次序遍历二叉树

void traveseForestPreOrder(pForestLink F);  // 先序遍历森林
void traveseForestPostOrder(pForestLink F); // 后根遍历森林

void destroyCSTree(pCSTree *BT);    // 销毁二叉树
void destroyForest(pForestLink *F); // 销毁森林

void freeDatas(pElemType *datas, int n);

#endif