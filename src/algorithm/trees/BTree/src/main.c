/*
 * @Author       : elu
 * @Date         : 2022-02-14 23:08:55
 * @LastEditTime : 2022-02-19 23:03:37
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/btree.h"


int main(int argc, char *argv[])
{
    int t = 3;

    char s[100] = {0};
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

    printf("start insert.  s = %s\n", s);
    for(i=0; i<n; ++i)
    {
        BTreeInsert(&T, s[i]);
        // printf("-- n = %d , %p-- \n", T->n, &T->n);
    }

    BTreeTraverse(T);

    KEYTYPE k;
    while(1)
    {
        printf("input key to delete: ");
        scanf("%c", &k);
        while(getchar()!='\n')  ;
        
        // printf("k = [%c]\n", k);
        Position position;
        // 务必先确认待删关键字在树中
        position = BTreeSearch(T, k);
        if(position)
            T = BTreeDelete(T, k);
        else
            printf("%c is not in b-tree.\n", k);
        
        
        BTreeTraverse(T);
    }

    return 0;
}