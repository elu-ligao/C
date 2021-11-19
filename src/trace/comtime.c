#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TIME_HMS	8
#define DATE_LEN	10

void GetTime_HHMMSS(char *sTime)
{
    time_t  timep;
    struct tm *tp;
    
    time(&timep);
    tp = localtime(&timep);

    memset(sTime, 0, TIME_HMS+1);
    sprintf(sTime, "%.2d%.2d%.2d", tp->tm_hour, tp->tm_min, tp->tm_sec);
}


void GetCurDate(char *sDate)
{
    time_t  timep;
    struct  tm  *tp;

    time(&timep);
    tp = localtime(&timep);

    memset(sDate, 0, DATE_LEN+1);
    sprintf(sDate, "%.4d%.2d%.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
}


void GetDateTime(char *sDateTime)
{
    char sTime[TIME_HMS+1] = {0};
    char sDate[DATE_LEN] = {0};

    GetTime_HHMMSS(sTime);
    GetCurDate(sDate);
    sprintf(sDateTime, "%s%s", sDate, sTime);
}
