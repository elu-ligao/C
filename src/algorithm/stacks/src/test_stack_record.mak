CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += 

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_stack_record
TARGET = ../bin/$(TAGNAME)

SRCS =  \
		test_stack_record.c 		\
		stack_record.c


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
