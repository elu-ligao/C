#ifndef _AVL_H_
#define _AVL_H_

// --------- 平衡二叉树的二叉链表存储表示 --------
typedef int     KeyType;    // 关键字类型
typedef struct InfoType
{
    int curLevel;       // 当前层数
    int totalLevel;     // 总层数
    int index;          // 中序遍历后排列索引位置
    int rlFlag;         // 属于父节点左右子树标志位 1-是父节点的左子树 2-是父节点的右子树
    int nodeNum;        // 以该节点为根的二叉树包含的节点数目
    int balanceFactor;  // 平衡因子 值为该节点左子树与右子树的深度只差 绝对值不应大于1 
    int height;         // 高度 
}InfoType;   // 其他数据项类型
/* 每个节点的数据域 */
typedef struct avlElement
{
    /* data */
    KeyType     key;        // 关键字项
    InfoType    otherInfo;  // 其他数据项
}avlElement, *pavlElement;      
/* 二叉树节点 */
typedef struct avlNode
{
    /* data */
    pavlElement data;          // 数据域
    struct avlNode *lchild, *rchild, *parent;
}avlNode, *pavlTree;

// 新建节点
pavlElement newAvlElement();

// 创建二叉树节点
pavlTree newAvlNode();

// 创建二叉树
pavlTree createAVLTree(pavlElement elemlists, int n);

// 查找关键字域为key的节点
pavlTree searchAvl(pavlTree T, pavlElement key);

// 返回最小键值的对象
pavlTree minAvl(pavlTree T);

// 返回最大键值的对象
pavlTree maxAvl(pavlTree T);

// 返回键值仅次于(更小)给定键的对象
pavlTree predecessorAvl(pavlTree T, pavlElement key);

// 返回键值仅大于(更大)给定键的对象
pavlTree successorAvl(pavlTree T, pavlElement key);

// 中序遍历
void outputSortedAvl(pavlTree T, int *index, int rlFlat, int prtFlag);

// 插入
pavlTree insertAvl(pavlTree root, pavlElement e, int rlFlag, pavlTree p);

// 删除
int deleteAvl(pavlTree *T, pavlElement e);

// 按索引查找
pavlTree selectAvl(pavlTree T, int index);

// 根据元素值返回索引
int rankAvl(pavlTree T, pavlElement e);

// 判断是否为平衡树
int isBalanceTree(pavlTree T);

// 左右镜像 
void lrMirror(pavlTree T);

// 释放
void freeAvl(pavlTree *T);

int updateLevelInfo(pavlTree T, int curLevel, int totalLevel);

/*
 *  void drawBsTree(pavlTree T, int prtFlag)
 *  desc:   绘制树形图 
 *  input:  pavlTree T   avl根节点指针
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
void drawAvlTree(pavlTree T, int prtFlag);

#endif