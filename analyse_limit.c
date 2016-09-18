#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//headers
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

//defined headers
#include "types.h"

//max limits
#define MAX_ARGS 3
#define MAX_CMD 512
#define MAX_LINE 1024

#define CONFIG_FILE "config.txt"
#define CONFIG_VARIABLES 3

//error related
#define ERROR_FAILURE -1

#define SUCCESS 0
#define FAILURE 1

struct limits resource_limits;

int read_config(char *file) {
    FILE *fp;
    char s[MAX_LINE]= {0};
    int count[CONFIG_VARIABLES] = {13, 14, 15}, i;

    //ip assert
    
    //initialization
    fp = NULL;
    i = 0;

    //computation
    fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen failed :%s:%d:", 
                __FILE__, __LINE__);
        perror("");
        return ERROR_FAILURE;
    }
    if (fgets(s, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "fopen failed :%s:%d:", 
                __FILE__, __LINE__);
        perror("");
        return ERROR_FAILURE;
    }
    sscanf(&s[count[i]], "%f", &resource_limits.cpu_usage);
    i++;

    if (fgets(s, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "fopen failed :%s:%d:", 
                __FILE__, __LINE__);
        perror("");
        return ERROR_FAILURE;
    }
    sscanf(&s[count[i]], "%ld", &resource_limits.ram_size);
    i++;

    if (fgets(s, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "fopen failed :%s:%d:", 
                __FILE__, __LINE__);
        perror("");
        return ERROR_FAILURE;
    }
    sscanf(&s[count[i]], "%ld", &resource_limits.disk_io);
    i++;

    //op assert
    return SUCCESS;
}


float cpu_utilization (int pid, long time) {
    return 0;
}

int limit_resources (int pid) {
    char cmd[MAX_CMD];

#ifdef DEBUG
    fprintf(stderr, "in limit :%s:%d:%d\n", 
            __FILE__, __LINE__, pid);
#endif
    sprintf(cmd, "cpulimit -l %d -p %d > /tmp/null.txt & "
            "echo \"successfully limited to %d%%\"\n", 
            (int)(resource_limits.cpu_usage+0.5), pid,
            (int)(resource_limits.cpu_usage+0.5));
    if (system(cmd) < 0) {
        fprintf(stderr, "cpulimit failed :%s:%d:", 
                __FILE__, __LINE__);
        perror("");
        return ERROR_FAILURE;
    }
#ifdef DEBUG
    fprintf(stderr, "in limit :%s:%d:%d\n", 
            __FILE__, __LINE__, pid);
#endif
    return SUCCESS;
}
int analyse(FILE *fp, int pid, long time) {
    long mem;
    FILE *fp_tmp;
    char buff[MAX_LINE], cmd[MAX_CMD];
#ifdef DEBUG
    fprintf(stderr, "in analyse :%s:%d:%d:%ld\n", 
            __FILE__, __LINE__, pid, time);
#endif
    //ip assert
    
    //initialization 
    fp_tmp = NULL;

    //computation
    fprintf(fp, "CPU[%%]\tRAM[MB]\tDISK_IO[count]\n");
   
    while(1) {
        kill(pid, 0);
        if (errno == ESRCH) {
            return SUCCESS;
        }
        sprintf(cmd, "ps -p %d -o %%cpu > tmp", pid);
        system(cmd);

        fp_tmp = fopen("tmp", "r");
        fscanf(fp_tmp, "%s", buff);
        fscanf(fp_tmp, "%s", buff);
        fclose(fp_tmp);

        fprintf(fp, "%6s\t", buff);

        sprintf(cmd, "pmap %d | grep total > tmp", pid);
        system(cmd);
        fp_tmp = fopen("tmp", "r");
        fscanf(fp_tmp, "%s", buff);
        fscanf(fp_tmp, "%s", buff);
        fclose(fp_tmp);

        fprintf(fp, "%7s\t", buff);

        sprintf(cmd, "cat /proc/%d/io > tmp", pid);
        system(cmd);
        fp_tmp = fopen("tmp", "r");
        fscanf(fp_tmp, "rchar:%s", buff);
        fscanf(fp_tmp, "wchar:%s", buff);
        fscanf(fp_tmp, "syscr:%s", buff);
        fclose(fp_tmp);

        fprintf(fp, "%14s\n", buff);
    }

#ifdef DEBUG
    fprintf(stderr, "in analyse :%s:%d:%d:%ld\n", 
            __FILE__, __LINE__, pid, time);
#endif

    //op assert
    return SUCCESS;
}

int main (int argc, char *argv[], char *envp[]) {
    //declarations
    int pid;

#ifdef DEBUG
    print_arg(stderr, argc, argv);
#endif
    //ip assert
    if (argc < MAX_ARGS) {
        printf( "usage : %s <interval (usec)> < <program path>"
                " <arguments program> >\n", 
                argv[0]);
        return FAILURE;
    }

    //initialization
    //read config file and fill the global data structure
    if (read_config(CONFIG_FILE)) {
        return FAILURE;
    }
#ifdef DEBUG
    print_config(stderr);
#endif
    //computation
    pid = fork();
    if (pid == 0) {
        usleep(1000);
#ifdef DEBUG
        fprintf(stderr, "Child\n");
#endif
        printf("arv[4]: %s\n", argv[4]);
        if (execl(argv[2], argv[2], argv[3], argv[4], (char*)0) < 0) {
            fprintf(stderr, "execve failed:%s:%d:", 
                    __FILE__, __LINE__);
            perror("");
        }
    }
    else if (pid > 0) {
#ifdef DEBUG
        fprintf(stderr, "Parent\n");
#endif
        //limit the resources before starting of process
        limit_resources(pid);
        //analyse the utilization of resources by process
        //analyse(stdout, pid, atoi(argv[1]));
        wait(NULL);
    }
    else {
        fprintf(stderr, "fork failed:%s:%d:", 
                __FILE__, __LINE__);
        perror("");
    }

    //op assert
    return SUCCESS;
}
