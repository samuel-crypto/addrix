#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "addrix.h"

void printL(uint32_t *data);
int main(int argc, char **argv)
{
  // Example: encrypting all 0s data with all 0s key, ROUND/SPICINESS settings can be tweaked in addrix.h
  // To compile use: gcc main.c addrix.c -o main

  uint32_t data[] = {0, 0, 0, 0};
  const uint32_t original_key[] = {0, 0, 0, 0};

  // encrypt will overwrite the key so we are not using original_key
  uint32_t key[4];
  memcpy(key, original_key, sizeof(uint32_t) * 4);

  // CAREFUL! encrypt will OVERWRITE key
  encrypt(data, key);

  printf("Ciphertext: ");
  printL(data);

  printf("\r\n");


  // Decrypt
  memcpy(key, original_key, sizeof(uint32_t) * 4);

  decrypt(data, key);

  printf("Plaintext: ");
  printL(data);

  printf("\r\n");

  

  return 0;
}


void printL(uint32_t *data) {
  uint8_t *dood = (uint8_t *)data;
  for(unsigned int k = 0; k < 16; k++)
  {
    if(k == 0)
      printf("|");

    printf("%u", dood[k]);
    if((k + 1) % 4 != 0)
      printf(", ");
    else
    {
      printf("|");
      if(k < 15)
        printf("  |");
    } 
  }
}

