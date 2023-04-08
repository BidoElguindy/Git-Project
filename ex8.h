#ifndef EX8_h
#define EX8_h


void initBranch();
int branchExists(char* branch);
void createBranch(char* branch);
char* getCurrentBranch();
void printBranch(char* branch);
List* branchList(char* branch);
List* getAllCommits();

#endif
