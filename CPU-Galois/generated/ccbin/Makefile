TOPLEVEL := ../../..
APP=ccpbin
GCC=g++
include ../../common.mk
all:
	$(GCC) -c  -I$(GALOIS_ROOT)/include/ -I $(GALOIS_ROOT)/build/default/include/ -L$(GALOIS_ROOT)/build/default/lib/libgalois.a -O3 -std=c++11 -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS  -fopenmp -lgomp  -w  -fpermissive connected_component.cpp  -o xxss
	$(GCC)  xxss $(GALOIS_ROOT)/build/default/src/libgalois.a -lpthread -fopenmp -o cc
	mv cc $(TOPLEVEL)/bin/CPU-Galois/cc/cc
