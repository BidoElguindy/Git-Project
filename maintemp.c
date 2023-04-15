#include "List.h"
#include "Work.h"
#include "Commit.h"

int main(){
    
    //EXERCICE 1  
    
    printf("Test Exercice 1 \n");
    printf("\n");

	// Nom du fichier à hasher
    char* filename = "draft.txt";

    // Calcul du hash du fichier
    char* hash = sha256file(filename);
    if (hash == NULL) {
        printf("Erreur : impossible de calculer le hash du fichier %s\n", filename);
        return 1;
    }

    // Affichage du hash
    printf("Hash du fichier %s : %s\n", filename, hash);

    // Libération de la mémoire allouée pour le hash
    free(hash);
   
    //EXERCICE 2

    printf("\n");
    printf("Test Exercice 2 \n");
    printf("\n");

    // Création d'une nouvelle liste vide
    List* L = initList();

    // Création de deux nouvelles cellules pour la liste
    Cell* C1 = buildCell("1");
    Cell* C2 = buildCell("2");

    // Insertion de la première cellule en tant que première cellule de la liste
    insertFirst(L, C1);

    // Insertion de la deuxième cellule en tant que première cellule de la liste
    insertFirst(L, C2);

    // Conversion de la liste en chaîne de caractères
    char* s = ltos(L);

    // Affichage de la chaîne de caractères
    printf("La conversion de la liste en chaine de caracteres est %s\n", s);

    // Recherche de la deuxième cellule de la liste
    Cell* C3 = listGet(L, 2);

    // Vérification que la cellule retournée est bien la deuxième cellule de la liste
    assert(C3 == C1);

    // Recherche d'une cellule qui n'existe pas dans la liste
    Cell* C4 = searchList(L, "3");

    // Vérification que la fonction de recherche renvoie NULL
    assert(C4 == NULL);

    // Recherche d'une cellule qui existe dans la liste
    Cell* C5 = searchList(L, "1");

    // Vérification que la cellule retournée est bien la deuxième cellule de la liste
    assert(C5 == C1);

    // Conversion d'une chaîne de caractères en liste
    List* L2 = stol("3|4"); 
    printf("La conversion de la chaine de caracteres en liste donne\t");
    afficherListe(*L2);

    // Vérification que la nouvelle liste contient les éléments attendus
    assert(strcmp(ctos(*L2), "4") == 0);
    assert(strcmp(ctos((*L2)->next), "3") == 0);
   
    // Enregistrement de la liste dans un fichier
    ltof(L, "test.txt");
    
    // Lecture de la liste enregistrée dans le fichier
    List* L3 = ftol("test.txt");

    // Affichage de la liste lue à partir du fichier
    printf("Liste lue du fichier:\n");
    Cell* c = *L3;
    while (c) {
      printf("%s", c->data);
      c = c->next;
    }
    
    // Libération de la mémoire allouée pour les listes et les cellules
    free(s);
    freeList(L);
    freeList(L2);
    freeList(L3);
    

    //EXERCICE 3

    printf("\n");
    printf("Test Exercice 3 \n");
    printf("\n");

    List* L4 = listdir(".");                     // Test de la fonction listdir()
    printf("Liste des fichiers et répertoires du répertoire courant :\n");
    afficherListe(*L4);;                         // Affichage de la liste des fichiers et répertoires du répertoire courant
   
    char* file = "test.txt";
    int res = file_exists(file);                 // Test de la fonction file_exists()
    if(res == 1){
        printf("Le fichier %s existe\n", file);
    }
    else{
        printf("Le fichier %s n'existe pas\n", file);
    }
 
    char* from = "test.txt";
    char* to = "test_copy.txt";
    cp(to,from);                                 // Test de la fonction cp()
    printf("Le fichier %s a été copié dans %s\n", from, to);
   
    char* hash2 = "aabbccddeeff";
    char* path = hashToPath(hash2);             // Test de la fonction hashToPath()
    printf("Le chemin vers le fichier correspondant au hash %s est %s\n", hash2, path);
    
    char* file2 = "test_copy.txt";
    blobFile(file2);                            // Test de la fonction blobFile()
    
    free(path);                                 // Libération de la mémoire allouée
    freeList(L4);
    
    
    //EXERCICE 4 

    printf("\n");
    printf("Test Exercice 4 \n");
    printf("\n");

    WorkFile* wf1 = createWorkFile("file1");
    WorkFile* wf2 = createWorkFile("file2");
    
    char* wfts1 = wfts(wf1);
    printf("WorkFile 1 : %s\n", wfts1);
    char* wfts2 = wfts(wf2);
    printf("WorkFile 2 : %s\n", wfts2);
   
    WorkFile* wf3 = stwf("file3 123456 777");
    if (wf3 != NULL) {
        char* wfts3 = wfts(wf3);
        printf("WorkFile 3 : %s\n", wfts3);
    }

    WorkTree* wt = initWorkTree();
    int ret = appendWorkTree(wt, "file1", NULL, 1);
    if (ret == 0) {
        printf("WorkFile 1 ajouté dans WorkTree\n");
    }
    else {
        printf("Erreur lors de l'ajout de WorkFile 1 dans WorkTree\n");
    }
    
    ret = appendWorkTree(wt, "file2", "123456", 2);
    if (ret == 0) {
        printf("WorkFile 2 ajouté dans WorkTree\n");
    }
    else {
        printf("Erreur lors de l'ajout de WorkFile 2 dans WorkTree\n");
    }

    ret = appendWorkTree(wt, "file1", "456789", 3);
    if (ret == 0) {
        printf("WorkFile 1 mis à jour dans WorkTree\n");
    }
    else {
        printf("Erreur lors de la mise à jour de WorkFile 1 dans WorkTree\n");
    }

    int index = inWorkTree(wt, "file2");
    if (index != -1) {
        printf("WorkFile 2 trouvé dans WorkTree à l'indice %d\n", index);
    }
    else {
        printf("WorkFile 2 non trouvé dans WorkTree\n");
    }

    char* wtts1 = wtts(wt);
    printf("WorkTree : %s\n", wtts1);
    
    free(wf1);
    free(wf2);
    free(wf3);
    free(wfts1);
    free(wfts2);
    free(wtts1);
    free(wt->tab);
    free(wt);
    
    
    //EXERCICE 5 

    printf("\n");
    printf("Test Exercice 5 \n");
    printf("\n");

    // Test de la fonction getChmod
    printf("getChmod: %d\n", getChmod("test.txt"));

    // Test de la fonction setMode
    setMode(0644, "test.txt");

    // Test de la fonction hashToFile
    char* pathh = hashToFile("abcd1234");
    printf("hashToFile: %s\n", pathh);

    // Test de la fonction blobWorkTree
    WorkTree* wtt = initWorkTree();
    appendWorkTree(wtt, "testdir", NULL, 0);
    char* hashh = blobWorkTree(wtt);
    printf("blobWorkTree: %s\n", hashh);

    // Test de la fonction concat_paths
    char* result = concat_paths("/path/to/dir", "test.txt");
    printf("concat_paths: %s\n", result);
    free(result);

    // Test de la fonction isFile
    printf("isFile: %d\n", isFile("test.txt"));
     
    // Test de la fonction saveWorkTree

    //Création d'un WorkTree vide
    WorkTree *Wt = initWorkTree();

    // Ajout d'un fichier à ce WorkTree
    appendWorkTree(Wt, "test.txt", NULL, 0);

    // Ajout d'un répertoire à ce WorkTree
    appendWorkTree(Wt, "d4", NULL, 1);
    
    // Sauvegarde du WorkTree dans un répertoire temporaire
    char *hashhh = saveWorkTree(Wt, ".");
    
    // Vérification que le hash retourné n'est pas NULL
    if (hashhh == NULL) {
        printf("Erreur lors de la sauvegarde du WorkTree\n");
        return 1;
    }
    
    // Affichage du hash retourné
    printf("Le hash du WorkTree est : %s\n", hashhh);
    
    // Libération de la mémoire allouée pour le WorkTree et le hash
    free(Wt -> tab); //Fuite ??
    free(Wt);
    free(hashhh);
    
    
    //Exercice 6 

    printf("\n");
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
    Commit* C = initCommit();
    commitSet(C, "key1", "value1");
    commitSet(C, "key2", "value2");
    printf("Value for key1: %s\n", commitGet(C, "key1"));
    printf("Value for key2: %s\n", commitGet(C, "key2"));

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
    char* H = blobCommit(c2);
    printf("Blob hash: %s\n", H);
    free(H);

    // Libération de la mémoire
    freeCommit(C);
    freeCommit(c2);
    freeCommit(c3);
    freeCommit(c4);
     
    return 0;
}
