CC=gcc
CFLAGS += -Wall -std=gnu99 -fPIC
LDFLAGS += 

LDDIR += -L /mnt/e/lg/vscode/lib
TAGNAME = liblist_cursor.so
TARGET = /mnt/e/lg/vscode/lib/$(TAGNAME)
# TARGET = /mnt/e/lg/vscode/bin/$(TAGNAME)

SRCS =  \
		list_cursor.c 		\


INC = -I ../inc 

OBJS = $(SRCS:.c=.o)


$(TARGET):$(OBJS)
	gcc -shared -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) $^
# $(CC) $(LDDIR) -o $@ $^ $(LDFLAGS) 

.PHONY: clean cleanall
cleanall:
	rm -f $(TARGET) $(OBJS) 
clean:
	rm -f $(OBJS) 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 
