//#define MPIGPU
//#ifndef MPIGPU
extern bool findextrpptyexist(dir_decl *,char *);
extern char *MPIdtypenames[];
extern int findextrpptydatatype(dir_decl *,char  *);
void mpiinit(FILE *FP1,FILE *FP){
fprintf(FP1,"void FALCmpiinit(int argc,char **argv){\n");
  fprintf(FP1,"MPI_Init(&argc,&argv);\n");
  fprintf(FP1,"MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);\n");
fprintf(FP1,"MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);\n");
fprintf(FP1,"  gethostname(FALChostname,255);\n");
fprintf(FP1,"FALCsendbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"FALCrecvbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"FALCsendsize=(int *)malloc(sizeof(int)*FALCsize);\n");
fprintf(FP1,"FALCrecvsize=(int *)malloc(sizeof(int)*FALCsize);\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\nFALCsendsize[i]=FALCrecvsize[i]=0;}\n");
fprintf(FP1," FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);\n");
fprintf(FP1," FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);\n");
fprintf(FP1,"}\n");
}
gencommbuff(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
int indent=4;
fprintf(FP,"//communication buffer datatype\n struct FALCbuffer{\n %*s"" int *vid;\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP," %*s""%s *%s;\n",indent," ",ppts->t1->name,names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP,"};\n");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP, "%s *temp%s;\n",ppts->t1->name,names[i]);
}
ppts=ppts->next;
}
fprintf(stderr,"buff done\n");
}
fprintf(FP,"int FALCrank;//rank of process\n");
fprintf(FP,"char partitionfile[100];//second input\n");
 fprintf(FP," char FALChostname[256];//name of host on which process running\n");
  fprintf(FP,"int FALCsize;//total processes launched\n");
fprintf(FP," MPI_Status *FALCstatus;//used for MPI_Recv\nMPI_Request *FALCrequest;//Used for MPI_Isend\n");
fprintf(FP,"int *FALCsendsize;//send buffer size(for sending to remote machines)\n int *FALCrecvsize;\nint FALCmsgno;//message number for messages used in code\n int FALCnamount;\n");
fprintf(FP,"struct FALCbuffer *FALCsendbuff,*FALCrecvbuff;//send and receive buffer for synchronizing global state\n");
fprintf(FP,"//allocate buffer for communication \nvoid FALCallocbuff(struct FALCbuffer *buff,int tot,int size){\n");
 //   printf("%*s" "%s", indent, " "); 

fprintf(FP,"%*s""for(int i=0;i<tot;i++){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""buff[i].vid=(int *)malloc(sizeof(int)*size);\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"%*s""buff[i].%s=(%s *)malloc(sizeof(%s)*size);\n",indent," ",names[i],ppts->t1->name,ppts->t1->name);
}
ppts=ppts->next;
}
}
fprintf(stderr,"last buff done\n");
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"%*s""temp%s=(%s *)malloc(sizeof(%s)*size);\n",indent," ",names[i],ppts->t1->name,ppts->t1->name);
}
ppts=ppts->next;
}
}
fprintf(FP,"}\n");
fprintf(stderr,"gencommbuff done\n");
return;
}
gencommprefixcode(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){

int indent=4;
char *name;
int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(flag){
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"#pragma omp parallel for num_threads(32)\nfor(int i=0;i<%s.localpoints+%s.remotepoints;i++){\n",d1->name,d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,"%*s""temp%s[i]= (( %s *)(%s.extra))->%s[i];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
}
}
fprintf(FP1,"}\n");
}
}
gencommcode(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
int indent=4;
char *name;
int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(flag){
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"#pragma omp parallel for num_threads(32)\nfor(int i=0;i<%s.localpoints+%s.remotepoints;i++){\n",d1->name,d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,"%*s""temp%s[i]= (( %s *)(%s.extra))->%s[i];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
}
}
fprintf(FP1,"}\n");
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
//fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
//fprintf(FP1,"%*s""#pragma omp parallel for   num_threads(32)\n",indent," ");
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
fprintf(FP1,"%*s""#pragma omp parallel for   num_threads(32)\n",indent," ");
fprintf(FP1,"%*s""for(int i=%s.offset[kk-1];i<%s.offset[kk];i++){\n",indent," ",d1->name,d1->name);
fprintf(FP1,"%*s""sendbuff(i,%s,FALCsendsize,FALCsendbuff,kk-1);\n",indent*2," ",d1->name);
fprintf(FP,"void sendbuff(int i,HGraph &%s,int *sendsize,struct FALCbuffer *sendbuff,int kk){\n",d1->name);
fprintf(FP,"%*s""int loc=0;\n",indent," ");
fprintf(FP,"%*s""int flag=0;\n",indent," ");
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP,"%*s""if( (( %s *)(%s.extra))->%s[i]!=temp%s[i])flag=1;\n",indent," ",d1->extra_name,d1->name,names[i],names[i]);
}
}
fprintf(FP,"%*s""if(flag==1){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""loc=__sync_fetch_and_add(&sendsize[kk],1);\n",indent," ");
fprintf(FP,"%*s""sendbuff[kk].vid[loc]=%s.remotevertexid[i];\n",indent," ",d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP,"%*s""sendbuff[kk].%s[loc]=(( %s *)(%s.extra))->%s[i];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
fprintf(FP,"%*s"" temp%s[i]=(( %s *)(%s.extra))->%s[i];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
}
}
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP1,"%*s""}\n}\n",indent," ");
indent=4;
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""if(i<FALCrank)\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i].vid), FALCsendsize[i], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ");
fprintf(FP1,"%*s""if(i>FALCrank)\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i-1].vid), FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n}\n",indent*2," ");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""if(i<FALCrank)\n",indent," ");
 fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[i].vid,%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""if(i>FALCrank)\n",indent," ");
 fprintf(FP1,"%*sMPI_Recv(FALCrecvbuff[i-1].vid,%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n}\n",indent," ",d1->name);
}
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
int t1=   findextrpptydatatype(d1,names[i]);
fprintf(FP1, "// SEND_DATATYPE=%d\n", t1);
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""if(i<FALCrank)\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i].%s), FALCsendsize[i], %s, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ",names[i],MPIdtypenames[t1]);
fprintf(FP1,"%*s""if(i>FALCrank)\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i-1].%s), FALCsendsize[i-1], %s, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n}\n",indent*2," ",names[i],MPIdtypenames[t1]);
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""if(i<FALCrank)\n",indent," ");
 fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[i].%s,%s.npoints, %s,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);\n",indent*2," ",names[i],d1->name,MPIdtypenames[t1]);

fprintf(FP1,"%*s""if(i>FALCrank)\n",indent," ");
 fprintf(FP1,"%*sMPI_Recv(FALCrecvbuff[i-1].%s,%s.npoints, %s,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);\n}\n",indent*2," ",names[i],d1->name,MPIdtypenames[t1]);
}
else{
 fprintf(FP1,"//%s should be synchronized as it is a global var\n",names[i]);//assumption data type is int, not perferct.
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"if(FALCrank!=0)for(int i=1;i< FALCsize;i++)MPI_Isend(&%s,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n",names[i]);
fprintf(FP1,"if(FALCrank==0){\n");
fprintf(FP1,"%*s""int temp%s=0;\n",indent," ",names[i]);
fprintf(FP1,"%*s""for(int i=1;i<FALCsize;i++){\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&temp%s,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""%s+=temp%s;\n",indent*2," ",names[i],names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"%*s""for(int i=1;i< FALCsize;i++)MPI_Isend(&%s,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent," ",names[i]);
fprintf(FP1,"}\n");
fprintf(FP1,"else {\n");

fprintf(FP1,"%*s""\nFALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&%s,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent," ",names[i]);
fprintf(FP1,"}");
}
}
fprintf(FP1,"\nfor(int kk=0;kk<(FALCsize-1);kk++){\n");
fprintf(FP1,"%*s""MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);\n",indent," ");
fprintf(FP1,"%*s""#pragma omp parallel for   num_threads(32)\n",indent," ");
fprintf(FP1,"%*s""for(int i=0;i<FALCnamount;i++){ \n",indent," ");
fprintf(FP1,"%*s""int vertex= FALCrecvbuff[kk].vid[i];\n",indent*2," ");
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,"%*s""if( ( ( %s * )(%s.extra))->%s[vertex] > FALCrecvbuff[kk].%s[i])\n",indent*2," ",d1->extra_name,d1->name,names[i],names[i]);
fprintf(FP1,"%*s""( ( %s * )(%s.extra))->%s[vertex] = FALCrecvbuff[kk].%s[i];\n",indent*3," ",d1->extra_name,d1->name,names[i],names[i]);
}
}
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"//here only master node of a point has updated value, sync it over all nodes needed. future work\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++)FALCsendsize[i]=0;");

}
int gencommsinglestatement( char *name,dir_decl *d1, dir_decl *d2,FILE *FP,FILE *FP1){
int indent=0;
fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
fprintf(FP1,"%*s""#pragma omp parallel for   num_threads(32)\n",indent," ");
fprintf(FP1,"%*s""for(int i=%s.offset[kk-1];i<%s.offset[kk];i++){\n",indent," ",d1->name,d1->name);
fprintf(FP1,"%*s""sendbuffsingle(i,%s,FALCsendsize,FALCsendbuff,kk-1);\n",indent*2," ",d1->name);
fprintf(FP,"void sendbuffsingle(int i,HGraph &%s,int *sendsize,struct FALCbuffer *sendbuff,int kk){\n",d1->name);
fprintf(FP,"%*s""int loc=0;\n",indent," ");
fprintf(FP,"%*s""int flag=0;\n",indent," ");
for(int i=0;i<1;i++){
if(d1 &&name&&findextrpptyexist(d1,name)){
fprintf(FP,"%*s""if( (( %s *)(%s.extra))->%s[i]!=temp%s[i])flag=1;\n",indent," ",d1->extra_name,d1->name,name,name);
}
}
fprintf(FP,"%*s""if(flag==1){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""loc=__sync_fetch_and_add(&sendsize[kk],1);\n",indent," ");
fprintf(FP,"%*s""sendbuff[kk].vid[loc]=%s.local_to_orig[i];\n",indent," ",d1->name);
if(d1 &&name&&findextrpptyexist(d1,name)){
fprintf(FP,"%*s""sendbuff[kk].%s[loc]=(( %s *)(%s.extra))->%s[i];\n",indent," ",name,d1->extra_name,d1->name,name);
fprintf(FP,"%*s"" temp%s[i]=(( %s *)(%s.extra))->%s[i];\n",indent," ",name,d1->extra_name,d1->name,name);
}
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP1,"%*s""}\n}\n",indent," ");
indent=4;
fprintf(FP1,"if(FALCNRANK!=0){\n");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i].vid), FALCsendsize[0], MPI_INT, 0 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ");
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"%*s""MPI_Isend((FALCsendbuff[i].%s), FALCsendsize[0], MPI_INT, 0 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n}\n",indent*2," ",name);
fprintf(FP1,"if(FALCRANK==0){\nfor(int i=1;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[i].vid,%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",d1->name);
 fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[i].%s,%s.npoints, MPI_INT,i, FALCmsgno+1, MPI_COMM_WORLD,&FALCstatus[i]);\n\n",indent*2," ",name,d1->name);
/*if(d1 &&name&&findextrpptyexist(d1,name)){*///this commenting is not correct.
//}
fprintf(FP1,"//now rank zero process should synchronize the lock value\n");
fprintf(FP1,"%*s""MPI_Get_count(&FALCstatus[i], MPI_INT, &FALCnamount);\n",indent," ");
fprintf(FP1,"%*s""for(int kk=0;kk<FALCnamount;kk++){ \n",indent," ");
fprintf(FP1,"%*s""int vertex= FALCrecvbuff[i].vid[kk];\n",indent*2," ");
for(int i=0;i<1;i++){
//if(d1 &&name&&findextrpptyexist(d1,name)){//this commenting is not correct, temporary
fprintf(FP1,"%*s""if( ( ( %s * )(%s.extra))->%s[vertex]>i)\n",indent*2," ",d1->extra_name,d1->name,name);
fprintf(FP1,"%*s""( ( %s * )(%s.extra))->%s[vertex] = i;\n}\n}\n",indent*3," ",d1->extra_name,d1->name,name);
//}
}
fprintf(FP1,"//synchronization done \n");
fprintf(FP1,"FALCmsgno++%*s""}\n",indent," ");
fprintf(FP1,"for(int i=0;i<FALCsize;i++)FALCsendsize[i]=0;");
fprintf(FP1,"if(FALCNRANK==0){\nfor(int i=1;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""MPI_Isend( (( *%s) (%s.extra)->%s), %s.npoints, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n}\n}\n",indent*2," ",d1->extra_name,d1->name,name,d1->name);
fprintf(FP1,"if(FALCRANK!=0)\n");
fprintf(FP1,"%*s""MPI_Recv(( *%s) (%s.extra)->%s),%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",d1->extra_name,d1->name,name,d1->name);
fprintf(FP1,"//convert recieved global id to local/remote id?\n");
}
