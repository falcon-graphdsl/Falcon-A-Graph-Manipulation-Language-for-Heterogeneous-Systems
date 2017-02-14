//#include <thrust/device_vector.h>
//#include <thrust/host_vector.h>
#include<stdio.h>
struct dev_vec{
int pos;
int size;
int  D_Vec[2048];
dev_vec(){
pos=2046;
size=0;
}
 void add(int value){
if(size>pos){
printf("EXCESS\n");
exit(0);
//t1.size--;
return;
}
D_Vec[size]=value;
size++;
}
};
 struct dev_vec  create_coll_device( int n){
struct dev_vec t1;
t1.pos=2046;
//t1.size=n;
t1.size=0;
return t1;
}
 int get_elem_coll_device(struct dev_vec &t1,int &val){
t1.size--;
if(t1.size<=2046 && t1.size>=0){ val=t1.D_Vec[t1.size];return val;}
else{ return 124567890;}
}
 int add_elem_coll_device(struct dev_vec t1,int pos,int val){
if(t1.size>pos){ t1.D_Vec[t1.pos]=val;return 1;}
if(t1.size==pos && pos<t1.pos){
t1.D_Vec[pos]=val;
t1.size++;
return  1;
}
return 0;
}
 void add(struct dev_vec &t1,int value){
if(t1.size>t1.pos){
printf("EXCESS\n");
exit(0);
//t1.size--;
return;
}
t1.D_Vec[t1.size]=value;
t1.size++;
}
 void add_coll_device(struct dev_vec &t1,int value){
if(t1.size>t1.pos){
printf("EXCESS\n");
exit(0);
//t1.size--;
return;
}
t1.D_Vec[t1.size]=value;
t1.size++;
}
 void  test (){
struct dev_vec t1=create_coll_device(100);
for(int i=0;i<54;i++){
add_coll_device(t1,i*20);
}
int i;
while(t1.size>0){printf("%d ",get_elem_coll_device(t1,i));if((t1.size%10)==0)printf("\n");}
add_elem_coll_device(t1,250,245);

}

/*main(){
test<<<1,1>>>();
cudaDeviceSynchronize();
}*/



