TOPLEVEL := ../../..
APP=ccbin
NVCC=g++
all:
	$(NVCC) -O3 -fopenmp -fpermissive -W -I ../include/ connected_component.cpp -o cc
	mv cc $(TOPLEVEL)/bin/CPU/cc/cc
