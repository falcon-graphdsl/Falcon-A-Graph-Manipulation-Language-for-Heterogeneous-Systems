#include "externs.h"
printcodeoutnbrs(statement *t1,dir_decl *parent, dir_decl *point, dir_decl *outnbr){
int itr=4;
dir_decl *d2=point;
dir_decl *d1=outnbr;
dir_decl *d2p=parent;
//fprintf(stderr, "ITR=%d d1=%s d2=%s %s\n",itr,d1->name,d2->name,d2p->name);
char arr[200];
for(int i=0;i<200;i++)arr[i]='\0';
tree_expr *e5= t1->expr5;
if(e5!=NULL){ 
e5->rhs->printcode1(e5->rhs,arr);
fprintf(FP1,"int falcftadv%d=%s;\n",unnicnt,arr);
}
//printf("HERE\n");
d2->extra_name1=malloc(sizeof(char)*100);
if(d2->gpu)strcpy(d2->extra_name1,"id");
else strcpy(d2->extra_name1,d2->name);
if(MORPH_FLAG==0)fprintf(FP1,"int falcft%d=%s.index[%s+1]-%s.index[%s];\nint falcft%d=%s.index[%s];\n",unnicnt,d2p->name,d2->extra_name1,d2p->name,d2->extra_name1,unnicnt+1,d2p->name,d2->extra_name1);
if(MORPH_FLAG==1)fprintf(FP1,"int falcft%d=%s.outtotal[%s];\nint falcft%d=%s.index[%s];\n",unnicnt,d2p->name,d2->extra_name1,unnicnt+1,d2p->name,d2->extra_name1);
int union_type=0;
if(e5!=NULL)fprintf(FP1,"for(int falcft%d=falcftadv%d;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt,unnicnt+2,unnicnt,unnicnt+2);
else fprintf(FP1,"for(int falcft%d=0;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt+2,unnicnt,unnicnt+2);
fprintf(FP1,"int ut%d=%s*(falcft%d+falcft%d);\n int ut%d=%s.edges[ut%d%s].%s;\nint ut%d=%s.edges[ut%d%s].%s;\n",utcnt,utflagarr[utflag][0],unnicnt+1,unnicnt+2,utcnt+1,d2p->name,utcnt,utflagarr[utflag][1],union_string[union_type],utcnt+2,d2p->name,utcnt,utflagarr[utflag][2],union_string[union_type]);
fprintf(FP1,"int %s=ut%d;\n",d1->name,utcnt+1);
d2->extra_name=malloc(sizeof(char)*100);
if(d2p->extra_name!=NULL)strcpy(d2->extra_name,d2p->extra_name);
d1->extra_name1=malloc(sizeof(char)*100);
d1->extra_name=malloc(sizeof(char)*100);
d1->parent=d2p;
sprintf(d1->extra_name1,"ut%d",utcnt+1);
if(d2p!=NULL && d2p->extra_name!=NULL)strcpy(d1->extra_name,d2p->extra_name);
if(t1->stassign!=NULL) {
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
FOREACH_FLAG++;
t1->stassign->rhs->printcode1(t1->stassign->rhs,temp);
if(t1->next->sttype!=SBLOCK_STMT) fprintf(FP1,"%s;\n}\n",temp);
else fprintf(FP1,"%s;\n",temp);
}//else fprintf(FP1,"//stassing NULL");
unnicnt=unnicnt+3;
utcnt=utcnt+3;
}
printcodeinnbrs(statement *t1/*FOREACHSTMT */, dir_decl *parent/*graph*/,dir_decl *point/*p*/,dir_decl *innbrs/*t*/){
//foreach(t in p.innbrs)
dir_decl *d2p=parent;
dir_decl *d1=innbrs;
dir_decl *d2=point;
int union_type=0;
char arr[200];
for(int i=0;i<200;i++)arr[i]='\0';
tree_expr *e5= t1->expr5;
if(e5!=NULL){ 
e5->rhs->printcode1(e5->rhs,arr);
fprintf(FP1,"int falcftadv%d=%s;\n",unnicnt,arr);
}
//fprintf(stderr,"%d \n",utflag);
fprintf(FP1,"int falcft%d=%s.inindex[id+1]-%s.inindex[id];\nint falcft%d=%s.index[id];\n",unnicnt,d2p->name,d2p->name,unnicnt+1,d2p->name);
if(e5!=NULL)fprintf(FP1,"for(int falcft%d=falcftadv%d;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt,unnicnt+2,unnicnt,unnicnt+2);
 fprintf(FP1,"for(int falcft%d=0;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt+2,unnicnt,unnicnt+2);
fprintf(FP1,"int ut%d=%s*(falcft%d+falcft%d);\n int ut%d=%s.inedges[ut%d%s].%s;\nint ut%d=%s.inedges[ut%d%s].%s;\n",utcnt,utflagarr[utflag][0],unnicnt+1,unnicnt+2,utcnt+1,d2p->name,utcnt,utflagarr[utflag][1],union_string[union_type],utcnt+2,d2p->name,utcnt,utflagarr[utflag][2],union_string[union_type]);

d2->extra_name1=malloc(sizeof(char)*100);
d2->extra_name=malloc(sizeof(char)*100);
if(d2p->extra_name!=NULL)strcpy(d2->extra_name,d2p->extra_name);
//strcpy(d2->extra_name,d2p->extra_name);
if(d2->gpu)strcpy(d2->extra_name1,"id");
else strcpy(d2->extra_name1,t1->expr2->lhs->name);
d1->extra_name1=malloc(sizeof(char)*100);
d1->extra_name=malloc(sizeof(char)*100);
d1->parent=d2p;
sprintf(d1->extra_name1,"ut%d",utcnt+1);
if(d2p!=NULL && d2p->extra_name!=NULL)strcpy(d1->extra_name,d2p->extra_name);
if(t1->stassign!=NULL) {char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
FOREACH_FLAG++;
t1->stassign->rhs->printcode1(t1->stassign->rhs,temp);
unnicnt=unnicnt+3;
utcnt=utcnt+3;
if(t1->next->sttype!=SBLOCK_STMT) fprintf(FP1,"%s;\n}\n",temp);
else fprintf(FP1,"%s;\n",temp);

}
}
