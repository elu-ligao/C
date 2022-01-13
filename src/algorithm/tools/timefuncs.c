/*
 * @Author       : elu
 * @Date         : 2022-01-05 12:41:55
 * @LastEditTime : 2022-01-05 12:45:07
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#include "tools.h"

#include <sys/time.h>

float getTime()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday (&tv , &tz);

    float secs = tv.tv_sec % 1000 + tv.tv_usec / 1000000.0; 
    // printf("secs = %.6f\n", secs);
    return secs;
}

