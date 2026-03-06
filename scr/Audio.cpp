
#include "Audio.h"

void write_sysfs(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    if (write(fd, value, strlen(value)) < 0) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

/*int Buzzer(int argc, char *argv[])
{
    char path[128];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <duty_cycle_ns>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *duty_ns = argv[1];

    // Export the PWM if not already exported
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s", PWM_CHANNEL);
    if (!file_exists(path)) {
        write_sysfs(PWM_CHIP "/export", PWM_CHANNEL);
        sleep(1);  // Wait for sysfs to create pwm0 directory
    }

    // Set period
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/period", PWM_CHANNEL);
    write_sysfs(path, PWM_PERIOD_NS);

    // Set duty cycle
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/duty_cycle", PWM_CHANNEL);
    write_sysfs(path, duty_ns);

    // Enable PWM
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/enable", PWM_CHANNEL);
    write_sysfs(path, "1");

    printf("PWM set to duty cycle %s ns at 1 kHz\n", duty_ns);
    printf("Press Ctrl+C to stop. PWM remains active.\n");

    return 0;

}*/

int BuzzerDuty(int duty_ns)
{
    char path[128];

    // Export PWM if needed
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s", PWM_CHANNEL);
    if (!file_exists(path)) {
        write_sysfs(PWM_CHIP "/export", PWM_CHANNEL);
        sleep(1);
    }

    // Set period
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/period", PWM_CHANNEL);
    write_sysfs(path, PWM_PERIOD_NS);

    // Set duty cycle
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/duty_cycle", PWM_CHANNEL);
    char duty_str[16];
    snprintf(duty_str, sizeof(duty_str), "%d", duty_ns);
    write_sysfs(path, duty_str);

    // Enable PWM
    snprintf(path, sizeof(path), PWM_CHIP "/pwm%s/enable", PWM_CHANNEL);
    write_sysfs(path, "1");

    printf("AudioLevel: %d\n",duty_ns);

    return 0;
}

