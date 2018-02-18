#include<stdio.h>
fun(FILE *FP1,char *name, int dev,char *extraname,char *cp[],int extemp,char *pptsname,int flag,char *name2){
if(flag==0){
fprintf(FP1,"cudaSetDevice(%d);\ncudaMalloc((void **)(&%s.extra),sizeof(%s));\n",dev,name,extraname);
fprintf(FP1,"%s temp%d;\n",extraname,extemp);
fprintf(FP1,"%scudaMemcpy(&temp%d,%s.extra,sizeof(%s),cudaMemcpyDeviceToHost)%s%s\n",cp[0],extemp,name,extraname,cp[1],cp[2]);
}

fprintf(FP1,"temp%d.n%s=(( %s *)(%s.extra))->n%s;\n",extemp,pptsname,extraname,name2,pptsname);
}
//computes a pow b
int intpow(int a,int b){
int res=1;
for(int i=0;i<b;i++)res=a*res;
return res;
}

