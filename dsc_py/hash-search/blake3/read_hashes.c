#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "blake3.h"
#define HASH_SIZE 8
long block_size=150;
struct hashObject
{
    char byteArray[HASH_SIZE];
    long int nonce;
};


void printArray(unsigned char byteArray[HASH_SIZE],long int nonce, int arraySize)
{
    printf("printArray(): ");
    for (size_t i = 0; i < arraySize; i++)
    {
        printf("%02x ", byteArray[i]); // Print each byte in hexadecimal format
    }
    printf("Nonce : %ld\n",nonce);
}
struct hashObject *hashresults;

int main()
{
    struct stat statbuf;
    int fd = open("hashetree_1.bin", O_RDONLY);
    FILE *hash_file=fopen("../hashes_6.bin","r");
    if (fstat(fd, &statbuf) == -1) 
    {
        perror("fstat");
        return 1;
    }
    
    long file_size = statbuf.st_size;
    hashresults=( struct hashObject *)malloc(block_size*sizeof( struct hashObject)); 
    fread(hashresults,(block_size * sizeof(struct hashObject)),1,hash_file);
    for (int i=0;i<block_size;i++)
    {
        printArray(hashresults[i].byteArray,hashresults[i].nonce,8);
    }

   // for (int i=0;i<)
    close(fd);
    fclose(hash_file);
    return 0;
}