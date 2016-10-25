OBJS=libhep.a
all: $(OBJS) 
INCDIR=./include
CFLAGS+=-I$(INCDIR)
CFLAGS+=-Wall -Wextra
CFLAGS+=-g

libhep.a: libhep.o netdata.o
	ar r $@ $^

libhep.o: src/libhep.c $(INCDIR)/libhep.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

netdata.o: src/netdata.c $(INCDIR)/netdata.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

.PHONY: clean tests install

clean:
	rm -f $(OBJS) *.o
	make -C tests clean

tests: libhep.a
	make -C tests 

install: libhep.a
	cp $< /usr/local/lib

