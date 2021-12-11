/*
 * @Author       : elu
 * @Date         : 2021-12-10 17:04:16
 * @LastEditTime : 2021-12-11 19:20:24
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


/**
 * @brief   哈夫曼树， 采用数组存储，0号数组作为头结点，哈夫曼树结点从1号元素开始存放
 *          一颗有n个叶子结点的哈夫曼树，共有2n-1个结点，故共申请2n个大小的数组
 *      weight     pathlen    parent  lchild    rchild
 *      结点权重    路径长度    父节点  左孩子结点 右孩子结点     
 */

typedef struct HTNode
{
    int weight;
    int pathlen;
    int parent, lchild, rchild;
} HTNode, *pHuffmanTree;

typedef char **pHuffmanCode;    // 哈夫曼编码(二维数组)

pHuffmanTree initHuffmanTree(int n);    // 初始化有n个叶子结点的哈夫曼树
pHuffmanTree createHuffmanTree(int *weights, int n);       // 创建哈夫曼树
int *getWeights(char *datas, int *n);                      // 获取权重数据
int *selectTwoSmallest(pHuffmanTree HT, int n);  // 在1-n之间选择parent=0且权值最小的两个结点并返回

void updatePathlen(pHuffmanTree HT, int m);    // 更新路径长度

void showHuffman(pHuffmanTree HT);      // 遍历哈夫曼树
int nodeWPL(pHuffmanTree HT, int m);    // 结点m的带权路径长度
int WPL(pHuffmanTree HT);       // 哈夫曼树的带权路径长度(所有叶子结点的带权路径之和)

pHuffmanCode createHufmanCode(pHuffmanTree HT); // 构造哈夫曼编码
void showHuffmanCode(pHuffmanCode HC);   // 打印哈夫曼编码


void freeHT(pHuffmanTree *HT);
void freeHC(pHuffmanCode HC);


char *bin2hexstr(char* bin);
unsigned char *hexstr2bcd(char *hexstr);
char *bcd2hexstr(unsigned char *bcd);
char *hexstr2bin(char *hexstr);


#endif