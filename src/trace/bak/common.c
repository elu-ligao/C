#include "msgs.h"
#include "ipc.h"
#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>


/****************************************
 *函数名: 	iBcd2Str
 *说明:		bcd 码转 string
 *入参:		unsigned char *bcd  bcd码
 *			int len				bcd码长度
 *出参:		char *str			转换换的字符串
 *返回值:	0					成功
 *			-1					失败
 ***************************************/
int iBcd2Str(unsigned char *bcd, int len, unsigned char *str)
{
	int i;
	int a = 0;

	for(i=0; i<len; ++i)
	{
		a = bcd[i];
		if(a>255) return -1;
		sprintf(str, "%s%c%c", str, chr_asc[a>>4], chr_asc[a&15]);
	}

	return 0;
}

int iStr2Bcd(unsigned char *str, int len, unsigned char *bcd)
{
	int i = 0;
	int c = 0;

	if(len%2)
	{
		str[len] = '0';
		len += 1;
	}
	for(i=0; i<len; i+=2)
	{
		c = 0;
		if(isxdigit(str[i])&&isxdigit(str[i+1]))
		{
			c = isdigit(str[i])?(str[i]-'0')<<4:(toupper(str[i])-'A'+10)<<4;
			c += isdigit(str[i+1])?(str[i+1]-'0'):(toupper(str[i+1])-'A'+10);
			bcd[i/2] = c;
//			printf("c=[%d][%02X]\n", c, c);
		}
		else
		{
			printf("str[%d]=[%c], str[%d]=[%c]\n", str[i], str[i+1]);
			return -1;
		}
	}

	return 0;
}

int iReversal(char *src, int len)
{
	if(NULL == src) return -1;
	int i;
	char tmp;
	
	for(i=0; i<len/2; ++i)
	{
		tmp = src[i];
		src[i] = src[len-i-1];
		src[len-i-1] = tmp;
	}

	return 0;
}

int itoa(int a, int base, char *str)
{
	char buf[32+1] = {0};
	int div, mod;

	if(base > 10 && base != 16)
		return -1;

	while(1)
	{
		mod = a % base;
		div = a / base;
		if(16 == base)
			sprintf(buf, "%s%c", buf, chr_asc[mod]);
		else
			sprintf(buf, "%s%d", buf, mod);
		if(div == 0)	break;
		a = div;
	}
	iReversal(buf, strlen(buf));
	memcpy(str, buf, strlen(buf));
	return 0;
}

int getBitMap(unsigned char *src, int len, char *map)
{
	int i, a;
	char buf[128+1] = {0};
	char tmp[4+1] = {0};
	
	iBcd2Str(src, len, map);
	for(i=0; i<len*2; ++i)
	{
		if(map[i]>='A'&&map[i]<='F')
			a = map[i] - 'A' + 10;
		else
			a = map[i] - '0';
//	printf("a=[%d]\t", a);
		memset(tmp, 0, sizeof(tmp));
		itoa(a, 2, tmp);
//	printf("tmp=[%04d]\n", atoi(tmp));
		sprintf(buf, "%s%04d", buf, atoi(tmp));
	}

	memcpy(map, buf, sizeof(buf));
	
	return 0;
}

int setBitMap(char *buf, char *wtlb)
{
	int i, last, len;
	char sbuf[128+1] = {0};
	char tmp[4+1] = {0};
	char uc[32+1] = {0};
	char *str, *token;
	
	memset(sbuf, '0', 128);
	for(str=buf; ;str=NULL)
	{
		token = strtok(str, " ");
		if(NULL == token)
			break;
		last = atoi(token);
		sbuf[last-1] = '1';
	}
	if(last > 64)
		sbuf[0] = '1';
	else
		sbuf[64] = 0;

	len = strlen(sbuf);
	for(i=0;i<len;i+=4)
	{
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, sbuf+i, 4);
		last = strtol(tmp, &str, 2);
		sprintf(uc, "%s%X", uc, last);
	}
	memcpy(wtlb, uc, len/4);
//	iStr2Bcd(uc, strlen(uc), wtlb);
	return len;
}


int get_field_2lvar_bcd(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	int bcdlen = 0, strlen = 0;
	unsigned char buf[512] = {0};

	iBcd2Str(src, 1, buf);
	strlen = atoi(buf);
	bcdlen = strlen%2==0 ? strlen/2 : strlen/2+1;
	memset(buf, 0, sizeof(buf));
	iBcd2Str(src+1, bcdlen, buf);
	switch(k+1)
	{
		case 2:
			memcpy(pstIpc->field_02, buf, strlen);
			break;
		case 32:
			memcpy(pstIpc->field_32, buf, strlen);
			break;
		case 33:
			memcpy(pstIpc->field_33, buf, strlen);
			break;
		case 35:
			memcpy(pstIpc->field_35, buf, strlen);
			break;
		case 40:
			memcpy(pstIpc->field_40, buf, strlen);
			break;
		default:
			return -1;
	}

	pstIpc->len[k] = strlen;
	
	return bcdlen+1;
}


int get_field_3lvar_bcd(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	int bcdlen = 0, strlen = 0;
	unsigned char buf[512] = {0};

	iBcd2Str(src, 2, buf);
	strlen = atoi(buf);
	bcdlen = strlen%2==0 ? strlen/2 : strlen/2+1;
	memset(buf, 0, sizeof(buf));
	iBcd2Str(src+2, bcdlen, buf);
	switch(k+1)
	{
		case 36:
			memcpy(pstIpc->field_36, buf, strlen);
			break;
		case 60:
			memcpy(pstIpc->field_60, buf, strlen);
			break;
		default:
			return -1;
	}

	pstIpc->len[k] = strlen;
	return bcdlen+2;
}


int get_field_2lvar_asc(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	int bcdlen = 0;
	unsigned char buf[512] = {0};

	iBcd2Str(src, 1, buf);
	bcdlen = atoi(buf);
	switch(k+1)
	{
		case 100:
			memcpy(pstIpc->field_100, src+1, bcdlen);
			break;
		case 102:
			memcpy(pstIpc->field_102, src+1, bcdlen);
			break;
		case 103:
			memcpy(pstIpc->field_103, src+1, bcdlen);
			break;
		default:
			return -1;
	}
	pstIpc->len[k] = bcdlen;

	return bcdlen+1;
}


int get_field_3lvar_asc(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	int bcdlen = 0;
	unsigned char buf[4] = {0};

	iBcd2Str(src, 2, buf);
	bcdlen = atoi(buf);
	switch(k+1)
	{
		case 48:
			memcpy(pstIpc->field_48, src+2, bcdlen);
			break;
		case 54:
			memcpy(pstIpc->field_54, src+2, bcdlen);
			break;
		case 55:
			memcpy(pstIpc->field_55, src+2, bcdlen);
			break;
		case 61:
			memcpy(pstIpc->field_61, src+2, bcdlen);
			break;
		default:
			return -1;
	}
	pstIpc->len[k] = bcdlen;

	return bcdlen+2;
}

int get_field_dc_bcd(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	char buf[256] = {0};
	int bcdlen = 0;
	int strlen = FIELD_LEN[k];
	if(strlen <= 0)	return -1;

	bcdlen = strlen%2 ? strlen/2+1 : strlen/2;
	iBcd2Str(src, bcdlen, buf);

	switch(k+1)
	{
		case 3:
			memcpy(pstIpc->field_03, buf, strlen);
			break;
		case 4:
			memcpy(pstIpc->field_04, buf, strlen);
			break;
		case 7:
			memcpy(pstIpc->field_07, buf, strlen);
			break;
		case 11:
			memcpy(pstIpc->field_11, buf, strlen);
			break;
		case 12:
			memcpy(pstIpc->field_12, buf, strlen);
			break;
		case 13:
			memcpy(pstIpc->field_13, buf, strlen);
			break;
		case 14:
			memcpy(pstIpc->field_14, buf, strlen);
			break;
		case 15:
			memcpy(pstIpc->field_15, buf, strlen);
			break;
		case 22:
			memcpy(pstIpc->field_22, buf, strlen);
			break;
		case 23:
			memcpy(pstIpc->field_23, buf, strlen);
			break;
		case 25:
			memcpy(pstIpc->field_25, buf, strlen);
			break;
		case 26:
			memcpy(pstIpc->field_26, buf, strlen);
			break;
		case 53:
			memcpy(pstIpc->field_53, buf, strlen);
			break;
		case 66:
			memcpy(pstIpc->field_66, buf, strlen);
			break;
		case 70:
			memcpy(pstIpc->field_70, buf, strlen);
			break;
		case 90:
			memcpy(pstIpc->field_90, buf, strlen);
			break;
		default:
			return -1;
	}

	pstIpc->len[k] = strlen;
	return bcdlen;
}


int get_field_dc_asc(int k, unsigned char *src, st_ipc_def *pstIpc)
{
	int bcdlen = FIELD_LEN[k];
	int i = 0;
	switch(k+1)
	{
		case 37:
			memcpy(pstIpc->field_37, src, bcdlen);
			break;
		case 38:
			memcpy(pstIpc->field_38, src, bcdlen);
			break;
		case 39:
			memcpy(pstIpc->field_39, src, bcdlen);
			break;
		case 41:
			memcpy(pstIpc->field_41, src, bcdlen);
			break;
		case 42:
			memcpy(pstIpc->field_42, src, bcdlen);
			break;
		case 43:
			memcpy(pstIpc->field_43, src, bcdlen);
			break;
		case 49:
			memcpy(pstIpc->field_49, src, bcdlen);
			break;
		case 52:
			memcpy(pstIpc->field_52, src, bcdlen);
			break;
		case 64:
			memcpy(pstIpc->field_64, src, bcdlen);
			break;
		case 128:
			memcpy(pstIpc->field_128, src, bcdlen);
			break;
		default:
			return -1;
	}
	pstIpc->len[k] = bcdlen;
	return bcdlen;
}

#if 0
int main()
{
	unsigned char src[32+1] = {0};
	unsigned char dst[128] = {0};
	int i;

//	memcpy(src, "\x12\x34\x23\xf1\x33\x63\xa8\xbb", 8);

	strcpy(dst, "2 3 4 7 11 12 13 22 23 26 35 41 43 49 52 53 55 60 128");
//	getBitMap((unsigned char *)src, 8, dst);

	setBitMap(dst, src);
/*
	for(i=0; i<16; ++i)
	{
		printf("\\x%02X", src[i]);
	}
	printf("\n");
*/
	printf("src->[%s], len = %d\n", src, strlen(src));
	char tmp[128] = {0};
	iStr2Bcd(src, strlen(src), tmp);
	memset(dst, 0, sizeof(dst));
	getBitMap(tmp, 16, dst);
	printf("dst->[%s]\n", dst);
	
	memset(src, 0, sizeof(src));
	memset(dst, 0, sizeof(dst));
	strcpy(src, "12345");
	iStr2Bcd(src, 5, dst);

	for(i=0;i<3;++i)
		printf("\\x%02X", dst[i]);
	printf("\n");

	return 0;
}
#endif
