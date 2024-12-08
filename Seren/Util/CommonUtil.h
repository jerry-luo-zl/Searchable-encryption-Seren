#ifndef AURA_COMMONUTIL_H
#define AURA_COMMONUTIL_H

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include<math.h>
#include <stdio.h>


#define AES_BLOCK_SIZE 16
#define DIGEST_SIZE 32
#define GGM_SIZE 65536
#define HASH_SIZE 3

int aes_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *ciphertext);

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *plaintext);

void sha256_digest(unsigned char *plaintext, int plaintext_len,
                unsigned char *digest);

unsigned int hmac_digest(unsigned char *plaintext, int plaintext_len,
                 unsigned char *key, int key_len,
                 unsigned char *digest);

unsigned int key_derivation(unsigned char *plaintext, int plaintext_len,
                            unsigned char *key, int key_len,
                            unsigned char *digest);

void strXor(uint8_t * output, uint8_t *sourceA, uint8_t *sourceB, int len);

int convertArrayToInt(const uint8_t* arr, int len);

void arrPrint(const char* arr, int len);
#endif //AURA_COMMONUTIL_H
