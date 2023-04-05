#ifndef p1ex2_h
#define p1ex2_h



typedef struct cell{
    char* data;
    struct cell *next;

} Cell;
typedef Cell* List;

List *initList();
Cell * buildCell(char *ch);
void insertFirst(List *L,Cell *C);
char * ctos(Cell *c);
char* ltos(List* L);
Cell * listGet(List *L,int i);
Cell * searchList(List *L, char *str);
List * stol(char *s);




#endif