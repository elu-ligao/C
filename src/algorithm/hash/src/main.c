/*
 * @Author       : elu
 * @Date         : 2022-02-20 17:43:24
 * @LastEditTime : 2022-02-20 20:14:15
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "../inc/hashSep.h"
#include "../inc/hashQuad.h"

int main(int argc, char *argv[])
{
    HashTable H = InitializeTable(3);

    printf("H.Tablesize = %d\n", H->TableSize);
    
    char s[20] = {0};
    while(1)
    {
        scanf("%s", s);
        // printf("s = [%s]\n", s);
        if(memcmp(s, "###", 3) == 0)    break;
        H = Insert(H, s);
        memset(s, 0, sizeof(s));
    }

    TraverseHashTable(H);

    return 0;
}