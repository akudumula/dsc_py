#include "blake3.h"
#include <stdio.h>
#include <stdlib.h>

// Generate a 4-byte nonce
unsigned char *generate_nonce() {
  unsigned char *nonce = malloc(4);
  for (int i = 0; i < 4; i++) {
    nonce[i] = rand();
  }
  return nonce;
}

// Create a Blake3 hash of the given nonce
unsigned char * create_hash(unsigned char *nonce) {
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);
  blake3_hasher_update(&hasher, nonce, 4);
  unsigned char hash[12];
  blake3_hasher_finalize(&hasher, hash, 12);
  return hash;
}

// Store the given hash in binary format in the given file
void store_hash_binary(unsigned char *hash, FILE *file) {
  fwrite(hash, 1, 12, file);


        //   size_t bytesWritten = fwrite(bucket, 1, bucketSizeInBytes, file);
        // if (bytesWritten != bucketSizeInBytes)
        // {
        //     perror("Error writing to file");
        //     fclose(file);
        //     return 1;
        // }
}

// Save the nonce and hash in text format in the given file
void save_nonce_and_hash_text(unsigned char *nonce, unsigned char *hash, FILE *file) {
  fprintf(file, "%llu ", nonce);
  for (int i = 0; i < 12; i++) {
    fprintf(file, "%02x", hash[i]);
  }
  fprintf(file, "\n");
}

int main() {
  // Create a file to store the binary hashes
  FILE *binary_file = fopen("hashes.bin", "wb");
  if (binary_file == NULL) {
    printf("Error opening binary file\n");
    return 1;
  }

  // Create a file to store the nonce and hash in text format
  FILE *text_file = fopen("hashes.txt", "w");
  if (text_file == NULL) {
    printf("Error opening text file\n");
    return 1;
  }

  // Generate 100 hashes
  for (int i = 0; i < 100; i++) {
    // Generate a nonce
    unsigned char *nonce = generate_nonce();

    // Create a hash of the nonce
    unsigned char *hash = create_hash(nonce);

    // Store the hash in binary format
    store_hash_binary(hash, binary_file);

    // Save the nonce and hash in text format
    save_nonce_and_hash_text(nonce, hash, text_file);

    // Free the nonce and hash
    free(nonce);
    free(hash);
  }

  // Close the binary and text files
  fclose(binary_file);
  fclose(text_file);

  return 0;
}