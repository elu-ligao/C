#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "huffmanTree.h"

int testHuffman(int argc, char *argv[]);

void testHuffmanFile();

int main(int argc, char *argv[])
{
    //testHufman(argc, argv);

    testHuffmanFile();

    return 0;
}

int testHuffman(int argc, char *argv[])
{
    int n = 0;
    int opt;
    char line[200] = {0};

    while((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch (opt)
        {
        case 's':
            strcpy(line, optarg);
            break;
        
        default:
            fprintf(stderr, "Usage: %s [-s prerootseq] \n", argv[0]);
            return -1;
        }
    }

    int *weights = getWeights(line, &n);

    pHuffmanTree HT = createHuffmanTree(weights, n);
    if(!HT)
        return -1;

    showHuffman(HT);
    printf("WPL = %d\n", WPL(HT));

    int ridx = random()%n + 1;
    printf("index %d weight %d WPL %d\n", ridx, HT[ridx].weight, nodeWPL(HT, ridx));


    pHuffmanCode HC = createHufmanCode(HT);
    showHuffmanCode(HC);

    freeHT(&HT);
    freeHC(HC);
    free(weights);
    weights = NULL;

    return 0;
}



/**
 * @descripttion:   获取权重数据
 * @return {*}
 * @param {int} *n
 */
int *getWeights(char *datas, int *n)
{
    int i = 0;
    int num = 0;
    int *weights = NULL;
    // printf("datas: %s\n", datas);
    if(!strlen(datas))
    {
        srand(time(NULL));
        num = random()%21 + 10;
        weights = (int *)calloc(num, sizeof(int));
        for(i=0; i<num; ++i)
        {
            weights[i] = random()%100+1;
        }
    }
    else
    {
        int start = 0;
        for(i=0; i<strlen(datas); ++i)
        {
            if(!start && datas[i] != ' ')
            {
                start = 1;
            }
            if(start)
            {
                if(datas[i] == ' ' && datas[i+1] != ' ')
                {
                    ++num;
                } 
            }
        }
        if(datas[i-1] != ' ')   ++num;
        weights = (int *)calloc(num, sizeof(int));
        char *src, *token, *saveptr;
        for(i=0, src=datas; ; src=NULL, ++i)
        {
            token = strtok_r(src, " ", &saveptr);
            if(!token)  break;
            weights[i] = atoi(token);
        }
    }
    
    *n = i;    
    return weights;
}



int metchHC(pHuffmanCode HC, char *line)
{
    int n = atoi(HC[0]);
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        // printf("line = %s, HC[%d] = %s\n", line, i, HC[i]);
        if(strstr(line, HC[i]) == line)
            return i;
    }
    return -1;
}

void cnvtbit(char *in, unsigned char *out, size_t len)
{
    int i,j;
    int k;
	for(i=0;i<len;i+=8)
    {
		for(j=0;j<=7;j++)
		{
            k = in[i+j] - '0';
			if(i+j<len)
			{
				out[i/8] |= (k<<(7-j));
			}
			else
			{
				break;
			}
		}
        // printf(" -- out[%d] = %d\n", i/8, out[i/8]);
    }
}

void cnvtstr(unsigned char *in, char *out, int len)
{
    int i, j;
    unsigned char x;
    for(i=0; i<len; ++i)
    {
        x = in[i];
        // printf("x = %d\n", x);
        for(j=0; j<8; ++j)
        {
            // printf("x&(1<<(7-j)) = %d\n", x&(1<<(7-j)));
            if(x&(1<<(7-j)))
                out[(i<<3)+j] = '1';
            else
                out[(i<<3)+j] = '0';
        }
        // printf("out = %s\n", out);
        // break;
    }
}

int findIDX(pHuffmanTree HT, int character)
{
    int n = HT[0].weight;
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        if(HT[i].character == character)
            return i;
    }
    return -1;
}

int isAllZero(char *s)
{
    int n = strlen(s);
    for(int i=0; i<n; ++i)
        if(s[i] != '0')   
            return 0;
    return 1;
}

void decodeFile(pHuffmanTree HT, pHuffmanCode HC)
{
    char *codefile = "stdio.code.bit";
    FILE *fpcode = fopen(codefile, "rb");
    if(!fpcode) return;

    char *file = "./stdio.decode.bit.log";
    FILE *fp = fopen(file, "wb");
    if(!fp) return;

    unsigned char decodeLine[1024+1];
    char bitstr[8192];
    int i = 0, k = 0, len = 0;
    int idx = 0;
    char remain[16] = {0};
    unsigned char line[1024+1];

    memset(line, 0, sizeof(line));
    while(fread(line, sizeof(unsigned char), 2, fpcode) > 0)
    {
        len = line[0] << 8;
        len += line[1];
        // printf("len = %d\n", len);
        memset(line, 0, sizeof(line));
        fread(line, sizeof(unsigned char), len, fpcode);
        // if(len == 1)    printf("line = [%s]\n", line);
        memset(bitstr, 0, sizeof(bitstr));
        i = strlen(remain);
        // if(i>0) printf("------------ remain = %s\n", remain);
        strcpy(bitstr, remain);
        memset(remain, 0, sizeof(remain));
        cnvtstr(line, bitstr+i, len);
        // printf("------------- bitstr = %s\n", bitstr);
        // break;
        memset(decodeLine, 0, sizeof(decodeLine));
        i = 0;  k = 0;
        while(i<strlen(bitstr))
        {
            idx = metchHC(HC, bitstr+i);
            if(idx == -1)   
            {
                strcpy(remain, bitstr+i);
                // printf("remain = %s\n", remain);
                // printf("decode break.\n");
                break ;
            }
            if(len == 1 && isAllZero(bitstr+i))    break;
            decodeLine[k++] = HT[idx].character;
            i += strlen(HC[idx]);
        }
        // printf("decodeLine = [%s]\n", decodeLine);

        fwrite(decodeLine, 1, k, fp);
        memset(line, 0, sizeof(line));
    }

    fclose(fp);
    fclose(fpcode);
}




void codeFile(pHuffmanTree HT, pHuffmanCode HC)
{
    char *file = "./test_bst";
    char *codefile = "stdio.code.bit";
    FILE *fp = fopen(file, "rb");
    if(!fp) return;
    FILE *fpcode = fopen(codefile, "wb");
    if(!fpcode) return;
    unsigned char line[1024+1] = {0};
    // unsigned char codeLine[4096] = {0};
    char codeLine[8192] = {0};
    char remain[8] = {0};
    unsigned char codeBit[1024] = {0};
    unsigned char uclen[2] = {0};
    int i = 0, n = 0;
    int idx = 0, start = 0, len = 0;
    while((n = fread(line, 1, 1024, fp)))
    {
        memset(codeLine, 0, sizeof(codeLine));
        strcpy(codeLine, remain);
        start = strlen(remain);
        for(i=0; i<n; ++i)
        {
            // printf("line[%d] = %d\n", i, line[i]);
            idx = findIDX(HT, line[i]);
            if(idx < 1) return ;
            // printf("idx = %d, HC[idx] = %s\n", idx, HC[idx]);
            strcpy((char *)codeLine+start, HC[idx]);
            start += strlen(HC[idx]);
        }

        memset(remain, 0, sizeof(remain));
        len = (strlen(codeLine) >> 3) << 3;
        strcpy(remain, codeLine+(len));
        codeLine[len] = 0;

        // printf("codeLine = [%s]\n", codeLine);
        memset(codeBit, 0, sizeof(codeBit));

        cnvtbit(codeLine, codeBit, strlen(codeLine));
        len = (strlen(codeLine)+7)>>3;
        // printf("len = %d\n", len);
        uclen[0] = len>>8;
        uclen[1] = len&0xff;
        fwrite(uclen, 1, 2, fpcode);
        fwrite(codeBit, 1, len, fpcode);
    }

    if(strlen(remain))
    {
        // printf("remain = [%s]\n", remain);
        memset(codeBit, 0, sizeof(codeBit));

        cnvtbit(remain, codeBit, strlen(remain));
        len = (strlen(remain)+7)>>3;
        // printf(" ===== len = %d, codeBit[0] = %d\n", len, codeBit[0]);
        uclen[0] = len>>8;
        uclen[1] = len&0xff;
        fwrite(uclen, 1, 2, fpcode);
        fwrite(codeBit, 1, len, fpcode);        
    }

    fclose(fp);
    fclose(fpcode);
}


void testHuffmanFile()
{
    char *file = "./test_bst";

    FILE *fp = fopen(file, "rb");
    if(!fp) return;

    int weights[256] = {0};
    unsigned char line[1024] = {0};
    int i = 0, n = 0;
    int idx = 0;
    while((n = fread(line, 1, 1024, fp)))
    {
        for(i=0; i<n; ++i)
        {
            idx = line[i];
            weights[idx]++;
        }

        memset(line, 0, sizeof(line));
    }

    fclose(fp);

    pHuffmanTree HT = createHuffmanTree(weights, 256);
    pHuffmanCode HC = createHufmanCode(HT);

    // showHuffman(HT);
    // showHuffmanCode(HC);
    memset(weights, 0, sizeof(weights));
   
    codeFile(HT, HC);
    printf("codeFile success...\n");
    decodeFile(HT, HC);
    printf("decodeFile success...\n");
   
    freeHT(&HT);
    freeHC(HC);
}