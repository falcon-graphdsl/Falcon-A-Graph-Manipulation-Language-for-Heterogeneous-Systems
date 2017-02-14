SOURCE=symtab.c codegen.c tree.c astprint.c lex.yy.c y.tab.c aux.c 
MYPROGRAM= ../flcn
MYINCLUDES= ./
CC=g++
CFLAGS=  -w -fpermissive
$(MYPROGRAM): $(SOURCE) 
	lex lex.l
	yacc -d grammar.y
	$(CC) -g -I$(MYINCLUDES) $(CFLAGS) $(SOURCE) -o $(MYPROGRAM) 
