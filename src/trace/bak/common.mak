HEADIR = ../../include
TAGDIR = ../../lib
TAGNAME = libcommon.so
#TAGDIR = $(HOME)/mgyw/bin
#TAGNAME = commontest
TARGET = $(TAGDIR)/$(TAGNAME)

CPPFLAGS = -I$(HEADIR)
CFLAGS = -fPIC


OBJS = \
	common.o


sources = \
	common.c


$(TARGET) : $(OBJS)
	gcc -shared -o $@ $^
#	gcc -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) $^


include $(sources:.c=.d)

%.d : %.c 
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< > $@.$$$$; \
	sed 's, \($*\)\.o[ :]*, \1.o $@ : , g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY : clean
clean :
	rm -f $(OBJS) *.d.* *.d
