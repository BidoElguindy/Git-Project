#include "commit.h"
#include "Work.h"


//EXERCICE 6


kvp* createKeyVal(char* key, char* val) {
    kvp* k = malloc(sizeof(kvp)); // Allocation de la mémoire pour une nouvelle paire clé-valeur
    k->key = strdup(key); // Copie de la clé dans la structure
    k->value = strdup(val); // Copie de la valeur dans la structure
    return k; // Retourne la nouvelle paire clé-valeur allouée
}

void freeKeyVal(kvp* kv) {
    free(kv->key);
    free(kv->value);
    free(kv);
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
    // On utilise la fonction strtok pour extraire la clé et la valeur de la chaîne.
    // On récupère la première sous-chaîne jusqu'au premier délimiteur ':'.
    char* key = strtok(str, ":");
    // On récupère la deuxième sous-chaîne à partir du délimiteur ':'.
    char* value = strtok(NULL, ":");
    // On crée la structure kvp à partir de la clé et de la valeur extraites.
    return createKeyVal(key, value);
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

Commit* createCommit(char* hash) {
    // Allocation de mémoire pour un nouveau Commit
    Commit* c = initCommit();
    // Vérification si l'allocation de mémoire a été effectuée correctement
    if (c == NULL) {
        fprintf(stderr, "Error: could not allocate memory for commit\n");
        return NULL;
    }
    // Création de la clé "tree" avec la valeur correspondant à l'arbre de travail
    char* tree_val = kvts(stkv(strcat("tree:", hash)));
    // Ajout de la clé "tree" avec sa valeur dans le Commit
    commitSet(c, "tree", tree_val);
    // Retour du Commit
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
    char fname[100] = "/tmp/myfileXXXXXX";
    mkstemp(fname);
    ctf(c, fname);
    char *hash = sha256file(fname);
    char *path = hashtoPath(hash);
    char *ret = strdup(hash); // Sauvegarde du hash pour rendu de fonction
	hash[2] = '\0'; // 2 premiers cara du hash pour mkdir
	char output[1000];
	if (!file_exists(hash)) {
        sprintf(output,"mkdir %s",hash);
	    system(output);
    }
    free(hash);
	sprintf(output,"touch %s.c",path);
	system(output);
	sprintf(output,"%s.c",path);
	cp(output,fname); // Copie du contenu de wt vers le fichier .t
    printf("commit sauvegarde dans %s\n",path);
	free(path);
    return ret;
}

void freeCommit(Commit *c) {
    if (c!=NULL) {
        for (int i=0;i<c->n;i++) {
            freeKeyVal(c->T[i]);
        }
        free(c->T);
    }
    free(c);
}



int main() {
    // Test createKeyVal et freeKeyVal
    printf("Testing createKeyVal and freeKeyVal...\n");
    kvp* kv = createKeyVal("key", "value");
    printf("Key: %s, Value: %s\n", kv->key, kv->value);
    free(kv);

    // Test kvts et stkv
    printf("Testing kvts and stkv...\n");
    char* kv_string = "key=value\n";
    kvp* kv2 = stkv(kv_string);
    printf("Key: %s, Value: %s\n", kv2->key, kv2->value);
    char* kv_string2 = kvts(kv2);
    printf("String: %s\n", kv_string2);
    free(kv2);
    free(kv_string2);

    // Test initCommit et commitSet
    printf("Testing initCommit and commitSet...\n");
    Commit* c = initCommit(10);
    commitSet(c, "key1", "value1");
    commitSet(c, "key2", "value2");
    printf("Value for key1: %s\n", commitGet(c, "key1"));
    printf("Value for key2: %s\n", commitGet(c, "key2"));

    // Test createCommit
    printf("Testing createCommit...\n");
    Commit* c2 = createCommit("hash123");
    commitSet(c2, "key3", "value3");
    char* commit_string = cts(c2);
    printf("Commit string: %s\n", commit_string);
    free(commit_string);

    // Test commitGet
    printf("Testing commitGet...\n");
    char* value = commitGet(c2, "key3");
    printf("Value for key3: %s\n", value);
    free(value);

    // Test stc
    printf("Testing stc...\n");
    Commit* c3 = stc(commit_string);
    printf("Value for key3: %s\n", commitGet(c3, "key3"));
    free(commit_string);

    // Test ctf et ftc
    printf("Testing ctf and ftc...\n");
    ctf(c2, "test_commit.txt");
    Commit* c4 = ftc("test_commit.txt");
    printf("Value for key3: %s\n", commitGet(c4, "key3"));

    // Test blobCommit
    printf("Testing blobCommit...\n");
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

