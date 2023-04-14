

void initBranch() {
    FILE *f = fopen(".currentbranch", "w");
    fputs("master", f);
    fclose(f);
}

int branchExists(char* branch) {
    // Obtient la liste des fichiers et des répertoires dans le répertoire .refs
    List* refs = listdir(".refs");
    // Recherche la branche dans la liste obtenue et renvoie vrai si elle est trouvée
    return searchList(refs, branch) != NULL;
}

void createBranch(char* branch) {
    // Récupérer le hash du commit courant depuis HEAD
    char* headRef = getRef("HEAD");
    char* commitHash = getRef(headRef);

    // Créer la nouvelle référence de branche avec le hash du commit
    char* branchRef = malloc(strlen(branch) + 6);  // ajouter le préfixe ".refs/"
    sprintf(branchRef, ".refs/%s", branch);
    setRef(branchRef, commitHash);
    free(branchRef);

    // Libérer la mémoire allouée par getRef
    free(headRef);
    free(commitHash);
}

char* getCurrentBranch() {
    // Ouvre le fichier .current_branch en mode lecture
    FILE* file = fopen(".current_branch", "r");

    // Vérifie si le fichier est null
    if (file == NULL) {
        printf("Erreur: le fichier .current_branch n'a pas pu être ouvert.\n");
        exit(1);
    }

    // Alloue de la mémoire pour stocker le nom de la branche
    char* branchName = malloc(sizeof(char) * 100);

    // Lit le nom de la branche à partir du fichier .current_branch
    fscanf(file, "%s", branchName);

    // Ferme le fichier
    fclose(file);

    // Retourne le nom de la branche
    return branchName;
}


char* hashToPathCommit(char* hash) {
    // Alloue suffisamment de mémoire pour stocker le chemin vers le fichier commit
    char* buff = malloc(sizeof(char) * 100);
    // Construit le chemin vers le fichier commit en utilisant la fonction hashToPath pour récupérer le chemin du répertoire
    sprintf(buff, "%s.c", hashToPath(hash));
    return buff;
}


void printBranch(char* branch) {
    // Récupère le hash du commit associé à la branche donnée
    char* commit_hash = getRef(branch);
    // Récupère le commit associé au hash obtenu
    Commit* c = ftc(hashToPathCommit(commit_hash));
    // Boucle sur les commits jusqu'à ce qu'il n'y en ait plus
    while (c != NULL) {
        // Si un message descriptif existe pour le commit, l'affiche avec son hash
        if (commitGet(c, "message") != NULL)
            printf("%s -> %s\n", commit_hash, commitGet(c, "message"));
        else
            // Sinon, n'affiche que le hash
            printf("%s\n", commit_hash);
        // Récupère le hash du prédécesseur du commit courant s'il en existe un
        if (commitGet(c, "predecessor") != NULL) {
            commit_hash = commitGet(c, "predecessor");
            // Récupère le commit associé au hash du prédécesseur
            c = ftc(hashToPathCommit(commit_hash));
        } else {
            // S'il n'y a pas de prédécesseur, arrête la boucle
            c = NULL;
        }
    }
}


List* branchList(char* branch) {
    List* L = initList(); // Initialisation de la liste
    char* commit_hash = getRef(branch); // Récupération du hash du commit initial
    if (commit_hash == NULL) {
        printf("Erreur : la branche spécifiée n'existe pas.\n");
        return NULL;
    }
    Commit* c = ftc(hashToPathCommit(commit_hash)); // Récupération du commit correspondant
    while (c != NULL) {
        insertFirst(L, buildCell(commit_hash)); // Ajout du hash du commit courant à la liste
        if (commitGet(c, "predecessor") != NULL) {
            commit_hash = commitGet(c, "predecessor"); // Récupération du hash du commit prédécesseur
            c = ftc(hashToPathCommit(commit_hash)); // Récupération du commit correspondant
        } else {
            c = NULL;
        }
    }
    return L;
}

List* getAllCommits() {
    /* Initialisation de la liste L */
    List* L = initList();
    /* Récupération du contenu du dossier ".refs" */
    List* content = listdir(".refs");
    /* Parcours des branches dans le dossier ".refs" */
    for(Cell* ptr = *content; ptr != NULL; ptr = ptr->next) {
        /* Ignorer les fichiers cachés */
        if(ptr->data[0] == '.') {
            continue;
        }
        /* Récupération de la liste des commits pour chaque branche */
        List* list = branchList(ptr->data);
        /* Parcours de chaque commit de la branche */
        Cell* cell = *list;
        while(cell != NULL) {
            /* Ajout du commit dans la liste L s'il n'est pas déjà présent */
            if(searchList(L, cell->data) == NULL) {
                insertFirst(&L, buildCell(cell->data));
            }
            cell = cell->next;
        }
    }
    /* Retour de la liste L contenant tous les commits sans répétition */
    return L;
}
