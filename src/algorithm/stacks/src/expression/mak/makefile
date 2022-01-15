CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -lm

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_expression
TARGET = ../bin/$(TAGNAME)

SRCS =  \
		../src/main.c 		\
		../src/expression.c \
		../src/stack.c 


INC = -I ../inc 

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
