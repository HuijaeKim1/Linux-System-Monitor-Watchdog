#include <stdio.h>

double get_memory_usage(void);

int main(void){
    double memory_usage = get_memory_usage();

    if (memory_usage < 0){
        printf("Error Reading Memory Usage\n");
    }
    else
        printf("Memory Usage: %.2f%%\n", memory_usage);

    return 0;
}