#include "List.h"
#include "Work.h"
#include "Commit.h"
#include "Refs.h"
#include "Branch.h"
#include "Ex11.h"



void manuel() {
	printf("fonctions :\n");
	printf("\tinit : initialise le .refs\n");
	printf("\tlist-refs : liste les références existantes\n");
	printf("\tlist-add : liste les éléments présents dans le .add\n");
	printf("\tclear-add : réinitialise le .add\n");
	printf("\tcreate-ref <name> <hash> : crée une référence\n");
	printf("\tdelete-ref <name> : supprime une référence\n");
	printf("\tadd <elem1> [<elem2> <elem3> ...] : ajoute des éléments au .add\n");
	printf("\tcommit <branch_name> [-m <message>] : commit sur une branche\n");
    printf("myGit get-current-branch\n");
    printf("myGit branch <branch-name> : créer une branche\n");
    printf("myGit branch-print <branch-name>\n");
    printf("myGit checkout-branch <branch-name>\n");
    printf("myGit checkout-commit <pattern>\n");
    printf("myGit merge <branch> <message>\n");
}

int main(int argc, char** argv) {
	if (argc<2) {manuel(); return 0;}
	if (strcmp(argv[1],"init")==0) {
		initRefs();
        initBranch ();
		printf(".refs initialisé\n");
        printf("la branche courante est initialisé initialisé\n");
		return 0;
	}
	if (strcmp(argv[1],"list-refs")==0) {
		List *L = listdir(".refs");
		printf("références :\n");
		for (Cell *pt = *L;pt!=NULL;pt=pt->next) {
			if (pt->data[0]!='.') printf("    -\t%s\n",ctos(pt));
		}
		return 0;
	}
	if (strcmp(argv[1],"list-add")==0) {
		if (!file_exists(".add")) {
			printf(".add est vide (inexistant)\n");
			return 0;
		}
		WorkTree *wt = ftwt(".add");
		printf("éléments présents dans le .add :\n");
		printWorkTree(wt);
		freeWorkTree(wt);
		return 0;
	}
	if (strcmp(argv[1],"clear-add")==0) {
		if (file_exists(".add")) system("rm .add");
		system("touch .add");
		printf(".add réinitialisé\n");
		return 0;
	}
	if (strcmp(argv[1],"create-ref")==0) {
		if (argc!=4) {
			printf("mauvais nombre d'arguments\n");
			return 0;
		}
		createUpdateRef(argv[2],argv[3]);
		return 0;
	}
	if (strcmp(argv[1],"delete-ref")==0) {
		if (argc!=3) {
			printf("mauvais nombre d'arguments\n");
			return 0;
		}
		deleteRef(argv[2]);
		return 0;
	}
	if (strcmp(argv[1],"add")==0) {
		if (argc<3) {
			printf("mauvais nombre d'arguments\n");
			return 0;
		}
		for (int i=2;i<argc;i++) {
			myGitAdd(argv[i]);
			printf("%s ajouté\n",argv[i]);
		}
		return 0;
	}
	if (strcmp(argv[1],"commit")==0) {
		if (argc<3) {
			printf("mauvais nombre d'arguments\n");
			return 0;
		}
		char *buffer = NULL;
		if ((argc>3) && (strcmp(argv[4],"-m")==0)) {
			buffer = (char*) malloc(sizeof(char)*100);
			char tmp[100];
			for(int i=4; i<argc;i++){
				sprintf(tmp,"%s",argv[i]);
				strcat(buffer,tmp);
				if (i!=argc-1) strcat(buffer," ");
			}
		}
		myGitCommit(argv[2], buffer);
		free(buffer);
		return 0;
	}
    if(strcmp(argv[1],"get-current-branch")==0){
        char* branch=getCurrentBranch();
        printf("%s \n", branch);
        free(branch);
    }

    if(strcmp(argv[1],"branch")==0){
        if(!branchExists(argv[2]))
            createBranch(argv[2]);
        else{
            printf("The branch %s already exists \n", argv[2]);
        }
    }
    if(strcmp(argv[1],"branch-print")==0){
        if(!branchExists(argv[2]))
             printf("The branch %s does not exist \n", argv[2]);
        else{
           printBranch(argv[2]);
        }
    }

   
    if(strcmp(argv[1],"checkout-branch")==0){
        if(!branchExists(argv[2]))
             printf("The branch %s does not exist \n", argv[2]);
        else{
           myGitCheckoutBranch(argv[2]);
        }
    }
    if(strcmp(argv[1],"checkout-commit")==0){
        myGitCheckoutCommit(argv[2]);
    }
    if(strcmp(argv[1], "merge") == 0) {
        assert(argc == 4);
        commandMerge(argv[2], argv[3]);
	return 0;
}

/*

int main(int argc, char* argv[]){
  
    // ./myGit init
    if(strcmp(argv[1],"init")==0){
        initRefs();
        initBranch();
    }

    // ./myGit list-refs
    if(strcmp(argv[1],"list-refs")==0){
        printf("REFS : \n");
        List* l=listdir(".refs");
        for(Cell* ptr=(*l); ptr!=NULL;ptr=ptr->next){
            if(ptr->data[0]=='.')
                continue;
            char* contenu=getRef(ptr->data);
            printf("%s\t%s\n", ptr->data, contenu);
            free(contenu);
        }
        freeList(l);
    }

    // ./myGit create-ref
    if(strcmp(argv[1],"create-ref")==0){
        createUpdateRef(argv[2], argv[3]);
    }

    //./myGit delete-ref
    if(strcmp(argv[1],"delete-ref")==0){
        deleteRef(argv[2]);
    }

    // ./myGit add
    if(strcmp(argv[1],"add")==0){
        for(int i=2; i<argc;i++ ){
            myGitAdd(argv[i]);
        }
    }

    // ./myGit list-add
    if(strcmp(argv[1],"list-add")==0){
        printf("Zone de préparation : \n");
        if(file_exists(".add")){
            WorkTree* wt = ftwt(".add");
            printf("%s \n",wtts(wt));
            freeWorkTree(wt);
        }
    }

    // ./myGit clear-add
    if(strcmp(argv[1],"clear-add")==0){
        system ("rm .add") ;
    }

    // ./myGit commit <branch name> [-m <message>] 
    //NOT WORKING mkdir in pathtoFile, cant do the one without -m, segf
    if(strcmp(argv[1],"commit")==0){
        if(strcmp(argv[3], "-m")==0){
            myGitCommit(argv[2], argv[4]);
        }else{
            myGitCommit(argv[2], NULL);
        }
    }

    // ./myGit get-current-branch
    if(strcmp(argv[1],"get-current-branch")==0){
        char* branch=getCurrentBranch();
        printf("%s \n", branch);
        free(branch);
    }

    //./myGit branch <branch-name>
    if(strcmp(argv[1],"branch")==0){
        if(!branchExists(argv[2]))
            createBranch(argv[2]);
        else{
            printf("The branch %s already exists \n", argv[2]);
        }
    }

    // ./myGit branch-print <branch-name> 
    //NOT Working listdir in branchExists
    if(strcmp(argv[1],"branch-print")==0){
        if(!branchExists(argv[2]))
             printf("The branch %s does not exist \n", argv[2]);
        else{
           printBranch(argv[2]);
        }
    }

    // ./myGit checkout-branch <branch-name>
    //NOT Working listdir in branchExists
    if(strcmp(argv[1],"checkout-branch")==0){
        if(!branchExists(argv[2]))
             printf("The branch %s does not exist \n", argv[2]);
        else{
           myGitCheckoutBranch(argv[2]);
        }
    }

    // ./myGit checkout-commit <pattern>
    if(strcmp(argv[1],"checkout-commit")==0){
        myGitCheckoutCommit(argv[2]);
    }

    return 0;
}


int main() {
  printf("Test initBranch bon \n");
  initBranch();
  printf(".current branch file created with master branch.\n");

  printf("Test branchExists bon\n");
  char* branch1 = "branch1";
  char* branch2 = "branch2";
  char* branch3 = "branch3";
  createBranch(branch1);
  printf("Created new branch: %s\n", branch1);
  if (branchExists(branch1)) {
    printf("Branch %s exists.\n", branch1);
  }
  if (!branchExists(branch2)) {
    printf("Branch %s does not exist.\n", branch2);
  }

  printf("Test getCurrentBranch bon \n");
  char* currentBranch = getCurrentBranch();
  printf("Current branch is: %s\n", currentBranch);

  printf("Test hashToPathCommit \n");
  char* hash1 = "abc123";
  char* hash2 = "def456";
  char* path1 = hashToPathCommit(hash1);
  char* path2 = hashToPathCommit(hash2);
  printf("Path to commit with hash %s: %s\n", hash1, path1);
  printf("Path to commit with hash %s: %s\n", hash2, path2);

  printf("Test printBranch bon \n");
  printBranch("master");

  printf("Test branchList bon \n");
  List* branch1List = branchList(branch1);
  printf("Commits in branch %s:\n", branch1);
  printList(branch1List);

  printf("Test getAllCommits bon \n");
  List* allCommits = getAllCommits();
  printf("All commits in repository:\n");
  printList(allCommits);

  printf("Test restoreCommit\n");
  Commit* c1 =createCommit(sha256file("master"));
  restoreCommit(sha256file("master"));
  printf("Restored worktree to commit with hash %s.\n", hash1);

  printf("Test myGitCheckoutBranch\n");
  myGitCheckoutBranch(branch1);
  printf("Checked out branch %s.\n", branch1);

  printf("Test filterList\n");
  List* filteredList = filterList(allCommits, "abc");
  printf("Filtered list of commits containing 'abc':\n");
  printList(filteredList);

  printf("Test myGitCheckoutCommit \n");
  myGitCheckoutCommit(hash2);
  printf("Checked out commit with hash %s.\n", hash2);

  printf(" Test getElement, printList, and length\n");
  printf("Element at index 2 of filteredList: %s\n", getElement(filteredList, 2));
  printf("Length of filteredList: %d\n", length(filteredList));
  printList(filteredList);

  return 0;
}



int main(){
    printf("\n");
    printf("Test Exercice 7 \n");
    printf("\n");

    // Test de initRefs
    printf("Testing initRefs\n");
    initRefs();

    // Test de createUpdateRef
    printf("\nTesting createUpdateRef\n");
    char* hash = "abc123";
    char * c= "master2";
    createUpdateRef(c, hash);
    char* ref_value = getRef(c);
    printf("master ref value: %s\n", ref_value);
    


    // Test de deleteRef
    printf("\nTesting deleteRef\n");
    deleteRef(c);
    

    // Test de getRefs
    printf("\nTesting getRefs\n");
    createUpdateRef("HEAD", hash);
    char* head_ref_value = getRef("HEAD");
    printf("HEAD ref value: %s\n", head_ref_value);

    // Test de myGitAdd
    printf("\nTesting myGitAdd\n");
    myGitAdd("test.txt");
    myGitAdd("folder.txt");
    myGitAdd("test.txt");

    // Test myGitCommit()
    printf("\nCreating commit...\n");
    char branch_name[] = "master";
    char message[] = "Initial commit";
    myGitCommit(branch_name, message);
    printf("Commit created and stored in %s branch!\n", branch_name);

    return 0;
}
*/
