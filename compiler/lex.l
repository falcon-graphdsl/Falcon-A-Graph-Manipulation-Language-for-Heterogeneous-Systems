N%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]
ALPHANUM    [a-zA-Z][a-zA-Z0-9_]*
%{
#include <stdio.h>
#include "y.tab.h"
  class tree_typedecl; 
//#include "symtabold.h"
//#include "../include/tree.h"
// "#define"{WS}*{ALPHANUM}                               { yylval.text=malloc(sizeof(yytext));strcpy(yylval.text,yytext);printf("macro stmt %s\n",yylval.text);return STRING_LITERAL; }

extern void yyerror(const char *);  /* prints grammar violation message */

extern int sym_type(const char *);  /* returns type from symbol table */
int COMP_FLAG=0,DECL_FLAG=0,br_cnt=0;
//extern class globsymtable *currsymtab;
extern int LINENO,macroflag;
int sym_type(char *identifier){
//printf("***************SYM_TYPE****************************%s\n",identifier);
//tree_typedecl *x=currsymtab->sym->findtypedefsymbol(identifier);
//if(x!=NULL){
//printf("\n TYPEDEFED DATA TYPE\n");
//if(x->tpdef==1) {
//yylval.text=x->vname;
//return TYPEDEF_NAME;

//}

//yylval.text=x->vname;
//printf("returning identifier\n");
//return IDENTIFIER;
//}
//printf("returning identifier\n");
return IDENTIFIER;
}

static void comment(void);
static void hash_define();
static int check_type(void);
%}

%%
"/*".*"*/"                                    { /* comment();*/ printf("COMMENT %s ",yytext);}
"//".*                                    { /* consume //-comment */ }
"getType"                                {return GETTYPE;}    
"\#define"                               {printf("HASHDEFINE\n");macroflag=0;return HASH_DEFINE;}
"auto"					{ return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ return(CHAR); }
"const"					{ return(CONST); }
"continue"				{ return(CONTINUE); }
"default"				{ return(DEFAULT); }
"do"					{ return(DO); }
"double"				{ return(DOUBLE); }
"else"					{ return(ELSE); }
"enum"					{ return(ENUM); }
"single"                                { return (SINGLE);}
"extern"				{ return(EXTERN); }
"float"					{ return(FLOAT); }
"for"					{ return(FOR); }
"goto"					{ return(GOTO); }
"if"					{ return(IF); }
"inline"				{ return(INLINE); }
"int"					{ return(INT); }
"long"					{ return(LONG); }
"register"				{ return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ return(SHORT); }
"signed"				{ return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ return(STATIC); }
"struct"				{  return(STRUCT); }
"struct_rec"				{  return(STRUCT_REC); }
"switch"				{ return(SWITCH); }
"typedef"				{ return(TYPEDEF); }
"union"					{ return(UNION); }
"unsigned"				{ return(UNSIGNED); }
"void"					{ return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"                              { return ALIGNAS; }
"_Alignof"                              { return ALIGNOF; }
"_Atomic"                               { return ATOMIC; }
"bool"                                 { return BOOL; }
"_Complex"                              { return COMPLEX; }
"_Generic"                              { return GENERIC; }
"_Imaginary"                            { return IMAGINARY; }
"_Noreturn"                             { return NORETURN; }
"_Static_assert"                        { return STATIC_ASSERT; }
"_Thread_local"                         { return THREAD_LOCAL; }
"__func__"                              { return FUNC_NAME; }
"Graph"                                 {return GRAPH;}
"Edge"                                  {return EDGE;}
"Point"                                 {return POINT;}
"Set"                                   {return SET;}
"Collection"                            {return COLLECTION;}
"GPU"                                 { return GPU;}
"E_P"                                   {return E_P;}
"G_P"                                   {return G_P;}
"P_P"                                   {return P_P;}
"foreach"                                {return FOREACH;}
"parallel"                               {return PARALLEL;}
"sections"                               {return SECTIONS;}
"section"                               {return SECTION;}
"In "                                     {/*yylval.text=malloc(sizeof(yytext));strcpy(yylval.text,yytext);*/return INOLD;}
"false"                                  { yylval.ival=0; return B_CONSTANT;}
"true"                                  { yylval.ival=1; return B_CONSTANT;}


{ALPHANUM}					{ yylval.text=malloc(sizeof(yytext));strcpy(yylval.text,yytext);return check_type(); }

{HP}{H}+{IS}?				{ yylval.ival=atoi(yytext);return I_CONSTANT; }
{NZ}{D}*{IS}?				{ yylval.ival=atoi(yytext);return I_CONSTANT; }
"0"{O}*{IS}?				{ yylval.ival=atoi(yytext);return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ 
yylval.text=calloc(sizeof(yytext)+50,sizeof(char));for(int i=0;i<sizeof(yytext)+50;i++)yylval.text[i]='\0';
strcpy(yylval.text,yytext);return STRING_LITERAL; }
{D}+{E}{FS}?				{ yylval.fval=atof(yytext);return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ yylval.fval=atof(yytext);return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ yylval.fval=atof(yytext);return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ yylval.fval=atof(yytext);return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ yylval.fval=atof(yytext);return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ yylval.fval=atof(yytext);return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ yylval.text=calloc(sizeof(yytext)+100,sizeof(char));strcpy(yylval.text,yytext);return STRING_LITERAL; }

"..."					{ return ELLIPSIS; }
">>="					{ return RIGHT_ASSIGN; }
"<<="					{ return LEFT_ASSIGN; }
"+="					{ return ADD_ASSIGN; }
"ReduxSum="					{ return RADD_ASSIGN; }
"ReduxMul="					{ return RMUL_ASSIGN; }
"-="					{ return SUB_ASSIGN; }
"*="					{ return MUL_ASSIGN; }
"/="					{ return DIV_ASSIGN; }
"%="					{ return MOD_ASSIGN; }
"&="					{ return AND_ASSIGN; }
"^="					{ return XOR_ASSIGN; }
"|="					{ return OR_ASSIGN; }
">>"					{ return RIGHT_OP; }
"<<"					{ return LEFT_OP; }
"++"					{ return INC_OP; }
"--"					{ return DEC_OP; }
"->"					{ return PTR_OP; }
"&&"					{ return AND_OP; }
"||"					{ return OR_OP; }
"<="					{ return LE_OP; }
">="					{ return GE_OP; }
"=="					{ return EQ_OP; }
"!="					{ return NE_OP; }
";"					{ return ';'; }
("{"|"<%")				{ return '{'; }
("}"|"%>")				{ return '}'; }
","					{ return ','; }
":"					{ return ':'; }
"="					{ return '='; }
"("					{ br_cnt++;return '('; }
")"					{ br_cnt--;return ')'; }
("["|"<:")				{ return '['; }
("]"|":>")				{ return ']'; }
"."					{ return '.'; }
"&"					{ return '&'; }
"!"					{ return '!'; }
"~"					{ return '~'; }
"-"					{ return '-'; }
"+"					{ return '+'; }
"@"					{ return '@'; }
"*"					{ return '*'; }
"/"					{ return '/'; }
"%"					{ return '%'; }
"<"					{ return '<'; }
">"					{ return '>'; }
"^"					{ return '^'; }
"|"					{ return '|'; }
"?"					{ return '?'; }
\n                                      {LINENO++;}
{WS}					{ if((macroflag==0)||(macroflag==1 &&br_cnt==0)) {macroflag++; return WHITE_SPACE;}/* whitespace separates tokens */ }
.					{ /* discard bad characters */ }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}
void skipline(){
int c;

    while ((c = getchar()) != '\n');
}
static void hash_define(){
char def[1000];
int i=1,c;
def[0]='#';
printf("HASH DEFINE\n");
while((c=getchar())!='\n')def[i++]=c;
def[i]='\n';
yylval.text=malloc(sizeof(char)*(i+1));
strncpy(yylval.text,def,i);
(yylval.text)[i+1]='\0';
return STRING_LITERAL;
}

static void comment(void)
{
    int c;
    while (scanf("%c",&c) != 0){
        if (c == '*')
        {
            while ((scanf("%c",&c)) == '*')
                ;

            if (c == '/')
                return;
}
            if (c == 0)
                break;
printf("%c",c);
        }
    yyerror("unterminated comment");
}

static int check_type(void)
{
    switch (sym_type(yytext))
    {
    case TYPEDEF_NAME: {printf("here\n");               /* previously defined */
        return TYPEDEF_NAME;break;
               }
    case ENUMERATION_CONSTANT: {
       /* previously defined */
        return ENUMERATION_CONSTANT;

}
    default:                          /* includes undefined */
        return IDENTIFIER;
    }
}

/*"Points"                                  {return POINTS;}
"Edges"                                  {return EDGES;}
"Nbrs"                                   {return NBRS;}
"InNbrs"                                 {return INNBRS;}
"OutNbrs"                                 {return OUTNBRS;}
"Items"                                  {return ITEMS;}*/
