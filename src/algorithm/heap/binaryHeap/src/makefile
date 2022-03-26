CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += 

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_heap
TARGET = ../bin/$(TAGNAME)

SRCS =  \
		binHeap.c 		\
		main.c 		\


INC = -I ../inc 

OBJS = $(SRCS:.c=.o)


$(TARGET):$(OBJS)
	$(CC) $(LDDIR) -o $@ $^ $(LDFLAGS)  

.PHONY: clean cleanall
cleanall:
	rm -f $(TARGET) $(OBJS) 
clean:
	rm -f $(OBJS) 

# # -fno-stack-protector	GCC有一种针对缓冲区溢出的保护机制，可通过选项“-fno-stack-protector”来将其关闭
%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 
