/*
 * @Author       : elu
 * @Date         : 2022-02-18 11:59:36
 * @LastEditTime : 2022-02-19 10:03:32
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/btree2.h"


int main(int argc, char *argv[])
{
    int t = 3;

    char s[200] = {0};
    if(argc >= 2)
    {
        t = atoi(argv[1]);
    }
    if(argc >= 3)
    {
        strcpy(s, argv[2]);
    }

    BTree T = BTreeCreate(t);

    int i = 0; 
    int n = strlen(s);
    SearchResult R;

    printf("start insert.  s = %s\n", s);
    for(i=0; i<n; ++i)
    {
        R = BTreeSearch(T, s[i]);
        // printf("R.pt = %p, R.i = %d, R.tag = %d, s[i] = [%c]\n", R.pt, R.i, R.tag, s[i]);
        if(0 == R.tag)
            T = BTreeInsert(T, R.pt, R.i, s[i]);
        else
            printf("[%c] is existed in BTree, node is [%s], idx = %d\n", s[i], R.pt->keys, R.i);
    
    // BTreeTraverse(T);
    }

    BTreeTraverse(T);

    // KEYTYPE k;
    // while(1)
    // {
    //     printf("input key to delete: ");
    //     scanf("%c", &k);
    //     while(getchar()!='\n')  ;
        
    //     // printf("k = [%c]\n", k);
    //     Position position;
    //     // 务必先确认待删关键字在树中
    //     position = BTreeSearch(T, k);
    //     if(position)
    //         T = BTreeDelete(T, k);
    //     else
    //         printf("%c is not in b-tree.\n", k);
        
        
    //     BTreeTraverse(T);
    // }

    return 0;
}