#include <stdio.h>
#include "types.h"

void print_arg(FILE *fp, int count, char *argv[]) {
    int i;

    for (i = 0; i < count; i++) {
        fprintf(fp, "argv[%d]:%s\n", i, argv[i]);
    }
}
void print_config(FILE *fp) {
    fprintf(fp, "CPU usage : %f\n", resource_limits.cpu_usage);
    fprintf(fp, "RAM usage : %ld\n", resource_limits.ram_size);
    fprintf(fp, "Disk  I/O : %ld\n", resource_limits.disk_io);
}
