#include "Ex11.h"



WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
    // On initialise un nouveau WorkTree et une liste de conflits
    WorkTree* wt = initWorkTree();
    *conflicts = initList();

    // On parcourt les éléments du premier WorkTree
    for(int i=0; i<wt1->n; ++i) {
        // Si l'élément n'existe pas dans le deuxième WorkTree, on l'ajoute au nouveau WorkTree
        if(inWorkTree(wt2, wt1->tab[i].name) == -1) {
            appendWorkTree(wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
        }
    }

    // On parcourt les éléments du deuxième WorkTree
    for(int i=0; i<wt2->n; ++i) {
        // Si l'élément n'existe pas dans le premier WorkTree, on l'ajoute au nouveau WorkTree
        if(inWorkTree(wt1, wt2->tab[i].name) == -1) {
            appendWorkTree(wt, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);
        } else {
            // Sinon, on ajoute le nom de l'élément à la liste de conflits
            insertFirst(*conflicts, buildCell(wt2->tab[i].name));
        }
    }

    // On retourne le nouveau WorkTree
    return wt;
}




/*
ELIO Version: 
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts) {
  WorkTree *wt = initWorkTree();
  WorkFile wf1;
  WorkFile wf2;
  Cell *c;
  
  if(!wt1){
    printf("wt1 est NULL\n");
    if (!wt2){
      printf("wt2 est NULL\n");
    }
    freeWorkTree(wt);
    return wt2;
  }
  if (!wt2){
    printf("wt2 est NULL\n");
    freeWorkTree(wt);
    return wt1; 
  }
  
  for (int i = 0; i < wt1->n; i++) {
    wf1 = wt1->tab[i];
    for (int j = 0; j < wt2->n; j++) {
      wf2 = wt2->tab[j];
      if ((strcmp(wf1.name, wf2.name) == 0) && (strcmp(wf1.hash, wf2.hash) != 0)){
        insertFirst(*conflicts, buildCell(wf1.name));
        break;
      }
    }
  }
  for (int i = 0; i < wt1->n; i++) {
    wf1 = wt1->tab[i];
    c = searchList(*conflicts, wf1.name);
    if (!c)
      appendWorkTreeWf(wt, &wf1);
  }
  for (int j = 0; j < wt2->n; j++) {
    wf2 = wt2->tab[j];
    c = searchList(*conflicts, wf2.name);
    if (!c)
      appendWorkTreeWf(wt, &wf2);
  }
  return wt;
}

CHATGPT:
List* merge(char* remote_branch, char* message) {
  remove(".add");
  assert(branchExists(remote_branch));
  char* current_branch = getCurrentBranch();
  WorkTree* wt1 = btwt(current_branch);
  WorkTree* wt2 = btwt(remote_branch);
  List* l = NULL;
  WorkTree* wt = mergeWorkTrees(wt1, wt2, &l);
  if (length(l) == 0) {
    freeList(l);
    l = NULL;
    restoreWorkTree(wt, ".");
    wttf(wt, ".add");
    char* h = getRef(remote_branch);
    myGitCommit(current_branch, message);
    deleteRef(remote_branch);
  }
  free(current_branch);
  freeWorkTree(wt);
  freeWorkTree(wt1);
  freeWorkTree(wt2);
  return l;
}*/

WorkTree* btwt(char *branch){
  char *br_ref = getRef(branch); // Récupération de la référence du commit de la branche courante
  printf("br_ref = %s \n" , br_ref); // Affichage de la référence du commit de la branche courante
  char *hash_br = hashToPathCommit(br_ref); // Convertit la référence du commit en chemin d'accès de l'objet commit
  printf("hash_br = %s \n" , hash_br); // Affichage du chemin d'accès de l'objet commit
  Commit* c_br = ftc(hash_br); // Lecture de l'objet commit à partir du chemin d'accès
  char* hash_tree = commitGet(c_br , "tree"); // Récupération du hash de l'arbre de travail du commit
  char* hp_tree = hashToPath(hash_tree); // Convertit le hash de l'arbre en chemin d'accès de l'objet tree
  strcat(hp_tree , ".t"); // Concaténation de l'extension ".t" pour le chemin d'accès de l'objet tree
  WorkTree * wt =  ftwt(hp_tree); // Lecture de l'objet tree à partir du chemin d'accès
  free(br_ref); free(hash_br); freeCommit(c_br) ; free(hp_tree); // Libération de la mémoire allouée
  return wt; // Retourne l'objet WorkTree
}

List *merge(char *remote_branch, char *message) {
  // Récupération de la branche courante
  char* curr_br = getCurrentBranch();
  printf("curr_br = getCurrentBranch %s\n" , curr_br);
  
  // Récupération de l'arbre de travail de la branche courante
  WorkTree * wt_current = btwt(curr_br); 
  if(!wt_current) printf("worktree current branch NULL\n");
  
  // Récupération de l'arbre de travail de la branche distante
  printf("remote_branch %s\n" , remote_branch);
  WorkTree * wt_remote = btwt(remote_branch);
  if(!wt_remote) printf("worktree remote branch NULL\n");
  
  // Initialisation de la liste des conflits
  List *conflicts = initList();
  
  // Fusion des arbres de travail courant et distant
  WorkTree *wt = mergeWorkTrees(wt_current, wt_remote, &conflicts);
  printf("wt->n = %d\n", wt->n);
  printList(conflicts);
  
  // Si des conflits sont détectés, retourne la liste des conflits
  if (*conflicts){
    printf("conflit\n");
    free(curr_br);
    freeWorkTree(wt_current);
    freeWorkTree(wt_remote);
    freeWorkTree(wt);
    return conflicts;
  }
  
  // Récupération des références des branches courante et distante
  char *curr_ref = getRef(curr_br);
  printf("curr_ref = getRef(curr_br) %s\n" , curr_ref);
  char *rb_ref = getRef(remote_branch);
  printf("rb_ref = getRef(remote_branch) %s\n" , rb_ref);
  
  // Sauvegarde de l'arbre de travail fusionné et récupération de son hash
  char *hash = saveWorkTree(wt, ".");
  printf("*hash = saveWorkTree(wt , .) %s\n" , hash);
  
  // Création d'un commit avec le hash de l'arbre fusionné
  Commit *c = createCommit(hash);
  commitSet(c, "message", message);
  commitSet(c, "predecessor", curr_ref); // branche courante
  commitSet(c, "merged_predecessor", rb_ref); // branche distante
  
  // Conversion du commit en hash
  char* hash_c = blobCommit(c);
  printf("hash_c = blobCommit(c) %s\n" , hash_c);
  
  // Mise à jour des références
  createUpdateRef(curr_br, hash_c);
  createUpdateRef("HEAD", hash_c);
  // printf("createUpdateRef(HEAD, hash_c) %s\n" , getRef("HEAD")); // Décommenter pour afficher la référence HEAD
  
  // Suppression de la référence de la branche distante
  deleteRef(remote_branch);
  printf("deleteRef(remote_branch) %s\n" , remote_branch);
  
  // Restauration de l'arbre de travail initial
  restoreWorkTree(wt, ".");
  
  // Libération de la mémoire allouée
  free(curr_br);
  free(curr_ref);
  free(rb_ref);
  free(hash);
  free(hash_c);
  freeWorkTree(wt_current);
  freeWorkTree(wt_remote);
  freeWorkTree(wt);
  freeList(conflicts);
  freeCommit(c);

  return NULL;
}

void createDeletionCommit(char* branch, List* conflicts, char* message){
  char* curr = getCurrentBranch(); // Obtient la branche courante
  myGitCheckoutBranch(branch); // Change la branche courante pour celle spécifiée en paramètre
  WorkTree * wt = btwt(branch); // Obtient l'arbre de travail de la branche spécifiée
  system("rm .add"); // Supprime le fichier .add s'il existe
  
  if (wt){
    for (int i = 0 ; i < wt->n ; i++){ // Boucle sur les fichiers dans l'arbre de travail de la branche
      Cell * c = searchList(conflicts , wt->tab[i].name); // Vérifie si le fichier a un conflit
      if(!c){ // S'il n'y a pas de conflit
         myGitAdd(wt->tab[i].name); // Ajoute le fichier à l'index
      }
    }
    myGitCommit(branch , message); // Crée un commit pour la branche spécifiée avec le message spécifié
    myGitCheckoutBranch(curr); // Reviens à la branche courante
    freeWorkTree(wt); // Libère la mémoire de l'arbre de travail
  }
  free(curr); // Libère la mémoire de la variable contenant la branche courante
}


/*
int main(int argc , char** argv) {
  List *conflicts = merge(argv[1], argv[2]);
  createDeletionCommit(argv[1], conflicts, argv[2]);
  liberer(conflicts);
  return 0;
  
  WorkFile *wf1 = createWorkFile("wf1"); //(4.1)
  WorkFile *wf2 = createWorkFile("wf2");

  WorkFile *wf3 = createWorkFile("wf1");

  WorkTree *wt1 = initWorkTree(); //(4.4)

  appendWorkTreeWf(wt1, wf1); //(4.5-4.6)

  WorkTree *wt2 = initWorkTree();
  appendWorkTreeWf(wt1, wf2);
  appendWorkTreeWf(wt2, wf3);

  List *conflicts = initList();
  WorkTree *wt = mergeWorkTrees(wt1, wt2, &conflicts);
  char *s = wtts(wt);
  printf("%s\n", s);
  free(s);
  char *l = ltos(conflicts);
  printf("%s\n", l);
  free(l);

  freeWorkFile(wf1);
  freeWorkFile(wf2);
  freeWorkFile(wf3);
  freeWorkTree(wt1);
  freeWorkTree(wt2);
  freeWorkTree(wt);
  liberer(conflicts);
  

  //truncate -s 0 "$filename"
}

*/



