#include <stdio.h>
#include <string.h>
#include "allocator.h"

int main(void) {
    printf("Starting custom memory allocator test...\n");

    int *numbers = (int *)my_malloc(5 * sizeof(int));
    char *text = (char *)my_malloc(64 * sizeof(char));

    printf("Initial Allocation State:");
    dump_memory_state();

    printf("Freeing 'text pointer...\n");
    my_free(text);
    text = NULL;
    dump_memory_state();

    printf("Freeing 'numbers' pointer...\n");
    my_free(numbers);
    numbers = NULL;
    dump_memory_state();

    return 0;
}