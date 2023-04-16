#ifndef _REFS_H_
#define _REFS_H_

#include "Commit.h"


/* Q.1 La fonction iniRefs initialise le repertoire .refs */
void initRefs();

/* Q.2 La fonction createUpdateRef cree et/ou met a jour la ref ref_name avec le hash donne */
void createUpdateRef(char *ref_name, char *hash);
 
/* Q.3 La fonction deleteRef supprime la ref donnee */
void deleteRef(char* ref_name);

/* Q.4 La fonction getRef renvoie le hash de la ref donnee */
char* getRef(char* ref_name);

/* Q.5 La fonction "createFile" crée un fichier vide avec le nom spécifié en utilisant la commande système "touch" */
void createFile(char *file);

/* Q.5 La fonction myGitAdd ajoute le fichier ou repertoire donne dans le .add et le cree s'il n'existe pas */
void myGitAdd(char *file_or_folder);

/* Q.6 La fonction myGitCommit cree un commit et le stocke dans la branche (ref) donnee */
void myGitCommit(char* branch_name, char* message);


#endif 
