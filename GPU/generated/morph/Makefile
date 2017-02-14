TOPLEVEL := ../../..
APP=morph
NVCC=/usr/local/cuda/bin/nvcc
archtech=sm_20
all:
	$(NVCC) -O3  -arch=$(archtech) -I ./dmrheader falcondmr.cu -o dmr
	cp dmr $(TOPLEVEL)/bin/GPU/morph/
	$(NVCC) -O3  -arch=$(archtech) -I ../include dynamicsssp.cu -o dynamicsssp
	cp dynamicsssp $(TOPLEVEL)/bin/GPU/morph/
