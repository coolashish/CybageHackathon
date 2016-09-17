#ifndef __TYPES_H__
#define __TYPES_H__

struct limits {
    float cpu_usage;
    long ram_size;
    long disk_io;
};

extern struct limits resource_limits;

void print_config(FILE *fp);
#endif
