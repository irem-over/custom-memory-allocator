#include <stdio.h>
#include <string.h>
#include "allocator.h"

int main(void) {
    printf("Starting custom memory allocator test...\n");

    dump_memory_state();

    int *numbers = (int *)my_malloc(5 * sizeof(int));
    if (numbers == NULL) {
        printf("Allocation failed for numbers.\n");
        return -1;
    }

    for (size_t index = 0; index < 5; index++) {
        numbers[index] = (int)(index * 10);
    }

    char *text = (char *)my_malloc(64 * sizeof(char));
    if (text == NULL) {
        printf("Allocation failed for text.\n");
        return -1;
    }

    strcpy(text, "Embedded Systems and Low-lwvwl Architecture");

    printf("Payload Data Verification:\n");
    for (size_t index = 0; index < 5; index++) {
        printf("numbers[%zu] = %d\n", index, numbers[index]);
    }
    printf("text string = %s\n", text);

    dump_memory_state();

    return 0;
}