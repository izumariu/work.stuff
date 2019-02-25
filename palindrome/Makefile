palindrome: palindrome_macros.h sm64rng.h sm64rng.o palindrome.o ;$(CC) -o $@ $^

%.o: %.c ;$(CC) -c $<

clean: ;$(RM) *.o palindrome

.PHONY: clean