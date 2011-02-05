main.o: main.cpp
	g++ -c main.cpp -o main.o -fPIC -O2 -I /usr/include/python3.1/ -I /usr/local/include/panda3d -fpermissive

main: main.o
	g++ main.o -o main -fPIC -L /usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct

all: main
