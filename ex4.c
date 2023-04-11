#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ex4.h"
#define N_WF 20



WorkFile* createWorkFile(char* name){
    WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile)); // Alloue de la mémoire pour la structure WorkFile
    wf->name = strdup(name); // Copie la chaîne de caractères passée en argument pour le nom du fichier
    wf->hash = NULL; // Initialise le hash à NULL
    wf->mode = 0; // Initialise le mode à 0
    return wf; // Retourne le pointeur sur la structure WorkFile nouvellement créée
}

char* wfts(WorkFile* wf){
    char * ch = malloc (1000* sizeof(char)); // Allocation dynamique de mémoire pour la chaîne de caractères
    sprintf ( ch , " %s \t %s \t %d", wf->name , wf->hash , wf->mode ) ; // Formatage de la chaîne de caractères
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
    // allocation dynamique d'un pointeur WorkTree
    WorkTree* wt = malloc(sizeof(WorkTree));
    // allocation dynamique d'un tableau de pointeurs WorkFile
    wt->tab = malloc(N_WF * sizeof(WorkFile*));
    // initialisation de la taille maximale de la WorkTree
    wt->size = N_WF;
    // initialisation du nombre d'éléments dans la WorkTree à 0
    wt->n = 0;
    // renvoie le pointeur WorkTree initialisé
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
        } else { // sinon, mettre la propriété hash à NULL
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
    FILE* fp = fopen(file, "w");
    
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



WorkTree* ftwt(char* file) {
    // Ouvrir le fichier en mode lecture
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        // En cas d'erreur lors de l'ouverture du fichier
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n", file);
        return NULL;
    }
    // Initialiser les variables pour la lecture de la chaîne de caractères représentant le WorkTree
    char* str = NULL;
    size_t size = 0;
    ssize_t bytes_read = 0;
    // Lire le fichier et stocker la chaîne de caractères dans 'str'
    while ((bytes_read = getline(&str, &size, fp)) != -1) {}

    // Fermer le fichier
    fclose(fp);

    // Construire le WorkTree à partir de la chaîne de caractères 'str'
    WorkTree* wt = stwt(str);

    // Libérer la mémoire allouée pour la chaîne de caractères 'str'
    free(str);
    return wt;
}



int main(){
    WorkFile* wf1= createWorkFile("Wf1");
    char * cwf1 = wfts(wf1);
    printf("%s ,",cwf1);
    WorkTree* wt1 = initWorkTree();
    wt1->tab[0]=*(stwf(cwf1));
    printf("%s ,",wfts(&(wt1->tab[0])));
    appendWorkTree(wt1,cwf1, "OOO", 777);
    int indwf1 = inWorkTree(wt1,cwf1);
    printf("%d\t,",indwf1);
    appendWorkTree(wt1,"WF2", "OOO", 777);
    char* cwt1 = wtts(wt1);
    printf("%s\t,",cwt1);
    WorkTree* wt2= stwt(cwt1); 
    wttf(wt1,"etst.txt");
    WorkTree* wt3 = ftwt("etst.txt");

    return 1;
}
