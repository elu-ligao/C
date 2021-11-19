#ifndef _BST_H_
#define _BST_H_

// --------- 二叉排序树的二叉链表存储表示 --------
typedef int     KeyType;    // 关键字类型
typedef struct InfoType
{
    int curLevel;       // 当前层数
    int totalLevel;     // 总层数
    int index;          // 中序遍历后排列索引位置
    int rlFlag;         // 属于父节点左右子树标志位 1-是父节点的左子树 2-是父节点的右子树
}InfoType;   // 其他数据项类型
/* 每个节点的数据域 */
typedef struct bstElement
{
    /* data */
    KeyType     key;        // 关键字项
    InfoType    otherInfo;  // 其他数据项
}bstElement, *pbstElement;      
/* 二叉树节点 */
typedef struct bstNode
{
    /* data */
    pbstElement data;          // 数据域
    struct bstNode *lchild, *rchild, *parent;
}bstNode, *pbsTree;

// 新建节点
pbstElement newBstElement();

// 创建二叉树节点
pbsTree newBstNode();

// 创建二叉树
pbsTree createBSTree(pbstElement elemlists, int n);

// 查找关键字域为key的节点
pbsTree searchBst(pbsTree T, pbstElement key);

// 返回最小键值的对象
pbsTree minBst(pbsTree T);

// 返回最大键值的对象
pbsTree maxBst(pbsTree T);

// 返回键值仅次于(更小)给定键的对象
pbsTree predecessorBst(pbsTree T, pbstElement key);

// 返回键值仅大于(更大)给定键的对象
pbsTree successorBst(pbsTree T, pbstElement key);

// 中序遍历
// void outputSortedBst(pbsTree T, int *index, int rlFlat, int prtFlag);

// 插入
int insertBst(pbsTree *T, pbstElement e, pbsTree p);

// 删除
int deleteBst(pbsTree *T, pbstElement e);

// 释放
void freeBst(pbsTree *T);

/*
 *  void drawBsTree(pbsTree T, int prtFlag)
 *  desc:   删除数据域
 *  input:  pbsTree T   bst根节点指针
 *          int prtFlag   打印标志  (1-详细信息, 2-中序遍历)
 *              0   - 只打印树形节点
 *              1   - 树形节点 + 节点详细信息
 *              2   - 树形节点 + 中序遍历结果
 *              10  - 输出节点详细信息 + 树形节点
 *              12  - 输出节点详细信息 + 树形节点 + 中序遍历结果
 *              
 *  output: 
 *  return: success with 0 else failure
*/
void drawBsTree(pbsTree T, int prtFlag);

#endif