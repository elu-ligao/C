/*
 * @Author       : elu
 * @Date         : 2022-02-17 18:20:23
 * @LastEditTime : 2022-02-19 10:39:29
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../inc/btree2.h"

static int MBT = 0;         // m-阶B树的阶数
static int BTNODES = 1;     // 节点数
static int BTLEVEL = 1;     // 层数

/**
 * @descripttion:   找出最小下标i，使得 key < X.keys[i]
 * @return {*}
 * @param {BTree} q
 * @param {KEYTYPE} k
 */
int Search(BTree q, KEYTYPE k)
{
    int i = 0;
        
    while(i < q->n && k >= q->keys[i])
        ++i;

    return i;
}

/**
 * @descripttion:   B-树的搜索
 * @return {*}      {pt, i, tag}
 *                  查找成功，tag=1，指针pt所指节点中的第i个关键字等于key
 *                  否则tag=0，等于key的关键字应插入在指针pt所指节点中的第i个位置(原第i个关键字开始依次后移)
 * @param {BTree} T
 * @param {KEYTYPE} k
 */
SearchResult BTreeSearch(BTree T, KEYTYPE key)
{
    BTree p = T, q = NULL;  // p 指向待查节点，q指向双亲
    int found = 0, i = 0;
    SearchResult result = {NULL, 0, 0};

    // printf("BTreeSearch: ");
    while(p && !found)
    {
        // printf("[%s] -> parent[%s], ", p->keys, p->parent?p->parent->keys:"nil");
        i = Search(p, key);
        
        if(i<p->n && key == p->keys[i]) // 找到key
            found = 1;
        else    // 递归搜索对应子树
        {
            q = p;
            p = p->childs[i];
        }
    }
    // printf("\n");
    
    if(found)
    {
        result.pt = p;
        result.i = i;
        result.tag = 1;
    }
    else
    {
        result.pt = q;
        result.i = i;
        result.tag = 0;
    }
    
    return result;
}

BTree BTreeAllocNode()
{
    int m = MBT;
    BTree X = (BTree)calloc(1, sizeof(BTNode));
    if(!X)
    {
        perror("BTreeAllocNode error.\n");
        return NULL;
    }
    X->keys = (KEYTYPE *)calloc(m+10, sizeof(KEYTYPE));
    if(!X->keys)
    {
        perror("BTreeAllocNode keys error.\n");
        return NULL;
    }
    X->childs = (BTree *)calloc(m+10, sizeof(BTree));
    if(!X->childs)
    {
        perror("BTreeAllocNode childs error.\n");
        return NULL;
    }
    return X;
}

/**
 * @descripttion:   B-树的创建
 * @return {*}
 * @param {int} m
 */
BTree BTreeCreate(int m)
{
    if(m < 2)
    {
        perror("Check your B-Tree's m (m > 2).\n");
        return NULL;
    }
    else if(m > MAX_M)
    {
        perror("Please check B-Tree's m and MAX_M.\n");
        return NULL;
    }
    MBT = m;
    printf("MBT = %d\n", MBT);
    return BTreeAllocNode();
}

void PutKey(KEYTYPE *Keys, int i, KEYTYPE k)
{
    Keys[i] = k;
}

/**
 * @descripttion:   生成含(T, x, ap) 为新的根的根节点， q和ap作为子树指针
 * @return {*}
 * @param {BTree} q
 * @param {KEYTYPE} x
 * @param {BTree} ap
 */
BTree NewRoot(BTree q, KEYTYPE k, BTree ap)
{
    BTree X = BTreeAllocNode();
    X->n = 1;
    PutKey(X->keys, 0, k);
    X->childs[0] = q;
    X->childs[1] = ap;
    if(q)   q->parent = X;
    if(ap)  ap->parent = X;
    return X;
}

/**
 * @descripttion:   将x和ap分别插入到q->keys[i+1]和q->childs[i+1]
 * @return {*}
 * @param {BTree} q
 * @param {int} i
 * @param {KEYTYPE} x
 * @param {BTree} ap
 */
void Insert(BTree q, int i, KEYTYPE k, BTree ap)
{
    int j = i;
    // keys[0..i..] 下标i开始关键字依次后移一位
    for(j=q->n; j>i; --j)
        q->keys[j] = q->keys[j-1];

    // 第i个子树开始后面子树依次后移一位
    for(j=q->n; j>i; --j)
        q->childs[j+1] = q->childs[j];

    q->childs[i+1] = ap;
    if(ap)  ap->parent = q;

    PutKey(q->keys, i, k);    // k现在是q的keys[i]

    q->n += 1;  // q的关键字个数加1

    
    // for(j=0; j<=q->n; ++j)
    //     printf("q.c[%d] = [%s]\n", j, q->childs[j]?q->childs[j]->keys:"nil");

    return;    
}

/**
 * @descripttion:   将q.keys[mid+1..m-1], q.childs[mid+1..m]移入ap节点
 *                  q节点剩余前面0~mid-1 个key和0~mid个子树指针
 * @return {*}
 * @param {BTree} q
 * @param {int} mid
 * @param {BTree} ap
 */
void Split(BTree q, int mid, BTree ap)
{
    int m = MBT;
    int i = 0, j = 0;

    for(i=mid,j=0; i<m; ++i,++j)
    {
        ap->keys[j] = q->keys[i];
        q->keys[i] = 0;
    }
    ap->n = j;
    // printf("in Split: q=[%s], parent = [%s]\n", q->keys, q->parent?q->parent->keys:"nil");
    // ap->parent = q->parent;  
    q->n = mid;
    // ap 节点子树从1开始，对应原q的第mid+1个子树
    for(i=mid,j=1; i<m; ++i,++j)
    {
        // printf("Split: q.c[%d] = [%s], ", i+j, q->childs[i+j]?q->childs[i+j]->keys:"nil");
        ap->childs[j] = q->childs[1+i];     // 更新ap子树的父节点为ap本身，否则还是q的父节点
        if(ap->childs[j])   ap->childs[j]->parent = ap;
        // printf("ap.c[%d]=[%s]", j, ap->childs[j]?ap->childs[j]->keys:"nil");
        q->childs[1+i] = NULL;
    }
    // printf(", parent=[%s]\n", ap->childs[j]&&ap->childs[j]->parent?ap->childs[j]->parent->keys:"nil" );
    ap->parent = q->parent;
    // printf("in Split: ap=[%s], parent = [%s]\n", ap?ap->keys:"nil", ap&&ap->parent?q->parent->keys:"nil");

    return;
}

/**
 * @descripttion:   B-树的插入
 * @return {*}
 * @param {BTree} T
 * @param {KEYTYPE} k
 */
BTree BTreeInsert(BTree T, BTree q, int i, KEYTYPE k)
{
    int m = MBT;
    BTree ap = NULL, qp = NULL;
    int finished = 0;
    KEYTYPE x = k;

    // printf("q = %p\n", q);
    while(q && !finished)
    {
    // printf("q.keys = [%s], q.n = %d\n", q->keys, q->n);
        // 将x和ap分别插入到q->keys[i+1]和q->childs[i+1]
        Insert(q, i, x, ap);
    // printf("After Insert: q.keys = [%s], q.n = %d, parent = [%s]\n", q->keys, q->n, q->parent?q->parent->keys:"nil");
        if(q->n < m)    finished = 1;   // q关键字个数小于m，插入完成
        else    // 否则分裂节点q
        {
            int mid = (m+1) / 2;    // 取中间关键字，上升到父节点0~mid-1, mid, mid+1~m-1
            ap = BTreeAllocNode();
            
            x = q->keys[mid];
    // printf("Before Split: q.keys = [%s], q.n = %d, parent = [%s]\n", q->keys, q->n, q->parent?q->parent->keys:"nil");
            Split(q, mid, ap);  // 将q.keys[mid..m], q.childs[mid+1..m]移入ap节点
    // printf("After Split: q.keys = [%s], q.n = %d, parent = [%s]\n", q->keys, q->n, q->parent?q->parent->keys:"nil");
    // printf("ap.keys = [%s], ap.n = %d, parent = [%s]\n", ap->keys, ap->n, ap->parent?ap->parent->keys:"nil");

            BTNODES++;
            // qp 变更q的父节点，q上升到其父节点，原来的q
            qp = q;
            q = q->parent;
            qp->parent = q;
            // printf("q's parent [%s]\n", q?q->keys:"nil");
            if(q)
            {
                i = Search(q, x);
            }

        }
    }

    // T是空树(q初始值为NULL)，或者根节点已分裂为q和ap
    if(!finished)
    {
        // printf("NewRoot: T.keys = [%s], x = [%c], ap = [%s]\n", T->keys, x, ap?ap->keys:"");
        T = NewRoot(T, x, ap);  // 生成含(T, x, ap) 为新的根的根节点， T和ap作为子树指针
    // printf("NewRoot: T.keys = [%s], T.n = %d, T.c[0] = [%s]-parents[%s], T.c[1] = [%s]-parents[%s]\n", T->keys, T->n, T->childs[0]?T->childs[0]->keys:"nil", T->childs[0]->parent->keys, T->childs[1]?T->childs[1]->keys:"nil", T->childs[0]->parent->keys);
        BTLEVEL++;
        BTNODES++;
        
    }

// printf("BTreeInsert over: T=[%s], T.n = %d\n", T->keys, T->n);
//     int j;
//     for(j=0; j<=T->n; ++j)
//         printf("q.c[%d] = [%s]\n", j, T->childs[j]?T->childs[j]->keys:"nil");

    // printf("BTLEVEL = %d, BTNODES = %d\n", BTLEVEL, BTNODES);
    return T;
}


void TraverseKeys(BTree X, int nodeIdx)
{
    // return;
    if(0 == nodeIdx)
    {
        printf("\n\n");
    }
    if(X)
    {
        if(nodeIdx > 0) printf("  |  ");
        int i = 0;
        for(i=0; i<X->n; ++i)
        {
            if(i >= 1)   printf(" ");
            printf("%c", X->keys[i]);
        }
        printf(" [%s]", X->parent?X->parent->keys:"");
    }
    
}

void PutKeys(KEYTYPE *dst, KEYTYPE *src)
{
    memcpy(dst, src, strlen(src));
}

/**
 * @descripttion:   B-树的层级遍历
 * @return {*}
 * @param {BTree} T
 */
void BTreeTraverse(BTree T)
{
    int queueNum = 80;
    // BTree *Q = (BTree *)calloc(queueNum, sizeof(BTree));
    BTree Q[queueNum];
    int rear = 0, front = 0;
    Q[rear++] = T;

    int level = 0;  // 当前层数

// printf("BTNODES = %d, BTLEVEL = %d\n", BTNODES, BTLEVEL);
#if LEVEL_INF
    int keyinfoIdx = 0; // 层序遍历的节点索引
    struct NodeLevelInfo levelInf;
    levelInf.level = BTLEVEL;
    levelInf.nodeNum = BTNODES;
    levelInf.nodes = (int *)calloc(BTLEVEL, sizeof(int));
    levelInf.keyinfo = (struct Keyinfo *)calloc(BTNODES, sizeof(struct Keyinfo));
    
    levelInf.nodes[level] = 1;
    PutKeys(levelInf.keyinfo[keyinfoIdx].keys, T->keys);
    levelInf.keyinfo[keyinfoIdx].levle = level;
    levelInf.keyinfo[keyinfoIdx].node = 0;
    levelInf.keyinfo[keyinfoIdx].n = 1;
    levelInf.keyinfo[keyinfoIdx].flag = 1;
    keyinfoIdx++;
#else
    int *levelNodes = (int *)calloc(BTLEVEL, sizeof(int));
    levelNodes[0] = 1;
#endif
    
    int i = 0;
    int nodeIdx = 0;
    while(rear != front)
    {
        // printf("Q[front] = %p, level=%d, BTLEVEL [%d], ", Q[front], level, BTLEVEL);
        TraverseKeys(Q[front], nodeIdx++);
        if(Q[front] && level+1 < BTLEVEL)
        {
            // printf("\nQ[front] = %s, level=%d, BTLEVEL [%d]\n", Q[front]->keys, level, BTLEVEL);
#if LEVEL_INF
            levelInf.nodes[level+1] += Q[front]->n+1;    // 子树个数，关键字个数+1
#else
            levelNodes[level+1] += Q[front]->n+1;
#endif
            // 将内部节点的子树指针追加到队列
            for(i=0; i<=Q[front]->n; ++i)
            {
                Q[rear] = Q[front]->childs[i];
                // printf("Q[rear] in queue, rear = %d, [%s]\n", rear, Q[rear]?Q[rear]->keys:"");
#if LEVEL_INF
                if(Q[rear])
                {
                    PutKeys(levelInf.keyinfo[keyinfoIdx].keys, Q[rear]->keys);
                    levelInf.keyinfo[keyinfoIdx].levle = level + 1;
                    levelInf.keyinfo[keyinfoIdx].node = i;
                    levelInf.keyinfo[keyinfoIdx].n = Q[rear]->n;
                    levelInf.keyinfo[keyinfoIdx].flag = 1;
                    keyinfoIdx++;
                }
#endif                
                rear++;
                rear %= queueNum;
            }
        }
        // printf("nodeIdx = %d, levelInf.nodes[%d] = %d\n", nodeIdx, level, levelInf.nodes[level]);
#if LEVEL_INF
        if(nodeIdx == levelInf.nodes[level])
#else
        if(nodeIdx == levelNodes[level])
#endif
        {
            level++;
            nodeIdx = 0;
        }
        
        front++;
        front %= queueNum;
    }
    printf("\n");
    printf("front = %d, rear = %d, level = %d, BTLEVEL = %d, BTNODES = %d\n", front, rear, level, BTLEVEL, BTNODES);
        
    return;
}
