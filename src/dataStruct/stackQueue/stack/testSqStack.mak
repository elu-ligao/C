CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -ldl -lstackSequence

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = testSqStack
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  \
		initStackSequence.c      \
		testSqStack.c		\
		stackConvertBase.c 	\
		stackBrackets.c 	\
		evaluateExpressionSq.c 	\


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
