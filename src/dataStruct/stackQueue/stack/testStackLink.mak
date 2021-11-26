CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -ldl -lstackLink

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = testStackLink
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  \
		initStackLink.c      \
		testStackLink.c		\
		linkConvertBase.c 	\
		linkBrackets.c 	\
		evaluateExpressionLink.c 	\


INC = -I /mnt/e/lg/vscode/inc -I ./inc 

OBJS = $(SRCS:.c=.o)


$(TARGET):$(OBJS)
	$(CC) $(LDDIR) -o $@ $^ $(LDFLAGS) 

.PHONY: clean cleanall
cleanall:
	rm -f $(TARGET) $(OBJS) 
clean:
	rm -f $(OBJS) 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 
