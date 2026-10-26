#include <stdio.h>
#include <unistd.h>
#include "cpu.h"
#include "memory.h"

int main(void)
{
    unsigned long long total1 = 0;
    unsigned long long idle1 = 0;

    unsigned long long total2 = 0;
    unsigned long long idle2 = 0;

    // Read first CPU snapshot
    if (read_cpu(&total1, &idle1) != 0) {
        return 1;
    }

    while (1) {

        sleep(1);

        // Read second CPU snapshot
        if (read_cpu(&total2, &idle2) != 0) {
            return 1;
        }

        // Calculate differences between CPU snapshots
        unsigned long long delta_total = total2 - total1;
        unsigned long long delta_idle = idle2 - idle1;

        // Prevent division by zero
        if (delta_total == 0) {
            fprintf(stderr, "Error: No CPU time elapsed\n");
            return 1;
        }

        // Calculate CPU usage
        double cpu_usage =
            100.0 * (1.0 - (double)delta_idle / delta_total);

        // Calculate memory usage
        double memory_usage = get_memory_usage();


        if (memory_usage < 0) {
            fprintf(stderr, "Error reading memory usage\n");
            return 1;
        }

        // Display system statistics
        printf("CPU Usage: %.2f%%\n", cpu_usage);
        printf("Memory Usage: %.2f%%\n", memory_usage);
        printf("---------------------------\n");

        // Update CPU counters for next iteration
        total1 = total2;
        idle1 = idle2;
    }

    return 0;
}