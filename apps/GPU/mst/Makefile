TOPLEVEL := ../../../
APP=mst
FALCON= ../../../flcn
all:
	$(FALCON)  singlemst.flcn
	cp singlemst.cpp singlemst.cu
	cp singlemst.cu singlemst.h singlemstglobal.h $(TOPLEVEL)GPU/generated/mstbin/
	$(FALCON)  singlemst_ordered.flcn
	mv singlemst_ordered.cpp singlemst_ordered.cu
	mv singlemst.cu singlemst.h singlemstglobal.h $(TOPLEVEL)GPU/generated/mstbin/
	mv singlemst_ordered.cu singlemst_ordered.h singlemst_orderedglobal.h $(TOPLEVEL)GPU/generated/mstbin/
	$(FALCON)   singlemst_road_rand_rmat.flcn
	mv singlemst_road_rand_rmat.cpp singlemst_road_rand_rmat.cu
	mv  singlemst_road_rand_rmat.cu  singlemst_road_rand_rmat.h  singlemst_road_rand_rmatglobal.h $(TOPLEVEL)GPU/generated/mstbin/
	$(FALCON)   singlemstroad.flcn
	mv singlemstroad.cpp singlemstroad.cu
	mv  singlemstroad.cu  singlemstroad.h  singlemstroadglobal.h $(TOPLEVEL)GPU/generated/mstbin/
