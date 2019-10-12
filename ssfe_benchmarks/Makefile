run: bench
	sudo ./bench

bench: bench.cpp
	g++ -std=c++17 -O3 bench.cpp -o bench -I ../x86_64-native-linux-gcc/include -L ../x86_64-native-linux-gcc/lib -ldpdk -lpthread -lnuma -ldl