TOPLEVEL := ../../..
APP=mstbin
NVCC=/usr/local/cuda/bin/nvcc
all:
	$(NVCC) -O3 -arch=sm_20 -I ../include/ singlemst.cu -o mst
	mv mst $(TOPLEVEL)/bin/GPU/mst/mst
	$(NVCC) -O3 -arch=sm_20 -I ../include/ singlemst_road_rand_rmat.cu -o mstall
	mv mstall $(TOPLEVEL)/bin/GPU/mst/mstall
	$(NVCC) -O3 -arch=sm_20 -I ../include/ singlemst_ordered.cu -o mstordered
	mv mstordered $(TOPLEVEL)/bin/GPU/mst/mstordered
	$(NVCC) -O3 -arch=sm_20 -I ../include/ singlemstroad.cu -o mstroad
	mv mstroad $(TOPLEVEL)/bin/GPU/mst/mstroad
