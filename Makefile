all:
	g++ -std=c++11 src/main.cpp `pkg-config --libs --cflags opencv` -o run

gradient:
	g++ -std=c++11 src/gradient.cpp `pkg-config --libs --cflags opencv` -o gradient

clean:
	rm gradient