#include "Branch.h"

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


void restoreCommit(char* hash_commit) {
    // On récupère le chemin du commit à partir de son hash
    char* commit_path = hashToPathCommit(hash_commit);

    // On crée un objet commit à partir du chemin récupéré
    Commit* c = ftc(commit_path);

    // On récupère le hash de l'arbre associé au commit et on le convertit en chemin
    char* tree_hash = strcat(hashToPath(commitGet(c, "tree")), ".t");

    // On crée un objet worktree à partir du chemin de l'arbre
    WorkTree* wt = ftwt(tree_hash);

    // On restaure le worktree associé au commit
    restoreWorkTree(wt, ".");
}

void myGitCheckoutBranch(char* branch) {
    // On construit la commande Git pour changer de branche
    char* command = malloc(strlen("git checkout ") + strlen(branch) + 1);
    strcpy(command, "git checkout ");
    strcat(command, branch);

    // On exécute la commande
    system(command);

    // On libère la mémoire allouée pour la commande
    free(command);
}

List* filterList(List* L, char* pattern) {
    // On crée une nouvelle liste pour stocker les éléments filtrés
    List* filteredList = initList();

    // On parcourt chaque élément de la liste initiale
    for (Cell* current = *L; current != NULL; current = current->next) {
        // On vérifie si l'élément commence par le pattern donné
        if (strncmp(current->data, pattern, strlen(pattern)) == 0) {
            // Si c'est le cas, on ajoute l'élément à la liste filtrée
            insertFirst(&filteredList, buildCell(current->data));
        }
    }

    // On retourne la liste filtrée
    return filteredList;
}

/* Fonction qui retourne l'élément à l'indice i dans la liste */
char* getElement(List* L, int i) {
    int j = 0;
    Cell* ptr = *L;

    while (ptr != NULL && j < i) {
        ptr = ptr->next;
        j++;
    }

    if (ptr != NULL) {
        return ptr->data;
    } else {
        return NULL;
    }
}

/* Fonction qui affiche tous les éléments de la liste */
void printList(List* L) {
    Cell* ptr = *L;

    while (ptr != NULL) {
        printf("%s\n", ptr->data);
        ptr = ptr->next;
    }
}

/* Fonction qui retourne la longueur de la liste */
int length(List* L) {
    int count = 0;
    Cell* ptr = *L;

    while (ptr != NULL) {
        count++;
        ptr = ptr->next;
    }

    return count;
}

/* Fonction qui libère toute la mémoire allouée pour la liste */
void freeList(List* L) {
    Cell* ptr = *L;
    Cell* temp;

    while (ptr != NULL) {
        temp = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = temp;
    }

    *L = NULL;
}

void myGitCheckoutCommit(char* pattern) {
    // Récupération de la liste de tous les commits
    List* allCommits = getAllCommits();

    // Filtrage des commits qui commencent par le pattern
    List* filteredCommits = filterList(allCommits, pattern);

    // S'il ne reste plus qu'un commit après le filtre, on met à jour la référence HEAD et on restaure le worktree correspondant
    if (length(filteredCommits) == 1) {
        char* hash_commit = getElement(filteredCommits, 0);
        createUpdateRef("HEAD", hash_commit);
        restoreCommit(hash_commit);
    }
    // S'il ne reste plus aucun commit après le filtre, on affiche un message d'erreur
    else if (length(filteredCommits) == 0) {
        printf("No matching commit found.\n");
    }
    // S'il reste plusieurs commits après le filtre, on les affiche tous et on demande à l'utilisateur de préciser sa requête
    else {
        printf("Multiple matching commits found:\n");
        printList(filteredCommits);
        printf("Please specify your request with a full commit hash or additional filtering.\n");
    }

    // Libération de la mémoire allouée pour les listes
    freeList(allCommits);
    freeList(filteredCommits);
}
