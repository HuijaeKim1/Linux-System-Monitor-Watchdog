#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int main()
{
    unsigned long long total1 = 0;
    unsigned long long idle1 = 0;

    unsigned long long total2 = 0;
    unsigned long long idle2 = 0;

    // Read first CPU snapshot
    if (read_cpu(&total1, &idle1) != 0) {
        return 1;
    }

    while(1){

        sleep(1);

        // Read second CPU snapshot
        if(read_cpu(&total2, &idle2) != 0){
            return 1;
        }


        unsigned long long delta_total = total2 - total1;
        unsigned long long delta_idle = idle2 - idle1;

        // Prevent dividing by 0
        if (delta_total == 0) {
            fprintf(stderr, "Error: No CPU time elapsed\n");
            return 1;
        }


        double cpu_usage = 100 * (1 - (double)delta_idle/delta_total);

        printf("CPU Usage: %.2f%%\n", cpu_usage);

        total1 = total2;
        idle1 = idle2;

    }
    return 0;
}