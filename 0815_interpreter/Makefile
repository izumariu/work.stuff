CFLAGS=-D__DEBUG__
0815i: 0815_lib.h 0815_lib.o 0815i.o ;$(CC) -o $@ $^
%.o: %.c ;$(CC) $(CFLAGS) -c $<
clean: ;$(RM) 0815i debug *.o
.PHONY: clean