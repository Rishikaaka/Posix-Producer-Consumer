CC=gcc
LIB=-pthread

main: main.out
    ./$<

%.out : %.c
    $(CC) $(LIB) -o $@ $^


clean:
    rm -f *.out
