#ifndef EX5_h
#define EX5_h


int getChmod(const char * path );
void setMode (int mode , char * path );
char* blobWorkTree(WorkTree* wt);
char* saveWorkTree(WorkTree* wt, char* path) ;
char* saveWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);



#endif