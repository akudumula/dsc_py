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

