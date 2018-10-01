#!/bin/bash
lex -l lex.l
yacc -d grammar.y
g++ -g -w -fpermissive -I ./ astprint.c aux.c clustercommu.c codegen.c lex.yy.c  symtab.c tree.c  y.tab.c parse_cmd_line.c -o flcn
