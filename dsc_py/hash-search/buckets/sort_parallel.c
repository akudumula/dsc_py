#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "common.c"


size_t numBuckets;
size_t numberOfBucketsToSort;
size_t bucketSizeInBytes;

pthread_mutex_t file_lock;
struct hashObject *buckets;

void writebucketToFile( struct hashObject *bucket, int index ) {
    pthread_mutex_lock(&file_lock);

    FILE *file;
    // Open the binary file for reading and writing
    file = fopen("plot.memo", "wb");
    if (!file)
    {
        perror("Unable to open file");
        return;
    }

    if (fseeko(file, index, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    fwrite(bucket, 1, bucketSizeInBytes, file);
    fclose(file);

    pthread_mutex_unlock(&file_lock);
    return;
}

// Function to sort a bucket
void *sortBucket(void *arg)
{
    int thread_id = *(int *)arg;
    printf("Thread id - %d\n", thread_id);

    FILE *file;
    struct hashObject *bucket = (struct hashObject *) malloc( bucketSizeInBytes );
 
    // Open the binary file for reading and writing
    file = fopen("plot.memo", "rb+");
    if (!file)
    {
        perror("Unable to open file");
        return NULL;
    }

    size_t bucket_location = thread_id * bucketSizeInBytes;
    if (fseeko(file, bucket_location, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return NULL;
    }
    fclose(file);

    size_t sizeRead = fread(bucket, 1, bucketSizeInBytes, file);
    if ( sizeRead != bucketSizeInBytes ) {
        perror("File not read");
        return NULL;
    }

    printf("Thread id - %d, Starting Sort\n", thread_id);
    qsort(bucket, numberOfBucketsToSort, sizeof(struct hashObject), compareHashObjects);
    printf("Thread id - %d, Sorting Done\n", thread_id);
    

    writebucketToFile( bucket, thread_id );

    return NULL;
}

int main()
{
    numBuckets = (int)pow(2, PREFIX_SIZE * 8);
    numberOfBucketsToSort = (hashesPerBucketRead / numberOfHashesInBucket) * numBuckets;
    bucketSizeInBytes = numberOfBucketsToSort * sizeof(struct hashObject);

    printf("hashesPerBucketRead : %zu\n", hashesPerBucketRead);
    printf("Number of bucket : %zu\n", numBuckets);
    printf("Number of hashes per bucket to sort : %zu\n", numberOfHashesInBucket);
    printf("Number of bucket to sort : %zu\n", numberOfBucketsToSort);
    printf("Number of threads : %d\n", NUM_THREADS);

    if (pthread_mutex_init(&file_lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    pthread_t threads[NUM_THREADS];

    // Create threads to sort each bucket in parallel
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, sortBucket, &i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Write the sorted buckets back to the file
    // fseek(file, 0, SEEK_SET);
    // for (int i = 0; i < NUM_BUCKETS; i++)
    // {
    //     fwrite(buckets[i], sizeof(struct hashObject), BUCKET_SIZE, file);
    // }

    // fclose(file);

    return 0;
}
