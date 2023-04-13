#ifndef WORK_h
#define WORK_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "List.h"

#define N_WF 20


typedef struct {
    char * name;
    char * hash;
    int mode;
} WorkFile;

typedef struct {
    WorkFile * tab;
    int size;
    int n;
} WorkTree;

//EXERCICE 4 

// Q.1 La fonction createWorkFile permet de créer un WorkFile et de l’initialiser
WorkFile* createWorkFile(char* name);

/* Q.2 La fonction wfts permet de convertir un WorkFile en chaine de caractères 
contenant les différents champs séparés par des tabulations (caractère ’\t’) */
char* wfts(WorkFile* wf);

/* Q.3 La fonction stwf permet de convertir une chaine de caractères 
représentant un WorkFile en un WorkFile */
WorkFile* stwf(char* ch);

/* Q.4 La fonction initWorkTree permet d’allouer un WorkTree de taille fixée 
(donnée par une constante du programme) et de l’initialiser */
WorkTree* initWorkTree();

/* Q.5 La fonction inWorkTree vérifie la présence d’un fichier ou 
répertoire dans un WorkTree. Elle doit retourner la position du 
fichier dans le tableau s’il est présent, et -1 sinon */
int inWorkTree(WorkTree* wt, char* name);

/* Q.6 La fonction appendWorkTree ajoute un fichier ou répertoire au WorkTree 
(s’il n’existe pas déjà) */
int appendWorkTree(WorkTree* wt, char* name, char* hash, int xmode);

/* Q.7 La fonction wtts convertit un WorkTree en une chaîne de caractères 
composée des représentations des WorkFile séparées par un saut de ligne
(caractère ’\n’) */
char* wtts(WorkTree* wt);

/* Q.8 La fonction stwt convertit une chaîne de caractères représentant 
un WorkTree en un WorkTree */
WorkTree * stwt( char * ch );

/* Q.9 La fonction wttf écrit dans le fichier file la chaine de 
caractères représentant un WorkTree */
int wttf(WorkTree* wt, char* file);

/* Q.10 La fonction ftwt construit un WorkTree à partir d'un fichier qui 
contient sa représentation en chaine de caratères */
WorkTree* ftwt(char* file);


//EXERCICE 5

/* La fonction getChmod retourne les autorisations d'un fichier ou d'un 
répertoire sous forme d'un entier */
int getChmod(const char * path );

/* La fonction setMode permet de modifier les autorisations d'un fichier 
ou d'un répertoire */
void setMode (int mode , char * path );

/* Q.1 La fonction hashToFile crée un répertoire en utilisant le hash donné 
en paramètre. Elle retourne le chemin de ce répertoire */
char* hashToFile(char* hash);

/* Q.1 La fonction blobWorkTree crée un fichier temporaire représentant le 
WorkTree pour pouvoir ensuite créer l'enregistrement instantané du 
WorkTree (avec l'extension ".t"), Elle retourne enfin le hash du 
fichier temporaire */
char* blobWorkTree(WorkTree* wt);

/* Q.2 La fonction concat_paths concatène deux chemins (path1 et path2) 
pour créer un chemin absolu. Elle renvoie le chemin absolu résultant */
char *concat_paths(char *path1, char *path2);

/* Q.2 La fonction isFile vérifie si le chemin passé en paramètre correspond 
à un fichier régulier existant sur le système de fichiers en retournant 
une valeur booléenne */
int isFile(const char *path);

/* Q.2 La fonction saveWorkTree crée un enregistrement instantané de tout 
le contenu d'un WorkTree, puis de lui-même. Elle renvoie le hash du fichier
représentant l'état instantané du WorkTree. */
char* saveWorkTree(WorkTree* wt, char* path) ;

/* Q.3 La fonction isWorkTree vérifie si un fichier associé à un hash 
donné correspond à un WorkTree en cherchant un fichier avec l'extension ".t" 
ou à un fichier en cherchant un fichier sans l'extension ".t", et retourne 
1 pour un WorkTree, 0 pour un fichier normal et -1 si le fichier n'existe pas */
int isWorkTree(char* hash);

/* Q.3 La fonction restoreWorkTree permet de restaurer un WorkTree en utilisant
les fichiere et dossiers correspondant aux chemins fournis en argument */
void restoreWorkTree(WorkTree* wt, char* path);


#endif
