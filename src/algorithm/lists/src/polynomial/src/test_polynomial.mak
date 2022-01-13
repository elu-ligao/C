CC=gcc
CFLAGS += -Wall -std=gnu99
LDFLAGS += -ltools

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = test_polynomial
TARGET = ../bin/$(TAGNAME)

SRCS =  \
		list_link.c 		\
		polynomial.c 		\
		


INC = -I ../inc -I /mnt/e/lg/vscode/inc

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
