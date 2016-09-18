#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


#define MAX_LINE 1024
#define MAX_CMD 512

#define SUCCESS 0
#define FAILURE 1

int self_analyse(FILE *fp) {
    FILE *fp_tmp;
    char buff[MAX_LINE], cmd[MAX_CMD];
    //struct rusage usage;
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
    fprintf(fp, "\n---------------------------------------------------\n");
    fprintf(fp, "CPU[%%]\t\tRAM[KB]\t\tDISK_IO[count]\n");
    sprintf(cmd, "ps -p %d -o %%cpu > /tmp/null0", pid);
    system(cmd);
    buff[0] = '-';
    buff[1] = 0;
    fp_tmp = fopen("/tmp/null0", "r");
    fscanf(fp_tmp, "%s", buff);
    fscanf(fp_tmp, "%s", buff);
    fclose(fp_tmp);
    fprintf(fp, "%6s\t\t", buff);

    sprintf(cmd, "pmap %d | grep total > /tmp/null1", pid);
    system(cmd);
    fp_tmp = fopen("/tmp/null1", "r");
    buff[0] = '-';
    buff[1] = 0;
    fscanf(fp_tmp, "%s", buff);
    buff[0] = '-';
    buff[1] = 0;
    fscanf(fp_tmp, "%7s", buff);
    fclose(fp_tmp);

    fprintf(fp, "%7s\t\t", buff);
    //getrusage(RUSAGE_SELF, &usage);
    //fprintf(fp, "%7ld\t", 
    //usage.ru_ixrss+usage.ru_idrss+usage.ru_isrss+usage.ru_maxrss);
    //fprintf(fp, "%7ld\n", usage.ru_inblock+usage.ru_oublock);

    sprintf(cmd, "cat /proc/%d/io | grep sys > /tmp/null2", pid);
    system(cmd);
    fp_tmp = fopen("/tmp/null2", "r");
    buff[0] = '-';
    buff[1] = 0;

    fscanf(fp_tmp, "syscr: %s", buff);
    fprintf(fp, "read : %s ", buff);

    buff[0] = '-';
    buff[1] = 0;

    fscanf(fp_tmp, "\nsyscw: %s", buff);
    fprintf(fp, "write: %s\n", buff);
    fprintf(fp, "---------------------------------------------------\n");
    fclose(fp_tmp);


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
