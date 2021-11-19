#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "trace.h"


#define	GLOBAL_SECTION	"*GLOBAL*"
#define	LINE_LEN		512
#define	CFG_CNT_MAX		50
#define CFG_TAG_MAX		64
#define	CFG_VAR_MAX		512		

typedef struct{
	char tag[CFG_TAG_MAX];
	char var[CFG_VAR_MAX];
} t_cfg_def;

static t_cfg_def gtCfgList[CFG_CNT_MAX];
static int  nCfgCnt = 0;

int LoadSection(FILE *fp, char *section);


int LoadCfg(char *cfgfile, char *section)
{
	int 	rc;
	char	*sAbsFile;
	FILE	*fp;

	sAbsFile = getenv("CFGDIR");
	if(NULL == sAbsFile){
		sAbsFile = (char *)calloc(CFG_TAG_MAX, 1);
		sprintf(sAbsFile, "../cfg");
	}
	sprintf(sAbsFile, "%s/%s", sAbsFile, cfgfile);
	fp = fopen(sAbsFile, "r");
	if (fp == NULL){
		fprintf(stderr, "open sAbsFile[%s] error\n", sAbsFile);
		return -1;
	} 
	//printf("load cfgfile[%s] start...\n", sAbsFile);

/*
	rc = LoadSection(fp, GLOBAL_SECTION);
	if (rc){
		fclose(fp);
		return -1;
	}
*/
	rc = LoadSection(fp, section);
	if (rc){
		fclose(fp);
		return -1;
	}

	fclose(fp);


	return 0;
}


int LoadSection(FILE *fp, char *section)
{
	char line[LINE_LEN];
	int	 i, len, idx = 0;
	int  isSec = 0;

	len = strlen(section);
	if (len > CFG_TAG_MAX){
		return -1;
	}

	fseek(fp, 0, SEEK_SET);
	while(!feof(fp)){
		memset(line, 0, LINE_LEN);
		fgets(line, LINE_LEN, fp);
		/* skip 空行和注释 */
		if (line[0] == '\0' || line[0] == '\n' || line[0] == '\r' || line[0] == '#'){
			continue;
		}
		len = strlen(line);
		/* 截断后面的注释 */
		for (i=1; i<len; i++){
			if (line[i]=='\n' || line[i]=='#'|| line[i]=='\r'){
				line[i] = '\0';
				len = i;
				break;
			}
		}
		
		//printf("line = [%s]\n", line);

		if (line[0] == '['){
			line[0] = ' ';
			/* 只解读第一个匹配的字段 */
			for(i=1; i<strlen(line); ++i){
				if(line[i] == ' ' || line[i] == '\t')
					continue;
				break;
			}
			//printf("i = [%d], strlen(line) = [%d]\n", i, strlen(line));
			for(idx = i; idx < strlen(line); ++idx){
					//printf("line[%d] = [%c]\n", idx, line[idx]);
				if(line[idx] == ']'){
					line[idx] = '\0';
					break;
				}
				if(idx == strlen(line)){
					return -1;
				}
			}
			Trim(line);
			//printf("after trim line = [%s], section = [%s]\n", line, section);
			if (isSec){
				break;
			} else if (strcmp(line, section) == 0){
				isSec = 1;
				//printf("load section[%s]\n", section);
			}
		} else if (isSec){
			/* 解析匹配到的字段下的节点 */
			Trim(line);
			//printf("isSec = [%d], line = [%s]\n", isSec, line);
			for(idx=1; idx<len; idx++){
				if (line[idx] == '='){
					break;
				}
			}
			//printf("len = [%d],  = [%d]\n", len, idx);
			if (idx == len){
				return -1;
			} else {
				memcpy(gtCfgList[nCfgCnt].tag, line, idx);
				strcpy(gtCfgList[nCfgCnt].var, line + idx + 1);
				RightTrim(gtCfgList[nCfgCnt].tag);
				RightTrim(gtCfgList[nCfgCnt].var);
				LeftTrim(gtCfgList[nCfgCnt].var);
				//printf("load tag[%s], var[%s]\n", gtCfgList[nCfgCnt].tag, gtCfgList[nCfgCnt].var);
				nCfgCnt++;
			}
			if (nCfgCnt >= CFG_CNT_MAX){
				return -1;
			}
		}
	}
	
	if (isSec == 0 && strcmp(section, GLOBAL_SECTION) != 0){
			//printf("no section[%s] config info.\n", section);
		return -1;
	}

	return 0;
}

int GetCfg(char *tag, char *var)
{
	int i;
	for(i=0; i<CFG_CNT_MAX; i++){
		if (strcmp(gtCfgList[i].tag, tag) == 0){
			strcpy(var, gtCfgList[i].var);
			break;
		}
	}
	if (i == CFG_CNT_MAX){
		return -1;
	}
	return 0;
}
