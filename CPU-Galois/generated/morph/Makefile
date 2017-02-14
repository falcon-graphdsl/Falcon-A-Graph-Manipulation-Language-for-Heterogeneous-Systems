TOPLEVEL := ../../..
APP=dmr
GCC=g++
include ../../common.mk
all:
	$(GCC) -c -g -I$(GALOIS_ROOT)/include/ -I $(GALOIS_ROOT)/build/default/include/ -L$(GALOIS_ROOT)/build/default/lib/libgalois.a -O3 -std=c++11 -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS  -fopenmp -lgomp  -w  -fpermissive singledmr1.cpp  -o xxss
	$(GCC) -g xxss $(GALOIS_ROOT)/build/default/src/libgalois.a -lpthread -fopenmp -o dmr
	mv dmr ../../../bin/CPU-Galois/morph/
	$(GCC) -c -g -I$(GALOIS_ROOT)/include/ -I $(GALOIS_ROOT)/build/default/include/ -L$(GALOIS_ROOT)/build/default/lib/libgalois.a -O3 -std=c++11 -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS  -fopenmp -lgomp  -w  -fpermissive ssspcoll_dynamic.cpp  -o xxss
	$(GCC) -g xxss $(GALOIS_ROOT)/build/default/src/libgalois.a -lpthread -fopenmp -o dynamicsssp
	mv dynamicsssp ../../../bin/CPU-Galois/morph/
