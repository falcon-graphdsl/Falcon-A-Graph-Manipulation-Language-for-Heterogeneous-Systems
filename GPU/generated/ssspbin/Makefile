TOPLEVEL := ../../..
APP=ssspbin
NVCC=/usr/local/cuda/bin/nvcc
archtech=sm_20
all:
	$(NVCC) -O3  -arch=$(archtech) -I ../include/ newgpusssp1.cu -o sssp
	mv sssp $(TOPLEVEL)/bin/GPU/sssp/sssp
	$(NVCC) -O3  -arch=$(archtech) -I ../include/ singcollsssp.cu -o singcollsssp
	mv singcollsssp $(TOPLEVEL)/bin/GPU/sssp/singcollsssp
