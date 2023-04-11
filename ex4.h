
#ifndef EX4_h
#define EX4_h

typedef struct {
char * name ;
char * hash ;
int mode ;
} WorkFile ;

typedef struct {
WorkFile * tab ;
int size ;
int n ;
} WorkTree ;


WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);
WorkTree* initWorkTree();
int inWorkTree(WorkTree* wt, char* name) ;
int appendWorkTree(WorkTree* wt, char* name, char* hash, int
mode);
char* wtts(WorkTree* wt);
WorkTree * stwt ( char * ch );
int wttf(WorkTree* wt, char* file);
WorkTree* ftwt(char* file);


#endif
