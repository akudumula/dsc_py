#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include "blake3.h"

struct hash_input
{
    char *fingerprint;
    char *public_key;
    long nonce;
};


char *fingerprint;
char *public_key;
int threads_count;
int block_time;
char *hash_value;
long nonce=-1;
long hashes_generated=0;

void *generate_hashes(void *arg)
{
    int thread_id = *(int *)arg;
    printf("Thread id: %d\n",thread_id);
    struct hash_input *hash_inputs = (struct hash_input *)malloc(sizeof(struct hash_input));
    hash_inputs->fingerprint=fingerprint;
    hash_inputs->public_key=public_key;
    hash_inputs->nonce=(long)thread_id;
    char *hash_result = (char *)malloc(sizeof(char) * 24);
    struct timeval start, current;
    double elapsedTime = 0.0;
    const double targetTime = 6.0; // Duration in seconds for each thread

    //int tid = *((int *)threadId);

    gettimeofday(&start, NULL);
    printf("Assignment complete\n");
    while((elapsedTime < targetTime) || (nonce!=-1))
    {
        /* code */
        //printf("Thread %d - Point 1\n", thread_id);
        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, hash_inputs, sizeof(struct hash_input));
        blake3_hasher_finalize(&hasher, hash_result, 24);
        //printf("Thread %d - Point 2\n", thread_id);
        if (strcmp(hash_result,hash_value)==0)
        {
            nonce = hash_inputs->nonce;
            break;
        }
        hash_inputs->nonce=hash_inputs->nonce+threads_count;
        gettimeofday(&current, NULL);
        elapsedTime = (current.tv_sec - start.tv_sec) + (current.tv_usec - start.tv_usec) / 1000000.0;

    } 
    if (thread_id == threads_count-1)
    {
        hashes_generated=hash_inputs->nonce;
    }
    //printf("Elapsed time: %ld.%06ld\n", (long int) current.tv_sec, (long int) current.tv_usec);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc !=6)
    {
        printf("Usage: %s string_arg1 string_arg2 numeric_arg\n",argv[0]);
        return 1;
    }

    //Access string arguments (Fingerprint and public key)
    fingerprint = argv[1];
    public_key = argv[2];

    //access number of threads and block_time
    threads_count = atoi(argv[3]);
    block_time = atoi(argv[4]);

    //access hash
    hash_value = argv[5];

    

    //use these values
    printf("Fingerprint : %s\n",fingerprint);
    printf("Public key : %s\n",public_key);
    printf("Thread count : %d\n",threads_count);
    printf("Block_time : %d\n",block_time);
    printf("Hash_Value : %s\n",hash_value);

    pthread_t threads[threads_count];
    int threadsIds[threads_count];
    // int threadsIds = (int)malloc(threads_count * sizeof(int));

    for (int i = 0; i < threads_count; i++)
    {
        threadsIds[i] = i;
        pthread_create(&threads[i], NULL, generate_hashes, &threadsIds[i]);
    }

    for (int i = 0; i < threads_count; i++) {
        pthread_join(threads[i], NULL);
    } 

    // hash_input2 = (struct hash_input *) malloc (sizeof(struct hash_input));
    // hash_input1->fingerprint=fingerprint;
    // hash_input1->public_key=public_key;
    // hash_input1->nonce=1;

    // hash_input2->fingerprint=fingerprint;
    // hash_input2->public_key = public_key;
    // hash_input2->nonce=0;
    printf("Hashes generated: %ld\n",hashes_generated);
    printf("Nonce : %ld\n",nonce);

}