#include "Commit.h"
#include "Work.h"


//EXERCICE 6


kvp* createKeyVal(char* key, char* val) {
    kvp* k = malloc(sizeof(kvp)); // Allocation de la mémoire pour une nouvelle paire clé-valeur
    k->key = strdup(key); // Copie de la clé dans la structure
    k->value = strdup(val); // Copie de la valeur dans la structure
    return k; // Retourne la nouvelle paire clé-valeur allouée
}

void freeKeyVal(kvp* kv) {
    if (kv != NULL){
        free(kv->key);
        free(kv->value);
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
    // Initialisation les champs de la structure Commit
    c->n = 0;
    c->size = COMMIT_SIZE;
    // Allocation de l'espace pour le tableau de pointeurs de type kvp*
    c->T = malloc(sizeof(kvp*) * COMMIT_SIZE);
    // Initialisation de chaque pointeur du tableau à NULL
    for (int i = 0; i < COMMIT_SIZE; i++) {
        c->T[i] = NULL;
    }
    return c; // Renvoie un pointeur vers la structure de Commit initialisée
}

unsigned long hashF(char * str){
    unsigned long hash = 0;
    int c;
    while((c = *str++)!='\0'){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

void commitSet(Commit* c, char* key, char* value) {
    unsigned long index = hashF(key) % c->size; // Calcul de l'indice à partir de la clé
    unsigned long i = index;
    do {
        if (c->T[i] == NULL || strcmp(c->T[i]->key, key) == 0) { // Si la case est vide ou contient la clé cherchée
            c->T[i] = createKeyVal(key, value); // On insère la paire dans la case courante
            c->n++;
            return;
        }
        i = (i + 1) % c->size; // Prochaine case, avec wrapping
    } while (i != index); // Tant qu'on n'a pas fait un tour complet
    fprintf(stderr, "Error: table de hachage pleine, impossible d'insérer la paire (%s,%s)\n", key, value);
}

Commit* createCommit(char* hash){ 
    Commit* c = initCommit(); 
    commitSet(c,"tree", hash);
    return c;
}

char* commitGet(Commit* c, char* key) {
    int index = hashF(key) % c->size;  // Calcul de l'indice initial
    int i = 0;
    while (c->T[index] != NULL && i < c->size) { // Tant que la case n'est pas vide et qu'on a pas parcouru toutes les cases
        if (strcmp(c->T[index]->key, key) == 0) { // Si la clé est trouvée
            return c->T[index]->value;  // On retourne la valeur correspondante
        }
        i++;
        index = (index + 1) % c->size; // On avance à la case suivante avec le probing linéaire
    }
    return NULL;  // La clé n'a pas été trouvée
}


char* cts(Commit* c) {
    char* str = malloc(sizeof(char) * 1000);
    str[0] = '\0'; // Initialisation de la chaîne de caractères vide
    for (int i = 0; i < c->size; i++) {
        if (c->T[i] != NULL) {
            char* kv_str = kvts(c->T[i]); // Conversion du couple clé-valeur en une chaîne de caractères
            strcat(str, kv_str); // Ajout de la chaîne de caractères représentant le couple clé-valeur à la chaîne résultante
            strcat(str, "\n"); // Ajout d'un saut de ligne entre les couples
            free(kv_str); // Libération de la mémoire allouée pour la chaîne de caractères représentant le couple clé-valeur
        }
    }
    return str;
}

Commit* stc(char* ch) {
    Commit* c = initCommit(); // Initialisation d'un nouveau Commit vide
    char* tok = strtok(ch, "\n"); // Découpage de la chaîne de caractères en sous-chaînes représentant chaque couple (clé, valeur)
    while (tok != NULL) {
        kvp* kv = stkv(tok); // Conversion de la sous-chaîne représentant un couple (clé, valeur) en une structure kvp
        commitSet(c, kv->key, kv->value); // Ajout du couple (clé, valeur) au Commit
        free(kv->key); // Libération de la mémoire allouée pour la clé
        free(kv->value); // Libération de la mémoire allouée pour la valeur
        free(kv); // Libération de la mémoire allouée pour la structure kvp
        tok = strtok(NULL, "\n"); // Passage à la sous-chaîne suivante
    }
    return c;
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

void freeCommit(Commit *c) {
    if (c!=NULL) { // Vérifie si le Commit c n'est pas NULL
        for (int i=0;i<c->n;i++) { // Parcourt tous les éléments de la table de hachage du Commit c
            freeKeyVal(c->T[i]); // Libère la mémoire allouée pour chaque élément de la table de hachage
        }
        free(c->T); // Libère la mémoire allouée pour la table de hachage
    }
    free(c); // Libère la mémoire allouée pour le Commit c
}


/*

int main() {
    printf("Test Exercice 6 \n");
    printf("\n");
    
     // Test createKeyVal et freeKeyVal
    printf("Testing createKeyVal and freeKeyVal \n");
    kvp* kv = createKeyVal("key", "value");
    printf("Key: %s, Value: %s\n", kv->key, kv->value);
    free(kv);

    // Test kvts et stkv
    printf("Testing kvts and stkv\n");
    char* kv_string = "key: value\n";
    kvp* kv2 = stkv(kv_string);  
    printf("Key: %s, Value: %s\n", kv2->key, kv2->value);
    char* kv_string2 = kvts(kv2);
    printf("String: %s\n", kv_string2);
    free(kv2);
    free(kv_string2);

    // Test initCommit et commitSet
    printf("Testing initCommit and commitSet\n");
    Commit* c = initCommit();
    commitSet(c, "key1", "value1");
    commitSet(c, "key2", "value2");
    printf("Value for key1: %s\n", commitGet(c, "key1"));
    printf("Value for key2: %s\n", commitGet(c, "key2"));

    // Test createCommit
    printf("Testing createCommit\n");
    Commit* c2 = createCommit("hash123");
    commitSet(c2, "key3", "value3");
    char* commit_string = cts(c2);
    printf("Commit string: %s\n", commit_string);

    // Test commitGet
    printf("Testing commitGet\n");
    char* value = commitGet(c2, "key3");
    printf("Value for key3: %s\n", value);
    free(value);

    // Test stc
    printf("Testing stc\n");
    Commit* c3 = stc(commit_string);
    printf("Value for key3: %s\n", commitGet(c3, "key3"));
    free(commit_string);

    // Test ctf et ftc
    printf("Testing ctf and ftc\n");
    ctf(c3, "test_commit.txt");
    Commit* c4 = ftc("test_commit.txt");
    printf("Value for key3: %s\n", commitGet(c4, "key3"));

    // Test blobCommit
    printf("Testing blobCommit\n");
    char* hash = blobCommit(c2);
    printf("Blob hash: %s\n", hash);
    free(hash);

    // Libération de la mémoire
    freeCommit(c);
    freeCommit(c2);
    freeCommit(c3);
    freeCommit(c4);
     
    return 0;
}

*/
