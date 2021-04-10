CFLAGS=-Wall -ansi -pedantic

CFILES=textLinkedList.c textRealloc.c myText.c
OBJS:=$(CFILES:%.c=%.o)

a.out:$(OBJS)
	${CC} $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -f a.out
	rm -f -- *.o

.PHONY: clean
