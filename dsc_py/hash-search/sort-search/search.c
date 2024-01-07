#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 36
#define HASH_SIZE 24
#define ITERATIONS 1000

// Function to get a random hash from file
char* getRandomInput(FILE *file, char* hash) {
    fseek(file, 0, SEEK_END); // Move to the end of the file to get its size
    long fileSize = ftell(file);
    long recordSize = MAX_LINE_LENGTH; // Assuming fixed record size
    int numLines = (fileSize / recordSize) - 1;
    // printf("%d\n", numLines);

    int randomLine = rand() % numLines;

    long offset = randomLine * recordSize;
    fseek(file, offset, SEEK_SET);
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file); // Read the record

    // char hash[HASH_SIZE+1];
    memcpy(hash, line, HASH_SIZE+1);
    hash[HASH_SIZE] = '\0';

    return hash;
}

// Function to perform binary search on the file
int binarySearch(FILE *file, const char *target) {
    fseek(file, 0, SEEK_END); // Move to the end of the file to get its size
    size_t fileSize = ftell(file);
    long recordSize = MAX_LINE_LENGTH; // Assuming fixed record size

    size_t low = 0;
    size_t high = (fileSize / recordSize) - 1;
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        size_t offset = mid * recordSize;
        
        char hash[HASH_SIZE+1];
        fseek(file, offset, SEEK_SET); // Move to the middle record
        char line[MAX_LINE_LENGTH];
        fgets(line, MAX_LINE_LENGTH, file); // Read the record
        printf("%s\n", line);
        
        // printf("%s", line);
        memcpy(hash, line, HASH_SIZE+1);
        hash[HASH_SIZE] = '\0';
        int cmp = strcmp(hash, target);
        
        if (cmp == 0) {
            return mid; // Found the record
        } else if (cmp < 0) {
            low = mid + 1; // Target is in the right half
        } else {
            high = mid - 1; // Target is in the left half
        }
    }
    
    return -1; // Record not found
}

int main() {
    FILE *file = fopen("sorted_hashes_250M.txt", "r");
    if (file == NULL) {
        perror("Unable to open the file");
        return 1;
    }

    double times[ITERATIONS];

    for (int i =0; i < ITERATIONS; i++) {
        char hash[HASH_SIZE+1];
        getRandomInput(file, hash);
        
        //start time
        clock_t start_time = clock();

        int result = binarySearch(file, hash);

        // if (result != -1) {
        //     printf("Record found at line %d\n", result + 1); // Line numbers start from 1
        // } else {
        //     printf("Record not found\n");
        // }

        // Record the end time
        clock_t end_time = clock();
        // Calculate the elapsed time in seconds
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        
        times[i] = elapsed_time;
        // Print the results
        printf("Iteration : %04d, Elapsed time: %lf seconds, \n", i,elapsed_time);
    }

    // Open the file for writing
    FILE *file2 = fopen("times.txt", "w");
    if (file2 == NULL) {
        perror("Error opening file");
        return 1;
    }

    double sum = 0;
    // Write the time results to the file
    for (size_t i = 0; i < ITERATIONS; i++) {
        sum += times[i];
        fprintf(file2, "%lf\n", times[i] );
    }

    printf("Average search time - %lf\n", sum/ITERATIONS);

    fclose(file);
    fclose(file2);
    return 0;
}
