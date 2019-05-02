CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS) # 9cc depends on $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h # All .o files are dependent on 9cc.h

test: 9cc # test depends on 9cc
	./9cc -test
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*
