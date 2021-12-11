#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treePCLink.h"


pTreeLink newTreeNode()
{
    pTreeLink p = (pTreeLink)calloc(1, sizeof(TreeNode));
    if(!p)
    {
        perror("new TreeNode Error.\n");
        return NULL;
    }
    p->data = (pElemType)calloc(1, sizeof(ElemType));
    if(!p->data)
    {
        perror("new ElemData Node Error.\n");
        return NULL;
    }
    p->sibling = (childLink)calloc(1, sizeof(CNode));
    if(!p->sibling)
    {
        perror("new ChildNode Error.\n");
        return NULL;
    }
    return p;
}

/**
 * @descripttion:   构造空树
 * @return {*}
 */
pTreeLink *initTree(int n)
{
    pTreeLink *T = (pTreeLink*)calloc(n+1, sizeof(pTreeLink));
    if(!T)
    {
        perror("initTree Error.\n");
        return NULL;
    }
    int i = 0;
    for(i=0; i<n+1; ++i)
    {
        T[i] = newTreeNode();
        if(!T[i])   return NULL;
    }
    return T;
}
/**
 * @descripttion:   根据n个结点的数据，采用先根遍历创建一个包含头结点的树，头结点存放树的总结点数
 * @return {*}
 * @param {pElemType} datas     R A D # E # # B # C F G # H # K # # # #
 * @param {int} n
 */
pTreeLink *createTreePreRoot(pElemType *datas, int n)
{
    pTreeLink *T = initTree(n);
    if(!*T) return NULL;
    int i = 0, j = 0;
    int parent = 0;
    int num = 0;
    while(j < n)
    {
        if(datas[j]->sVal != '#')     // 是data
        {
            ++i;
            childLink child = T[parent]->sibling;
            while (child->next)
            {
                child = child->next;    // 将父节点的孩子指针移动到最后一个
            }
            
            child->next = (childLink)calloc(1, sizeof(CNode));
            if(!child->next)
            {
                perror("Error For Sibling's Next.\n");
                return NULL;
            }
            child->next->index = i;     // 将当前结点作为父节点的孩子链在孩子链表后面

            memcpy(T[i]->data, datas[j], sizeof(ElemType));    // 当前元素赋值
            T[i]->parent = parent;      // 给当前结点的父节点标记
            
            parent = i;
            ++num;
        }
        else if(datas[j]->sVal == '#')     // 遇到#
        {
            parent = T[parent]->parent;
        }
        ++j;
    }
    
    T[0]->data->iVal = num;
    return T;
}
/**
 * @descripttion:   根据n个结点的数据，采用后根遍历创建一个包含头结点的树，头结点存放树的总结点数
 *              从最后倒数第二个开始往前，最后一个是根结点，不是#则其父节点是后一个，为#parent=parent.parent
 * @return {*}
 * @param {pElemType} datas     # # # D # E A # B # # # G # H # K F C R
 * @param {int} n
 */
pTreeLink *createTreePostRoot(pElemType *datas, int n)
{
    pTreeLink *T = initTree(n);
    if(!*T) return NULL;
    int i = (n+1)>>1, j = n-1;   // i从最后一个开始存放, j从datas最后一个开始读取
    int parent = 0;
    int num = 0;
    // 最后一个是根结点
    memcpy(T[i]->data, datas[j], sizeof(ElemType)); 
    T[i]->parent = parent;
    parent = i;
    ++num;
    while(--j >= 0)
    {
        if(datas[j]->sVal != '#')     // 是data
        {
            --i;
            childLink child = T[parent]->sibling;
            childLink nextchild = child->next;
            // 孩子要插在siling.next         
            childLink curchild = (childLink)calloc(1, sizeof(CNode));
            if(!curchild)
            {
                perror("Error For Sibling's Next.\n");
                return NULL;
            }
            curchild->index = i;     
            child->next = curchild;
            curchild->next = nextchild;

            memcpy(T[i]->data, datas[j], sizeof(ElemType));    // 当前元素赋值
            T[i]->parent = parent;      // 给当前结点的父节点标记            
            parent = i;
            ++num;
        }
        else if(datas[j]->sVal == '#')     // 遇到#
        {
            parent = T[parent]->parent;
        }
    }
    
    T[0]->data->iVal = num;
    return T;
}
/**
 * @descripttion:   销毁树
 * @return {*}  
 * @param {pTreeLink} *T
 */
void destroyTree(pTreeLink *T)
{
    int i = 0;
    int n = T[0]->data->iVal;
    childLink p = NULL, r = NULL;
    for(i=0; i<=n; ++i)
    {
        p = T[i]->sibling->next;
        while(p)
        {
            r = p;
            p = p->next;
            free(r);
            r = NULL;
        }

        free(T[i]->sibling);
        T[i]->sibling = NULL;
        free(T[i]->data);
        T[i]->data = NULL;
        T[i]->parent = 0;
    }
    free(*T);
    *T = NULL;
}

/**
 * @descripttion:   清空树
 * @return {*}
 * @param {pTreeLink} T
 */
void clearTree(pTreeLink *T)
{
    int i = 0;
    int n = T[0]->data->iVal;
    childLink p = NULL, r = NULL;
    for(i=0; i<=n; ++i)
    {
        p = T[i]->sibling->next;
        while(p)
        {
            r = p;
            p = p->next;
            free(r);
            r = NULL;
        }
        memset(T[i]->data, 0, sizeof(ElemType));
        T[i]->parent = 0;
    }
}

/**
 * @descripttion:   判断树是否为空
 * @return {*}
 * @param {pTreeLink} T
 */
bool treeIsEmpty(pTreeLink *T)
{
    return T[0]->data->iVal == 0;
}


int getDeepth(pTreeLink *T, int cur)
{
    if(cur < 1 || cur > T[0]->data->iVal)
    {
        return -1;
    }
    childLink p = T[cur]->sibling->next;
    if(!p)  return 1;   // 是叶子结点，cur深度为1
    int maxDeepth = 0, deepth = 0;
    while(p)
    {
        deepth = getDeepth(T, p->index);
        maxDeepth = deepth > maxDeepth ? deepth : maxDeepth;
        p = p->next;
    }
    return maxDeepth + 1;
}

/**
 * @descripttion:   树的深度
 * @return {*}
 * @param {pTreeLink} *T
 * @param {int} n
 */
int treeDepth(pTreeLink *T)
{
    int root = rootOfTree(T);

    return getDeepth(T, root);
}

/**
 * @descripttion:   返回树的根结点
 * @return {*}
 * @param {pTreeLink} *T
 */
int rootOfTree(pTreeLink *T)
{
    int i = 1;
    int root = 0;
    while(1)         // 找到根结点
    {
        if(T[i]->parent == 0)
        {
            root = i;
            break;
        }
        ++i;
    }    
    return root;
}

/**
 * @descripttion:   给cur结点赋值d
 * @return {*}
 * @param {pTreeLink} T
 * @param {int} cur
 * @param {pElemType} d
 */
void assign(pTreeLink *T, int cur, pElemType d)
{
    memcpy(T[cur], d, sizeof(ElemType));
}

/**
 * @descripttion:   返回cur结点的数据域
 * @return {*}
 * @param {pTreeLink} T
 * @param {int} cur
 */
pElemType value(pTreeLink *T, int cur)
{
    if(cur < 1 || cur > T[0]->data->iVal)   return NULL;
    return T[cur]->data;
}

/**
 * @descripttion:   返回cur结点的父节点
 * @return {*}
 * @param {pTreeLink} *T
 * @param {int} cur
 */
int praent(pTreeLink *T, int cur)
{
    if(cur < 1 || cur > T[0]->data->iVal)   return -1;
    return T[cur]->parent;
}

/**
 * @descripttion:   返回cur结点的左子树根结点
 * @return {*}
 * @param {pTreeLink} *T
 * @param {int} cur
 */
int leftChild(pTreeLink *T, int cur)
{
    if(cur < 1 || cur > T[0]->data->iVal)   return -1;
    if(T[cur+1]->parent != cur) return -1;  // 下一个存储单元的父节点是当前结点，则cur+1就是cur的最左子树
    return cur+1; 
}

/**
 * @descripttion:   返回cur结点的右子树根结点(定义最右子树为子树中的最后一个结点)
 * @return {*}
 * @param {pTreeLink} *T
 * @param {int} cur
 */
int rightChild(pTreeLink *T, int cur)
{
    if(cur < 1 || cur > T[0]->data->iVal)   return -1;
    int root = praent(T, cur);
    childLink p = T[root]->sibling->next;
    int hasLeft = 0;
    while(p)    // 仅有一个子树结点是认为左子树结点， 非右子树
    {
        hasLeft = 1;
        p = p->next;
    }
    if(hasLeft)
    {
        return p->index;
    }
    return -1;
}

/**
 * @descripttion:   对先根遍历顺序创建的先根遍历树先根遍历  R A D # E # # B # C F G # H # K # # # #
 *                  先根遍历#代表前面结点没有后续子结点
 *  parent = 0
 *  i from 1 to n or parent != 0: 
 *      if i.parent = parent, cout>>i, parent = i
 *      else cout>>#, parent = i.parent
 * @return {*}
 * @param {pTreeLink} *T
 */
char *traverseTreePreRootPre(pTreeLink *T)
{
    int parent = 0;
    int i = 1, n = T[0]->data->iVal;
    char *s = (char *)calloc(4*n+1, sizeof(char));
    int k = 0;
    while(i<=n || parent)
    {
        if(T[i]->parent == parent)
        {
            // printf("%c ", T[i]->data->sVal);
            s[k++] = T[i]->data->sVal;
            s[k++] = ' ';
            parent = i++;
        }
        else
        {
            // printf("# ");
            s[k++] = '#';
            s[k++] = ' ';
            parent = T[parent]->parent;
        }
    }
    // printf("\n");
    printf("traverseTreePreRootPre: %s\n", s);
    return s;
}


/**
 * @descripttion:   对先根遍历顺序创建的先根遍历树后根遍历
 *                  后根遍历树 对照先根遍历， #号和前面的一一对应
 *                  D # E # A # B # G # H # K # F # C # R #
 *          后跟遍历#代表后面结点没有后续子结点 # # # D # E A # B # # # G # H # K F C R
 * @return {*}
 * @param {pTreeLink} *T
 */
char *traverseTreePreRootPost(pTreeLink *T)
{
    int parent = 0;
    int i = 1, n = T[0]->data->iVal;
    char *s = (char *)calloc(4*n+1, sizeof(char));
    if(!s)  return NULL;
    int k = 0;
    while(i<=n || parent)
    {
        if(T[i]->parent == parent)
        {
            // printf("# ");
            s[k++] = '#';
            s[k++] = ' ';
            parent = i++;
        }
        else
        {
            // printf("%c ", T[parent]->data->sVal);
            s[k++] = T[parent]->data->sVal;
            s[k++] = ' ';
            parent = T[parent]->parent;
        }
    }
    // printf("\n");
    printf("traverseTreePreRootPost: %s\n", s);
    return s;
}

/** 
 * @descripttion:   对后根遍历顺序创建的后根遍历树采用后根遍历
 *                  # # # D # E A # B # # # G # H # K F C R
 * @return {*}
 * @param {pTreeLink} *T
 */
char *traverseTreePostRootPost(pTreeLink *T)
{
    int n = T[0]->data->iVal;
    int i = 4*n, k = n, parent = n;     // i是遍历输出字符串数组，k是树数组，parent初始指向根T[n]结点
    char *s = (char *)calloc(4*n+1, sizeof(char));
    s[--i] = ' ';                   // 先把根结点确定，在最后
    s[--i] = T[k]->data->sVal;
    --k;
    while(i >= 2 || parent)
    {
        s[--i] = ' ';
        if(T[k]->parent == parent)  // 当前k结点是parent的子节点
        {
            parent = k;
            s[--i] = T[k]->data->sVal;
            --k;
        }
        else    // 当前k结点不是parent的子节点, 输出#, 更新parent = k.parent
        {
            s[--i] = '#';
            parent = T[parent]->parent;
        }
    }
    printf("traverseTreePostRootPost: %s\n", s);
    return s;
}

void postRootTreePreTraverse(pTreeLink *T, int n, char *s)
{
    int start = strlen(s);
    s[start++] = T[n]->data->sVal;
    s[start++] = ' ';
    childLink p = T[n]->sibling->next;
    while(p)
    {
        postRootTreePreTraverse(T, p->index, s);
        p = p->next;
    }
    start = strlen(s);
    s[start++] = '#';
    s[start++] = ' ';
}

/**
 * @descripttion:   对后根遍历顺序创建的后根遍历树采用先根遍历
 * @return {*}
 * @param {pTreeLink} *T
 */
char *traverseTreePostRootPre(pTreeLink *T)
{
    int n = T[0]->data->iVal;
    char *s = (char *)calloc(4*n+1, sizeof(char));
    if(!s)  return NULL;
    postRootTreePreTraverse(T, n, s);
    printf("traverseTreePostRootPre: %s\n", s);
    return s;
}


/**
 * @descripttion:   按带双亲的孩子链表数组的数组顺序遍历树结构
 * @return {*}
 * @param {pTreeLink} *T
 */
void traverseTree(pTreeLink *T)
{
    int i = 1, n = T[0]->data->iVal;
    childLink p = NULL;
    printf("index  parent  data  sibing\n");
    for(i=1; i<=n; ++i)
    {
        printf("%-6d %-7d %c    ", i, T[i]->parent, T[i]->data->sVal);
        p = T[i]->sibling->next;
        while(p)
        {
            printf(" -> %-2d", p->index);
            p = p->next;
        }
        printf(" -> ^\n");
    }
}

// int insertChild(pTreeLink *T, pTreeLink c, int p, int i);    // 将c插在结点p的第i课子树下
// int deleteChild(pTreeLink *T, int p, int i); // 删除T中p结点的第i棵子树

