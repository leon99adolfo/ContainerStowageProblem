gcc -I /usr/local/include/ -c *.cpp
g++ -o main -L /usr/local/lib/ *.o -lgecodesupport -lgecodekernel -lgecodeint -lgecodeminimodel -lgecodeset -lgecodesearch
./main
