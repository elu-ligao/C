CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += 

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = testALGraph
TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  \
		algraph.c 		\
		algHeap.c 		\
		vexavl.c 		\
		testALGraph.c	\
		../adjacencyMatrix/graphQueue.c 	\



INC = -I /mnt/e/lg/vscode/inc -I./inc -I../adjacencyMatrix/inc

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
