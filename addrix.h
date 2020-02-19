#ifndef __ADDRIX_H__
#define __ADDRIX_H__

// Encryption constants
#define ROUNDS 32

// Key scheduling magic
// (This is what happens when you eat crap at a cafe)
#define MAGIC0 0x2357DEAD
#define MAGIC1 0x8BADF00D
#define MAGIC2 0xBADDCAFE
#define MAGIC3 0xDEAD2BAD

// Key seasoning rounds
#define SPICINESS 8

#include <stdio.h>
#include <stdint.h>

void doRound(uint32_t *data, const uint32_t *roundKey, unsigned int roundNumber);
void undoRound(uint32_t *data, const uint32_t *roundKey, unsigned int roundNumber);
void seasonKey(uint32_t *key);
void nextRoundKey(uint32_t *seasonedKey, unsigned int roundNumber);

void encrypt(uint32_t *plaintext, uint32_t *key);
void decrypt(uint32_t *ciphertext, uint32_t *key);

#endif
