HEADIR = /mnt/e/lg/vscode/inc
TAGDIR = /mnt/e/lg/vscode/lib
TAGNAME = libtools.so
TARGET = $(TAGDIR)/$(TAGNAME)

CPPFLAGS = -I$(HEADIR)
CFLAGS = -fPIC -std=gnu99

INC = -I /mnt/e/lg/vscode/inc 

SRCS = \
	timefuncs.c \
	arrayfuncs.c \

OBJS = $(SRCS:.c=.o)


$(TARGET):$(OBJS)
	gcc -shared -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) $^

.PHONY: clean cleanall
cleanall:
	rm -f $(TARGET) $(OBJS) 
clean:
	rm -f $(OBJS) 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) $(LDDIR) -o $@ -c $< 