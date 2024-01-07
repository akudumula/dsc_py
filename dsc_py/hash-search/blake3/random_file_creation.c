#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILE_SIZE_GB 10
#define BUFFER_SIZE_MB 1
#define BUFFER_SIZE (BUFFER_SIZE_MB * 1024 * 1024)  // Buffer size in bytes

int main() {
    FILE *file;
    uint8_t *buffer;
    size_t buffer_size = BUFFER_SIZE;
    size_t file_size = FILE_SIZE_GB * 1024ULL * 1024 * 1024;  // File size in bytes

    // Open file for writing in binary mode
    file = fopen("random_data2.bin", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Allocate buffer
    buffer = (uint8_t *)malloc(buffer_size);
    if (buffer == NULL) {
        perror("Error allocating buffer");
        fclose(file);
        return 1;
    }

    // Write random data to the file in chunks
    while (file_size > 0) {
        // Generate random data
        for (size_t i = 0; i < buffer_size; i++) {
            buffer[i] = (uint8_t)rand();
        }

        // Write buffer to file
        size_t bytes_to_write = (file_size < buffer_size) ? file_size : buffer_size;
        size_t bytes_written = fwrite(buffer, 1, bytes_to_write, file);
        if (bytes_written != bytes_to_write) {
            perror("Error writing to file");
            free(buffer);
            fclose(file);
            return 1;
        }

        file_size -= bytes_written;
    }

    // Close file and free buffer
    fclose(file);
    free(buffer);

    printf("File created successfully: random_data2.bin\n");

    return 0;
}
