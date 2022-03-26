/*
 * @Author       : elu
 * @Date         : 2022-01-22 23:20:57
 * @LastEditTime : 2022-01-23 00:15:47
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct P24
{
    int upper;
    int down;
} P24;

int count24(int *nums, int n)
{
    int i = 0, opr = 4;
    int a, b, c;
    struct P24 *p24 = (struct P24 *)calloc(n-1, sizeof(struct P24)); 
    for(i=0; i<n-2; ++i)    
    {
        p24[i].upper = nums[i];
        p24[i].down = 1;
    }
    
    for(i=n-1; i>=1; --i)
    {
        a = nums[i];
        b = nums[i-1];
    }
}


int main(int argc, char *argv[])
{
    int nums[4] = {0};
    int i = 0;
    int over = 0;

    while(1)
    {
        over = 0;
        for(i=0; i<4; ++i)
        {
            scanf("%d", &nums[i]);
            if(nums[i] == 0)    over++;
        }
        if(over == 4)  break;

        if(count24(nums, 4))   printf("YES\n");
        else    printf("NO\n");
    }

    return 0;
}