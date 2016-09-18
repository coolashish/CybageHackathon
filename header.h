#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
//#define DEBUG
#define WORD_MAX_SIZE 64
#define SENT_MAX_SIZE 1024
#define TABLE_SIZE 1024

typedef struct node {
    char *str;
    int count;
    struct node *next;
} node;

char * ReadFile (char *filename);
int AllocateMemory (char **word, char **sent, node ***sentTable, node ***wordTable);
int Hash (char * str);
int InsertInHash (char *str, int index, node **table);
node * CreateNode (char *str);
int InitTable (node **table);
int IterateTable (FILE * fp, node ** table);
void ToLower (char *string);
int self_analyse(FILE *fp);
