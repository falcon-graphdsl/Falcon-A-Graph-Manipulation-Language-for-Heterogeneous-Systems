TOPLEVEL := ../../../
APP=cc
FALCON= ../../../flcn
all:
	$(FALCON)  connected_component.flcn
	mv connected_component.cpp connected_component.cu
	mv connected_component.cu connected_component.h connected_componentglobal.h $(TOPLEVEL)GPU/generated/ccbin/
