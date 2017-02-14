
TOPLEVEL := ../../..
APP=bfsbin
NVCC=g++
all:
	$(NVCC) -O3 -fopenmp -fpermissive -W -Wno-sign-compare  -Wno-maybe-uninitialized -g -I ./ falcondmrcpu.cpp -o dmrcpu
	mv dmrcpu $(TOPLEVEL)/bin/CPU/morph/
	$(NVCC) -O3 -fopenmp -fpermissive -W  -Wno-sign-compare -Wno-maybe-uninitialized  -g -I ./ dynamicsssp.cpp -o dynamicsssp
	mv dynamicsssp $(TOPLEVEL)/bin/CPU/morph/
