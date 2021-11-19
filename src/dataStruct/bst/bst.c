#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bst.h"

/*
 *  pbstElement newBstElement()
 *  desc:   新建数据节点
 *  input: 
 *  output: 
 *  return: success pbstElement 数据节点指针
 *          fail    NULL    
*/
pbstElement newBstElement()
{
    pbstElement e = (pbstElement)calloc(1, sizeof(bstElement));
    if(NULL == e)
    {
        perror("newBstElement error.\n");
        return NULL;
    }
    return e;
}

/*
 *  pbsTree newBstNode()
 *  desc:   创建二叉树节点
 *  input: 
 *  output: 
 *  return: success pbsTree bst节点指针
 *          fail    NULL
*/
pbsTree newBstNode()
{
    pbsTree pNode = (pbsTree)calloc(1, sizeof(bstNode));
    if(NULL == pNode)
    {
        perror("newBstNode error.\n");
        return NULL;
    }
    pNode->data = newBstElement();
    if(NULL == pNode->data)
    {
        return NULL;
    }
    return pNode;
}

/*
 *  pbsTree createBSTree(pbstElement elemlists)
 *  desc:   创建二叉树
 *  input:  pbstElement elemlists   数据域元素列表
 *  output: pbsTree root    bst根节点指针
 *  return: success pbsTree bst根节点指针
 *          fail    NULL
*/
pbsTree createBSTree(pbstElement elemlists, int n)
{
    pbsTree root = NULL;
    int i;
    for(i=0; i<n; ++i)
    {
        insertBst(&root, &elemlists[i], NULL);
    }
    return root;
}

/*
 *  pbsTree searchBst(pbsTree T, KeyType key)
 *  desc:   查找关键字域为key的节点
 *  input:  pbsTree T   bst根节点指针
 *          KeyType key 关键数据域
 *  output: 
 *  return: success pbsTree bst节点指针
 *          fail       NULL
*/
pbsTree searchBst(pbsTree T, pbstElement find)
{
    if(T)
    {
        if(T->data->key == find->key)       // 根节点关键域与待查找关键域相等，find it, return
        {
            return T;
        }
        else if(find->key < T->data->key)   // 待查找关键域小于根节点，往左子树继续查找
        {
            return searchBst(T->lchild, find);
        }
        else    // 否则往右子树查找 
        {
            return searchBst(T->rchild, find);
        }
    }
    return NULL;
}

/*
 *  pbsTree minBst(pbsTree T)
 *  desc:   返回最小键值的对象
 *  input: 
 *  output: 
 *  return: success pbsTree bst节点指针
 *          fail    NULL
*/
pbsTree minBst(pbsTree T)
{
    pbsTree p = NULL;
    pbsTree t = T;
    while(t)
    {
        p = t;
        t = t->lchild;
    }
    return p;
}

/*
 *  pbsTree maxBst(pbsTree T)
 *  desc:   返回最大键值的对象
 *  input: 
 *  output: 
 *  return: success pbsTree bst节点指针
 *          fail    NULL
*/
pbsTree maxBst(pbsTree T)
{
    pbsTree p = NULL;
    pbsTree t = T;
    while(t)
    {
        p = t;
        t = t->rchild;
    }
    return p;
}

/*
 *  pbsTree predecessorBst(pbsTree T, pbstElement key)
 *  desc:   返回键值仅次于(更小)给定键的对象
 *  input:  pbsTree T   bst根节点指针
 *          pbstElement key 指定的关键值    
 *  output: 
 *  return: success pbsTree bst键值仅次于(更小)给定键的对象节点指针
 *          fail    NULL
*/
pbsTree predecessorBst(pbsTree T, pbstElement key)
{
    pbsTree x = searchBst(T, key);      // 先找到关键域key的节点x
    if(x->lchild)                       // x有左子树，则左子树最大的节点就是仅次于x的节点
    {
        return maxBst(x->lchild);       // 返回左子树最大节点
    }
    // 否则沿着x向上遍历，第一次左拐的节点就是所求仅次于x的节点
    pbsTree p = x->parent;
    while (p)
    {
        if(p->rchild == x)      // x是p的右节点，即p是x左拐的父节点
            return p;   
        x = p;                  // 继续往上查找
        p = x->parent;
    }
    
    return NULL;
}

/*
 *  pbsTree successorBst(pbsTree T, pbstElement key)
 *  desc:   返回键值仅大于(更大)给定键的对象
 *  input:  pbsTree T   bst根节点指针
 *          pbstElement key 指定的关键值    
 *  output: 
 *  return: success pbsTree bst键值仅大于(更大)给定键的对象节点指针
 *          fail    NULL
*/
pbsTree successorBst(pbsTree T, pbstElement key)
{
    pbsTree x = searchBst(T, key);      // 先找到关键域key的节点x
    if(x->rchild)                       // x有右子树，则右子树最小的节点就是仅大于x的节点
    {
        return minBst(x->rchild);       // 返回右子树最小节点
    }
    // 否则沿着x向上遍历，第一次右拐的节点就是所求仅大于x的节点
    pbsTree p = x->parent;
    while (p)
    {
        if(p->lchild == x)      // x是p的左节点，即p是x右拐的父节点
            return p;   
        x = p;                  // 继续往上查找
        p = x->parent;
    }
    
    return NULL;
}

/*
 *  void outputSortedBst(pbsTree T)
 *  desc:   中序遍历
 *  input:  pbsTree T   bst根节点指针
 *  output: 中序遍历的结果
 *  return: 
*/
void outputSortedBst(pbsTree T, int *index, int rlFlat, int prtFlag)
{
    if(T)
    {
        if(T->lchild)
        {
            outputSortedBst(T->lchild, index, 1, prtFlag);
        }
        T->data->otherInfo.index = *index;
        T->data->otherInfo.rlFlag = rlFlat;
        *index += 1;
        if(prtFlag == 1)
            printf("key = %d, index = %d, rlFlag = %d, curLevel = %d, totalLevel = %d\n", 
            T->data->key, T->data->otherInfo.index, T->data->otherInfo.rlFlag, T->data->otherInfo.curLevel, T->data->otherInfo.totalLevel);
        else if(prtFlag == 2)
            printf("%-3d", T->data->key);
        if(T->rchild)
        {
            outputSortedBst(T->rchild, index, 2, prtFlag);
        }
    }
}

int countTreeLevel(pbsTree T, int total)
{
    int l = 0, r = 0;
    if(T)
    {
        l = countTreeLevel(T->lchild, total+1);
        r = countTreeLevel(T->rchild, total+1);
        return l > r ? l : r;
    }
    return total;
}

void updateLevelInfo(pbsTree T, int curLevel, int totalLevel)
{
    if(totalLevel <= 0)
    {
        totalLevel = 0;
        totalLevel = countTreeLevel(T, totalLevel);
        T->data->otherInfo.totalLevel = totalLevel;
        T->data->otherInfo.curLevel = 0;
    }
    pbsTree r = T;
    if(r)
    {
        if(r->lchild)
        {
            r->lchild->data->otherInfo.curLevel = curLevel;
            r->lchild->data->otherInfo.totalLevel = totalLevel;
            updateLevelInfo(r->lchild, curLevel+1, totalLevel);
        }
        if(r->rchild)
        {
            r->rchild->data->otherInfo.curLevel = curLevel;
            r->rchild->data->otherInfo.totalLevel = totalLevel;
            updateLevelInfo(r->rchild, curLevel+1, totalLevel);
        }
    }
}


/*
 *  void insertBst(pbsTree &T, pbstElement e)
 *  desc:   插入数据域
 *  input:  pbsTree T   bst根节点指针
 *          pbstElement e   数据域指针
 *  output: 
 *  return: success with 0 else failure
*/
int insertBst(pbsTree *T, pbstElement e, pbsTree p)
{
    if(!(*T))
    {
        pbsTree S = newBstNode();
        
        S->data->key = e->key;
        S->data->otherInfo = e->otherInfo;
        S->parent = p;
        *T = S;
        // 找到根节点，更新更节点总层数
        pbsTree r = p;
        pbsTree prev = S;
        int level = 1;
        while(r)
        {
            level++;
            prev = r;
            r = r->parent;
        }
        // now prev is tht root
        prev->data->otherInfo.totalLevel = prev->data->otherInfo.totalLevel > level ? prev->data->otherInfo.totalLevel : level;
        // update each node's level info
        updateLevelInfo(prev, 1, prev->data->otherInfo.totalLevel);

        return 0;
    }
    else if((*T)->data->key == e->key)
    {
        // fprintf(stderr, "elem key [%d] existed.\n", e->key);
        return -1;
    }
    else if((*T)->data->key > e->key)
    {
        return insertBst(&(*T)->lchild, e, *T);
    }
    else if((*T)->data->key < e->key)
    {
        return insertBst(&(*T)->rchild, e, *T);
    }
}

/*
 *  void deleteBst(pbsTree *T, pbstElement e)
 *  desc:   删除数据域
 *  input:  pbsTree T   bst根节点指针
 *          pbstElement e   数据域指针
 *  output: 
 *  return: success with 0 else failure
*/
int deleteBst(pbsTree *T, pbstElement e)
{
    // find e
    // printf("to be find element key = %d\n", e->key);
    pbsTree TT = *T;
    pbsTree S = searchBst(*T, e);
    // printf("S is %d[%p]\n", S ? S->data->key : -1, S);
    if(NULL == S)
        return -1;
    // printf("find the deleted element %d[%p], father is %d[%p]\n", S->data->key, S, S->parent ? S->parent->data->key : -1, S->parent);

    if(NULL == S->lchild && NULL == S->rchild)  // S is leaf node
    {
        // delete S
        if(S != *T) // S is not the root
        {
            if(S->data->otherInfo.rlFlag == 1)  // S is left child
            {
                S->parent->lchild = NULL;
            }
            else    // S is right child
            {
                S->parent->rchild = NULL;
            }
        }
    }   // end S is leaf node
    else if(S->lchild && S->rchild)     //  S have two children node
    {
        pbsTree leftMax = maxBst(S->lchild);
        if(leftMax->data->otherInfo.rlFlag == 1)    // S's left child's max node is left node
        {
            // leftMax is S->left
            pbsTree Sl = S->lchild;     // S has only left child
            S->lchild->parent = S->parent;
            S->lchild->rchild = leftMax->rchild;
            leftMax->rchild = S->rchild;
            S->rchild->parent = leftMax;
            if(leftMax->rchild)
            {
                leftMax->rchild->parent = S->lchild;
            }
            if(S != *T) // S is not the root
            {
                if(S->data->otherInfo.rlFlag == 1)  // S is left child
                {
                    S->parent->lchild = Sl;
                }
                else    // S is right child
                {
                    S->parent->rchild = Sl;;
                }
            }
            else
                TT = Sl;
        }   // end S's left child's max node is left node
        else    // S's left child's max node is right node
        {
            // change S and leftMax, then delete new S, connect new S's child if new S has
            
            // leftMax has only left child
            if(leftMax->lchild)
            {
                leftMax->lchild->parent = leftMax->parent;
            }
            leftMax->parent->rchild = leftMax->lchild;

            leftMax->lchild = S->lchild;
            S->lchild->parent = leftMax;
            leftMax->rchild = S->rchild;
            S->rchild->parent = leftMax;
            leftMax->parent = S->parent;

            if(S != *T) // S is not the root
            {
                if(S->data->otherInfo.rlFlag == 1)  // S is left child
                {
                    S->parent->lchild = leftMax;
                }
                else    // S is right child
                {
                    S->parent->rchild = leftMax;
                }
            }
            else
                TT = leftMax;
        }
    }   // end S have two children node
    else    // S have only one child
    {
        pbsTree Schild = S->lchild ? S->lchild : S->rchild;
        Schild->parent = S->parent;     // S's child's parent direct to S'parent
        if(S != *T) // S is not the root
        {
            if(S->data->otherInfo.rlFlag == 1)  // S is left child
            {
                S->parent->lchild = Schild;
            }
            else    // S is right child
            {
                S->parent->rchild = Schild;;
            }
        }
        else
            TT = Schild;
    }   
    // free S
    free(S->data);
    S->data = NULL;
    S->lchild = NULL;
    S->rchild = NULL;
    S->parent = NULL;
    free(S);
    S = NULL;

    *T = TT;
    updateLevelInfo(*T, 1, 0);
    return 0;
}

/*
 *  void freeBst(pbsTree *T)
 *  desc:   释放二叉树内存
 *  input:  pbsTree T   bst根节点指针
 *  output: 
 *  return: 
*/
void freeBst(pbsTree *T)
{
    pbsTree S = *T;
    pbsTree left, right;
    if(S)
    {
        left = S->lchild;
        right = S->rchild;
        
        free(S->data);
        S->data = NULL;
        S->parent = NULL;
        S->lchild = NULL;
        S->rchild = NULL;
        free(S);
        S = NULL;

        freeBst(&left);
        freeBst(&right);
    }
}


#if 1
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
void drawBsTree(pbsTree T, int prtFlag)
{
    int index = 0;
    outputSortedBst(T, &index, 0, prtFlag/10);

    int i, j, k;
    int start = 0, last = 0, totalLevel = T->data->otherInfo.totalLevel;
    int maxData = 1 << totalLevel;  // 最后一层最多节点个数
    int curMaxData = 0;             // 当前层最多节点个数
    pbsTree *curbfs = (pbsTree *)calloc(maxData, sizeof(pbsTree));    // 存储当前层的节点
    pbsTree *bfs = (pbsTree *)calloc(maxData, sizeof(pbsTree));    // 存储下一层的节点
    char level[maxData];         // 保存当前层有无数据标志
    char nextlevel[maxData];     // 保存下一层有无数据标志
    char sData[maxData*3], sLink[maxData*3];

    // 初始化根节点为下一层信息
    memset(nextlevel, 0, sizeof(nextlevel));
    nextlevel[0] = '1';
    memset(bfs, 0, sizeof(bfs));
    bfs[0] = T;
    for (i=1; i<=totalLevel; ++i)
    {
        last = 0;
        memset(level, 0, sizeof(level));
        memcpy(level, nextlevel, sizeof(level));
        memset(curbfs, 0, sizeof(pbsTree)*maxData);
        memcpy(curbfs, bfs, sizeof(pbsTree)*maxData);

        memset(nextlevel, '0', sizeof(nextlevel));
        nextlevel[sizeof(nextlevel)-1] = 0;
        memset(bfs, 0, sizeof(bfs));

        curMaxData = 1 << (i-1);
        // 输出数据并更新下一层信息
        index = 0;
        memset(sData, 0, sizeof(sData));
        for(j=0; j<curMaxData; ++j)
        {
            if(level[j] == '1')
            {
                start = curbfs[j]->data->otherInfo.index;
                for(k=0; k<start-last; ++k)
                {
                    strcat(sData, "   ");
                }
                last = start;

                // if(curbfs[j]->data->otherInfo.rlFlag == 2)
                // {
                //     sprintf(sData, "%s%-3d", sData, curbfs[j]->data->key);
                // }
                // else
                // {
                //     sprintf(sData, "%s%3d", sData, curbfs[j]->data->key);
                // }
                sprintf(sData, "%s%-3d", sData, curbfs[j]->data->key);
                last += 1;

                if(curbfs[j]->lchild)
                {
                    index = 2*j;    // 2*(j+1)-1-1
                    bfs[index] = curbfs[j]->lchild;
                    nextlevel[index] = '1';
                }
                if(curbfs[j]->rchild)
                {
                    index = 2*j+1;  // 2*(j+1)-1-1+1
                    bfs[index] = curbfs[j]->rchild;
                    nextlevel[index] = '1';
                }
            }
        }
        strcat(sData, "\n");
        printf("%s", sData);

        // 输出连接符  
        last = 0;
        memset(sLink, 0, sizeof(sLink));
        // if(i > 1)
        {
            for(j=0; j<(curMaxData<<1); ++j)
            {
                if(nextlevel[j] == '1')
                {
                    start = bfs[j]->data->otherInfo.index;
                    if(bfs[j]->data->otherInfo.rlFlag == 1)         // left
                    {
                        for(k=0; k<start-last; ++k)
                        {
                            strcat(sLink, "   ");
                        }

                        strcat(sLink, "|--");

                        for(k=start+1; k<bfs[j]->parent->data->otherInfo.index; ++k)
                        {
                            strcat(sLink, "---");
                        }

                        if(bfs[j]->parent->rchild)
                            strcat(sLink, "/\\-");
                        else
                            strcat(sLink, "/  ");
                        last = bfs[j]->parent->data->otherInfo.index + 1;
                    }   // end if left
                    else if(bfs[j]->data->otherInfo.rlFlag == 2)    // right
                    {
                        if(bfs[j]->parent->lchild)  // have left brother
                        {
                            for(k=0; k<start-last; ++k)
                            {
                                strcat(sLink, "---");
                            }
                            strcat(sLink, "-| ");
                            last = bfs[j]->data->otherInfo.index + 1;
                        }
                        else    // only right self
                        {
                            // get parent node
                            for(k=last; k<bfs[j]->parent->data->otherInfo.index; ++k)
                            {
                                strcat(sLink, "   ");
                            }
                            strcat(sLink, "  \\");

                            for(k=bfs[j]->parent->data->otherInfo.index; k<start-1; ++k)
                            {
                                strcat(sLink, "---");
                            }
                            strcat(sLink, "-| ");

                            last = bfs[j]->data->otherInfo.index+1;
                        }   
                    }   // end else if right
                }   // end if
            }   // end for
            strcat(sLink, "\n");
            printf("%s", sLink);
        }   // end if i > 1
    }

    index = 0;
    outputSortedBst(T, &index, 0, prtFlag%10);
    printf("\n");
}

#else

void drawBsTree(pbsTree T)
{
    int index = 0;
    outputSortedBst(T, &index, 0, 0);

    int i, j, k;
    int start = 0, last = 0, totalLevel = T->data->otherInfo.totalLevel;
    int maxData = 1 << totalLevel;  // 最后一层最多节点个数
    int curMaxData = 0;             // 当前层最多节点个数
    pbsTree *curbfs = (pbsTree *)calloc(maxData, sizeof(pbsTree));    // 存储当前层的节点
    pbsTree *bfs = (pbsTree *)calloc(maxData, sizeof(pbsTree));    // 存储下一层的节点
    char level[maxData];         // 保存当前层有无数据标志
    char nextlevel[maxData];     // 保存下一层有无数据标志

    // 初始化根节点为下一层信息
    memset(nextlevel, 0, sizeof(nextlevel));
    nextlevel[0] = '1';
    memset(bfs, 0, sizeof(bfs));
    bfs[0] = T;
    for (i=1; i<=totalLevel; ++i)
    {
        last = 0;
        memset(level, 0, sizeof(level));
        memcpy(level, nextlevel, sizeof(level));
        memset(curbfs, 0, sizeof(pbsTree)*maxData);
        memcpy(curbfs, bfs, sizeof(pbsTree)*maxData);

        memset(nextlevel, '0', sizeof(nextlevel));
        nextlevel[sizeof(nextlevel)-1] = 0;
        memset(bfs, 0, sizeof(bfs));

        curMaxData = 1 << (i-1);
        // 输出数据并更新下一层信息
        index = 0;
        for(j=0; j<curMaxData; ++j)
        {
            if(level[j] == '1')
            {
                
                start = curbfs[j]->data->otherInfo.index;
                for(k=0; k<start-last; ++k)  printf("   ");
                last = start;

                if(curbfs[j]->data->otherInfo.rlFlag == 2)
                    printf("%-3d", curbfs[j]->data->key);
                else
                    printf("%3d", curbfs[j]->data->key);
                last += 1;

                if(curbfs[j]->lchild)
                {
                    index = 2*j;    // 2*(j+1)-1-1
                    bfs[index] = curbfs[j]->lchild;
                    nextlevel[index] = '1';
                }
                if(curbfs[j]->rchild)
                {
                    index = 2*j+1;  // 2*(j+1)-1-1+1
                    bfs[index] = curbfs[j]->rchild;
                    nextlevel[index] = '1';
                }
            }
        }
        printf("\n");

        // 输出斜杠 
        last = 0;
        for(j=0; j<curMaxData; ++j)
        {
            if(level[j] == '1')
            {
                start = curbfs[j]->data->otherInfo.index;
                for(k=0; k<start-last; ++k)  printf("   ");
                last = start;

                if(curbfs[j]->lchild)
                {
                    printf(" / ");
                }
                else
                {
                    printf("   ");
                }
                last += 1;
                if(curbfs[j]->rchild)
                {
                    printf("\\  ");
                }
                else
                {
                    printf("   ");
                }
                last += 1;
            }
        }
        printf("\n");
    }
}

#endif


