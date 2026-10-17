#include <stdio.h>

double get_memory_usage(void){

    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp == NULL){
        perror("open");
        return -1.0;
    }

    // Read and extract memory values
    char line[256];
    unsigned long mem_total;
    unsigned long mem_available;
    int found_total = 0;
    int found_available = 0;

    while(fgets(line, 256, fp) != NULL){
        if(sscanf(line, "MemTotal: %lu", &mem_total) == 1){
            found_total = 1;
        }

        if(sscanf(line, "MemAvailable: %lu", &mem_available) == 1){
            found_available = 1;
        }

        if(found_total == 1 && found_available == 1){
            break;
        }

    }

    // Error Handling
    if (found_total == 0 || found_available == 0 || mem_total == 0 || mem_available > mem_total){
        fclose(fp);
        return -1.0;
    }

    double memory_usage = ((double)(mem_total - mem_available) / mem_total) * 100;
    
    fclose(fp);

    return memory_usage;
}

