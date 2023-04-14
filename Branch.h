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