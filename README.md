Welcome to the A-Heterogeneous-DSL-for-CPU-and-GPU wiki!

This work is part of the ACM TACO publication Falcon: A Graph Manipulation Language for Heterogeneous Systems ( http://dl.acm.org/citation.cfm?id=2842618). This code can be used freely for any academic research works. If you are using the code please cite the above paper.



ROOT FOLDER = ./

There is a folder ROOT_FOLDER/apps. Under that folder there are folders CPU, GPU and CPU-Galois folders, which contains DSL codes for different benchmarks. 

The DSL codes in these folders will be compiled to C++/CUDA code and written in CPU/GPU/CPU-Galois folders under ROOT_FOLDER (./).

There is a folder ROOT_FOLDER/bin. (ROOT_FOLDER=./)

The C/C++ codes generated  will be compiled to binaries and written to bin folder under ROOT_FOLDER. under bin folder binaries, there are folders CPU, GPU anf CPU-Galois.

input graph can  be in binary .gr or edgelist format. conversion of edgelist to .gr can be done with code under the  folder utility in ROOT_FOLDER.
edgelist format has following syntax with edges sorted in ascending order of src vertex.
npoints nedges (line 0)
src dst weight (line 1 to nedges)

Morph algorithm
For dynamic sssp addition of edges should be given as a text file. input graph should be in .gr/edgelist format.

For DMR algorithm the inputs and node files and pre-processed nbrs(three neighbours of each triangle) files.
if command line argument to dmr is r25k , inputs expected by algorithm are r25k.node,r25k.ele,r25.poly,r25k.nbr

Set the GALOIS_ROOT to appropriate directory in your machine. (modify the CPU-Galois/common.mk file)
If Galois is not installed, comment the Make of Galois-codes in MakeFile under ROOT FOLDER.
Do a make from ROOT DIRECTORY.

**Requirements**

1)gcc-4.8

2)cuda compiler and nvidia GPU with compute capability 2.0 or higher. If you have GPU with compute capability higher than 2.0, makefile can be modified to compiler with corresposing archictechure or remove the architecture flag in maeke file.

3)Galois framework from ISS  group, UT-Austin, if dsl code which uses global worklist are used.


Who do I talk to?

Repo owner or admin - cnunnikrishnan@gmail.com

**A Distributed memory Version of Falcon using MPI.**
A distributed version of Falcon which runs on distributed heterogeneous systems is available in DH-Falcon folder. It is the  work 
accepted in IEEE CLUSTER 2017 titled "Unnikrishnan Cheramangalath, Rupesh Nasre and Y N Srikant. DH-Falcon: A language for large-scale graph processing on Distributed Heterogeneous systems."
