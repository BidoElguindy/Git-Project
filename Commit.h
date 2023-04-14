#ifndef commit_h
#define commit_h

#include "Work.h"

#define COMMIT_SIZE 100

typedef struct key_value_pair {
    char* key;
    char* value;
} kvp;

typedef struct hash_table {
    kvp** T;
    int n;
    int size;
} HashTable;

typedef HashTable Commit;

/* Q.1 La fonction createKeyVal permet d'allouer et d'initilaiser 
un element */
kvp* createKeyVal(char* key, char* val);

/* Q.1 La fonction freeKeyVal permet de liberer la memoire associe a
un element */
void freeKeyVal(kvp* kv);

/* Q.2 La fonction kvts permet de convertir un element en une chaine 
de caracteres de la forme "cle : valeur"*/
char* kvts(kvp* k);

/* Q.2 La fonction stkv fait la conversion inverse */
kvp* stkv(char* str);

/* Q.3 La fonction initCommit permet d'allouer et d'initialiser un
Commit de taille fixee (donnee par une constante du programme)*/
Commit* initCommit();

/* Q.4 La fonction hashF est la fonction de hachage */
unsigned long hashF(char * str);

/* Q.5 La fonction commitSet insere la paire (key, value) dans la table
en gerant les collisions par adressage ouvert et probing lineaire */
void commitSet(Commit* c, char* key, char* value);

/* Q.6 La fonction createCommit alloue et initialise un Commit, puis 
ajoute l'element obligatoire correspondant a la cle "tree" */
Commit* createCommit(char* hash);

/* Q.7 La fonction commitGet cherche dans la table s'il existe un element
dont la cle est key (en sachant que les conflits sont resolus par adressage 
ouvert et probing lineaire). Elle retorune la valeur de l'element s'il existe
et NULL sinon */
char* commitGet(Commit* c, char* key);

/* Q.8 La fonction cts prend en entrée un pointeur vers un Commit, et retourne 
une chaîne de caractères représentant ce Commit. La chaîne de caractères 
retournée est composée des chaînes de caractères représentant chacun des 
couples (clé, valeur), séparées par un saut de ligne */
char* cts(Commit* c);

/* Q.8 La fonction stc prend en entrée une chaîne de caractères représentant 
un Commit, et retourne un pointeur vers le Commit représenté par la chaîne de 
caractères. La chaîne de caractères doit être composée des chaînes de caractères 
représentant chacun des couples (clé, valeur), séparées par un saut de ligne */
Commit* stc(char* ch);

/* Q.9 La fonction ctf ecrit dans le fichier file la chaine de caracteres reprsentant 
le commit c */
void ctf(Commit* c, char* file);

/* Q.9 La fonction ftc permet de charger un Commit depuis un fichier le 
representant */
Commit* ftc(char* file);

/* Q.10 La fonction blobCommit cree un enregistrement instantane d'un Commit
en passant par un fichier temporaire. Elle retourne le hash du fichier temporaire   */
char* blobCommit(Commit* c);

//Libère Commit
void freeCommit(Commit *c);

// Retourne le WorkTree correspondant à la dernière version de la branche donnée en paramètre.
WorkTree* btwt(const char* branch);


#endif
