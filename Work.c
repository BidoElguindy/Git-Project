#include "Work.h"

//EXERCICE 4

WorkFile* createWorkFile(char* name){
    WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile)); // Alloue de la mémoire pour la structure WorkFile
    wf->name = strdup(name); // Copie la chaîne de caractères passée en argument pour le nom du fichier
    wf->hash = NULL; // Initialise le hash à NULL
    wf->mode = 0; // Initialise le mode à 0
    return wf; // Retourne le pointeur sur la structure WorkFile nouvellement créée
}


char* wfts(WorkFile* wf){
    char * ch = malloc (1000* sizeof(char)); // Allocation dynamique de mémoire pour la chaîne de caractères
    sprintf (ch, "%s \t %s \t %d", wf->name, wf->hash, wf->mode) ; // Formatage de la chaîne de caractères
    return ch ; // Retourne la chaîne de caractères formatée
}


WorkFile* stwf(char* ch) {
    char name[100], hash[100];
    int mode;
    int num_fields = sscanf(ch, "%s %s %d", name, hash, &mode);  // Récupération des champs de la chaîne "ch" et stockage dans les variables "name", "hash" et "mode"
    if (num_fields != 3) {  // Si le nombre de champs n'est pas égal à 3
        return NULL;  // Retourne NULL
    }
    WorkFile* wf = createWorkFile(name);  // Crée un nouveau WorkFile à partir du nom "name" et stocke l'adresse de ce WorkFile dans la variable "wf"
    wf->hash = strdup(hash);  // Copie la chaîne "hash" dans l'attribut "hash" du WorkFile "wf"
    wf->mode = mode;  // Stocke la valeur de "mode" dans l'attribut "mode" du WorkFile "wf"
    return wf;  // Retourne l'adresse du WorkFile créé
}


WorkTree* initWorkTree() {
    // Allocation dynamique d'un pointeur WorkTree
    WorkTree* wt = malloc(sizeof(WorkTree));
    // Allocation dynamique d'un tableau de pointeurs WorkFile
    wt->tab = malloc(N_WF * sizeof(WorkFile*));
    // Initialisation de la taille maximale de la WorkTree
    wt->size = N_WF;
    // Initialisation du nombre d'éléments dans la WorkTree à 0
    wt->n = 0;
    // Renvoie le pointeur WorkTree initialisé
    return wt;
}


int inWorkTree(WorkTree* wt, char* name) {
    // Parcourir chaque élément du tableau de WorkFile dans le WorkTree
    for (int i = 0; i < wt->n; i++) {
        // Si le nom de l'élément actuel correspond au nom recherché,
        // retourner l'indice correspondant dans le tableau
        if (strcmp(wt->tab[i].name, name) == 0) {
            return i;
        }
    }
    // Si le nom n'a pas été trouvé, retourner -1
    return -1;
}


int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    int ind = inWorkTree(wt, name); // vérifier si le fichier ou répertoire existe déjà dans le WorkTree
    if (ind >= 0) { // si le fichier ou répertoire existe déjà, retourner une erreur
        printf("Nom : %s déjà dans WT \n", name);
        return -4;
    }
    if (wt->size > wt->n) { // si le tableau n'est pas plein, ajouter le nouveau WorkFile
        wt->tab[wt->n].mode = mode; // ajouter le mode au nouveau WorkFile
        wt->tab[wt->n].name = strdup(name); // copier le nom du nouveau WorkFile dans l'ancien WorkFile
        if (hash != NULL) { // si le hash est présent, copier la propriété hash du nouveau WorkFile dans l'ancien WorkFile
            wt->tab[wt->n].hash = strdup(hash);
        } 
        else { // sinon, mettre la propriété hash à NULL
            wt->tab[wt->n].hash = NULL;
        }
        wt->n++; // mettre à jour le nombre d'éléments dans le tableau
        return 0; // retourner 0 pour indiquer que l'ajout s'est bien déroulé
    }
    return ind; // retourner l'indice du WorkFile existant pour indiquer que l'ajout a échoué
}


char* wtts(WorkTree* wt) {
    char* str = (char*)malloc(sizeof(char) * 1024); // allouer une mémoire suffisamment grande pour stocker la chaîne de caractères
    str[0] = '\0'; // initialiser la chaîne de caractères à la chaîne vide
    for (int i = 0; i < wt->n; i++) {
        char* wf_str = wfts(&wt->tab[i]); // convertir chaque WorkFile en chaîne de caractères en utilisant la fonction wfts
        strcat(str, wf_str); // ajouter la chaîne de caractères au résultat final
        strcat(str, "\n"); // ajouter un saut de ligne
        free(wf_str); // libérer la mémoire allouée pour la chaîne de caractères
    }
    return str;
}


WorkTree* stwt(char* str) {
    WorkTree* wt = (WorkTree*)malloc(sizeof(WorkTree)); // allouer une mémoire pour la structure WorkTree
    wt->size = N_WF; // initialiser la taille par défaut du tableau à N_WF
    wt->n = 0; // initialiser le nombre d'éléments dans le tableau à 0
    wt->tab = (WorkFile*)malloc(sizeof(WorkFile) * wt->size); // allouer une mémoire pour le tableau de WorkFile
    char* pch = strtok(str, "\n"); // séparer la chaîne de caractères en lignes en utilisant le caractère '\n'
    while (pch != NULL) {
        WorkFile* wf = stwf(pch); // convertir la ligne en WorkFile en utilisant la fonction stwf
        int pos = inWorkTree(wt, wf->name); // vérifier si le fichier ou répertoire existe déjà dans le WorkTree
        if (pos != -1) { // si le fichier ou répertoire existe déjà, mettre à jour ses propriétés
            free(wt->tab[pos].hash); // libérer la mémoire allouée pour la propriété hash de l'ancien WorkFile
            wt->tab[pos].hash = strdup(wf->hash); // copier la propriété hash du nouveau WorkFile dans l'ancien WorkFile
            wt->tab[pos].mode = wf->mode; // mettre à jour la propriété mode de l'ancien WorkFile
            free(wf); // libérer la mémoire allouée pour le nouveau WorkFile
        } else { // sinon, ajouter le nouveau WorkFile au tableau
            if (wt->n == wt->size) { // si le tableau est plein, agrandir sa taille
                wt->size *= 2;
                wt->tab = (WorkFile*)realloc(wt->tab, sizeof(WorkFile) * wt->size);
            }
            wt->tab[wt->n] = *wf; // ajouter le nouveau WorkFile à la fin du tableau
            wt->n++; // mettre à jour le nombre d'éléments dans le tableau
        }
        pch = strtok(NULL, "\n"); // passer à la ligne suivante
    }
    return wt;
}


int wttf(WorkTree* wt, char* file) {
    
    // ouverture du fichier en mode écriture
    FILE* fp = fopen(file, "a");
    
    // gestion d'erreur en cas de fichier non ouvert
    if (fp == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", file);
        return -1;
    }

    // obtention de la chaîne de caractères représentant le WorkTree
    char* str = wtts(wt);

    // écriture de la chaîne de caractères dans le fichier
    fputs(str, fp);
    
    // fermeture du fichier et libération de la mémoire allouée
    fclose(fp);
    free(str);

    return 0;
}
WorkTree* ftwt(char *file) {
	// On ouvre le fichier en mode lecture.
	FILE *f = fopen(file,"r");
	// Si l'ouverture a échoué, on affiche un message d'erreur et on retourne NULL.
	if (f==NULL) {
		printf("erreur lors de l'ouverture du fichier : %s\n",file);	
		return NULL;
	}
	// On crée un buffer pour stocker les lignes du fichier.
	char buffer[256];
	// On alloue de la mémoire pour stocker l'arborescence au format string.
	char* strtree = (char*) malloc(1000*sizeof(char));
	while (fgets(buffer,256,f)) {
		// On ajoute chaque ligne lue au buffer.
		strcat(strtree,buffer);
	}
	// On ferme le fichier.
	fclose(f);
	// On convertit le string contenant l'arborescence en un WorkTree.
	WorkTree *wt = stwt(strtree);
	// On libère la mémoire allouée pour le string.
	free(strtree);
	// On retourne le pointeur vers le WorkTree.
	return wt;
}

//EXERCICE 5

int getChmod(const char* path) {
    struct stat ret;

    if (stat(path, &ret) == -1) {
        return -1;
    }

    return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | 
           (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) |
           (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH); 
}

void setMode (int mode , char * path ) {
    char buff [100];
    sprintf ( buff , "chmod %d %s ", mode , path ) ;
    system ( buff ) ;
}

char* hashToFile(char* hash) {
    // Créer une copie de la chaîne de caractères hash
    char* ch2 = strdup(hash);
    // Mettre le caractère null ('\0') à l'indice 2 pour avoir un chemin
    // de la forme XX/XXXX.../hash
    ch2[2] = '\0';
    // Vérifier si le répertoire hash existe, sinon le créer
    struct stat st;
    if (stat(ch2, &st) == -1) {
        mkdir(ch2, 0700);
    }
    free(ch2);
    // Retourner le chemin complet du répertoire hash
    return hashToPath(hash);
}

char * blobWorkTree(WorkTree* wt) {
    // Créer un nom de fichier temporaire unique dans le répertoire /tmp
    char fname[100] = "/tmp/myfileXXXXXX";
    int fd = mkstemp(fname);
    // Écrire le contenu du WorkTree dans le fichier temporaire
    wttf(wt, fname);
    // Calculer le hash du fichier temporaire
    char* hash = sha256file(fname);
    // Créer le chemin complet du fichier temporaire représentant le WorkTree
    char* ch = hashToFile(hash);
    strcat(ch, ".t");
    // Copier le fichier temporaire vers le fichier représentant l'état instantané
    // du WorkTree (avec l'extension ".t")
    cp(ch, fname);
    free(ch);
    // Retourner le hash du fichier temporaire
    return hash;
}


char *concat_paths(char *path1, char *path2) {
    // Alloue de la mémoire pour le chemin absolu résultant
    char *result = malloc(strlen(path1) + strlen(path2) + 1);
    // Vérifie si l'allocation de mémoire a échoué
    if (result == NULL) {
        printf("Error: unable to allocate memory\n");
        return NULL;
    }
    // Copie path1 dans result
    strcpy(result, path1);
    // Ajoute un slash à result
    strcat(result, "/");
    // Ajoute path2 à result
    strcat(result, path2);
    // Renvoie le chemin absolu résultant
    return result;
}

int isFile(const char *path) {
    struct stat path_stat; // déclare une structure pour stocker les informations sur le fichier
    stat(path, &path_stat); // appelle la fonction stat() pour récupérer les informations sur le fichier et les stocker dans la structure
    return S_ISREG(path_stat.st_mode); // vérifie si le fichier est un fichier régulier en utilisant la macro S_ISREG() sur le champ st_mode de la structure path_stat, puis retourne 1 si c'est le cas et 0 sinon
}


char* saveWorkTree(WorkTree* wt, char* path) {
    // Affichage du chemin du WorkTree en cours de sauvegarde
    printf("saveworktree path : %s\n",path);
    
    // Pour chaque élément du WorkTree
    for (int i=0;i<wt->n;i++) {
        // Concaténation du chemin absolu du WorkTree avec le nom de l'élément
        char *absPath = concat_paths(path,wt->tab[i].name);
        
        // Affichage de l'élément en cours de sauvegarde
        printf("sauvegarde de %s\n",absPath);
        
        // Si l'élément est un fichier
        if (isFile(absPath) == 1) {
            printf("regular file\n");
            
            // Création d'un blob représentant le contenu du fichier
            blobFile(absPath);
            
            // Calcul du hash SHA256 du fichier
            wt->tab[i].hash = sha256file(absPath);
            
            // Récupération des permissions (chmod) du fichier
            wt->tab[i].mode = getChmod(absPath);
        }
        // Si l'élément est un répertoire
        else {
            printf("dir\n");
            
            // Initialisation d'un nouveau WorkTree pour le sous-répertoire
            WorkTree *wt2 = initWorkTree();
            
            // Liste des éléments du sous-répertoire
            List *L = listdir(absPath);
            
            for(Cell* ptr = *L; ptr != NULL; ptr = ptr->next) {
                // Si l'élément ne commence pas par un point (fichier caché)
                if (ptr->data[0] != '.') {
                    // Ajout de l'élément au WorkTree
                    appendWorkTree(wt2,ptr->data,NULL,0);
                }
            }
            // Libération de la mémoire allouée pour la liste
            freeList(L);
                
            // Sauvegarde du sous-répertoire et récupération de son hash
            wt->tab[i].hash = saveWorkTree(wt2,absPath);
                
            // Récupération des permissions (chmod) du répertoire
            wt->tab[i].mode = getChmod(absPath);
            // Libération de la mémoire allouée pour le sous-WorkTree
            freeWorkTree(wt2);
        }
        // Libération de la mémoire allouée pour le chemin absolu
        free(absPath);
    }
    // Affichage d'une information de sortie de la fonction
    printf("exit saveworktree path : %s\n",path);
    
    // Création d'un blob représentant l'état instantané du WorkTree
    return blobWorkTree(wt);
}


void freeWorkTree(WorkTree *wt) {
    if (wt != NULL){ // Vérification que le pointeur n'est pas NULL
        for (int i=0;i<wt->n;i++) { // Itération sur tous les fichiers de l'arbre de travail
            free(wt->tab[i].name);
            free(wt->tab[i].hash); // Libération de la mémoire du fichier courant
        }
        free(wt->tab); // Libération de la mémoire du tableau de fichiers
        free(wt); // Libération de la mémoire de l'arbre de travail
    }
}

void freeWorkFile(WorkFile *wf) {
    if (wf != NULL){ // Vérification que le pointeur n'est pas NULL
	    free(wf->name); // Libération de la mémoire de la chaîne de caractères représentant le nom du fichier
        free(wf->hash); // Libération de la mémoire de la chaîne de caractères représentant le hash du contenu du fichier
	    free(wf); // Libération de la mémoire du fichier
    }
}

void printWorkTree(WorkTree *wt) {
	int i=0; // Initialisation de l'indice i
	while (i<wt->n) { // Itération sur tous les fichiers de l'arbre de travail
		printf("%s\n",wt->tab[i].name); // Affichage du nom du fichier courant
		i++; // Incrémentation de l'indice
	}
}


int isWorkTree(char* hash) {
    // On vérifie si le hash correspond à un WorkTree en cherchant un fichier avec l'extension ".t"
    if (file_exists(strcat(hashToPath(hash), ".t"))) {
        return 1;
    }
    // On vérifie si le hash correspond à un fichier en cherchant un fichier sans l'extension ".t"
    if (file_exists(hashToPath(hash))) {
        return 0;
    }
    // Si le fichier n'existe pas, on retourne -1
    return -1;
}


void restoreWorkTree(WorkTree* wt, char* path) {
    // Parcourt tous les éléments du WorkTree
    for (int i = 0; i < wt->n; i++) {
        // Construit le chemin absolu du fichier ou dossier
        char* absPath = concat_paths(path, wt->tab[i].name);
        // Construit le chemin absolu de la sauvegarde correspondante
        char* copyPath = hashToPath(wt->tab[i].hash);
        // Récupère le hash du fichier ou dossier
        char* hash = wt->tab[i].hash;

        // Vérifie si l'élément est un fichier ou un dossier
        if (isFile(hash) == 1) { // si c'est un fichier
            // Copie la sauvegarde vers le chemin absolu du fichier
            cp(copyPath, absPath);
            // Définit les autorisations du fichier
            setMode(getChmod(copyPath), absPath);
        } else if (isWorkTree(hash) == 1) { // si c'est un dossier
            // Ajoute l'extension ".t" pour construire le chemin absolu de la sauvegarde du dossier
            strcat(copyPath, ".t");
            // Crée un nouveau WorkTree à partir de la sauvegarde du dossier
            WorkTree* nwt = ftwt(copyPath);
            // Restaure le WorkTree à partir de la sauvegarde
            restoreWorkTree(nwt, absPath);
            // Définit les autorisations du dossier
            setMode(getChmod(copyPath), absPath);
        } else {
            // Si l'élément n'est ni un fichier ni un dossier, affiche un message d'erreur
            printf("Error: unknown file type\n");
        }
        // Libère la mémoire allouée pour les chemins absolus
        free(absPath);
        free(copyPath);
    }
}

