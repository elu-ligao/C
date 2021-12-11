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

void codeFile(pHuffmanTree HT, pHuffmanCode HC)
{
    char *file = "./stdio.log";
    char *codefile = "stdio.code.bcd";
    FILE *fp = fopen(file, "r");
    if(!fp) return;
    FILE *fpcode = fopen(codefile, "w");
    if(!fpcode) return;
    char line[1024] = {0};
    // unsigned char codeLine[4096] = {0};
    char codeLine[4096] = {0};
    int i = 0, n = 0;
    int idx = 0, start = 0;
    char *hexstr ;
    unsigned char *bcd;
    while(fgets(line, 1024, fp))
    {
        memset(codeLine, 0, sizeof(codeLine));
        // printf("[%s]", line);
        // start = 2;
        start = 0;
        n = strlen(line);
        for(i=0; i<n; ++i)
        {
            idx = line[i];
            strcpy((char *)codeLine+start, HC[idx]);
            start += strlen(HC[idx]);
        }
        // printf("start = %d\n", start);
        // start -= 2;
        // codeLine[0] = start >> 8;
        // codeLine[1] = start & 0xFF;
        // printf("codeLine[0] = %d, codeLine[1] = %d\n", codeLine[0], codeLine[1]);
        // fwrite(codeLine, 1, start+2, fpcode);

        // printf("codeLine = %s\n", codeLine);
        hexstr = bin2hexstr(codeLine);
        // printf("bin2hexstr success..hexstr = %s\n", hexstr);
        bcd = hexstr2bcd(hexstr);
        // printf("hexstr2bcd success..\n");  
        n = bcd[0]<<4;
        n += bcd[1];
        fwrite(bcd, 1, n+2, fpcode);
        memset(line, 0, sizeof(line));
        free(hexstr);   hexstr = NULL;
        free(bcd);  bcd = NULL;
    }
    fclose(fp);
    fclose(fpcode);
}

int metchHC(pHuffmanCode HC, char *line)
{
    int n = atoi(HC[0]);
    int i = 0;
    for(i=1; i<=n; ++i)
    {
        if(strstr(line, HC[i]) == line)
            return i;
    }
    return -1;
}

void decodeFile(pHuffmanTree HT, pHuffmanCode HC)
{
    char *codefile = "stdio.code.bcd";
    FILE *fpcode = fopen(codefile, "r");
    if(!fpcode) return;

    char *file = "./stdio.decode.bcd.log";
    FILE *fp = fopen(file, "w");
    if(!fp) return;

    char *decodeLine;
    int i = 0, n = 0, k = 0;
    int idx = 0;

    unsigned char *line;
    unsigned char lenLine[2+1] = {0};
    char *hexstr, *binstr;
    while(1)
    {
        memset(lenLine, 0, sizeof(lenLine));
        if(2 == fread(lenLine, sizeof(unsigned char), 2, fpcode))
        {
            n = lenLine[0] << 4;
            n += lenLine[1];
            
            line = (unsigned char *)calloc(n+2+1, sizeof(unsigned char));
            memcpy(line, lenLine, 2);
            if(n == fread(line+2, sizeof(unsigned char), n, fpcode))
            {
                // memset(decodeLine, 0, sizeof(decodeLine));
                hexstr = bcd2hexstr(line);
                // printf("hexstr = %s\n", hexstr);
                binstr = hexstr2bin(hexstr);
                i = 0;  k = 0, n = strlen(binstr);
                
                decodeLine = (char *)calloc(n+1, sizeof(char));
                while(i<n)
                {
                    idx = metchHC(HC, binstr+i);
                    if(idx == -1)   
                    {
                        printf("decode error.\n");
                        return ;
                    }
                    decodeLine[k++] = idx;
                    i += strlen(HC[idx]);
                }

                fwrite(decodeLine, 1, strlen(decodeLine), fp);
                free(line); line = NULL;
                free(decodeLine);   decodeLine = NULL;
                free(hexstr);   hexstr = NULL;
                free(binstr);   binstr = NULL;
            }
        }
        else
        {
            break;
        }
    }
    #if 0
    while(1)
    {
        line = (unsigned char *)calloc(2, sizeof(unsigned char));
        if(2 == fread(line, sizeof(unsigned char), 2, fpcode))
        {
            n = line[0] << 8;
            n += line[1];
            
            free(line);
            line = (unsigned char *)calloc(n+1, sizeof(unsigned char));
            if(n == fread(line, 1, n, fpcode))
            {
                memset(decodeLine, 0, sizeof(decodeLine));
                i = 0;  k = 0;
                while(i<n-1)
                {
                    idx = metchHC(HC, (char *)line+i);
                    if(idx == -1)   
                    {
                        printf("decode error.\n");
                        return ;
                    }
                    decodeLine[k++] = idx;
                    i += strlen(HC[idx]);
                }

                fwrite(decodeLine, 1, strlen(decodeLine), fp);
                free(line);
                line = NULL;
            }
        }
        else
        {
            break;
        }
    }
    #endif

    fclose(fp);
    fclose(fpcode);
}

void testHuffmanFile()
{
    char *file = "./stdio.log";

    FILE *fp = fopen(file, "r");
    if(!fp) return;

    int weights[128] = {0};
    char line[1024] = {0};
    int i = 0, n = 0;
    int idx = 0;
    while(fgets(line, 1024, fp))
    {
        n = strlen(line);
        for(i=0; i<n; ++i)
        {
            idx = line[i];
            weights[idx]++;
        }

        memset(line, 0, sizeof(line));
    }

    fclose(fp);

    pHuffmanTree HT = createHuffmanTree(weights, 128);
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