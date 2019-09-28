run: bench
	sudo ./bench

bench: bench.cpp
	g++ -std=c++17 -O3 bench.cpp -o bench -I ../build/include -L ../build/lib -ldpdk -lpthread -lnuma -ldl