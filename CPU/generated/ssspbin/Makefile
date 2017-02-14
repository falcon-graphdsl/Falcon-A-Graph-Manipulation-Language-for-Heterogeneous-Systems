TOPLEVEL := ../../..
APP=ssspbin
NVCC=g++
all:
	$(NVCC) -O3 -fopenmp -fpermissive -W -I ../include/ newgpusssp1.cpp -o sssp
	mv sssp $(TOPLEVEL)/bin/CPU/sssp/sssp
