#ifndef EX11_h
#define EX11_h

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List**conflicts);
List* merge(char* remote branch, char* message);
void createDeletionCommit(char* branch, List* conflicts, char* message);



#endif