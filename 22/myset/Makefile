CFLAGS=-Wall -ansi -pedantic
LDFLAGS=

CFILES=myset.c set.c str.c cmdline.c
OBJS:=$(CFILES:%.c=%.o)


a.out:$(OBJS)
	${CC} $(OBJS) ${LDFLAGS} -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -f a.out
	rm -f -- *.o

.PHONY: clean
