#include "p1ex2.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


List *initList(){
    List *l = malloc(sizeof(List));
    *
    
    return l;
}
Cell * buildCell(char *ch){
    Cell * c=malloc(sizeof(Cell));
    c->data=strdup(ch);
    c->next=NULL;
    return c;
}

void insertFirst(List *L,Cell *C){
    C->next=*L;
    L=&C;
}

char * ctos(Cell *c){
    
    return c->data;
}

char* ltos(List* L) {
  if (L== NULL) return "\0";
    int len = 0;
    Cell* curr = *L;
    while (curr) {
        len += strlen(curr->data) + 1; // +1 pour '|'
        curr = curr->next;
    }
    len++;
    char* res = (char*) malloc(sizeof(char) * len);
    curr = *L;
    while (curr != NULL) {
        strcat(res, ctos(curr));
        strcat(res, "|"); // ajouter '|'
        curr = curr->next;
    }
    res[strlen(res) - 1] = '\0'; // supp le dernier '|'
    return res;
}

Cell* listGet(List* L, int i){
  if (L == NULL) return NULL;
  Cell * curr = *L;
  int cpt = 1;
  while(curr && cpt<i){
    cpt++;
    curr = curr->next;
  }
  return curr;

}

Cell * searchList(List *L, char *str){
    Cell* tmp =*L;
    while(tmp && (strcmp(ctos(tmp),str)!=0)){
        tmp=tmp->next;
    }
    return tmp;
}

List* stol(char* s) {
    List* L = initList();

    char* ch= strtok(s, "|");
    while (ch) {
        Cell* c = buildCell(ch);
        insertFirst(L, c);
        ch = strtok(NULL, "|");
    }

    return L;
}


void ltof(List* L, char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Cell* curr = *L;
    while (curr != NULL) {
        fprintf(file, "%s\n", curr->data);
        curr = curr->next;
    }

    fclose(file);
}

List* ftol(char* path){
  FILE* f = fopen(path, "r");
  char buff[100];
  List* lres = initList();
  Cell* c;
  while(fgets(buff, 100, f)){
    c = buildCell(buff);
    insertFirst(lres, c);
  }
  fclose(f);
  return lres;
}

int main() {
    List * l = initList();
    char * c1 = "Bido";
    char * c2 = "Carole";
    Cell * cell = buildCell(c1);
    insertFirst(l,cell);
    insertFirst(l,buildCell(c2));
    char * listochaine = ltos(l);
    Cell * ieme = listGet(l,1);
    Cell * c1_recherche = searchList(l, c1);
    List * lstol = stol("Omar|Aly|");
    ltof(l,"ltoftest.txt"); 
    List * lftol = ftol("ltoftest.txt");
    return 0;
}