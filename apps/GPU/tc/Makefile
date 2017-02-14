TOPLEVEL := ../../../
APP=cc
FALCON= ../../../flcn
all:
	$(FALCON)  undirected_triangle_counting.flcn
	mv undirected_triangle_counting.cpp undirected_triangle_counting.cu
	mv undirected_triangle_counting.cu undirected_triangle_counting.h undirected_triangle_countingglobal.h $(TOPLEVEL)GPU/generated/tcbin/
