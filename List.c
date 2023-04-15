#include "List.h"

//EXERCICE 1 

int hashFile(char* source, char* dest){
    // Vérification que les fichiers source et dest existent
    FILE* sourceFile = fopen(source, "r");
    if (sourceFile == NULL) {
        printf("Le fichier source n'existe pas.\n");
        return 0;
    }
    fclose(sourceFile);
    FILE* destFile = fopen(dest, "w");
    if (destFile == NULL) {
        printf("Le fichier destination n'existe pas.\n");
        return 0;
    }
    fclose(destFile);
	
    // Déclaration d'un tampon de 1000 octets pour stocker la commande shell sha256sum
    char buffer[1000];
    // Création de la commande shell sha256sum qui calcule le hash du fichier source
    // et redirige la sortie vers le fichier destination en utilisant ">"
    sprintf(buffer, "sha256sum %s > %s", source, dest);
    // Exécution de la commande shell stockée dans le tampon buffer sha256sum
    system(buffer);
    return 1;
}

char* sha256file(char* file){
    // Définition d'un modèle pour le nom du fichier temporaire
    char template [] = "/tmp/myfileXXXXXX";
    // Définition d'un tableau de caractères pour stocker le nom du fichier temporaire
    char fname[1000];
    // Copie du modèle de nom de fichier temporaire dans le tableau
    strncpy(fname, template, sizeof(fname) - 1);
    // Ajout du caractère nul à la fin du tableau pour le terminer en tant que chaîne de caractères valide
    fname[sizeof(fname) - 1] = '\0'; 
    // Création d'un fichier temporaire unique avec le nom défini
    int fd = mkstemp(fname);
    
	// Si l'ouverture du fichier temporaire a échoué, afficher une erreur et quitter la fonction
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }
    
	// Calcul du hash du fichier d'entrée et stockage du résultat dans le fichier temporaire
    if (!hashFile(file, fname)) {
        close(fd);
        return NULL;
    }
    
	// Définition d'un tableau de caractères pour stocker le hash
    char hash[1000];
    
	// Ouverture du fichier temporaire en mode lecture seule
    FILE *f = fopen(fname, "r");
    // Si l'ouverture du fichier temporaire a échoué, afficher une erreur et quitter la fonction
    if (f == NULL) {
        perror("fopen");
        close(fd);
        return NULL;
    }
    
	// Lecture de la première ligne du fichier temporaire, qui contient le hash
    fgets(hash, sizeof(hash), f);
    
	// Fermeture du fichier temporaire
    fclose(f);
    
	// Fermeture du descripteur de fichier du fichier temporaire
    close(fd);
    
	// Allocation d'un tableau de caractères de la même taille que le hash pour stocker le résultat
    char *m = malloc(sizeof(hash));
    
	// Si l'allocation de mémoire a échoué, afficher une erreur et quitter la fonction
    if (m == NULL) {
        perror("malloc");
        return NULL;
    }
   
    // Lecture du hash dans le tableau de caractères alloué
    sscanf(hash, "%s", m); 
    
	// Retourne le tableau de caractères contenant le hash
    return m;
}


//EXERCICE 2  


List *initList(){
    // Allocation dynamique d'un pointeur vers une nouvelle liste
    List *l = (List *) (malloc(sizeof(List)));
    *l = NULL;
    // Retour du pointeur vers la nouvelle liste
    return l;
}


Cell * buildCell(char *ch){
    // Allocation de la mémoire pour une nouvelle cellule
    Cell * c = (Cell *) (malloc(sizeof(Cell)));

    // Copie de la chaîne de caractères passée en paramètre dans la cellule
    c->data = strdup(ch);

    // Initialisation du pointeur suivant à NULL car il n'y a pas encore de cellule suivante
    c->next = NULL;

    // Retour de la cellule nouvellement créée
    return c;
}


void insertFirst(List *L, Cell *C){
    // Le pointeur next de la cellule a ajouter pointe vers l'ancienne premiere cellule de la liste
    C->next = *L;
    
    // La liste pointe maintenant vers la nouvelle cellule ajoutée en tant que première cellule
    *L = C;
}


char * ctos(Cell *c){ 
    //Renvoie le pointeur vers la chaine de caracteres stockee dans la cellule.
    return c->data;
}

char* ltos(List* L) {
    // Si la liste est vide, retour d'une chaine de caracteres vide
    if (*L == NULL) {
        return "";
    }

    // Initialisation d'un pointeur sur la liste et une variable pour stocker la longueur totale
    List ptr = *L;
    int len = strlen(ptr->data);

    // Calcul de la longueur totale de la chaine en parcourant la liste
    while (ptr->next != NULL) {
        // Ajout de la longueur de la chaine actuelle, plus 1 pour le separateur '|'
        len += strlen(ptr->next->data) + 1;
        ptr = ptr->next;
    }

    // Allocation memoire pour stocker la chaine resultante, plus 1 pour le caractere nul
    char* ch = malloc(sizeof(char) * (len + 1));

    // Construction de la chaine resultante
    // Copie de la premiere chaine dans la chaine resultante
    ptr = *L;
    strcpy(ch, ptr->data);
    ptr = ptr->next;
    // Parcourt de la liste, concatenation de chaque chaine avec le separateur '|'
    while (ptr != NULL) {
        strcat(ch, "|");
        strcat(ch, ptr->data);
        ptr = ptr->next;
    }

    // Retour de la chaine resultante
    return ch;
}


Cell* listGet(List* L, int i){
  // Si la liste L est NULL ou l'index i est invalide, il n'y a pas d'elements a renvoyer 
  if (L == NULL || i <= 0) return NULL;

  // On initialise un pointeur 'cell' a l'adresse du premier element de la liste 
  Cell * cell = *L;

  // On initialise un compteur 'cpt' a 1 pour savoir a quel element on est rendu 
  int cpt = 1;

  // On boucle tant que l'on a pas atteint le ieme element ou la fin de la liste 
  while(cell && cpt<i){
    // On incremente le compteur 
    cpt++;

    // On passe au prochain element de la liste
    cell = cell->next;
  }

  // Si l'index est supérieur au nombre d'éléments dans la liste, on renvoie NULL
  if (!cell) return NULL;

  // On retourne le ieme element de la liste 
  return cell;
}


Cell * searchList(List *L, char *str){
  //On initialise un pointeur 'tmp' a l'adresse du premier element de la liste 
  Cell* tmp = *L;

  //On boucle tant que l'on n'a pas atteint la fin de la liste ou l'element recherche 
  while(tmp && (strcmp(ctos(tmp),str)!=0)){
    //On passe a l'element suivant de la liste 
    tmp = tmp->next;
  }

  //On retourne l'element recherche ou NULL s'il n'est pas dans la liste 
  return tmp;
}


List* stol(char* s){
    int pos = 0;      // La position actuelle dans la chaîne de caractères
    int n_pos = 0;    // La prochaine position de séparation dans la chaîne de caractères
    int size = strlen(s); // La taille de la chaîne de caractères en entrée
    int sep = '|';        // Le séparateur utilisé pour séparer les éléments de la liste
    char* ptr;        // Pointeur vers le prochain séparateur dans la chaîne de caractères
    char* result = malloc(sizeof(char)*1000); // Une chaîne de caractères pour stocker chaque élément de la liste
    List *L = initList();  // Initialisation de la liste chainée

    // Boucle while qui parcourt la chaîne de caractères en entrée
    while (pos < size){
        // Recherche du prochain séparateur dans la chaîne de caractères
        ptr = strchr(s+ pos, sep); 

        // Si aucun séparateur n'est trouvé, on considère que la fin de la chaîne de caractères a été atteinte
        if (ptr == NULL)
            n_pos = size+1; 

        // Sinon, on note la position du prochain séparateur
        else{
          n_pos = ptr - s + 1; 
        }
        // On copie l'élément de la chaîne de caractères correspondant à la position actuelle et à la prochaine position de séparation
        memcpy(result, s+pos, n_pos - pos - 1); 

        // On ajoute un caractère de fin de chaîne pour terminer la chaîne de caractères correspondant à l'élément
        result[n_pos - pos - 1] = '\0';

        // On met à jour la position actuelle dans la chaîne de caractères
        pos = n_pos;

        // On insère l'élément dans la liste chainée
        insertFirst(L, buildCell(result));
    }
    free(result);
    // On retourne la liste chainée
    return L;
}


void afficherListe(List liste) {
    Cell* actuel = liste;

    while (actuel != NULL) {
        printf("%s\n", actuel->data);
        actuel = actuel->next;
    }
}


void ltof(List* L, char* path) {
    // Ouverture du fichier pour l'écriture
    FILE* file = fopen(path, "w");

    // Vérification de la bonne ouverture du fichier
    if (file == NULL) {
        // Affichage d'un message d'erreur si l'ouverture a échoué
        printf("Error opening file for writing.\n");
        return;
    }

    // Initialisation de la variable "cell" avec le premier élément de la liste
    Cell* cell = *L;

    // Parcourt la liste jusqu'à la fin
    while (cell != NULL) {
        // Écriture de la donnée de la cellule courante dans le fichier
        fprintf(file, "%s\n", cell->data);
        // Passage à la cellule suivante
        cell = cell->next;
    }

    // Fermeture du fichier
    fclose(file);
}


List* ftol(char* path){
  // Ouverture du fichier en mode lecture
  FILE* f = fopen(path, "r");
  
  // Vérification de la bonne ouverture du fichier
  if(f == NULL){
    printf("Error opening file.\n");
    return NULL;
  }

  // Initialisation d'une nouvelle liste vide
  List* lres = initList();
  // Initialisation d'un pointeur vers une cellule
  Cell* cell;
  char buff[1000];
  
  // Lecture des lignes du fichier et création d'une cellule pour chaque ligne
  while(fgets(buff, 100, f)){
    // Création d'une cellule pour la ligne courante
    cell = buildCell(buff);
    // Insertion de la cellule en début de liste
    insertFirst(lres, cell);
  }
  
  // Fermeture du fichier
  fclose(f);
  
  // Retourne la liste résultante
  return lres;
}


//EXERCICE 3 

List* listdir(char* root_dir){ // Déclaration de la fonction listdir prenant en paramètre le chemin du répertoire à lister
    DIR *dp; // Pointeur vers le répertoire ouvert
    struct dirent *ep; // Structure dirent qui contiendra les informations de chaque élément du répertoire
    List* L = initList(); // Initialisation d'une liste vide pour stocker les noms des éléments du répertoire
    *L = NULL; // Initialisation du premier élément de la liste à NULL
    Cell* temp_cell; // Pointeur vers une cellule temporaire qui sera insérée dans la liste
    dp = opendir (root_dir); // Ouverture du répertoire donné en paramètre
    if (dp != NULL){ // Si l'ouverture du répertoire a réussi
        while ((ep = readdir (dp)) != NULL) { // Parcours de tous les éléments du répertoire
            temp_cell = buildCell(ep->d_name); // Construction d'une cellule contenant le nom de l'élément courant
            insertFirst(L,temp_cell); // Insertion de la cellule en début de liste
            List ptr = *L; // Initialisation d'un pointeur pour parcourir la liste
            while (ptr != NULL){ // Parcours de tous les éléments de la liste
                ptr = ptr->next; // Passage à l'élément suivant
            }
        }
        (void) closedir (dp); // Fermeture du répertoire
    }
    else{ // Si l'ouverture du répertoire a échoué
        perror ("Couldn't open the directory"); // Affichage d'un message d'erreur
        return NULL; // Retourne NULL pour signaler l'erreur
    }
    return L; // Retourne la liste contenant les noms des éléments du répertoire
}


int file_exists(char *file){            
    List* L = listdir(".");            // Initialisation d'une liste chainée contenant les noms des fichiers et des répertoires du répertoire courant avec la fonction listdir()
    Cell* c = searchList(L,file);      // Recherche de la cellule contenant le nom du fichier passé en paramètre avec la fonction searchList()
    if(c!=NULL){                       // Vérifie que la cellule contenant le nom du fichier a été trouvée
        freeList(L);
        return 1;                      // Retourne 1 si le fichier existe
    }
    freeList(L);
    return 0;                          // Retourne 0 si le fichier n'existe pas
}


void cp(char *to, char *from){
    if(file_exists(from)==1){              // Vérifie que le fichier source existe
        FILE* f1 = fopen(from, "r");       // Ouvre le fichier source en mode lecture
        FILE* f2 = fopen(to, "w");         // Ouvre le fichier de destination en mode écriture
        char buff[1024];                   // Buffer pour stocker chaque ligne lue du fichier source
        while(fgets(buff,sizeof(buff),f1)!=NULL){           // Lit chaque ligne du fichier source et l'écrit dans le fichier de destination
            fputs(buff,f2);
        }
        fclose(f1);             // Ferme les fichiers source et de destination
        fclose(f2);
    }
}


char* hashToPath(char* hash){ 
    char* res = malloc(strlen(hash) + 1);           // Allocation dynamique de mémoire pour une chaîne de caractères res de longueur égale à la longueur de hash plus 1 (pour le caractère de fin de chaîne)
    if (res == NULL) {                              // Vérification si l'allocation de mémoire a réussi
        return NULL;                                // Retourne NULL s'il y a eu une erreur
    }
    memcpy(res,hash,2);                             // Copie des deux premiers caractères de hash dans res à l'aide de la fonction memcpy
    strcat(res,"/");                                // Ajout du caractère / à la fin de la chaîne res à l'aide de la fonction strcat
    strcat(res,hash+2);                             // Ajout du reste du hash (à partir du troisième caractère) à la fin de la chaîne res à l'aide de la fonction strcat
    return res;                                     // Retourne le pointeur vers la nouvelle chaîne de caractères res.
}


void blobFile(char* file) {
    char* hash = sha256file(file);  // Calcul du hash du fichier d'entrée
    if (hash == NULL) {             // Vérification si le calcul du hash a réussi
        printf("Impossible de calculer le hash du fichier %s\n", file);
        return;
    }
    
    char* ch2 = strdup(hash);  // Copie du hash dans une nouvelle chaîne de caractères
    ch2[2] = '\0';             // Ajout du caractère de fin de chaîne après le deuxième caractère de la chaîne ch2
    
    if (!file_exists(ch2)) {  // Vérification si le dossier correspondant au préfixe du hash existe
        char buff[100];
        sprintf(buff, "mkdir %s", ch2);  // Création de la commande shell pour créer le dossier correspondant
        system(buff);         // Exécution de la commande shell
    }
    
    char* ch = hashToPath(hash);  // Création du chemin vers le fichier de destination
    cp(ch, file);                 // Copie du fichier source vers le fichier de destination
    printf("Instantané enregistré sous %s\n", ch);  // Affichage du chemin vers le fichier de destination
    
    free(hash);  // Libération de la mémoire allouée
    free(ch);
}

void freeCell(Cell *c) {
	if (c) {
		freeCell(c->next);
		free(c->data);
	}
	free(c);
}

void freeList(List *l) {
	freeCell(*l);
	free(l);
}

/*
int main(){
    
    //EXERCICE 1  

	// Nom du fichier à hasher
    char* filename = "draft.txt";

    // Calcul du hash du fichier
    char* hash = sha256file(filename);
    if (hash == NULL) {
        printf("Erreur : impossible de calculer le hash du fichier %s\n", filename);
        return 1;
    }

    // Affichage du hash
    printf("Hash du fichier %s : %s\n", filename, hash);

    // Libération de la mémoire allouée pour le hash
    free(hash);
    
    //EXERCICE 2

    // Création d'une nouvelle liste vide
    List* L = initList();

    // Création de deux nouvelles cellules pour la liste
    Cell* C1 = buildCell("1");
    Cell* C2 = buildCell("2");

    // Insertion de la première cellule en tant que première cellule de la liste
    insertFirst(L, C1);

    // Insertion de la deuxième cellule en tant que première cellule de la liste
    insertFirst(L, C2);

    // Conversion de la liste en chaîne de caractères
    char* s = ltos(L);

    // Affichage de la chaîne de caractères
    printf("La conversion de la liste en chaine de caracteres est %s\n", s);

    // Recherche de la deuxième cellule de la liste
    Cell* C3 = listGet(L, 2);

    // Vérification que la cellule retournée est bien la deuxième cellule de la liste
    assert(C3 == C1);

    // Recherche d'une cellule qui n'existe pas dans la liste
    Cell* C4 = searchList(L, "3");

    // Vérification que la fonction de recherche renvoie NULL
    assert(C4 == NULL);

    // Recherche d'une cellule qui existe dans la liste
    Cell* C5 = searchList(L, "1");

    // Vérification que la cellule retournée est bien la deuxième cellule de la liste
    assert(C5 == C1);

    // Conversion d'une chaîne de caractères en liste
    List* L2 = stol("3|4"); 
    printf("La conversion de la chaine de caracteres en liste donne\t");
    afficherListe(*L2);

    // Vérification que la nouvelle liste contient les éléments attendus
    assert(strcmp(ctos(*L2), "4") == 0);
    assert(strcmp(ctos((*L2)->next), "3") == 0);

    // Enregistrement de la liste dans un fichier
    ltof(L, "test.txt");

    // Lecture de la liste enregistrée dans le fichier
    List* L3 = ftol("test.txt");

    // Affichage de la liste lue à partir du fichier
    printf("Liste lue du fichier:\n");
    Cell* c = *L3;
    while (c) {
      printf("%s", c->data);
      c = c->next;
    }

    // Libération de la mémoire allouée pour les listes et les cellules
    free(s);
    free(C1);
    free(C2);
    free(L);
    free(L2);
    free(L3);

    //EXERCICE 3

    List* L4 = listdir(".");                     // Test de la fonction listdir()
    printf("Liste des fichiers et répertoires du répertoire courant :\n");
    afficherListe(*L4);;                         // Affichage de la liste des fichiers et répertoires du répertoire courant
    
    char* file = "test.txt";
    int res = file_exists(file);                 // Test de la fonction file_exists()
    if(res == 1){
        printf("Le fichier %s existe\n", file);
    }
    else{
        printf("Le fichier %s n'existe pas\n", file);
    }
    
    char* from = "test.txt";
    char* to = "test_copy.txt";
    cp(to,from);                                // Test de la fonction cp()
    printf("Le fichier %s a été copié dans %s\n", from, to);
    
    char* hash2 = "aabbccddeeff";
    char* path = hashToPath(hash2);             // Test de la fonction hashToPath()
    printf("Le chemin vers le fichier correspondant au hash %s est %s\n", hash2, path);
    
    char* file2 = "test_copy.txt";
    blobFile(file2);                            // Test de la fonction blobFile()
    
    free(path);                                 // Libération de la mémoire allouée
    free(L4);

    return 0;
}

*/
