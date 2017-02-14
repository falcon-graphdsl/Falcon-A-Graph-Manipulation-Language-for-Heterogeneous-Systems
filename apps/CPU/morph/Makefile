TOPLEVEL := ../../..
APP=bfs
FALCON= ../../../flcn
all:
	$(FALCON)  falcondmrcpu.flcn -ptype 1 -pdim 2 -mtype 2
	mv falcondmrcpu.cpp falcondmrcpu.h falcondmrcpuglobal.h $(TOPLEVEL)/CPU/generated/morph/
	$(FALCON)  dynamicsssp.flcn -ptype 1 -pdim 2 -mtype 2
	mv dynamicsssp.cpp dynamicsssp.h dynamicssspglobal.h $(TOPLEVEL)/CPU/generated/morph/
