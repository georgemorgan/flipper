#include <flipper/flipper.h>

int uart0_configure(void) {
    printf("Configured the uart0.\n");
    return lf_success;
}

int uart0_ready(void) {
    printf("Checking if the uart0 is ready.\n");
    return lf_success;
}

void uart0_enable(void) {
    printf("Enabling the uart0 bus.\n");
}

void uart0_disable(void) {
    printf("Disabling the uart0 bus.\n");
}

void uart0_put(uint8_t byte) {
    printf("Putting '%c' to the uart0 bus.\n", byte);
}

uint8_t uart0_get(void) {
    printf("Getting from the uart0 bus.\n");
    return '\0';
}

int uart0_write(void *src, uint32_t length) {
    printf("Writing to the uart0 bus: %s\n", src);
    return lf_success;
}

int uart0_read(void *dst, uint32_t length) {
    printf("Reading from the uart0 bus: ");
    fgets(dst, length, stdin);
    fseek(stdin, 0, SEEK_END);
    return lf_success;
}
