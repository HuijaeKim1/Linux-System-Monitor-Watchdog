#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"

// Read CPU statistics from /proc/stat
int read_cpu(unsigned long long *total, unsigned long long *idle)
{
    FILE *file;

    file = fopen("/proc/stat", "r");

    if (file == NULL) {
        perror("Error opening /proc/stat");
        return 1;
    }

    unsigned long long user, nice, system, idle_time, iowait;

    // Read the first five CPU counters
    if (fscanf(file, "cpu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle_time, &iowait) != 5) {
        fprintf(stderr, "Error reading CPU data\n");
        fclose(file);
        return 1;
    }

    // Calculate CPU totals
    *total = user + nice + system + idle_time + iowait;
    *idle = idle_time + iowait;

    fclose(file);

    return 0;
}

