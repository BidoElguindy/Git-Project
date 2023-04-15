#ifndef _REFS_H_
#define _REFS_H_
#include "Commit.h"


/*initialise le repertoire .refs*/
void initRefs();

/*cree et/ou met a jour la ref ref_name avec le hash donne*/
void createUpdateRef(char *ref_name, char *hash);
 

/*supprime la ref donnee*/
void deleteRef(char* ref_name);

/*renvoie le hash de la ref donnee*/
char* getRef(char* ref_name);

/*raccourci pour creer un fichier avec touch*/
void createFile(char *file);

/*ajoute le fichier ou repertoire donne dans le .add et le cree s'il n'existe pas*/
void myGitAdd(char *file_or_folder);

/*cree un commit et le stocke dans la branche (ref) donnee*/
void myGitCommit(char* branch_name, char* message);




#endif 
