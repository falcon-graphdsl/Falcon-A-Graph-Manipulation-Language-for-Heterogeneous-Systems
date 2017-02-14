TOPLEVEL := ../../..
APP=sssp
FALCON= ../../../flcn
all:
	$(FALCON) falcondmr.flcn -ptype 1 -pdim 2 -mtype 1
	mv falcondmr.cpp falcondmr.cu
	mv falcondmr.cu falcondmr.h falcondmrglobal.h $(TOPLEVEL)/GPU/generated/morph/
	$(FALCON) dynamicsssp.flcn -ptype 1 -pdim 2 -mtype 1
	mv dynamicsssp.cpp dynamicsssp.cu
	mv dynamicsssp.cu dynamicsssp.h dynamicssspglobal.h $(TOPLEVEL)/GPU/generated/morph/
