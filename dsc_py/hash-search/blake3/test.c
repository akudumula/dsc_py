#include <stdint.h>
#include <stdio.h>
#include "blake3.h"

int main() {
    size_t test = 2500000000;

    int result = (test + 1) % 250000000;
    printf("%d", result);
}