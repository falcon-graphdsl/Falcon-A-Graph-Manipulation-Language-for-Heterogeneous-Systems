TOPLEVEL := ../../../
APP=sssp
FALCON= ../../../flcn
all:
	$(FALCON)  newgpusssp1.flcn
	$(FALCON)  singcollsssp.flcn 
	mv newgpusssp1.cpp newgpusssp1.cu
	mv singcollsssp.cpp singcollsssp.cu
	mv newgpusssp1.cu newgpusssp1.h newgpusssp1global.h $(TOPLEVEL)GPU/generated/ssspbin/
	mv singcollsssp.cu singcollsssp.h singcollssspglobal.h $(TOPLEVEL)GPU/generated/ssspbin/
