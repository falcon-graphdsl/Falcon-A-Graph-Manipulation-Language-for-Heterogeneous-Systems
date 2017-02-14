TOPLEVEL := ../../../
APP=bfs
FALCON= ../../../flcn
all:
	$(FALCON)  levelbfs.flcn 
	$(FALCON)  singcollbfs.flcn
	mv levelbfs.cpp levelbfs.cu 
	mv singcollbfs.cpp singcollbfs.cu 
	mv levelbfs.cu levelbfs.h levelbfsglobal.h $(TOPLEVEL)GPU/generated/bfsbin/
	mv singcollbfs.cu singcollbfs.h singcollbfsglobal.h $(TOPLEVEL)GPU/generated/bfsbin/
