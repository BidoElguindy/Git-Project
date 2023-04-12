#ifndef WORK_h
#define WORK_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define N_WF 20


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
int appendWorkTree(WorkTree* wt, char* name, char* hash, int xmode);
char* wtts(WorkTree* wt);
WorkTree * stwt( char * ch );
int wttf(WorkTree* wt, char* file);
WorkTree* ftwt(char* file);


int getChmod(const char * path );
void setMode (int mode , char * path );
char* blobWorkTree(WorkTree* wt);
char* saveWorkTree(WorkTree* wt, char* path) ;
char* saveWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);

#endif