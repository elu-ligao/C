/*
 * @Author       : elu
 * @Date         : 2022-01-01 09:18:52
 * @LastEditTime : 2022-01-03 17:40:37
 * @LastEditors  : Please set LastEditors
 * @Description  :  
 */
#ifndef _SORTS_H
#define _SORTS_H

#define MAXSIZE 10000000      // 顺序表的最大长度

typedef int KeyType;    // 顺序表关键字类型

typedef struct 
{
    int x;
} InfoType;

typedef struct 
{
    KeyType key;
    // InfoType otherinfo; 
} RedType;

typedef struct 
{
    RedType r[MAXSIZE+1];       // r[0]用作哨兵或者闲置
    int length;     // 顺序表长度
} SqList;           // 顺序表类型

/* ****************************** 插入排序 ****************************** */
// 直接插入排序
void insertSort(SqList *L);

// 折半插入排序
void binsertSort(SqList *L);

// 希尔排序
void shellInsert(SqList *L, int dk);
// dk 数组是增量数组，应该使增量序列中的值递减，没有除1外的公因子，且最后一个必须为1
// 当增量序列dk[k] = 2^(t-k+1)-1，时间复杂度O(n^(3/2)), t 为排序趟数，1<=k<=log(2,n+1)
void shellSort(SqList *L, int *dk, int k);



/* ****************************** 交换排序 ****************************** */
// 冒泡排序     非常慢啊
void bubbleSort(SqList *L);

// 快速排序
void quickSort(SqList *L);   // 主调函数
void qSort(SqList *L, int low, int high);
int partition(SqList *L, int low, int high);


/* ****************************** 选择排序 ****************************** */
// 简单选择排序
void selectSort(SqList *L);

// 堆排序
void heapSort(SqList *L);
// 初建堆
void createHeap(SqList *L);
// 筛选法调整堆
void heapAdjust(SqList *L, int s, int m);


/* ****************************** 归并排序 ****************************** */
// 归并排序主调函数
void mergeSort(SqList *L);
// 归并排序 R[low..high] 归并排序后存入 T[low..high]
void mSort(RedType *R, RedType *T, int low, int high);
// 相邻两个有序子序列的归并
void merge(RedType *R, RedType *T, int low, int mid, int high);



/* ****************************** 基数排序 ****************************** */
#define MAXNUM_KEY  10       // 关键字项数的最大值
#define MAX_SPACE   MAXSIZE      // 最大记录数
#define RADIX       10      // 关键字基数，也是10禁止整数的基数
typedef char KeyTypeC;

typedef struct 
{
    KeyTypeC     keys[MAXNUM_KEY];       // 关键字
    // InfoType    otherinfo;              // 其他信息域
    int next;
} SLCell;   // 静态链表的结点类型

typedef struct 
{
    SLCell  r[MAX_SPACE+1];       // 静态链表的可利用空间，r[0]为头结点
    int     keynum;     // 记录的当前关键字个数
    int     recnum;     // 静态链表的当前长度
} SLList;   // 静态链表类型

typedef int ArrType[RADIX];      // 数组类型


// 基数排序主调函数
void radixSort(SLList *L);
// 分发
void distribute(SLCell *r, int i, ArrType f, ArrType e);
// 收集
void collect(SLCell *r, int i, ArrType f, ArrType e);







void showArray(SqList *L);

#endif