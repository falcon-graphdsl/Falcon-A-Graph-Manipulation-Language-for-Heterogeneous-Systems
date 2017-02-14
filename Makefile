MYPROGRAM= flcn
MYINCLUDES= ./
CC=g++
CFLAGS=  -w -fpermissive 
APPSALL=sssp bfs mst cc 
APPSALLGEN=ssspbin bfsbin mstbin ccbin
CUDACODES=tc 
CUDACODESGEN=tcbin 
MORPHCODES=morph
all:  $(MYPROGRAM) $(APPSALL)   $(MORPHCODES) $(APPSALLGEN) $(CUDACODES)
$(MORPHCODES):
	make -C apps/GPU/$@
	make -C GPU/generated/$@
	make -C apps/CPU/$@
	make -C CPU/generated/$@
	make -C CPU-Galois/generated/$@
$(MYPROGRAM):
	@cd compiler;make 
$(APPSALL):
	make -C apps/GPU/$@ 
	make -C apps/CPU/$@
	make -C apps/CPU-Galois/$@
$(APPSALLGEN):
	make -C GPU/generated/$@
	make -C CPU/generated/$@
	make -C CPU-Galois/generated/$@
$(CUDACODES):
	make -C apps/GPU/$@ 
	make -C apps/CPU/$@
$(CUDACODESGEN):
	make -C GPU/generated/$@
	make -C CPU/generated/$@
clean:
	rm GPU/generated/ssspbin/*.cu
	rm GPU/generated/ssspbin/*.h
	rm GPU/generated/bfsbin/*.cu
	rm GPU/generated/bfsbin/*.h
	rm GPU/generated/mstbin/*.cu
	rm GPU/generated/mstbin/*.h
	rm CPU/generated/ssspbin/*.h
	rm CPU/generated/ssspbin/*.cpp
	rm CPU/generated/bfsbin/*.h
	rm CPU/generated/bfsbin/*.cpp
	rm CPU/generated/mstbin/*.cpp
	rm CPU/generated/mstbin/*.h
	rm bin/*/*/*
