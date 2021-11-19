#include <stdio.h>
#include <string.h>


void RightTrim(char *str)
{
    int i;

    for(i=strlen(str)-1; i>=0; i--){
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r'){
            break;
        }
    }
    str[i+1] = '\0';
}

void LeftTrim(char *str)
{
    char *s = str;
    int i;

    for(i=0; i<strlen(str); i++){
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r'){
            continue;
        } else {
            break;
        }
    }
    strcpy(str, s+i);
}

void Trim(char *str)
{
	RightTrim(str);
	LeftTrim(str);
}

#if 0
int main()
{
	char s[40] = "trace.defaultlevel ";
	
	RightTrim(s);
	printf("s[%s]\n", s);

	return 0;
}
#endif
