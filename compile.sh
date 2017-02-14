#!/bin/bash
cd compiler
lex lex.l
yacc -d grammar.y
g++ -g -I./ -w -fpermissive  symtab.c codegen.c tree.c astprint.c lex.yy.c y.tab.c aux.c  -o ../flcn 

