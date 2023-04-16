#include "Commit.h"
#include "Work.h"


//EXERCICE 6


kvp* createKeyVal(char* key, char* val) {
    // Allouer de la mémoire pour la nouvelle paire
    kvp* kv = malloc(sizeof(kvp));
    if (kv == NULL) {
        // Impossible d'allouer de la mémoire pour la paire
        return NULL;
    }
    // Copier la clé dans la paire allouée dynamiquement
    kv->key = strdup(key);
    if (kv->key == NULL) {
        // Impossible de copier la clé, libérer la mémoire allouée pour la paire
        free(kv);
        return NULL;
    }
    // Copier la valeur dans la paire allouée dynamiquement
    kv->value = strdup(val);
    if (kv->value == NULL) {
        // Impossible de copier la valeur, libérer la mémoire allouée pour la clé et la paire
        free(kv->key);
        free(kv);
        return NULL;
    }
    // Retourner la paire nouvellement créée
    return kv;
}


void freeKeyVal(kvp* kv) {
    // Vérifier que la paire n'est pas nulle
    if (kv != NULL){
        // Vérifier que la clé n'est pas nulle
        if (kv->key != NULL) {
            // Libérer la mémoire allouée pour la clé
            free(kv->key);
        }
        // Vérifier que la valeur n'est pas nulle
        if (kv->value != NULL) {
            // Libérer la mémoire allouée pour la valeur
            free(kv->value);
        }
        // Libérer la mémoire allouée pour la paire
        free(kv);
    }
}


char* kvts(kvp* k) {
    // Allocation de la mémoire pour le buffer
    char* buff = malloc(sizeof(char) * 100);
    // Formatage du buffer en utilisant sprintf pour concaténer les clés et les valeurs de la paire clé-valeur
    sprintf(buff, "%s:%s", k->key, k->value);
    // Retourne le buffer nouvellement formaté
    return buff;
}

kvp* stkv(char* str) {                     
    char* p = strchr(str, ':');            // Trouver la première occurrence de ':' dans la chaîne str et renvoyer un pointeur vers l'endroit où il a été trouvé
    if (p == NULL) {                       // Si le caractère ':' n'est pas trouvé, retourner NULL
        return NULL;
    }
    size_t len_key = p - str;              // Calculer la longueur de la clé en soustrayant la position de ':' de la position de début de la chaîne str
    char* key = (char*) malloc(sizeof(char) * (len_key + 1));   // Allouer de la mémoire pour la clé en fonction de la longueur calculée et ajouter 1 pour le caractère nul de fin de chaîne
    strncpy(key, str, len_key);            // Copier la sous-chaîne correspondant à la clé dans la chaîne key
    key[len_key] = '\0';                   // Ajouter le caractère nul de fin de chaîne à la fin de la clé
    kvp* k = createKeyVal(key, p + 1);     // Appeler la fonction createKeyVal avec la clé et la sous-chaîne correspondant à la valeur, en allouant de la mémoire pour la structure kvp et les chaînes de caractères key et value
    free(key);                             // Libérer la mémoire allouée pour key, car elle a été copiée dans la structure kvp par createKeyVal
    return k;                              // Renvoyer la structure kvp
}


Commit* initCommit() {
    // Allocation de l'espace pour la structure de Commit
    Commit* c = malloc(sizeof(Commit));
    // Vérification de l'allocation de mémoire pour la structure Commit
    if (c == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation de mémoire pour le commit\n");
        return NULL;
    }
    // Initialisation les champs de la structure Commit
    c->n = 0;
    c->size = COMMIT_SIZE;
    // Allocation de l'espace pour le tableau de pointeurs de type kvp*
    c->T = malloc(sizeof(kvp*) * COMMIT_SIZE);
    // Vérification de l'allocation de mémoire pour le tableau de pointeurs
    if (c->T == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation de mémoire pour le tableau de pointeurs\n");
        free(c);
        return NULL;
    }
    // Initialisation de chaque pointeur du tableau à NULL
    for (int i = 0; i < COMMIT_SIZE; i++) {
        c->T[i] = NULL;
    }
    return c; // Renvoie un pointeur vers la structure de Commit initialisée
}

unsigned long hashF(char * str){
    unsigned long hash = 0; // initialise la valeur du hachage à 0
    int c; // déclare une variable entière pour stocker la valeur ASCII de chaque caractère de la chaîne
    while((c = *str++)!='\0'){ // parcourt les caractères de la chaîne jusqu'à ce que le caractère nul soit rencontré
        hash = c + (hash << 6) + (hash << 16) - hash; // calcule la valeur du hachage en utilisant sdbm
    }
    return hash; // retourne la valeur de hachage calculée
}


void commitSet(Commit* c, char* key, char* value) {
    int p = hashF(key) % c->size; // obtenir l'index où stocker la paire clé-valeur en utilisant la fonction de hachage "hashF"
    while (c->T[p] != NULL) { // boucler jusqu'à trouver un emplacement vide dans la table de hachage
        if (strcmp(c->T[p]->key, key) == 0) { // si une paire clé-valeur avec la clé donnée existe déjà dans la table de hachage, libérer la mémoire
            freeKeyVal(c->T[p]);
            break;
        }
        p = (p+1) % c->size; // probing linéaire : passer à l'emplacement suivant en utilisant une méthode de sondage linéaire
    }
    kvp* pair = createKeyVal(key, value); // créer une nouvelle paire clé-valeur avec la clé et la valeur données
    if (pair != NULL) { // si la paire clé-valeur a été créée avec succès
        c->T[p] = pair; // stocker la paire clé-valeur à l'emplacement approprié dans la table de hachage
        c->n++; // incrémenter le nombre de paires clé-valeur stockées dans la table de hachage
    }
}

Commit* createCommit(char* hash){ 
    Commit* c = initCommit(); // crée un nouvel objet Commit en appelant la fonction initCommit()
    commitSet(c,"tree", hash); // définit la clé "tree" avec la valeur de hachage donnée en utilisant la fonction commitSet()
    return c; // retourne l'objet Commit créé
}


char* commitGet(Commit* c, char* key) {
    int index = hashF(key) % c->size;  // Calcul de l'indice initial
    int i = 0;
    while (i < c->size && c->T[index] != NULL) { // Tant que la case n'est pas vide et qu'on a pas parcouru toutes les cases
        if (c->T[index] != NULL && strcmp(c->T[index]->key, key) == 0) { // Si la clé est trouvée
            return c->T[index]->value;  // On retourne la valeur correspondante
        }
        i++;
        index = (index + 1) % c->size; // On avance à la case suivante avec le probing linéaire
    }
    return NULL;  // La clé n'a pas été trouvée
}

char* cts(Commit* c){
    char* res= (char*)malloc(sizeof(char)*KVTS_SIZE*c->n); // alloue de la mémoire pour stocker les paires clé-valeur sous forme de chaîne de caractères
    res[0]='\0'; // initialiser la chaîne de caractères résultat à la chaîne vide
    char * kvtss = NULL;
    for(int i=0; i< c->size ; i++){ // boucle à travers tous les emplacements de la table de hachage
        if(c->T[i] != NULL){ // si une paire clé-valeur est stockée à l'emplacement actuel
            kvtss = kvts(c->T[i]); // convertir la paire clé-valeur en une chaîne de caractères
            strcat(res, kvtss); // concaténer la chaîne de caractères résultat avec la chaîne de caractères représentant la paire clé-valeur
            strcat(res,"\n"); // ajouter un saut de ligne à la fin de chaque paire clé-valeur
            free(kvtss); // libérer la mémoire allouée pour la chaîne de caractères représentant la paire clé-valeur
        }
    }
    return res; // retourne la chaîne de caractères résultat contenant toutes les paires clé-valeur stockées dans la table de hachage
}


Commit *stc(char *str) {
    Commit *c = initCommit(); // initialiser un nouveau commit vide
    char *key;
    char *value;
    char *line = strtok(str, "\n"); // divise la chaîne de caractères "str" en lignes individuelles en utilisant "\n" comme délimiteur
    
    while (line != NULL) { // boucle à travers chaque ligne dans la chaîne de caractères "str"
        key = strtok(line, ":"); // divise la ligne actuelle en deux parties: clé et valeur, en utilisant ":" comme délimiteur
        value = strtok(NULL, ":");
        commitSet(c, key, value); // ajoute la paire clé-valeur à la table de hachage du commit
        line = strtok(NULL, "\n"); // passer à la ligne suivante
    }
    
    return c; // retourne le commit rempli avec les paires clé-valeur extraites de la chaîne de caractères "str"
}


void ctf(Commit* c, char* file) {
    // Ouverture du fichier en mode écriture
    FILE *fp = fopen(file, "w");

    // Vérification que l'ouverture du fichier s'est bien passée
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    // Conversion du commit en chaîne de caractères
    char* commit_str = cts(c);

    // Écriture de la chaîne de caractères dans le fichier
    fputs(commit_str, fp);

    //Libération de la mémoire
    free(commit_str);

    // Fermeture du fichier
    fclose(fp);
}

Commit* ftc(char* file) {
    // Ouverture du fichier en mode lecture
    FILE *fp = fopen(file, "r");

    // Vérification que l'ouverture du fichier s'est bien passée
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    // Lecture de la chaîne de caractères du fichier
    char buffer[10000];
    fgets(buffer, 10000, fp);

    // Fermeture du fichier
    fclose(fp);

    // Conversion de la chaîne de caractères en commit
    Commit* c = stc(buffer);

    return c;
}


char *blobCommit(Commit *c) {
    char fname[100] = "/tmp/myfileXXXXXX"; // Initialise une chaîne de caractères fname avec un chemin vers un fichier temporaire
    mkstemp(fname); // Crée un fichier temporaire vide avec le chemin fname
    ctf(c, fname); // Copie le contenu du Commit c dans le fichier temporaire fname
    char *hash = sha256file(fname); // Calcule le hash SHA-256 du fichier fname
    char *path = hashToPath(hash); // Génère un chemin de fichier unique basé sur le hash
    char *ret = strdup(hash); // Sauvegarde le hash pour le renvoyer en tant que résultat de la fonction
    hash[2] = '\0'; // Les 2 premiers caractères du hash sont utilisés pour créer un répertoire dans la fonction suivante
    char output[1000]; // Initialise une chaîne de caractères output
    if (!file_exists(hash)) { // Vérifie si le répertoire basé sur le hash n'existe pas déjà
        sprintf(output,"mkdir %s",hash); // Crée une commande shell pour créer le répertoire basé sur le hash
        system(output); // Exécute la commande shell pour créer le répertoire
    }
    free(hash); // Libère la mémoire allouée pour le hash
    sprintf(output,"touch %s.c",path); // Crée une commande shell pour créer un fichier C vide avec le chemin de fichier unique
    system(output); // Exécute la commande shell pour créer le fichier C vide
    sprintf(output,"%s.c",path); // Met à jour le chemin du fichier output pour qu'il pointe vers le fichier C vide
    cp(output,fname); // Copie le contenu du répertoire de travail vers le fichier C vide
    printf("commit sauvegarde dans %s\n",path); // Affiche le chemin de fichier unique pour le fichier C généré
    free(path); // Libère la mémoire allouée pour le chemin de fichier unique
    return ret; // Renvoie le hash SHA-256 calculé
}


void printCommit(Commit* c) {
    printf("Commit with %d entries:\n", c->n); // affiche le nombre de paires clé-valeur stockées dans le commit
    for (int i = 0; i < c->size; i++) { // boucle à travers tous les emplacements de la table de hachage
        if (c->T[i] != NULL) { // si une paire clé-valeur est stockée à l'emplacement actuel, l'afficher
            printf("%s: %s\n", c->T[i]->key, c->T[i]->value);
        }
    }
}


void commitDeleteValue(Commit* c, char* value) {
    // parcours de toutes les cases de la table de hachage
    for (int i = 0; i < c->size; i++) {
        // si la case est occupée et que la valeur correspond
        if (c->T[i] != NULL && strcmp(c->T[i]->value, value) == 0) {
            // on supprime la paire clé-valeur de la table
            freeKeyVal(c->T[i]);
            c->T[i] = NULL;
            c->n--;
            return;
        }
    }
}


void freeCommit(Commit *c) {
    if (c!=NULL) { // Vérifie si le Commit c n'est pas NULL
        for (int i=0;i<c->n;i++) { // Parcourt tous les éléments de la table de hachage du Commit c
            freeKeyVal(c->T[i]); // Libère la mémoire allouée pour chaque élément de la table de hachage
        }
        free(c->T); // Libère la mémoire allouée pour la table de hachage
    }
    free(c); // Libère la mémoire allouée pour le Commit c
}

