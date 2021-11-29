HEADIR = ./inc
TAGDIR = /mnt/e/lg/vscode/lib
TAGNAME = libsqQueue.so
TARGET = $(TAGDIR)/$(TAGNAME)

CPPFLAGS = -I$(HEADIR)
CFLAGS = -fPIC -std=gnu99


OBJS = \
	sqQueue.o \


sources = \
	sqQueue.c \


$(TARGET) : $(OBJS)
	gcc -shared -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) $^
#	gcc -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) $^


include $(sources:.c=.d)

%.d : %.c 
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< > $@.$$$$; \
	sed 's, \($*\)\.o[ :]*, \1.o $@ : , g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY : clean cleanall
clean :
	rm -f $(OBJS) *.d.* *.d 
cleanall:
	rm -f $(OBJS) $(TARGET) *.d.* *.d 
