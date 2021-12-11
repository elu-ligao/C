#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#if 0
char *bin2hexstr(char* bin);
unsigned char *hexstr2bcd(char *hexstr);
char *bcd2hexstr(unsigned char *bcd);
char *hexstr2bin(char *hexstr);


int main()
{
    char line[100] = {0};
    char *hexline;
    
    strcpy(line, "011010101101011");
    
    hexline = bin2hexstr(line);
    
    printf("%s\n", hexline);
    
    unsigned char *bcd = hexstr2bcd(hexline);
    int n = bcd[0]<<4;
    n += bcd[1];
	
    int i = 0;
    for(; i<n+2; ++i)
        printf("%02x ", bcd[i]);
	printf("\n");
    
    char *hexstr = bcd2hexstr(bcd);
    printf("%s\n", hexstr);
    
	char *bin = hexstr2bin(hexstr);
	printf("%s\n", bin);

    return 0;
}
#endif


char *bcd2hexstr(unsigned char *bcd)
{
    int n = bcd[0]<<8;
    n += bcd[1];
    // printf("bcd2hexstr n = %d\n", n);
    char *hexstr = (char *)calloc((n<<1)+1,sizeof(char));
    int c = 0;
    int i = 0, k = 0;
    for(; i<n; ++i)
    {
        c = bcd[i+2]>>4;
		
        if(c>9)    c = c - 10 + 'A';
        else    c = c + '0';

        hexstr[k++] = c;
        
        c = bcd[i+2]&0x0F;
		
        if(c>9)    c = c - 10 + 'A';
        else    c = c + '0';

        hexstr[k++] = c;
    }
    return hexstr;
}

unsigned char*hexstr2bcd(char *hexstr)
{
    char sLen[7] = {0};
    strncpy(sLen, hexstr, 6);
    int n = ((atoi(sLen)+3)>>2)+6;
    char *line = NULL;
    
    if(n%2)
    {
        line = (char *)calloc(n+2, 
            sizeof(char));
        strcpy(line, hexstr);
        line[n] = '0';
        n += 1;
    }
    else
    {
        line = (char *)calloc(n+1, 
            sizeof(char));
        strcpy(line, hexstr);
    }
    
	
    unsigned char *bcd = (unsigned char*)calloc((n>>1)+2+1, sizeof(unsigned char));
    int i = 0, c = 0;
    char xd = 0;
    for(i=0; i<n; i+=2)
    {
        c = 0;
        if(isxdigit(line[i]) && isxdigit(line[i+1]))
        {
            xd = toupper(line[i]);
            if(isdigit(xd))
                c = (xd-'0')<<4;
            else
                c = (xd - 'A' + 10)<<4;
            xd = toupper(line[i+1]);
            if(isdigit(xd))
                c += xd-'0';
            else
                c += xd-'A'+10;
            bcd[2+(i>>1)] = c;
        }  
        else
        {
            printf("%d %s not hex digit.\n", i, line+i);  
            return NULL;      
        } 
    }
    bcd[0] = n>>9;
    bcd[1] = (n>>1)&0xFF;
    return bcd;
}
        
    
char *bin2hexstr(char *line)
{    
    int n = strlen(line);
    int b = (4-n%4)%4;
    int N = n + b;
    // printf("N = %d, b = %d\n", N, b);
    char *newLine = (char *)calloc(N+1, sizeof(char));
    strncpy(newLine, line, n);
    
    
    int i = 0;
    for(; i<b; ++i)
    {
        newLine[n+i] = '0';
    }
    
    
    int c = 0;
    int k = 0;
	
    char *hexline = (char *)calloc(N/4+6+1, sizeof(char));
	sprintf(hexline, "%06d", n);
    for(i=0; i<N; i+=4)
    {
        c = 0;
        for(k=0;k<4;++k)
        {
            c += (newLine[i+k]-'0') << (4-k-1);
        }     
		if(c>9)	c = c - 10 + 'A';
		else 	c = c + '0';   
        hexline[6+i/4] = c;
    }
    free(newLine);
    newLine = NULL;
    //printf("%s \n", hexline);
    return hexline;
}


char *hexstr2bin(char *hexstr)
{
	char sLen[7] = {0};
	strncpy(sLen, hexstr, 6);
	int n = atoi(sLen);
	int N = (n+3) >> 2;
    
	char *bin = (char *)calloc((N<<2)+1, sizeof(char));
    
	memset(bin, '0', n);
    
	// printf("hexstr = [%s], n = %d, N = %d\n", hexstr, n, N);
	int i = 0, k = 0, t = 0;
	int c = 0;
	for(i=0; i<N; ++i)
	{
		c = toupper(hexstr[i+6]);
		if(isxdigit(c))
		{
			if(isdigit(c))
			{
				c = c - '0';
			}
			else
			{
				c = c - 'A' + 10;
			}
			
			t = 0;
			k = i << 2;
			while(c)
			{
				// printf("c >> (3-t) + '0' = %c\n", (c >> (3-t)) + '0');
				bin[k+t] = (c >> (3-t)) + '0';
				c %= (1<<(3-t));
				++t;
			}
		}
	}
	bin[n] = 0;
    

	return bin;
}