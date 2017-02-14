TOPLEVEL := ../../..
APP=bfsbin
NVCC=/usr/local/cuda/bin/nvcc
all:
	$(NVCC) -O3 -arch=sm_20 -g -I ../include/ levelbfs.cu -o levelbfs
	mv levelbfs $(TOPLEVEL)/bin/GPU/bfs/levelbfs
	$(NVCC) -O3 -arch=sm_20 -g -I ../include/ singcollbfs.cu -o singcollbfs
	mv singcollbfs $(TOPLEVEL)/bin/GPU/bfs/singcollbfs
