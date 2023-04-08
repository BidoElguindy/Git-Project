
#ifndef EX4_h
#define EX4_h

typedef struct {
char * name ;
char * hash ;
int mode ;
} WorkFile ;

typedef struct {
WorkFile * tab ;
int size ;
int n ;
} WorkTree ;

int getChmod(const char * path );
void setMode (int mode , char * path );
WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);
/*La fonction prend une chaîne de caractères ch représentant un WorkFile et retourne un pointeur vers une structure WorkFile nouvellement allouée. La fonction utilise sscanf pour extraire les champs name, hash et mode de la chaîne de caractères. Si le nombre de champs extraits est différent de 3, cela signifie que la chaîne de caractères ne représentait pas un WorkFile valide, la fonction retourne donc NULL. Sinon, elle alloue un nouveau WorkFile, copie les champs extraits dans la structure et retourne un pointeur vers la structure allouée.*/
WorkTree* initWorkTree();
int inWorkTree(WorkTree* wt, char* name) ;
/*La fonction parcourt le tableau wt->tab en comparant le champ name de chaque élément avec la chaîne de caractères 
name en utilisant la fonction strcmp. Si une correspondance est trouvée, la fonction retourne l'indice de l'élément dans le tableau. 
Si aucun élément ne correspond, la fonction retourne -1.*/
int appendWorkTree(WorkTree* wt, char* name, char* hash, int
mode);
/*La fonction commence par chercher le fichier ou répertoire dans le WorkTree en appelant la fonction inWorkTree. Si le fichier ou répertoire est déjà présent dans le WorkTree, la fonction retourne l'indice correspondant.

Si le fichier ou répertoire n'est pas déjà présent, la fonction vérifie si le WorkTree est plein en comparant wt->n avec wt->size. Si le WorkTree est plein, la fonction retourne -1.

Sinon, la fonction crée un nouveau WorkFile en utilisant la fonction createWorkFile, lui affecte le hash et le mode, et l'ajoute à la fin du tableau wt->tab. Enfin, la fonction libère la mémoire allouée pour le WorkFile et retourne l'indice du fichier ou répertoire dans le tableau.*/
char* wtts(WorkTree* wt);
/*La fonction commence par allouer une mémoire suffisamment grande pour stocker la chaîne de caractères en utilisant la fonction malloc. La taille allouée est de 1024 octets, mais elle peut être ajustée en fonction de la taille maximale attendue de la chaîne de caractères. La fonction initialise la chaîne de caractères à la chaîne vide en affectant le caractère nul à son premier élément.

La fonction parcourt ensuite le tableau wt->tab en appelant la fonction wfts pour convertir chaque WorkFile en chaîne de caractères. Elle ajoute la chaîne de caractères au résultat final en utilisant la fonction strcat, et ajoute un saut de ligne à la fin en utilisant la chaîne "\n". La fonction libère la mémoire allouée pour la chaîne de caractères correspondant à chaque WorkFile.

Enfin, la fonction retourne la chaîne de caractères résultante.*/
WorkTree * stwt ( char * ch );
/*La fonction commence par allouer une mémoire pour la structure WorkTree wt, en initialisant la taille du tableau wt->tab à 10 et le nombre d'éléments dans le tableau à 0. Elle alloue également une mémoire pour le tableau de WorkFile.

La fonction utilise la fonction strtok pour séparer la chaîne de caractères en lignes en utilisant le caractère '\n'. Pour chaque ligne, la fonction appelle la fonction stwf pour convertir la ligne en WorkFile. Elle utilise ensuite la fonction inWorkTree pour vérifier si le fichier ou répertoire existe déjà dans le WorkTree. Si c'est le cas, elle met à jour les propriétés du WorkFile existant. Sinon, elle ajoute le nouveau WorkFile à la fin du tableau. Si le tableau est plein, elle agrandit sa taille en*/
int wttf(WorkTree* wt, char* file);
/*Si le fichier a été ouvert avec succès, la fonction convertit le WorkTree en chaîne en utilisant la fonction wtts. Il écrit ensuite la chaîne au fichier en utilisant fputs, et ferme le fichier en utilisant fclose.*/
WorkTree* ftwt(char* file);
/*Si le fichier a été ouvert avec succès, la fonction lit son contenu dans une chaîne en utilisant getline. Elle ferme ensuite le fichier en utilisant fclose.

Ensuite, la fonction analyse la chaîne en utilisant la fonction stwt pour créer un WorkTree.

Enfin, la fonction libère la chaîne et renvoie le WorkTree.*/


#endif
