CC=gcc
CFLAGS += -Wall -std=c11
LDFLAGS += -ldl -lstringList -ltrace

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = stringList
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  test.c      \
		initStringList.c 	\
		generalizeList.c 	\




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
	$(CC) $(CFLAGS) $(INC) $(LDDIR) -o $@ -c $< 
