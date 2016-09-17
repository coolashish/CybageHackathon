#include "header.h"

int Hash (char * str) {
    char ch;
    int index = 0, i = 0, power = 1000;
    while ((ch = str[i]) != '\0') {
        if (ch >= 'a' && ch <= 'z')
            index += ((ch - 'a') * power) % TABLE_SIZE; 
        //else if (ch >= 'A' && ch <= 'Z')
        else
            index += ((ch - 'A') * power) % TABLE_SIZE; 
        if (power != 1)
            power -= 1;
        i++;
    }
    if (index < 0)
        index = 1;
    index %= TABLE_SIZE;
    return index;
}

int InsertInHash (char *str, int index, node **table) {
    struct node *tmp, *p;
    if (!table[index]) {
        tmp = CreateNode(str);
        if (!tmp)
            return -1;
        table[index] = tmp;
    }
    else {
        p = table[index];
        while (p) {
            if (!strcmp(p->str, str)) {
                p->count += 1;
                return 0;
            }
            p = p->next;
        }
        p = table[index];
        tmp = CreateNode(str);
        if (!tmp)
            return -1;
        tmp->next = p;
        table[index] = tmp;
    }
    return 0;
}

node * CreateNode (char *str) {
    node *tmp = (node *)malloc(sizeof(node));
    if (!tmp) {
#ifdef DEBUG
        printf("Error in memory allocation\n");
#endif
        return NULL;
    }
    tmp->str = malloc(strlen(str) + 1);
    if (!(tmp->str)) {
#ifdef DEBUG
        printf("Error in memory allocation\n");
#endif  
        return NULL;
    }
    strcpy(tmp->str, str);
    tmp->count = 1;
    tmp->next = NULL;
    return tmp;
}

int InitTable (node **table) {
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    return 0;
}

int IterateTable (node ** table) {
    int i = 0;
    node *p;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (!table[i])
            continue;
        p = table[i];
        while (p) {
            printf("Text: %s\tcount: %d\n", p->str, p->count);
            p = p->next;
        }
    }
    return 0;
}
