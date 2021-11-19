CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -ltrace -ldl -lavl

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_avl
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  init_avl.c      \
		test_avl.c		\


INC = -I /mnt/e/lg/vscode/inc -I ../inc

OBJS = $(SRCS:.c=.o)


$(TARGET):$(OBJS)
	$(CC) $(LDDIR) -o $@ $^ $(LDFLAGS) 

.PHONY: clean cleanall
cleanall:
	rm -f $(TARGET) $(OBJS) 
clean:
	rm -f $(OBJS) 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) $(LDDIR) -o $@ -c $< $(LDFLAGS)
