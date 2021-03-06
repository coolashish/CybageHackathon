#include "header.h"

#define MAX_LINE 1024
#define MAX_CMD 512

#define SUCCESS 0
#define FAILURE 1

int self_analyse(FILE *fp) {
    FILE *fp_tmp;
    char buff[MAX_LINE], cmd[MAX_CMD];
    struct rusage usage;
    int pid;
#ifdef DEBUG
    fprintf(stderr, "in analyse :%s:%d:%d:%ld\n", 
            __FILE__, __LINE__, pid, time);
#endif
    //ip assert
    
    //initialization 
    fp_tmp = NULL;
    pid = getpid();

    //computation
    fprintf(fp, "CPU[%%]\tRAM[KB]\tDISK_IO[count]\n");
    sprintf(cmd, "ps -p %d -o %%cpu > tmp", pid);
    system(cmd);
    buff[0] = '-';
    buff[1] = 0;
    fp_tmp = fopen("tmp", "r");
    fscanf(fp_tmp, "%s", buff);
    fscanf(fp_tmp, "%s", buff);
    fclose(fp_tmp);
    printf("%6s\t", buff);

    getrusage(RUSAGE_SELF, &usage);
    fprintf(fp, "%7ld\t", 
    usage.ru_ixrss+usage.ru_idrss+usage.ru_isrss+usage.ru_maxrss);
    fprintf(fp, "%7ld\n", usage.ru_inblock+usage.ru_oublock);
    return SUCCESS;
}
/*
int main() {
    int a[100000]={0}, i;
    sleep(5);
    a[1000] = 10;
    printf("%d\n", a[1000]);
    for (i = 0; i < 100000; i++)
        printf("%d", a[i]);
    printf("\n");
    self_analyse(stdout);
    return 0;
}
*/
