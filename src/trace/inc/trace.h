#ifndef _TRANCE_H_
#define _TRACE_H_

#define SYS_CFG			"sys.cfg"
#define GROUP_SECTION	"trace-log"
#define TRACE_DFT_LEVEL	"TRACE_DFT_LEVEL"

#define TIME_HMS    8
#define DATE_LEN    10
#define DATETIME_LEN	20


#define TRACE_FILE_LEN  128
extern char sLogFile[TRACE_FILE_LEN+24];
extern int nLogLevel;


#define TRACE_NONE 0

#define TRACE_FATAL 1

#define TRACE_ERROR 2

#define TRACE_WARNING 3

#define TRACE_INFO 4

#define TRACE_DEBUG 5

#define Log(A, format,args...)	\
((nLogLevel > 5 || sLogFile == NULL || nLogLevel < (A)) ? 0 : LogMsg(sLogFile, A, __FILE__, __LINE__, format, ##args))

#define LogFatal(format,args...)	Log(TRACE_FATAL, format, ##args)

#define LogError(format,args...)	Log(TRACE_ERROR, format, ##args)

#define LogWarning(format,args...)	Log(TRACE_WARNING, format, ##args)

#define LogInfo(format,args...)		Log(TRACE_INFO, format, ##args)
		
#define LogDebug(format,args...)	Log(TRACE_DEBUG, format, ##args)

#define LogHex(A, data, len, info) \
((nLogLevel > 5 || sLogFile == NULL || nLogLevel < (A)) ? 0 : LogMsgHex(sLogFile, A, __FILE__, __LINE__, data, len, info))

#define LogFatalHex(data, len, info)	    LogHex(TRACE_FATAL, data, len, info)

#define LogErrorHex(data, len, info)	    LogHex(TRACE_ERROR, data, len, info)

#define LogWarningHex(data, len, info)	    LogHex(TRACE_WARNING, data, len, info)

#define LogInfoHex(data, len, info)		    LogHex(TRACE_INFO, data, len, info)
		
#define LogDebugHex(data, len, info)	    LogHex(TRACE_DEBUG, data, len, info)	
		

extern void GetTime_HHMMSS(char *);
extern void GetCurDate(char *);
extern void GetDateTime(char *);
extern void RightTrim(char *str);
extern void LeftTrim(char *str);
extern void Trim(char *str);
extern int LoadCfg(char *cfgfile, char *section);
extern int GetCfg(char *tag, char *var);

extern int GetTraceFile(char *filename);
extern int InitTrace(char *filename);
extern int LogMsg(char *logfile, int level, const char *filename, int line, const char *fmt, ...);
extern int LogMsgHex(char *logfile, int level, const char *filename, int line, unsigned char *data, int len, char *info);

#endif
