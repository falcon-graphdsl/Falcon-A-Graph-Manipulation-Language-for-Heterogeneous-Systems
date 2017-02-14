#ifndef SET_H
#define SET_H
class Set {
public:
int tot_size;
int *elems;
int *parent;
int *size;
void *settype;
int set(){
elems=parent=size=NULL;
settype=NULL;
}
};

#endif

