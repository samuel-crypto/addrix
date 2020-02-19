#include "addrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define mask 0xFFFFFFFF
#define rotr32(val, rot) (val >> rot) | (val << (-rot & mask))
#define rotl32(val, rot) (val << rot) | (val >> (-rot & mask))

// Painfully simple round function
void doRound(uint32_t *data, const uint32_t *roundKey, unsigned int roundNumber)
{
  // One can ponder on whether or not this is a good way to prevent slide attacks
  data[3] ^= roundNumber;

  // XOR it up!
  data[0] ^= roundKey[0];
  data[1] ^= roundKey[1];
  data[2] ^= roundKey[2];
  data[3] ^= roundKey[3];

  // Rotate it!
  data[0] = rotr32(data[0], 7);
  data[1] = rotr32(data[1], 9);
  data[3] = rotr32(data[3], 13);

  // Addition step
  data[0] = data[0] + data[1];
  data[1] = data[1] + data[2];
  data[2] = data[2] + data[3];
  data[3] = data[3] + data[0];

  // Big rotate
  uint32_t tmp = data[0];
  data[0] = data[1];
  data[1] = data[2];
  data[2] = data[3];
  data[3] = tmp;
}

void undoRound(uint32_t *data, const uint32_t *roundKey, unsigned int roundNumber)
{
  // Big unrotate
  uint32_t tmp = data[3];
  data[3] = data[2];
  data[2] = data[1];
  data[1] = data[0];
  data[0] = tmp;

  // Subtract step
  data[3] = data[3] - data[0];
  data[2] = data[2] - data[3];
  data[1] = data[1] - data[2];
  data[0] = data[0] - data[1];

  // Reverse rotations
  data[0] = rotl32(data[0], 7);
  data[1] = rotl32(data[1], 9);
  data[3] = rotl32(data[3], 13);

  // XOR step
  data[0] ^= roundKey[0];
  data[1] ^= roundKey[1];
  data[2] ^= roundKey[2];
  data[3] ^= roundKey[3];  

  // Remove constant
  data[3] ^= roundNumber;
}

void seasonKey(uint32_t *key)
{
  uint32_t MAGIC[] = {MAGIC0, MAGIC1, MAGIC2, MAGIC3};
  for(unsigned int i = 0; i < SPICINESS; i++)
    doRound(key, MAGIC, i);
}

void nextRoundKey(uint32_t *seasonedKey, unsigned int roundNumber)
{
  uint32_t MAGIC[] = {MAGIC0, MAGIC1, MAGIC2, MAGIC3};
  doRound(seasonedKey, MAGIC, ROUNDS + roundNumber);
}

void encrypt(uint32_t *plaintext, uint32_t *key)
{
  seasonKey(key);

  for(unsigned int i = 0; i < ROUNDS; i++)
  {
    nextRoundKey(key, i);
    doRound(plaintext, key, i);
  }
}

void decrypt(uint32_t *ciphertext, uint32_t *key)
{
  seasonKey(key);

  uint32_t roundKeys[ROUNDS][4];

  // For now, set the 0th round key to the key, this will change as soon as we get to the key scheduling loop
  roundKeys[0][0] = key[0];
  roundKeys[0][1] = key[1];
  roundKeys[0][2] = key[2];
  roundKeys[0][3] = key[3];

  // Key scheduling loop
  for(unsigned int i = 0; i < ROUNDS; i++)
  {
    //roundKeys[i] = previous key scheduling output or the initial key in the case of i = 0
    nextRoundKey(roundKeys[i], i);

    if(i < ROUNDS - 1)
    {
      roundKeys[i + 1][0] = roundKeys[i][0];
      roundKeys[i + 1][1] = roundKeys[i][1];
      roundKeys[i + 1][2] = roundKeys[i][2];
      roundKeys[i + 1][3] = roundKeys[i][3];
    }
  }

  for(int j = ROUNDS - 1; j >= 0; j--)
  {
    unsigned int i = (unsigned int)j;
    undoRound(ciphertext, roundKeys[i], i);
  }
}
