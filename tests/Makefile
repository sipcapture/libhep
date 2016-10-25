PROGS=test1
OBJS=$(patsubst %,%.o,$(PROGS))

CFLAGS += -I../include
CFLAGS += -g
CFLAGS += -Wall -Wextra
LDFLAGS += -L.. -lhep

TEST_TARGET=run_tests
TESTS=./do_tests

all: $(OBJS) $(PROGS) $(TEST_TARGET) 

$(OBJS): %.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

$(PROGS): %: %.o
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS)


run_tests: $(PROGS)
	perl $(TESTS)

.PHONY: clean

clean:	
	rm -f $(PROGS) $(OBJS) test*.out 
