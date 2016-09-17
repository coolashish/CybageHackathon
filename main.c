#include "header.h"
char delim[] = {'.', '!', '?', '\n', '\0'};


int main (int argc, char **argv) {
    char *sent, *word, *file;
    char *saveptr1, *saveptr2;
    int index = 0;
    node ** sentTable, **wordTable;

    if((AllocateMemory(&sent, &word, &sentTable, &wordTable)) < 0) {
            return -1;
    }

    if(!(file = ReadFile(argv[1]))) {
            return -1;
    }
    
    sent = strtok_r(file, delim, &saveptr1);
    while (sent) {
#ifdef DEBUG
        printf("Sentence: %s\n", sent);
#endif
        if (*sent == ' ')
            sent += 1;
        index = Hash(sent);
        if ((InsertInHash(sent, index, sentTable)) < 0) {
            return -1;
        }
        word = strtok_r(sent, " ", &saveptr2);
        while (word) {
#ifdef DEBUG
        printf("Word: %s\n", word);
#endif
            index = Hash(word);
            if ((InsertInHash(word, index, wordTable)) < 0) {
                return -1;
            }
            sent = NULL;
            word = strtok_r(sent, " ", &saveptr2);
        }
        file = NULL;
        sent = strtok_r(file, delim, &saveptr1);
    }
    IterateTable(stdout, sentTable);
    IterateTable(stdout, wordTable);

    return 0;
}

int AllocateMemory (char **word, char **sent, node ***sentTable, node ***wordTable) {
    *sent = (char *)malloc(SENT_MAX_SIZE);
    if (!*sent) {
#ifdef DEBUG
        printf("Memory allocation failed\n");
#endif
        return -1;
    }
    *word = (char *)malloc(WORD_MAX_SIZE);
    if (!*word) {
#ifdef DEBUG
        printf("Memory allocation failed\n");
#endif
        return -1;
    }
    *sentTable = (node **)malloc(TABLE_SIZE * sizeof(node **));
    if (!*sentTable) {
#ifdef DEBUG
        printf("Memory allocation failed\n");
#endif
        return -1;
    }
    InitTable(*sentTable);
    *wordTable = (node **)malloc(TABLE_SIZE * sizeof(node **));
    if (!*wordTable) {
#ifdef DEBUG
        printf("Memory allocation failed\n");
#endif
        return -1;
    }
    InitTable(*wordTable);
    return 0;
}

char * ReadFile (char *filename) {
    FILE * fp = fopen(filename, "rb");
    long fsize;
    char *file = NULL;
    if (!fp) {
#ifdef DEBUG
        printf("Cannot open %s file\n", filename);
#endif
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    file = malloc(fsize + 1);
    fread(file, fsize, 1, fp);
    fclose(fp);

    file[fsize] = '\0';
    return file;
}
