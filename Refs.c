#include "refs.h"

void initRefs(){
    if(!file_exists(".refs")){
        system("mkdir .refs");
        system("touch .refs/master");
        system("touch .refs/HEAD");
    }
}

void creerUpdateRef(char* ref_name, char* hash) {
    // Construire le nom de fichier de la référence à partir du nom de référence donné
    char filename[100];
    sprintf(filename, ".refs/%s", ref_name);

    // Ouvrir le fichier en mode d'écriture
    FILE* fp = fopen(filename, "w");

    // Vérifier si l'ouverture du fichier a réussi
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    // Écrire le hash dans le fichier
    fprintf(fp, "%s", hash);

    // Fermer le fichier
    fclose(fp);
}



void deleteRef(char* ref_name) {
    // On construit le chemin absolu du fichier de référence
    char filename[100];
    sprintf(filename, ".refs/%s", ref_name);

    // Si le fichier existe, on le supprime
    if (fileExists(filename)) {
        if (unlink(filename) == -1) { // unlink supprime le fichier
            perror("unlink"); // Affiche le message d'erreur associé à errno
            exit(EXIT_FAILURE); // Arrête le programme en cas d'erreur
        }
    } else { // Si le fichier n'existe pas
        printf("Reference %s does not exist.\n", ref_name);
    }
}

char* getRefs(char* ref_name) {
    // Alloue de la mémoire pour le résultat
    char *result = malloc(sizeof(char)*256);
    // Récupère la liste de tous les fichiers dans le répertoire ".refs"
    List* l = listdir(".refs");
    // Vérifie si la référence existe
    if (searchList(l,ref_name)==NULL) {
        printf("The reference %s does not exist\n",ref_name);
        return NULL;
    }
    // Construit le chemin du fichier de la référence
    char buff[256];
    sprintf(buff,".refs/%s",ref_name);
    // Ouvre le fichier
    FILE *fp = fopen(buff,"r");
    // Vérifie si le fichier a été ouvert correctement
    if (fp == NULL) {
        printf("Error opening file %s\n",ref_name);
        return NULL;
    }
    // Lit le contenu du fichier et le stocke dans le résultat
    fgets(result,256,fp);
    // Ferme le fichier
    fclose(fp);
    // Retourne le résultat
    return result;
}
void createFile(char *file) {
    char buff[100];
    sprintf(buff,"touch %s",file);
    system(buff);
}

/*
 Ajoute un fichier ou un dossier dans la zone de préparation (add)
 */
void myGitAdd(char *file_or_folder) {
    WorkTree *wt;
    
    // Vérification de l'existence du fichier .add
    if (!fileExists(".add")) {
        printf("creating .add\n");
        createFile(".add");
        wt = initWorkTree();
    }
    else {
        wt = ftwt(".add");
    }
    
    // Vérification de l'existence du fichier ou dossier à ajouter
    if (fileExists(file_or_folder)) {
        // Ajout du fichier ou dossier à la zone de préparation
        int res = appendWorkTree(wt, file_or_folder, NULL, 0);
        if (res == -2) {
            printf("file or folder already in .add\n");
            return;
        }
        wttf(wt, ".add");
    }
    else {
        printf("file or folder %s does not exist\n", file_or_folder);
    }
}


void myGitCommit(char* branch_name, char* message){
    // On vérifie si .refs est initialisé
    if(!file_exists(".refs")){
        printf("Il faut d'abord initialiser les references du projet\n");
        return; 
    }
    // On vérifie si la branche existe
    List *l = listdir(".refs");
    if (searchList(l,branch_name)==NULL){
        printf("La branche %s n'existe pas\n",branch_name);
        free(l);
        return;
    }
    free(l);
    // On récupère le hash du commit précédent
    char* last_hash = getRefs(branch_name);
    // On récupère le hash de HEAD
    char* head_hash = getRefs("HEAD");
    // Si HEAD n'est pas sur la branche, on ne peut pas faire le commit
    if(strcmp(last_hash,head_hash)!=0){
        printf("HEAD doit pointer sur le dernier commit de la branche\n");
        free(last_hash);
        free(head_hash);
        return;
    }
    free(head_hash);
    // On vérifie si .add contient des éléments à commiter
    if (!file_exists(".add")) {
        free(last_hash);
        printf("le .add doit contenir des elements\n");
        return;
    }
    printf("check\n");
    // On crée un WorkTree à partir des éléments de .add
    WorkTree *wt = ftwt(".add");
    // On vide .add
    system("rm .add");
    printf("check\n");
    // On sauvegarde l'instantané dans un fichier .t et on récupère son hash
    char *hashwt = saveWorkTree(wt, ".");
    printf("elements sauvegardes\n");
    // On crée le commit correspondant
    Commit *c = createCommit(hashwt);
    printf("commit cree avec hash : %s\n",hashwt);
    free(hashwt);
    // On ajoute le prédécesseur si il existe
    if (strlen(last_hash)!=0){
        commitSet(c,"predecessor",last_hash);
    }
    free(last_hash);
    // On ajoute le message si il est défini
    if(message!= NULL){
        commitSet(c, "message", message);
    }
    printf("check avant blobcommit\n");
    // On sauvegarde le commit dans un fichier .c et on récupère son hash
    char *hashc = blobCommit(c);
    printf("check apres blobcommit\n");
    freeCommit(c);
    // On met à jour la branche avec le nouveau commit
    createUpdateRefs(branch_name, hashc);
    // On met à jour HEAD avec le bon commit aussi
    createUpdateRefs("HEAD", hashc);
    free(hashc);
    // On libère le WorkTree utilisé
    freeWorkTree(wt);
}
