#include "p1ex3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

List* listdir(char* root_dir){
    List* L=initList();
    DIR * dp = opendir ( root_dir ) ;
    struct dirent * ep ;
    if ( dp != NULL ){
        while (( ep = readdir ( dp ) ) != NULL ){
            Cell* c=buildCell(ep->d_name);
            insertFirst(L,c);
        }
    }
    return L;
}

int file_exists(char *file){
    List* L=listdir(".");
    Cell* c=searchList(L,file);
    if(c!=NULL){
        return 1;
    }
    return 0;
}

void cp(char *to, char *from){
    //verifie si le ficier existe
    if(file_exists(from)==1){
        FILE* f1= fopen(from, "r");
        FILE* f2= fopen(to, "w");
        char buff[1024];
        while(fgets(buff,sizeof(buff),f1)!=NULL){
            fputs(buff,f2);
        }
        fclose(f1);
        fclose(f2);
    }
}

char* hashToPath(char* hash){
    char* res = malloc(strlen(hash) + 1); 
    if (res == NULL) {
        return NULL; 
    }
    memcpy(res,hash,2);
    strcat(res,"/");
    strcat(res,hash+2);
    return res;
}

void blobFile ( char * file ) {
    char * hash = sha256file ( file ) ;
    char * ch2 = strdup ( hash ) ;
    ch2 [2] = '/0' ;
    if (! file_exists ( ch2 ) ){
        char buff [100];
        sprintf ( buff , "mkdir %s ", ch2 ) ;
        system ( buff ) ;
    }
    char * ch = hashToPath ( hash ) ;
    cp( ch , file ) ;
}

//Version chatgpt

void blobFile(char* file) {
    if (fileExists(file)) {
        char hash[SHA256_DIGEST_LENGTH * 2 + 1];
        sha256file(file, hash);
        char* path = hashToPath(hash);
        mkdir(path, 0777);
        char destFile[strlen(path) + strlen(hash) + 2];
        sprintf(destFile, "%s/%s", path, hash);
        if (!fileExists(destFile)) {
            cp(destFile, file);
        }
        free(path);
    }
}
