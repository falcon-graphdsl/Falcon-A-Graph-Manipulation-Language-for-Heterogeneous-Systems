//#define MPIGPU
//#ifndef MPIGPU
extern bool findextrpptyexist(dir_decl *,char *);
extern char *MPIdtypenames[];
extern int findextrpptydatatype(dir_decl *,char  *);
void mpiinitgpu(FILE *FP1,FILE *FP){
fprintf(FP1,"void FALCmpiinit(int argc,char **argv){\n");
  fprintf(FP1,"MPI_Init(&argc,&argv);\n");
  fprintf(FP1,"MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);\n");
fprintf(FP1,"MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);\n");
fprintf(FP1,"  gethostname(FALChostname,255);\n");
fprintf(FP1,"cudaMalloc(&FALCsendbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCrecvbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCsendsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCrecvsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){");
fprintf(FP1,"int temp=0;\n");
fprintf(FP1,"cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}\n");
fprintf(FP1," FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);\n");
fprintf(FP1," FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);\n");
fprintf(FP1,"}\n");
}
void mpiinitmultigpu(FILE *FP1,FILE *FP){
fprintf(FP1,"void FALCmpiinit(int argc,char **argv){\n");
  fprintf(FP1,"MPI_Init(&argc,&argv);\n");
  fprintf(FP1,"MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);\n");
fprintf(FP1,"MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);\n");
fprintf(FP1,"  gethostname(FALChostname,255);\n");
  fprintf(FP1,"cudaSetDevice(FALCrank);\n");
fprintf(FP1,"cudaMalloc(&FALCsendbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCrecvbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCsendsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCrecvsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){");
fprintf(FP1,"int temp=0;\n");
fprintf(FP1,"cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}\n");
fprintf(FP1," FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);\n");
fprintf(FP1," FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);\n");
fprintf(FP1,"}\n");
}
gencommbuffgpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
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

fprintf(FP,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP,"%*s""for(int i=0;i<tot;i++){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""cudaMemcpy( &temp,&buff[i],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent," ");
fprintf(FP,"%*s""cudaMalloc(&(temp.vid),sizeof(int)*size);\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"%*s""cudaMalloc(&(temp.%s),sizeof(%s)*size);\n",indent," ",names[i],ppts->t1->name);
}
ppts=ppts->next;
}
}
fprintf(FP,"%*s""cudaMemcpy(&buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);\n",indent," ");
fprintf(stderr,"last buff done\n");
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"%*s""cudaMalloc(&(temp%s),sizeof(%s)*size);\n",indent," ",names[i],ppts->t1->name);
}
ppts=ppts->next;
}
}
fprintf(FP,"}\n");
fprintf(stderr,"gencommbuff done\n");
return;
}
gencommprefixcodegpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){

int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
int indent=4;
char *name;
if(flag==1){
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"sendprefix<<<(%s.localpoints+%s.remotepoints)/1024+1,1024>>>(%s",d1->name,d1->name,d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,",temp%s",names[i]);
}
}
fprintf(FP1,");\ncudaDeviceSynchronize();\n");
fprintf(FP,"__global__ void sendprefix(GGraph %s",d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,",%s *temp%s",ppts->t1->name,names[i]);
}
ppts=ppts->next;
}

}
fprintf(FP,"){\n");
fprintf(FP,"int id=threadIdx.x+blockDim.x*blockIdx.x;\n");
fprintf(FP,"if(id<(%s.localpoints+%s.remotepoints)){\n",d1->name,d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"temp%s[id]=((%s *)(%s.extra))->%s[id];\n",names[i],d1->extra_name,d1->name,names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP,"}\n}\n");
}
}
gencommcodegpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
int indent=4;
char *name;
int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
if(flag){
fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
fprintf(FP1,"%*s""int offstart,offend;\n",indent," ");
//fprintf(FP1,"%*s""cudaMemcpy(offstart,%s.offset[k-1],sizeof(int),cudaMemcpyDeviceToHost);\n",indent," ",d1->name);
//fprintf(FP1,"%*s""cudaMemcpy(offend,%s.offset[k],sizeof(int),cudaMemcpyDeviceToHost);\n",indent," ",d1->name);
fprintf(FP1,"%*s""offstart=%s.offset[kk-1];\n",indent," ",d1->name);
fprintf(FP1,"%*s""offend=%s.offset[kk];\n",indent," ",d1->name);
fprintf(FP1,"sendbuff<<<(offend-offstart)/1024+1,1024>>>(%s,FALCsendsize,FALCsendbuff",d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP1, ",temp%s",names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP1,",kk-1,offstart,(offend-offstart));\n");
fprintf(FP1,"}\n");
fprintf(FP1,"cudaDeviceSynchronize();\n");
fprintf(FP,"__global__ void sendbuff(GGraph %s,int *sendsize,struct FALCbuffer *sendbuff,",d1->name);

for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP, "%s *temp%s,",ppts->t1->name,names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP,"int kk,int off,int totelems){\n");

fprintf(FP,"%*s""int id=threadIdx.x+blockDim.x*blockIdx.x;\n",indent," ");
fprintf(FP,"%*s""if(id <totelems){\n",indent," ");
fprintf(FP,"%*s""int loc=0;\n",indent," ");
fprintf(FP,"%*s""int flag=0;\n",indent," ");
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP,"%*s""if( (( %s *)(%s.extra))->%s[id+off]!=temp%s[id+off])flag=1;\n",indent," ",d1->extra_name,d1->name,names[i],names[i]);
}
}
fprintf(FP,"%*s""if(flag==1){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""loc=atomicAdd(&sendsize[kk],1);\n",indent," ");
fprintf(FP,"%*s""sendbuff[kk].vid[loc]=%s.remotevertexid[id+off];\n",indent," ",d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP,"%*s""sendbuff[kk].%s[loc]=(( %s *)(%s.extra))->%s[id+off];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
fprintf(FP,"%*s"" temp%s[id+off]=(( %s *)(%s.extra))->%s[id+off];\n",indent," ",names[i],d1->extra_name,d1->name,names[i]);
}
}
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP,"}\n");
indent=4;
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ");
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n\n",indent*2," ");
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"}");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCrecvbuff[i],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.vid,%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCrecvbuff[i-1],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*sMPI_Recv(temp.vid,%s.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"}");
}
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
int t1=findextrpptydatatype(d1,names[i]);
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, %s, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ",names[i],MPIdtypenames[t1]);
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&FALCsendsize[i-1],sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, %s, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent*2," ",names[i],MPIdtypenames[t1]);
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"}");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCrecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,%s.npoints, %s,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);\n",indent*2," ",names[i],d1->name,MPIdtypenames[t1]);
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCrecvbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*sMPI_Recv(temp.%s,%s.npoints, %s,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);\n",indent*2," ",names[i],d1->name,MPIdtypenames[t1]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}");
}
else{
 fprintf(FP1,"//%s should be synchronized as it is a global var\n",names[i]);//assumption data type is int, not perferct.
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"if(FALCrank!=0){\nfor(int i=1;i< FALCsize;i++){\n");
fprintf(FP1,"int temp;\n");
fprintf(FP1,"cudaMemcpyFromSymbol(&temp,%s,sizeof(int),0,cudaMemcpyDeviceToHost);\n",names[i]);
fprintf(FP1,"MPI_Isend(&temp,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n");
fprintf(FP1,"}\n}\n");
fprintf(FP1,"if(FALCrank==0){\n");
fprintf(FP1,"%*s""int temp%s=0;\n",indent," ",names[i]);
fprintf(FP1,"%*s""int temp0;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpyFromSymbol(&temp0,%s,sizeof(int),0,cudaMemcpyDeviceToHost);\n",indent," ",names[i]);
fprintf(FP1,"%*s""for(int i=1;i<FALCsize;i++){\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&temp%s,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""temp0+=temp%s;\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""cudaMemcpyToSymbol(%s,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);\n",indent," ",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"%*s""for(int i=1;i< FALCsize;i++)MPI_Isend(&temp0,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"else {\n");

fprintf(FP1,"%*s""\nFALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""int temp0;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&temp0,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent," ",names[i]);
fprintf(FP1,"%*s""cudaMemcpyToSymbol(%s,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);\n",indent," ",names[i]);
fprintf(FP1,"}\n");
}
}
fprintf(FP,"\n__global__ void update(GGraph %s,struct FALCbuffer *recvbuff,int FALCnamount,int kk){\n",d1->name);
fprintf(FP,"%*s""int id=blockIdx.x*blockDim.x+threadIdx.x;\n",indent," ");
fprintf(FP,"%*s""if(id <FALCnamount){\n",indent," ");
fprintf(FP,"%*s""int vertex= recvbuff[kk].vid[id];\n",indent*2," ");
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP,"%*s""if( ( ( %s * )(%s.extra))->%s[vertex] > recvbuff[kk].%s[id])\n",indent*2," ",d1->extra_name,d1->name,names[i],names[i]);
fprintf(FP,"%*s""( ( %s * )(%s.extra))->%s[vertex] = recvbuff[kk].%s[id];\n",indent*3," ",d1->extra_name,d1->name,names[i],names[i]);
}
}
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP1,"\nfor(int kk=0;kk<(FALCsize-1);kk++){\n");
fprintf(FP1,"%*s""MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);\n",indent," ");
fprintf(FP1,"%*s""update<<< FALCnamount/1024+1,1024>>>(%s,FALCrecvbuff,FALCnamount,kk);\n",indent," ",d1->name);
fprintf(FP1,"}\n");
fprintf(FP1,"cudaDeviceSynchronize();\n");
fprintf(FP1,"//here only master node of a point has updated value, sync it over all nodes needed. future work\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){");
fprintf(FP1,"int temp=0;\n");
fprintf(FP1,"cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}\n");

}
int gencommsinglegpu(char *name, dir_decl *d1, dir_decl *d2, FILE *FP,FILE *FP1){




}
int gencommsinglecodegpu(FILE *FP1,FILE *FP,dir_decl *d1, char *names,int cnt){
int indent=4;
char *name=names;
fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
fprintf(FP1,"%*s""int offstart,offend;\n",indent," ");
fprintf(FP1,"%*s""offstart=%s.offset[kk-1];\n",indent," ",d1->name);
fprintf(FP1,"%*s""offend=%s.offset[kk];\n",indent," ",d1->name);
fprintf(FP1,"sendbuffsingle<<<(offend-offstart)/1024+1,1024>>>(%s,FALCsendsize,FALCsendbuff",d1->name);
fprintf(FP1, ",temp%s",names);
fprintf(FP1,",kk-1,offstart,(offend-offstart));\n");
fprintf(FP1,"}\n");
fprintf(FP1,"cudaDeviceSynchronize();\n");
fprintf(FP,"__global__ void sendbuffsingle(GGraph %s,int *sendsize,struct FALCbuffer *sendbuff,",d1->name);

fprintf(FP, "int *temp%s,",names);
fprintf(FP,"int kk,int off,int totelems){\n");

fprintf(FP,"%*s""int id=threadIdx.x+blockDim.x*blockIdx.x;\n",indent," ");
fprintf(FP,"%*s""if(id <totelems){\n",indent," ");
fprintf(FP,"%*s""int loc=0;\n",indent," ");
fprintf(FP,"%*s""int flag=0;\n",indent," ");
fprintf(FP,"%*s""if( (( %s *)(%s.extra))->%s[id+off]!=temp%s[id+off])flag=1;\n",indent," ",d1->extra_name,d1->name,names,names);
fprintf(FP,"%*s""if(flag==1){\n",indent," ");
indent+=4;
fprintf(FP,"%*s""loc=atomicAdd(&sendsize[kk],1);\n",indent," ");
fprintf(FP,"%*s""sendbuff[kk].vid[loc]=%s.local_to_orig[id+off];\n",indent," ",d1->name);
fprintf(FP,"%*s""sendbuff[kk].%s[loc]=(( %s *)(%s.extra))->%s[id+off];\n",indent," ",names,d1->extra_name,d1->name,names);
fprintf(FP,"%*s"" temp%s[id+off]=(( %s *)(%s.extra))->%s[id+off];\n",indent," ",names,d1->extra_name,d1->name,names);
indent-=4;
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP,"}\n");
indent=4;
fprintf(FP1,"if(FALCRANK!=0){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[0]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[0]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT,0,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ",name);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(FALCrank==0){\n",indent," ");
fprintf(FP1,"for(int i=1;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[0]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Recv((temp.vid), %s.npoints, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""MPI_Recv((temp.%s), %s.npoints, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n\n",indent*2," ",names,d1->name);
fprintf(FP1,"//now update here\n");
fprintf(FP1,"%*s""MPI_Get_count(&FALCstatus[i], MPI_INT, &FALCnamount);\n",indent," ");
fprintf(FP1,"%*s""updatesingle<<< FALCnamount/1024+1,1024>>>(%s,FALCrecvbuff,FALCnamount,i);\n",indent," ",d1->name);
fprintf(FP1,"cudaDeviceSynchronize();\n");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%s ftt;\n",d1->extra_name);
fprintf(FP1,"cudaMemcpy(&ftt,( (%s *)(%s.extra)),sizeof(struct %s),cudaMemcpyDeviceToHost);\n",d1->extra_name,d1->name,d1->extra_name);
fprintf(FP1,"for(int i=1;i<FALCsize;i++)MPI_Send(temp.%s,%s.npoints,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCRequest[i]);\n",name,d1->name); 
fprintf(FP1,"}\n");
fprintf(FP1,"if(FALCRANK!=0){\n");
fprintf(FP1,"%s ftt;\n",d1->extra_name);
fprintf(FP1,"cudaMemcpy(&ftt,( (%s *)(%s.extra)),sizeof(struct %s),cudaMemcpyDeviceToHost);\n",d1->extra_name,d1->name,d1->extra_name);
fprintf(FP1,"MPI_Recv(temp.%s,%s.npoints,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCstatus[0]);\n}\n",name,d1->name); 
fprintf(FP1,"cudaMemcpy(( (%s *)(%s.extra)),sizeof(struct %s),cudaMemcpyHostToDevice);\n",d1->extra_name,d1->name,d1->extra_name);
fprintf(FP,"\n__global__ void updatesingle(GGraph %s,struct FALCbuffer *recvbuff,int FALCnamount,int kk){\n",d1->name);
fprintf(FP,"%*s""int id=blockIdx.x*blockDim.x+threadIdx.x;\n",indent," ");
fprintf(FP,"%*s""if(id <FALCnamount){\n",indent," ");
fprintf(FP,"%*s""int vertex= recvbuff[kk].vid[id];\n",indent*2," ");
fprintf(FP,"%*s""if( ( ( %s * )(%s.extra))->%s[vertex] > kk)\n",indent*2," ",d1->extra_name,d1->name,names,names);
fprintf(FP,"%*s""( ( %s * )(%s.extra))->%s[vertex] =kk;\n",indent*3," ",d1->extra_name,d1->name,names,names);
fprintf(FP,"%*s""}\n",indent," ");
fprintf(FP,"}\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){");
fprintf(FP1,"int temp=0;\n");
fprintf(FP1,"cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}\n");

}
//#endif
