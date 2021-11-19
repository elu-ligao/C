CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -linit -ltrace -ldl -lbst

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_bst
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  \
		init_bst.c      \
		test_bst.c		\


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
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< $(LDDIR) $(LDFLAGS)
