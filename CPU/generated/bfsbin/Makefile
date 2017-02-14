TOPLEVEL := ../../..
APP=bfsbin
NVCC=g++
all:
	$(NVCC) -O3 -fopenmp -fpermissive -W  -g -I ../include/ levelbfs.cpp -o levelbfs
	mv levelbfs $(TOPLEVEL)/bin/CPU/bfs/levelbfs
