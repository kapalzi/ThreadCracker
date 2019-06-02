libs=-pthread -latomic
standard=-std=c++17
debug_flags=-ggdb -Wall -fpermissive -Wunused-function

all: remove main

main: main.o
	g++ main.o md5.cpp -o main ${debug_flags} ${libs} ${standard}

main.o: main.cpp
	g++ -c main.cpp -o main.o ${debug_flags} ${libs} ${standard}

remove:
	rm -f main main.o