TOPLEVEL := ../../..
APP=mstbin
NVCC=g++
all:
	$(NVCC) -fpermissive -fopenmp -W -O3 -I ../include/ singlemst.cpp -o mst
	mv mst $(TOPLEVEL)/bin/CPU/mst/mst
