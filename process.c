
#include <stdio.h>
#include <errno.h>

// Returns process state on success, -1 on error
int check_process(int pid)
{
    char path[256];
    char line[256];
    char state = '\0';
    int found_state = 0;

    // Validate PID
    if (pid <= 0) {
        fprintf(stderr, "Error: Invalid PID\n");
        return -1;
    }

    // Construct the process status file path
    int len = snprintf(path, sizeof(path),
                       "/proc/%d/status", pid);

    // Check for formatting errors or truncation
    if (len < 0 || (size_t)len >= sizeof(path)) {
        fprintf(stderr, "Error: Invalid process path\n");
        return -1;
    }

    // Open /proc/[PID]/status
    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        if (errno == ENOENT) {
            fprintf(stderr, "Process %d does not exist\n", pid);
        } else {
            perror("Error opening process status");
        }
        return -1;
    }

    // Read process information line by line
    while (fgets(line, sizeof(line), fp) != NULL) {

        // Extract state character
        if (sscanf(line, "State: %c", &state) == 1) {
            found_state = 1;
            break;
        }
    }

    // Close file
    fclose(fp);

    // Verify that the state was found
    if (!found_state) {
        fprintf(stderr, "Error: Process state not found\n");
        return -1;
    }

    // Return state as integer character code
    return (int)state;
}
