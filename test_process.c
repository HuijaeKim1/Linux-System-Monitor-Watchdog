
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "process.h"

int main(int argc, char *argv[])
{
    // Require exactly one PID argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    // Convert string argument into a PID
    char *endptr;
    errno = 0;

    long input_pid = strtol(argv[1], &endptr, 10);

    // Validate conversion and PID range
    if (errno != 0 || endptr == argv[1] ||
        *endptr != '\0' ||
        input_pid <= 0 || input_pid > INT_MAX) {
        fprintf(stderr, "Error: Invalid PID\n");
        return 1;
    }

    int pid = (int)input_pid;

    // Get process state
    int state = check_process(pid);

    if (state == -1) {
        fprintf(stderr, "Failed to check PID %d\n", pid);
        return 1;
    }

    // Display process information
    printf("Process PID: %d\n", pid);
    printf("Process State: %c\n", state);

    return 0;
}
