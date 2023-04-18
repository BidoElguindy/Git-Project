#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ex4.h"



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

    // Retourner le chemin complet du répertoire hash
    return hashToPath(hash);
}

void blobWorkTree(WorkTree* wt) {
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
    free(h)

    // Retourner le hash du fichier temporaire
    return hash;
}

char *concat_paths(char *path1, char *path2) {
    char *result = malloc(strlen(path1) + strlen(path2) + 1);
    if (result == NULL) {
        printf("Error: unable to allocate memory\n");
        return NULL;
    }
    strcpy(result, path1); // Copy path1 to result
    strcat(result, "/"); // Append a slash to result
    strcat(result, path2); // Append path2 to result
    return result;
}

char *saveWorkTree(WorkTree *wt, char *path) {
    for (int i = 0; i < wt->n; i++) {
        char *absPath = concat_paths(path, wt->tab[i].name);
        if (isFile(absPath) == 1) {
            blobFile(absPath);
            wt->tab[i].hash = sha256file(absPath);
            wt->tab[i].mode = getChmod(absPath);
        } else {
            WorkTree *wt2 = initWorkTree();
            List *L = listdir(absPath);
            for (Cell *ptr = *L; ptr != NULL; ptr = ptr->next) {
                if (ptr->data[0] == '.') {
                    continue;
                }
                appendWorkTree(wt2, ptr->data, 0, NULL);
            }
            wt->tab[i].hash = saveWorkTree(wt2, absPath);
            wt->tab[i].mode = getChmod(absPath);
        }
    }
    return blobWorkTree(wt);
}

int isWorkTree(char *hash) {
    if (file_exists(strcat(hashToPath(hash), ".t"))) {
        return 1;
    }
    if (file_exists(hashToPath(hash))) {
        return 0;
    }
    return -1;
}

void restoreWorkTree(WorkTree *wt, char *path) {
    for (int i = 0; i < wt->n; i++) {
        char *absPath = concat_paths(path, wt->tab[i].name);
        char *copyPath = hashToPath(wt->tab[i].hash);
        char *hash = wt->tab[i].hash;

        if (isWorkTree(hash) == 0) { // si c’est un fichier
            cp(absPath, copyPath);
            setMode(getChmod(copyPath), absPath);
        } else {
            if (isWorkTree(hash) == 1) { // si c’est un répertoire
                strcat(copyPath, ".t");
                WorkTree *nwt = ftwt(copyPath);
                restoreWorkTree(nwt, absPath);
                setMode(getChmod(copyPath), absPath);
            }
        }
    }
}
