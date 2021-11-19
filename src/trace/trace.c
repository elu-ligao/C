#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/timeb.h>
#include <ctype.h>
#include <time.h>
#include "trace.h"


int  nLogLevel = 0;
char sLogFile[TRACE_FILE_LEN+24] = {0};
char sFile[20] = {0};

static const char *g_level[] = {
	"NONE", "FATAL", "ERROR", "WARNING", "INFO", "DEBUG"
};


/*****************************************************************************
  **  函数名称: LogMsg
  **  功能描述: 
  **  当前版本: 1.0.0.0
  **  作    者:
  **  修    改：
  **  输入参数: int level	--trace level	
  **	
  **			int line	--log line in file	
  **
  **  输出参数:
  **  返回结果：0
  ****************************************************************************/
int LogMsg(char *logfile, int level, const char *filename, int line, const char *fmt, ...)
{
	va_list		ap;
	FILE		*fp;
	char		sCurTime[TIME_HMS + 1] = {0};
	char		sMillitm[4] = {0};
	// struct timeb	tp = {0};
	struct timespec tp = {0};

	if(level > nLogLevel)	return -1;

	GetTime_HHMMSS(sCurTime);
	// ftime(&tp);
	clock_gettime(CLOCK_REALTIME, &tp);
	snprintf(sMillitm, sizeof(sMillitm), "%03ld", tp.tv_nsec/1000000);

	fp = fopen(sLogFile, "ab+");
	if (fp != NULL){
		fprintf(fp, "[%08d][%.6s:%.3s][%-16s][%04d][%-7s]", 
			getpid(), sCurTime, sMillitm, filename, line, g_level[level]);
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
		fprintf(fp, "\n");
		fflush(fp);
	} else {
//		printf("fopen [%s] error[%d %s]", sLogFile, errno, strerror(errno));
		return -1;
	}
	
	fclose(fp);
	return 0;
}

int LogMsgHex(char *logfile, int level, const char *filename, int line, unsigned char *data, int len, char *info)
{
	int i;
	int prev;
	int curr;
	va_list		ap;
	FILE		*fp;
	char		sCurTime[TIME_HMS + 1] = {0};
	char		sMillitm[4] = {0};
	// struct timeb	tp = {0};
	struct timespec tp = {0};

	GetTime_HHMMSS(sCurTime);
	// ftime(&tp);
	clock_gettime(CLOCK_REALTIME, &tp);
	snprintf(sMillitm, sizeof(sMillitm), "%03ld", tp.tv_nsec/1000000);

	fp = fopen(sLogFile, "ab+");
	if (fp == NULL)
	{
		return -1;
	}

	fprintf(fp, "[%08d][%.6s:%.3s][%-16s][%04d][%-7s]: %-20s: len=[%d]\n",
		getpid(), sCurTime, sMillitm, filename, line, g_level[level], info, len);
	fprintf(fp, "---------------------------------------------------------------------------\n");
	prev = curr = 0;
	for (i = 0; i < len; i++)
	{
		if (i == (prev + 16))
		{
			i = prev;
			curr = prev + 16;
			fprintf(fp, "    |    ");
			for (; i < curr; i++)
			{
				if (isprint(data[i]))
					fprintf(fp, "%c", data[i]);
				else
					fprintf(fp, ".");
				if(i==(prev +7))
					fprintf(fp, " ");
			}
			fprintf(fp, "\n");
			prev = curr;
		}
		fprintf(fp, "%02x ", data[i]);
		if(i == (prev +7))
			fprintf(fp, " ");
	}

	if (i != curr)
	{
		curr = i;
		for (; i < (prev + 16); i++)
		{
			fprintf(fp, "   ");
			if(i == (prev +7))
				fprintf(fp, " ");
		}
		fprintf(fp, "    |    ");
		for (i = prev ; i < curr; i++)
		{
			if (isprint(data[i]))
				fprintf(fp, "%c", data[i]);
			else
				fprintf(fp, ".");
			if(i==(prev +7))
				fprintf(fp, " ");
		}
	}
	fprintf(fp, "\n");
	fprintf(fp, "---------------------------------------------------------------------------\n");

	fflush(fp);
}


int SetTraceLevel()
{
	int  rc;
	char level[2] = {0};
	char sCfgFile[TRACE_FILE_LEN] = {0};

	strcpy(sCfgFile, SYS_CFG);
	rc = LoadCfg(sCfgFile, GROUP_SECTION);
	if (rc){
		printf("############# LoadCfg error. rc = %d ###############\n", rc);
		return -1;
	}

	rc = GetCfg(TRACE_DFT_LEVEL, level);
	if (rc){
		return -1;
	}
	nLogLevel = atoi(level);

	return 0;
}


int InitTrace(char *filename)
{
	int 	rc;
	int		level = 5;

	SetTraceLevel();
	strcpy(sFile, filename);
/*
	rc = GetTraceFile(filename);
	if (rc) {
		return -1;
	}
*/
	return 0;
}

int GetTraceFile(char *filename)
{
	char 	sDate[DATE_LEN+1] = {0};
	char 	sDateTime[DATETIME_LEN+1] = {0};
	char	sPath[TRACE_FILE_LEN] = {0};
	char 	pLogDir[80] = {0};
	char	pCmd[TRACE_FILE_LEN+20] = {0};
	char	*p;

	GetCurDate(sDate);
	p = getenv("LOGDIR");
	if(NULL == p){
		sprintf(pLogDir, "../log");
	}else{
		strcpy(pLogDir, p);
	}
	sprintf(sPath, "%s/%s", pLogDir, sDate);
	sprintf(pCmd, "mkdir -p %s", sPath);
	system(pCmd);
	memset(sLogFile, 0, sizeof(sLogFile));
	if(NULL == filename)
		sprintf(sLogFile, "%s/%s.log", sPath, sFile);
	else
		sprintf(sLogFile, "%s/%s.log", sPath, filename);
	//printf("GetTraceFile [%s] success.\n", sLogFile);

	return 0;
}
