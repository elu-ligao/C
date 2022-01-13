#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sorts.h"


/**
 * @descripttion:   直接插入排序
 *                  r[0] 作为哨兵, i=2->n, 如果r[i] < r[i-1], 则要将r[i]前移, 直到r[i] < r[0]
 *                  继续下一轮比较
 * @return {*}
 * @param {SqList} L
 */
void insertSort(SqList *L)
{
    // printf("(*L).length = %d\n", (*L).length);
    int i = 0, j = 0;
    for(i=2; i<=(*L).length; ++i)
    {
        if((*L).r[i].key < (*L).r[i-1].key)   // r[i] < r[i-1] 时才需要前移r[i]
        {
            (*L).r[0] = (*L).r[i];    // 哨兵站
            
            for(j=i-1; (*L).r[0].key < (*L).r[j].key; --j)    // i-1 到 小于哨兵的所有元素后移
                (*L).r[j+1] = (*L).r[j];  

            (*L).r[j+1] = (*L).r[0];      // 此时即是插入的适当位置
        }
    }
}


/**
 * @descripttion:   折半插入排序
 *                  在直接插入的基础上，插入时不是挨个比较，因为待插入的记录是已排好序的，故采用二分查找
 *                  找到合适的位置就插入，这里直接插入是O(n)，折半插入是O(log(2,n))
 * @return {*}
 * @param {SqList} L
 */
void binsertSort(SqList *L)
{
    int i = 0, j = 0;
    int low, high, mid;
    for(i=2; i<=(*L).length; ++i)
    {
        if((*L).r[i].key < (*L).r[i-1].key)
        {
            (*L).r[0] = (*L).r[i];
            low = 1;
            high = i-1;
            while(low <= high)  // 二分查找插入的位置
            {
                mid = low + ((high - low)>>1);
                if((*L).r[0].key < (*L).r[mid].key)
                    high = mid - 1;
                else
                    low = mid + 1;
            }
            
            for(j=i-1; j>=high+1; --j)  // 后移记录
                (*L).r[j+1] = (*L).r[j];
            
            (*L).r[high+1] = (*L).r[0];   // 插入r[i]
        }
    }
}

/**
 * @descripttion:   希尔排序
 *                  对增量为dk 的子序列做插入排序, 实为直接插入排序，只是数组是以dk为增量的跳跃数组的元素
 * @return {*}
 * @param {SqList} L
 * @param {int} dk
 */
void shellInsert(SqList *L, int dk)
{
    // printf("(*L).length = %d, dk = %d\n", (*L).length, dk);
    int i = 0, j = 0;
    for(i=dk+1; i<=(*L).length; ++i)
    {
        if((*L).r[i].key < (*L).r[i-dk].key)
        {
            (*L).r[0] = (*L).r[i];

            for(j=i-dk; j>0 && (*L).r[0].key < (*L).r[j].key; j-=dk)
                (*L).r[j+dk] = (*L).r[j];

            (*L).r[j+dk] = (*L).r[0];
            
            // showArray(L);
        }
    }
    
}

/**
 * @descripttion:       希尔排序主调用
 * @return {*}
 * @param {SqList} L
 * @param {int} *dk     增量数组, 逆序递减并且最后一个必为1，且序列中的值没有除1外的公因子
 * @param {int} k       增量数组个数
 */
void shellSort(SqList *L, int *dk, int k)
{
    // checkDK(int *dk, int k); 
    int i = 0;
    for(i=0; i<k; ++i)
        shellInsert(L, dk[i]);
}

/**
 * @descripttion:   冒泡排序
 *                  两两比较相邻关键字，如果是逆序的就交换他们
 * @return {*}
 * @param {SqList} L
 */
void bubbleSort(SqList *L)
{
    int m = (*L).length - 1;
    int flag = 1;   // 用来标记一趟排序过程中是否有交换操作
    int i = 0;
    RedType t;
    while(m>0 && flag)
    {
        flag = 0;   // 本趟排序过程初始化交换操作标志为0 
        for(i=1; i<=m; ++i)
        {
            if((*L).r[i].key > (*L).r[i+1].key)
            {
                flag = 1;   // 存在交换
                t = (*L).r[i];
                (*L).r[i] = (*L).r[i+1];
                (*L).r[i+1] = t;
            }
        }
        --m;    // r[m] 已确定，下一趟确定r[m-1]
    }
}



/**
 * @descripttion:   主调函数
 * @return {*}
 * @param {SqList} L
 */
void quickSort(SqList *L)
{
    qSort(L, 1, (*L).length);
}

/**
 * @descripttion:   对(*L).r[low..high] 做快速排序
 * @return {*}
 * @param {SqList} L
 * @param {int} low     起始位置
 * @param {int} high    结束位置
 */
void qSort(SqList *L, int low, int high)
{
    int pivotloc = 0;   // 枢轴位置
    if(low < high)
    {
        // 将(*L).r[low, hign] 一分为二，pivotloc是枢轴位置，前面元素的均小于(*L).r[pivotloc]，后面元素为大于(*L).r[pivotloc]的元素
        pivotloc = partition(L, low, high); 
        qSort(L, low, pivotloc);    // 对左子表递归排序
        qSort(L, pivotloc+1, high); // 对右子表递归排序
    }
}

/**
 * @descripttion:   将(*L).r[low, hign] 一分为二，pivotloc是枢轴位置
 *                  前面元素的均小于(*L).r[pivotloc]，后面元素为大于(*L).r[pivotloc]的元素
 *                  先将(*L).r[row] 保存到(*L).r[0]，从high 开始判断与(*L).r[0]的大小，如果更小，往row的位置移动
 *                  再    
 * @return {*}      返回是枢轴的索引
 * @param {SqList} L
 * @param {int} low
 * @param {int} high
 */
int partition(SqList *L, int low, int high)
{
    (*L).r[0] = (*L).r[low];  // 用子表的第一个记录作为枢纽值
    while(low < high)
    {
        while(low < high && (*L).r[high].key >= (*L).r[0].key)  --high;
        (*L).r[low] = (*L).r[high];   // 将比枢轴小的记录移动到前面
        while(low < high && (*L).r[low].key <= (*L).r[0].key) ++low;
        (*L).r[high] = (*L).r[low];   // 将比枢轴大的记录移动到后面
    }
    (*L).r[low] = (*L).r[0];  // 枢轴位置赋值
    return low;
}



/**
 * @descripttion:   简单选择排序
 *                  从(*L).r[i]开始(1<=i<=n-1)，每趟排序选择最小元素r[k]，与r[i]交换
 * @return {*}
 * @param {SqList} L
 */
void selectSort(SqList *L)
{
    int i = 0, j = 0, k = 0;
    RedType t;
    for(i=1; i<(*L).length; ++i)
    {
        for(k=i, j=i+1; j<=(*L).length; ++j)
            if((*L).r[j].key < (*L).r[k].key)
                k = j;
        
        if(k > i)   // 有比r[k] 更小的值
        {
            t = (*L).r[i];
            (*L).r[i] = (*L).r[k];
            (*L).r[k] = t;
        }
    }
}



/**
 * @descripttion:   堆排序 主调函数
 *                  将堆顶记录和当前未经排序子序列(*L).r[1..i]中最后一个记录互换
 *                  这样交换后r[i]是当前未排序的记录的最大记录
 *                  然后重新调整r[1..i-1]为大根堆，重复以上步骤
 * @return {*}
 * @param {SqList} L
 */
void heapSort(SqList *L)
{
    createHeap(L);  // 把无序序列(*L).r[1..(*L).length] 建成大根堆
    int i = 0;
    for(i=(*L).length; i>1; --i)
    {
        RedType x = (*L).r[1];     // 将堆顶记录和当前未经排序子序列(*L).r[1..i]中最后一个记录互换
        (*L).r[1] = (*L).r[i];
        (*L).r[i] = x;
        heapAdjust(L, 1, i-1);  // 重新调整r[1..i-1]为大根堆
    }
}


/**
 * @descripttion:   初建堆(大根堆)
 *                  只有一个结点的树必是堆，在完全二叉树中，所有序号大于n/2的结点都是叶子结点，
 *                  故以这些结点为根的子树均已是堆，这样，只需利用筛选法，从最后一个分支结点n/2开始，
 *                  依次将序号为n/2, n/2-1, ..., 2, 1 的结点作为根的子树都调整为大根堆即可
 * @return {*}
 * @param {SqList} L
 */
void createHeap(SqList *L)
{
    int n = (*L).length;
    int i = 0;
    for(i=n/2; i>0; --i)
        heapAdjust(L, i, n);
}


/**
 * @descripttion:   筛选法调整堆
 *                  在r[s+1..m]已经是堆的情况下，将r[s..m]调整为大根堆
 *                  沿着结点较大的子结点往下筛选，子结点值更大就交换父子结点的值，然后递归筛选交换后的子结点
 * @return {*}
 * @param {SqList} L
 * @param {int} s
 * @param {int} m
 */
void heapAdjust(SqList *L, int s, int m)
{
    RedType rc = (*L).r[s];
    int i = 0;
    for(i=s<<1; i<=m; i<<=1)
    {
        if(i<m && (*L).r[i].key < (*L).r[i+1].key)    ++i;    // 判断左右子结点，取key值更大的子结点用来交换筛选
        if(rc.key >= (*L).r[i].key)    break;  // 说明以r[s]为根的子树已经是堆，不必做任何处理
        (*L).r[s] = (*L).r[i];    // 交换父子结点(子结点的值给父结点，最后给子结点赋值，这样在循环内少2个交换步骤)
        s = i;  // 递归子树
    }
    (*L).r[s] = rc;
}



/**
 * @descripttion:   归并排序主调函数
 * @return {*}
 * @param {SqList} L
 */
void mergeSort(SqList *L)
{
    mSort((*L).r, (*L).r, 1, (*L).length);
}


/**
 * @descripttion:   归并排序 R[low..high] 归并排序后存入 T[low..high]
 * @return {*}
 * @param {RedType} *R
 * @param {RedType} *T
 * @param {int} low
 * @param {int} high
 */
void mSort(RedType *R, RedType *T, int low, int high)
{
    if(low == high)
    {
        T[low] = R[low];
    } 
    else
    {
        int mid = low + ((high-low)>>1);
        RedType *S = (RedType*)calloc(high+1, sizeof(RedType));
        mSort(R, S, low, mid);  // 前半部分递归
        mSort(R, S, mid+1, high);
        merge(S, T, low, mid, high);
        free(S);
        S = NULL;
    }
}


/**
 * @descripttion:   相邻两个有序子序列的归并
 *                  每次分别从R[low..mid]和R[mid+1..high]中取一个记录比较，较小者放入T[low..high]中
 *                  直到有一个列表为空， 最后将另一个列表剩余元素复制到T中
 * @return {*}
 * @param {RedType} *R
 * @param {RedType} *T
 * @param {int} low
 * @param {int} mid
 * @param {int} high
 */
void merge(RedType *R, RedType *T, int low, int mid, int high)
{
    int i = low, j = mid+1, k = low;
    while(i<=mid && j<=high)
    {
        if(R[i].key <= R[j].key)    T[k++] = R[i++];
        else    T[k++] = R[j++];
    }
    
    while(i<=mid)   T[k++] = R[i++];
    while(j<=high)  T[k++] = R[j++];
}








/**
 * @descripttion:   基数排序主调函数
 *                  对L做基数排序，使得L成为按关键字自小到大的有序静态链表，(*L).r[0]为头结点
 * @return {*}
 * @param {SLList} L
 */
void radixSort(SLList *L)
{
    int i = 0;
    for(i=0; i<(*L).recnum; ++i)   // 将L改造为静态链表,初始next指向下一个单元,最后一个元素的next指向0
        (*L).r[i].next = i+1;
    (*L).r[(*L).recnum].next = 0;

    ArrType f, e;   // 自队列的头尾指针
    for(i=0; i<(*L).keynum; ++i)   // 按最低位有限依次对各关键字进行分配和收集
    {
        distribute((*L).r, (*L).keynum-i-1, f, e);   // 第i趟分配 从低位到高位，i = (*L).keynum-i-1
        collect((*L).r, i, f, e);      // 第i趟收集
    }
}

// 这里key 就是
int getRadix(KeyTypeC k)
{
    return k - '0';
}


/**
 * @descripttion:   分发
 *                  按第i个关键字key[i] 建立RADIX 个子表，使同一表中记录的keys[i]相同
 *                  f[0..RADIX-1] e[0..RADIX-1] 分别指向各子表中第一个和最后一个记录
 * @return {*}
 * @param {SLCell} *r
 * @param {int} i
 * @param {ArrType} f
 * @param {ArrType} e
 */ 
void distribute(SLCell *r, int i, ArrType f, ArrType e)
{
    int j = 0;
    for(j=0; j<RADIX; ++j)  f[j] = 0;   // 初始化各子表
    
    int p = 0;
    for(p=r[0].next; p>0; p=r[p].next)  // 依次对r中的记录分配到对应的子表
    {
        j = getRadix(r[p].keys[i]); // 将记录中的第i个关键字映射到[0..RADIX-1]对应子表
        if(!f[j])  
            f[j] = p;   // 如果第j个子表为空，直接将p插入
        else    // e[j] 是第j个子表最后一个元素在r中的索引,r[e[j]] 就是子表中最后一个元素
            r[e[j]].next = p;   // 否则将p链到子表中最后一个元素的后面
        e[j] = p;   // 更新尾指针指向, f和e 都是存储的元素在r中的索引
    }
}

int getNextRadix(ArrType f, int j)
{
    while(++j < RADIX)
    {
        if(f[j] != 0)
        {
            break;
        }
    }
    return j;
}


/**
 * @descripttion:   收集
 *                  按keys[i] 自小至大地将f[0..RADIX-1]所指各子表依次连接成一个链表
 * @return {*}
 * @param {SLCell} *r
 * @param {int} i
 * @param {ArrType} f
 * @param {ArrType} e
 */
void collect(SLCell *r, int i, ArrType f, ArrType e)
{
    int j = 0;
    while(!f[j])  // 找到第一个非空子表
        j = getNextRadix(f, j);
    r[0].next = f[j];   // 将第一个子表链到r[0]后面
    int t = e[j];
    while (j<RADIX)
    {
        j = getNextRadix(f, j);
        while(j<RADIX-1 && !f[j])   // 找到下一个非空子表
            j = getNextRadix(f, j);
        
        if(j < RADIX && f[j])    // 非空子表存在就将其链到上一个子表后面
        {
            r[t].next = f[j];
            t = e[j];
        }
    }
    r[t].next = 0;  // t是最后一个子表的最后一个元素结点索引, 将其指向0
}