#ifndef EX11_h
#define EX11_h

#include "Branch.h"

//EXERCICE 11

// Q.1 La fonction mergeWorkTrees prend en entrée deux worktrees (wt1 et wt2) et un pointeur vers une liste (conflicts) qui sera remplie avec les noms des fichiers/répertoires en conflit. Elle renvoie un nouveau worktree (wt) qui ne contient que les fichiers/répertoires qui ne sont pas en conflit entre les deux worktrees. 

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);


// Q.2 qui fusionne la branche courante avec la branche passee en parametre si aucun conflit n’existe.
List* merge(char* remote_branch, char* message);

//Q.3 cree et ajoute un commit de suppression sur la branche branch, correspondant a la suppression des elements de la liste conflicts
void createDeletionCommit(char* branch, List* conflicts, char* message);

WorkTree* btwt(char *branch);


#endif
