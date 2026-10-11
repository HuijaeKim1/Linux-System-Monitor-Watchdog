#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



// When program receives SIGINT, stop_requested changes to 1 and exits the program
volatile sig_atomic_t stop_requested = 0;

// Signal handler
void handle_signal(int signum) {
    if (signum == SIGINT) {
        stop_requested = 1;
    }
}

int main() {
    FILE *file;

    // Register SIGINT handler
    struct sigaction sa = {0};
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;


    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error registering signal handler");
        return 1;
    }

    // Continuously monitor CPU temperature
    while (!stop_requested) {

        file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");

        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        int raw_temp;

        // Read temperature and check for errors
        if (fscanf(file, "%d", &raw_temp) != 1) {
            fprintf(stderr, "Error reading temperature\n");
            fclose(file);
            return 1;
        }

        // Convert millidegrees Celsius to Celsius
        float converted_temp = raw_temp / 1000.0;

        printf("Temperature: %.2f °C\n", converted_temp);

        fclose(file);

        // Wait one second before next measurement
        if (!stop_requested) {
            sleep(1);
        }
    }

    printf("\nTemperature monitoring stopped safely.\n");

    return 0;
}