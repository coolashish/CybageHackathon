#include "header.h"
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define NO_OF_THREADS 4

char delim[] = {'.', '!', '?', '\n', '\0'};
int file_size;

struct ThreadShareToLower {
    char *string;
    int from;
    int till;
};

int main (int argc, char **argv) {
    char *sent, *word, *file, *dupSent;
    char *saveptr1, *saveptr2;
    int index = 0, sentIndex = 0;
    node ** sentTable, **wordTable;


    if((AllocateMemory(&sent, &word, &sentTable, &wordTable)) < 0) {
            return -1;
    }

    if(!(file = ReadFile(argv[1]))) {
            return -1;
    }
    //ThreadToLowerController(file);

    
    sent = strtok_r(file, delim, &saveptr1);
    while (sent) {
#ifdef DEBUG
        printf("Sentence: %s\n", sent);
#endif
        ToLower(sent);
        if (*sent == ' ')
            sent += 1;
        dupSent = (char *)malloc(strlen(sent) + 1);
        if (!dupSent) {
#ifdef DEBUG
            printf("Memory allocation failed\n");
#endif
            return -1;
        }
        strcpy(dupSent, sent);
        /*
        index = Hash(sent);
        if ((InsertInHash(sent, index, sentTable)) < 0) {
            return -1;
        }
        */
        word = strtok_r(sent, " ", &saveptr2);
        sentIndex = 0;
        while (word) {
#ifdef DEBUG
        printf("Word: %s\n", word);
#endif
            index = Hash(word);
            sentIndex = (sentIndex + index) % TABLE_SIZE;
            if ((InsertInHash(word, index, wordTable)) < 0) {
                return -1;
            }
            sent = NULL;
            word = strtok_r(sent, " ", &saveptr2);
        }
        if ((InsertInHash(dupSent, sentIndex, sentTable)) < 0) {
            return -1;
        }
        free(dupSent);
        file = NULL;
        sent = strtok_r(file, delim, &saveptr1);
    }


    FILE *fp1;
    fp1 = fopen(argv[2], "w");
    if (!fp1) {
        return -1;
    }

    BeautifyOutput(fp1, wordTable);
    BeautifyOutput(fp1, sentTable);
    //IterateTable(fp1, sentTable);
    //IterateTable(fp1, wordTable);

    fclose(fp1);

    self_analyse(stdout);

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

    file_size = fsize;
    file[fsize] = '\0';
    return file;
}

void ThreadToLowerController(char *string) {
    int thread;
    pthread_t *t;
    struct ThreadShareToLower *tmp;

    tmp  = (struct ThreadShareToLower *)malloc
            (sizeof(struct ThreadShareToLower));

    tmp->string = string;

    t = (pthread_t *)malloc(sizeof(pthread_t)*NO_OF_THREADS);
    for (thread = 0; thread < NO_OF_THREADS; thread++) {
        tmp->from = (file_size / NO_OF_THREADS) * thread;
        tmp->till = tmp->from + (file_size / NO_OF_THREADS);
        if(pthread_create(&t[thread], NULL, ThreadToLower, tmp)){
            fprintf(stderr, "create failed\n");
            exit(-1);
        }
    }
    for(thread = 0; thread<NO_OF_THREADS;thread++)
        pthread_join(t[thread], NULL);

}

void *ThreadToLower (void *arg) {
    int i = 0;
    char *string;
    int from;
    int till;

    string = ((struct ThreadShareToLower *)arg)->string;
    from = ((struct ThreadShareToLower *)arg)->from;
    till = ((struct ThreadShareToLower *)arg)->till;

    for (i = from; i < till; i++)
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i] += 'a' - 'A';
    return NULL;
}

void ToLower (char *string) {
    int i = 0;
    for (i = 0; string[i] != '\0'; i++)
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i] += 'a' - 'A';
    return;
}

void BeautifyOutput (FILE *fp, void *ptr) {
    int i;
    pthread_t *t;
    struct threadShare *print;
    t = (pthread_t *)malloc(sizeof(pthread_t)*NO_OF_THREADS);
   
    print = (struct threadShare *)malloc(sizeof(struct threadShare));
    print->fp = fp;
    print->ptr = ptr;
    for  (i = 0; i < NO_OF_THREADS; i++) {
        print->from = (TABLE_SIZE/NO_OF_THREADS) * i;
        print->till = print->from + (TABLE_SIZE/NO_OF_THREADS);
        if(pthread_create(&t[i], NULL, ThreadIterateTable, print)){
            fprintf(stderr, "create failed\n");
            exit(-1);
        }
    }
    for(i = 0; i<NO_OF_THREADS;i++)
        pthread_join(t[i], NULL);

    return;
}
