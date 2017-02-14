TOPLEVEL := ../../..
APP=tcbin
NVCC=/usr/local/cuda/bin/nvcc
archtech=sm_20
all:
	$(NVCC) -O3  -arch=$(archtech) -I ../include/ undirected_triangle_counting.cu -o tc
	mv tc $(TOPLEVEL)/bin/GPU/tc/tc
