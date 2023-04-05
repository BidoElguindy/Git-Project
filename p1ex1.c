#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "p1ex1.h"



int hashFile(char* source, char* dest){
	char buffer[1000];
	sprintf(buffer,"sha256sum %s > %s",source,dest );
	system(buffer);
	return 1;
}
//inzoinivnczn
char* sha256file(char* file){
	static char template [] =" /tmp/myfileXXXXXX" ;
	char fname[1000];
	strcpy(fname ,template);
	int fd = mkstemp(fname);
	
	hashFile(file,fname);

	char hash[1000];
	FILE *f =fopen(fname,"r");
	fgets(hash,1000,f);
	char *m=malloc(sizeof(char));
	sscanf(hash,"%s",m);
	return m;
}

int main(){
	hashFile("main.c","etst.txt");
	char * c = sha256file("etst.txt");
}
