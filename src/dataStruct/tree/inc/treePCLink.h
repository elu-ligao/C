
#ifndef _TREE_P_CHILDLINK_H_
#define _TREE_P_CHILDLINK_H_

#include <stdbool.h>

#define DATA_CHAR_LEN   1
//  --- 带双亲的孩子链表
/***    index parent data childLink   ***/
/**                                                         T (头结点)
 *          0   -1      10  ->^                             |
 *          1   5       A   -> 3-> 5-> ^                    R
 *          2   5       B   -> ^                          / | \
 *          3   5       C   -> ^                        A   B   C
 *          4   1       D   -> 6-> ^                   / \      |
 *          5   0       R   -> 0-> 1-> 2-> ^          D   E     F
 *          6   1       E   -> ^                              / | \ 
 *          7   3       F   -> 7-> 8-> 9-> ^                 G  H  I
 *          8   7       G   -> ^
 *          9   7       H   -> ^
 *          10  7       K   -> ^
 * **/
typedef struct ElemType         // 数据域
{
    int iVal;
    char sVal;
} ElemType, *pElemType;

typedef struct CNode            // 子树域
{
    int index;
    struct CNode *next;
} CNode, *childLink;


typedef struct TreeNode         // 树结点 
{
    int parent;         // 父节点
    pElemType data;     // 数据域
    childLink sibling;  // 子树域
} TreeNode, *pTreeLink;

pTreeLink newTreeNode();    // 新建树结点
pTreeLink *initTree(int n);       // 构造空树
void destroyTree(pTreeLink *T);         // 销毁树
pTreeLink *createTreePreRoot(pElemType *datas, int n);   // 先根遍历创建树
pTreeLink *createTreePostRoot(pElemType *datas, int n);  // 后根遍历创建树
void clearTree(pTreeLink *T);        // 清空树
bool treeIsEmpty(pTreeLink *T);      // 判断树是否为空
int treeDepth(pTreeLink *T);         // 树的深度
int rootOfTree(pTreeLink *T);        // 返回树的根结点
void assign(pTreeLink *T, int cur, pElemType d);  // 给cur结点赋值d
pElemType value(pTreeLink *T, int cur);      // 返回cur结点的数据域
int praent(pTreeLink *T, int cur);     // 返回cur结点的父节点
int leftChild(pTreeLink *T, int cur);  // 返回cur结点的左子树根结点
int rightChild(pTreeLink *T, int cur); // 返回cur结点的右子树根结点
// int insertChild(pTreeLink *T, pTreeLink c, int p, int i);    // 将c插在结点p的第i课子树下
// int deleteChild(pTreeLink *T, int p, int i); // 删除T中p结点的第i棵子树
char *traverseTreePreRootPre(pTreeLink *T);         // 前根遍历前根顺序创建的树
char *traverseTreePreRootPost(pTreeLink *T);        // 后根遍历前根顺序创建的树
char *traverseTreePostRootPost(pTreeLink *T);       // 后根遍历后根顺序创建的树
char *traverseTreePostRootPre(pTreeLink *T);        // 前根遍后前根顺序创建的树
void traverseTree(pTreeLink *T);

#endif