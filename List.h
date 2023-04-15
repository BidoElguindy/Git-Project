#ifndef List_h
#define List_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h> 


typedef struct cell{
    char* data;
    struct cell *next;

} Cell;
typedef Cell* List;

//EXERCICE 1 

/* Q.2 : La fonction hashFile calcule le hash du contenu du premier 
fichier (source) et l'ecrit dans le deuxième fichier (dest) */
int hashFile(char* source, char* dest);

/* Q.3 : La fonction sha256file renvoie une chaine de caracteres 
contenant le hash du fichier donne en parametre (file) */
char* sha256file(char* file);

//EXERCICE 2

// Q.1 : La fonction initList initialise une liste vide
List *initList();

/* Q.2 : La fonction buildCell alloue et retourne une cellule de 
la liste */ 
Cell * buildCell(char *ch);

// Q.3 : La fonction insertFirst ajoute un element en tete de liste 
void insertFirst(List *L,Cell *C);

/* Q.4 : la fonction ctos retourne la chaine de caracteres representee 
par la cellure passee en parametre (c) */
char * ctos(Cell *c);

/* Q.4 : la fonction ltos transforme une liste en une chaine de caracteres
sous la forme suivante chaine1|chaine2|chaine3|... */
char* ltos(List* L);

// Q.5 : La fonction listGet revoie le ieme element d'une liste
Cell * listGet(List *L,int i);

/* Q.6 : La fonction searchList recherche un element dans une liste 
a partir de son contenu et renvoie une reference vers lui ou NULL 
s’il n’est pas dans la liste */
Cell * searchList(List *L, char *str);

/* Q.7 : La fonction stol permet de transformer une chaine de 
caracteres representant une liste en une liste chainee */
List * stol(char *s);

// Q.8 : La fonction ltof permet d'ecrire une liste dans un fichier 
void ltof(List* L, char* path);

/* Q.8 : La fonction ftol permet d'ecrire une liste enregistree dans 
un fichier */
List* ftol(char* path);

// Fonction supplémentaire pour les tests 
void afficherListe(List liste);

//EXERCICE 3

/* Q.1 La fonction listdir prend en parametre une adresse et renvoie 
une liste contenant le noms des fichiers et repertoires qui s’y trouvent */
List *listdir(char* root_dir);

/* Q.2 La fonction file_exists retourne 1 si le fichier existe dans le 
repertoire courant et 0 sinon en utilisant la fonction precedente */
int file_exists(char *file);

/* Q.3 La fonction cp copie le contenu d’un fichier vers un autre, 
en faisant une lecture ligne par ligne du fichier source */
void cp(char *to, char *from);

/* Q.4 La fonction hashToPath retourne le chemin d’un fichier 
a partir de son hash */
char* hashToPath(char* hash);

/* Q.5 La fonction blobFile enregistre un instantane du fichier 
donne en entree */
void blobFile(char* file);

//Libère une liste 
void freeList(List *l);

//Libère une cellule 
void freeCell(Cell *c);

#endif
