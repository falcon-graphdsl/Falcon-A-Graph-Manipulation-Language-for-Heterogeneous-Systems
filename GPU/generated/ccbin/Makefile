TOPLEVEL := ../../..
APP=ccbin
NVCC=/usr/local/cuda/bin/nvcc
archtech=sm_20
all:
	$(NVCC) -O3  -arch=$(archtech) -I ../include/ connected_component.cu -o cc
	mv cc $(TOPLEVEL)/bin/GPU/cc/cc
