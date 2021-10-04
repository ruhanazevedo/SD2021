#gcc main.c -o main
#outputDIR: output

result: main.o
	gcc output/main.o -g -o output/result

main.o: main.c headers/data.h
	gcc -c main.c -g -o output/main.o

clean:
	rm -rf output/* 