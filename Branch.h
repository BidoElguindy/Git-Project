#ifndef Branch_h
#define Branch_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h> 


void initBranch ();
//cree le fichier cache .current branch contenant la chaˆıne de caract`eres ”master”
int branchExists(char* branch);
//verifie l’existence d’une branche
void createBranch(char* branch);
//cr´ee une reference appel´ee branch, qui pointe vers le mˆeme commit que la reference HEAD
char* getCurrentBranch();
//lit le fichier cache, current branch pour retourner le nom de la branche courante.

 


char* hashToPathCommit(char* hash);
//Construit le chemin vers le fichier commit associé à un hash de commit
void printBranch(char* branch);
//parcourt la branche appelee branche, et pour chacun de ses commits, affiche son hash et son message descriptif
List* branchList(char* branch);
//ontruit et retourne une liste chainee contenant le hash de tous les commits de la branche
List* getAllCommits();
//renvoie la liste des hash des commits de toutes les branches

void restoreCommit(char* hash commit)  
//permet de restaurer le worktree associe a un commit dont le hash est donne en parametre.

void myGitCheckoutBranch(char* branch);


List* filterList(List* L, char* pattern);

void myGitCheckoutCommit(char* pattern);


/* Fonction qui retourne l'élément à l'indice i dans la liste */
char* getElement(List* L, int i) ;

/* Fonction qui affiche tous les éléments de la liste */
void printList(List* L);

/* Fonction qui retourne la longueur de la liste */
int length(List* L) ;

/* Fonction qui libère toute la mémoire allouée pour la liste */
void freeList(List* L) ;
