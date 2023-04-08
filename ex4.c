#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ex4.h"
#define N_WF 20
/*
int getChmod ( const char * path ) {
    struct stat ret ;
    if (stat(path,&ret) == -1) {
        return -1;
        }
    return ( ret.st_mode & S_IRUSR ) |( ret.st_mode & S_IWUSR ) |( ret.st_mode & S_IXUSR ) |( ret.st_mode & S_IRGRP ) |( ret.st_mode & S_IWGRP ) |( ret.st_mode & S_IXGRP ) | ( ret.st_mode & S_IROTH ) |( ret.st_mode & S_IWOTH ) |( ret.st_mode & S_IXOTH ) ;
    }
*/
void setMode (int mode , char * path ) {
    char buff [100];
    sprintf ( buff , "chmod %d %s ", mode , path ) ;
    system ( buff ) ;
    }

WorkFile* createWorkFile(char* name){
    WorkFile * wf=(WorkFile * )malloc(sizeof(WorkFile));
    wf->name=strdup(name);
    wf->hash=NULL;
    wf->mode=0;
    return 0;
}
char* wfts(WorkFile* wf){
    char * ch = malloc (1000* sizeof(char));
    sprintf ( ch , " %s \t %s \t %d", wf->name , wf->hash , wf->mode ) ;
    return ch ;
}

WorkFile* stwf(char* ch) {
    char name[100], hash[100];
    int mode;
    int num_fields = sscanf(ch, "%s %s %d", name, hash, &mode);
    if (num_fields != 3) {
        return NULL;
    }
    WorkFile* wf = createWorkFile(name);
    wf->hash = strdup(hash);
    wf->mode = mode;
    return wf;
}
WorkTree * initWorkTree () {
    WorkTree * wt = malloc ( sizeof ( WorkTree ) ) ;
    wt -> tab = malloc ( N_WF * sizeof ( WorkFile ) ) ;
    wt -> size = N_WF ;
    wt -> n = 0;
    return wt ;
}
int inWorkTree(WorkTree* wt, char* name) {
    for (int i = 0; i < wt->n; i++) {
        if (strcmp(wt->tab[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int appendWorkTree( WorkTree * wt , char * name , char * hash , int mode ) {
    int ind=inWorkTree ( wt , name );
    if (ind >= 0) {
        printf ("Nom : %s deja dans WT \n", name ) ;
        return -4;
    }
    if ( wt->size > wt -> n ) {
        wt -> tab [ wt -> n ]. mode = mode ;
        wt -> tab [ wt -> n ]. name = strdup( name ) ;
        if ( hash != NULL )
            wt-> tab [ wt-> n++].hash = strdup( hash ) ;
        else
            wt -> tab [ wt-> n++].hash = NULL ;
            return 0;
        }
    return ind;
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
    wt->size = 10; // initialiser la taille par défaut du tableau à 10
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
    // Open the file for writing
    FILE* fp = fopen(file, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", file);
        return -1;
    }

    // Convert the WorkTree to a string
    char* str = wtts(wt);

    // Write the string to the file
    fputs(str, fp);

    // Close the file and free the string
    fclose(fp);
    free(str);

    return 0;
}

WorkTree* ftwt(char* file) {
    // Open the file for reading
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", file);
        return NULL;
    }

    // Read the contents of the file into a string
    char* str = NULL;
    size_t size = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = getline(&str, &size, fp)) != -1) {}

    // Close the file
    fclose(fp);

    // Parse the string and create a WorkTree
    WorkTree* wt = stwt(str);

    // Free the string and return the WorkTree
    free(str);
    return wt;
}

