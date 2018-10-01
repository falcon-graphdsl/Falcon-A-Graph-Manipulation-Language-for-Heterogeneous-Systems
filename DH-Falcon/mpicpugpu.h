#ifndef MPICPUGPU
#define MPIGPUGPU
extern bool findextrpptyexist(dir_decl *,char *);
extern int findextrpptydatatype(dir_decl *,char  *);
void mpiinitcpugpu(FILE *FP1,FILE *FP){
fprintf(FP1,"void FALCmpiinit(int argc,char **argv){\n");
  fprintf(FP1,"MPI_Init(&argc,&argv);\n");
  fprintf(FP1,"MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);\n");
fprintf(FP1,"MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);\n");
fprintf(FP1,"FALCsize*=2;\n");
fprintf(FP1,"  gethostname(FALChostname,255);\n");
fprintf(FP1,"FALCsendbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"FALCrecvbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCgpusendbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCgpurecvbuff,sizeof(struct FALCbuffer )*FALCsize);\n");
fprintf(FP1,"FALCsendsize=(int *)malloc(sizeof(int)*FALCsize);\n");
fprintf(FP1,"FALCrecvsize=(int *)malloc(sizeof(int)*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCgpusendsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"cudaMalloc(&FALCgpurecvsize,sizeof(int)*FALCsize);\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"int temp=0;\n");
fprintf(FP1,"cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);\n");
fprintf(FP1,"FALCsendsize[i]=FALCrecvsize[i]=0;}\n");
fprintf(FP1," FALCcpustatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);\n");
fprintf(FP1," FALCcpurequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);\n");
fprintf(FP1," FALCgpustatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);\n");
fprintf(FP1," FALCgpurequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);\n");
fprintf(FP1,"}\n");
}


gencommbuffcpugpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
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
fprintf(FP, "%s *temp%s, *tempgpu%s;\n",ppts->t1->name,names[i],names[i]);
}
ppts=ppts->next;
}
fprintf(stderr,"buff done\n");
}
fprintf(FP,"int FALCrank;//rank of process\n");
fprintf(FP,"char partitionfile[100];//second input\n");
 fprintf(FP," char FALChostname[256];//name of host on which process running\n");
  fprintf(FP,"int FALCsize;//total processes launched\n");
fprintf(FP," MPI_Status *FALCstatus,*FALCgpustatus;//used for MPI_Recv\nMPI_Request *FALCrequest,*FALCcpurequest;//Used for MPI_Isend\n");
fprintf(FP,"int *FALCsendsize,*FALCgpusendsize;//send buffer size(for sending to remote machines)\n int *FALCrecvsize,*FALCgpurecvsize;\nint FALCmsgno,FALCgpumsgno;//message number for messages used in code\n int FALCnamount,FALCgpunamount;\n");
fprintf(FP,"struct FALCbuffer *FALCsendbuff,*FALCgpusendbuff,*FALCrecvbuff,*FALCgpurecvbuff;//send and receive buffer for synchronizing global state\n");
fprintf(FP,"//allocate buffer for communication \nvoid FALCgpuallocbuff(struct FALCbuffer *buff,int tot,int size){\n");
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
fprintf(FP,"%*s""}\n",indent," ");
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP,"%*s""cudaMalloc(&(tempgpu%s),sizeof(%s)*size);\n",indent," ",names[i],ppts->t1->name);
}
ppts=ppts->next;
}
}
fprintf(FP,"}\n");
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
gencommprefixcodecpugpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
int indent=4;
char *name;
int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(flag){
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"sendprefix<<<(%s.localpoints+%s.remotepoints)/1024+1,1024>>>(%s",d1->name,d1->name,d1->name);
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,",tempgpu%s",names[i]);
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
gencommcodecpugpu(FILE *FP1,FILE *FP,dir_decl *d1, char **names,int cnt){
int indent=4;
char *name;
int flag=0;
for(int i=0;i<cnt;i++)if(d1 &&names[i]&&findextrpptyexist(d1,names[i]))flag=1;
if(flag){
if(d1->ppts==NULL)fprintf(FP1,"PPTS NULL\n");
if(d1 && d1->parent && d1->parent->ppts!=NULL)fprintf(FP1,"PARENT PPTS NOT NULL\n");
if(d1 && d1->parent && d1->parent->ppts==NULL)fprintf(FP1,"PARENT PPTS NULL\n");
fprintf(FP1,"for(int kk=1;kk<FALCsize;kk++){\n");
fprintf(FP1,"%*s""int offstart,offend;\n",indent," ");
fprintf(FP1,"%*s""offstart=%s.gpupart.offset[kk-1];\n",indent," ",d1->name);
fprintf(FP1,"%*s""offend=%s.gpupart.offset[kk];\n",indent," ",d1->name);
//sendbuff<<<(offend-offstart)/1024+1,1024>>>(hgraph.gpupart,FALCgpusendsize,FALCgpusendbuff,tempdistgpu,kk,offstart,(offend-offstart));//FALCgpusendbuff[0]

fprintf(FP1,"sendbuff<<<(offend-offstart)/1024+1,1024>>>(%s.gpupart,FALCgpusendsize,FALCgpusendbuff",d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP1, ",tempgpu%s",names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP1,",kk-1,offstart,(offend-offstart));\n");
fprintf(FP1,"cudaDeviceSynchronize();\n");
fprintf(FP1,"%*s""#pragma omp parallel for   num_threads(32)\n",indent," ");
fprintf(FP1,"%*s""for(int i=%s.offset[kk]-1;i<%s.offset[kk];i++){\n",indent," ",d1->name,d1->name);
//        sendbuff(i,hgraph,FALCsendsize,FALCsendbuff,tempdist,kk);//FALCsendbuff[0]

fprintf(FP1,"%*s""sendbuff(i,%s,FALCsendsize,FALCsendbuff",indent*2," ",d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP1, ",temp%s",names[i]);
}
ppts=ppts->next;
}
}
fprintf(FP1,",kk-1);\n");
indent=4;
fprintf(FP1,"}\n");
fprintf(FP1,"%*s""}\n",indent," ");
indent=4;
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
fprintf(FP,"void sendbuff(int i,HGraph &%s,int *sendsize,struct FALCbuffer *sendbuff",d1->name);
for(int i=0;i<cnt;i++){
extra_ppts *ppts=d1->ppts;
while(ppts!=NULL){
if(!strcmp(ppts->name,names[i])){
fprintf(FP, ",temp%s",names[i]);
}
ppts=ppts->next;
}
}

fprintf(FP,",int kk);\n");
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
fprintf(FP1,"FALcmsgno=0;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank|| i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""int temp1=0;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCgpusendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpusendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);//msgno send GPU value same node.\n",indent," ");
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"else{//send from CPU to GPU of same node.rank==0, i==0. rank==1,i==2\n");
fprintf(FP1,"%*s"" MPI_Isend(FALCsendbuff[i].vid, FALCsendsize[i], MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent," ");
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i].vid, FALCsendsize[i], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i+1]);\n");
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);\n",indent*2," ");
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i-1].vid, FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n");
fprintf(FP1,"%*s""FaLCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);\n",indent*2," ");
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"int cnt=0,gpucnt=0;\n");
fprintf(stderr,"VID SEND DONE\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank|| i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""int temp1=0;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""for(int kk=0;kk<FALCsize/2;kk++){\n",indent," ");

fprintf(FP1,"%*s""if(kk<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=4*(FALCrank-kk);//4\n",indent, " ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.vid,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.vid,%s.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=2*(kk-FALCrank);//2\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.vid,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.vid,%s.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(kk==FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*sMPI_Recv(temp.vid,%s.npoints, MPI_INT,kk, 4*FALCrank, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"else {\n");
fprintf(FP1,"%*s""for(int kk=0;kk<FALCsize/2;kk++){\n",indent," ");
fprintf(FP1,"%*s""if(kk<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=2*(FALCrank-kk);\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].vid,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].vid,%s.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"if(kk >FALCrank){\n");
fprintf(FP1,"%*s""int kk1=2*(kk-FALCrank)-2;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].vid,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].vid,%s.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"if(kk==FALCrank){\n");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].vid,%s.npoints, MPI_INT,kk, 4*FALCrank+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");

fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");

}
for(int i=0;i<cnt;i++){
if(d1 &&names[i]&&findextrpptyexist(d1,names[i])){
fprintf(FP1,"FALCmsgno=0;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank|| i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""int temp1=0;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCgpusendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpusendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);//msgno send GPU value same node.\n",indent," ",names[i]);
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"else{//send from CPU to GPU of same node.rank==0, i==0. rank==1,i==2\n");
fprintf(FP1,"%*s"" MPI_Isend(FALCsendbuff[i].%s, FALCsendsize[i], MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent," ",names[i]);
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i].%s, FALCsendsize[i], MPI_INT, i ,FALCmsgn, MPI_COMM_WORLD,&FALCrequest[i+1]);\n",names[i]);
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i-1].%s, FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",names[i]);
fprintf(FP1,"%*s""FaLCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);\n\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""FALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1," cnt=0,gpucnt=0;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank|| i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""int temp1=0;\n",indent," ");
fprintf(FP1,"%*s""if(i==2*FALCrank+1){\n",indent," ");
fprintf(FP1,"%*s""for(int kk=0;kk<FALCsize/2;kk++){\n",indent," ");

fprintf(FP1,"%*s""if(kk<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=4*(FALCrank-kk);//4\n",indent, " ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",names[i],d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,%s.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",names[i],d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=2*(kk-FALCrank);//2\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",names[i],d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,%s.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",names[i],d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(kk==FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*sMPI_Recv(temp.%s,%s.npoints, MPI_INT,kk, 4*FALCrank, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);\n",indent*2," ",names[i],d1->name);
fprintf(FP1,"%*s""gpucnt++;\n",indent," ");
fprintf(FP1,"%*s""}",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"else {\n");
fprintf(FP1,"%*s""for(int kk=0;kk<FALCsize/2;kk++){\n",indent," ");
fprintf(FP1,"%*s""if(kk<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""int kk1=2*(FALCrank-kk);\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].%s,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",names[i],d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].%s,%s.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",names[i],d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"if(kk >FALCrank){\n");
fprintf(FP1,"%*s""int kk1=2*(kk-FALCrank)-2;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].%s,%s.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",names[i],d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].%s,%s.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",names[i],d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"if(kk==FALCrank){\n");
fprintf(FP1,"%*s""MPI_Recv(FALCrecvbuff[cnt].%s,%s.npoints, MPI_INT,kk, 4*FALCrank+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value\n",indent," ",names[i],d1->name);
fprintf(FP1,"%*s""cnt++;\n",indent," ");

fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
fprintf(FP1,"}\n");
/*
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpusendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i].%s, FALCsendsize[i], MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i+1]);\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&FALCsendsize[i-1],sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i-1].%s, FALCsendsize[i-1], MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i+1]);\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i==FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpusendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCgpusendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Isend(FALCsendbuff[i].%s, FALCsendsize[i], MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i+1]);\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}");
fprintf(FP1,"for(int i=0;i<FALCsize;i++){\n");
fprintf(FP1,"%*s""struct FALCbuffer temp;\n",indent," ");
fprintf(FP1,"%*s""if(i<FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpurecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*s""MPI_Recv(temp.%s,1024*1024, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Recv(FALCrecvbuff[i].%s,1024*1024, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i+1]);//msgno recv GPU Value\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i>FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpurecvbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
 fprintf(FP1,"%*sMPI_Recv(temp.%s,1024*1024, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Recv(FALCrecvbuff[i-1].%s,1024*1024, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i+1]);//msgno recv GPU Value\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""if(i==FALCrank){\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp,&(FALCgpurecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""int temp1;\n",indent*2," ");
fprintf(FP1,"%*s""cudaMemcpy( &temp1,&(FALCgpurecvsize[i]),sizeof(int),cudaMemcpyDeviceToHost);\n",indent*2," ");
fprintf(FP1,"%*s""MPI_Isend((temp.%s), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent*2," ",names[i]);
        fprintf(FP1,"MPI_Isend(FALCrecvbuff[i].%s, FALCrecvsize[i], MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i+1]);\n",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"}\n");*/
}
else{
 fprintf(FP1,"//%s should be synchronized as it is a global var\n",names[i]);//assumption data type is int, not perferct.
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"if(FALCrank!=0){\nfor(int i=1;i< FALCsize;i++){\n");
fprintf(FP1,"int temp;\n");
fprintf(FP1,"cudaMemcpyFromSymbol(&temp,GPU_%s,sizeof(int),0,cudaMemcpyDeviceToHost);\n",names[i]);
fprintf(FP1,"temp+=%s;\n",names[i]);
fprintf(FP1,"MPI_Isend(&temp,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n");
fprintf(FP1,"}\n}\n");
fprintf(FP1,"if(FALCrank==0){\n");
fprintf(FP1,"%*s""int temp%s=0;\n",indent," ",names[i]);
fprintf(FP1,"%*s""int temp0;\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpyFromSymbol(&temp0,GPU_%s,sizeof(int),0,cudaMemcpyDeviceToHost);\n",indent," ",names[i]);
fprintf(FP1,"temp0+=%s;\n",names[i]);
fprintf(FP1,"%*s""for(int i=1;i<FALCsize;i++){\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&temp%s,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""temp0+=temp%s;\n",indent*2," ",names[i]);
fprintf(FP1,"%*s""}\n",indent," ");
fprintf(FP1,"%*s""cudaMemcpyToSymbol(GPU_%s,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);\n",indent," ",names[i]);
fprintf(FP1,"%*s""%s=temp0;\n",indent," ",names[i]);
fprintf(FP1,"\nFALCmsgno++;\n");
fprintf(FP1,"%*s""for(int i=1;i< FALCsize;i++)MPI_Isend(&temp0,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);\n",indent," ");
fprintf(FP1,"}\n");
fprintf(FP1,"else {\n");

fprintf(FP1,"%*s""\nFALCmsgno++;\n",indent," ");
fprintf(FP1,"%*s""int temp0;\n",indent," ");
fprintf(FP1,"%*s""MPI_Recv(&temp0,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);\n",indent," ",names[i]);
fprintf(FP1,"%*s""cudaMemcpyToSymbol(%s,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);\n",indent," ",names[i]);
fprintf(FP1,"%*s""%s=temp0;\n",indent," ",names[i]);
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
#endif
