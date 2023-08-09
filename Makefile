default: builtin.o
	gcc src/main.c out/builtin.o -o out/main.out

builtin.o:
	gcc -c src/builtin.c -o out/builtin.o

debug:
	gcc -g src/main.c -o debug/main.out

clean:
	rm -f out/*