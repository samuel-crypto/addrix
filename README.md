# Addrix
## Quick about
A very fast & simple 128-bit cipher designed primarily to be a difficult exercise in cryptoanalysis. Addrix has not been artificially simplified and is in fact designed to withstand generic cryptoanalytic attacks. However, the round construction and key schedule are so simple that reduced round (and potentially the full) versions can be attacked with various adaptions on common attacks (differential & linear cryptoanalysis + rotational cryptoanalysis).

If you are new to this topic and the terminology is confusing, I highly recommend [this site](http://theamazingking.com/crypto-block.php).

## Cipher construction
The cipher consists of one basic building block - the round function. The round function takes as input a 128-bit number (round input) and a 128-bit round key. Every round also uses a round constant set to the round number (0 for round 0, 1 for round 1, etc).

Encryption is straightforward - take the plaintext and apply the round function consecutively with the correct round keys for the desired number of rounds, then output the ciphertext.

Key scheduling is also very trivial - simply encrypt your original key with **S** rounds where every round key is set to a magic number. After **S** rounds, every next round output is a round key. So for instance if you had `S = 8` and you needed *4* round keys, you would do the following: take key and apply round function 8 times with the magic number (see below) as the round key. The 9th round output is the first generated round key, the 10th round output is the second generated round key, e.t.c.

Keep in mind that I refer to this number **S** as **SPICINESS**.
The initial **S** rounds are called *'Key Seasoning'*.

### Magic Number
The magic number is a 128-bit number consisting of the following 32-bit numbers: `0x2357DEAD`, `0x8BADF00D`, `0xBADDCAFE`, `0xDEAD2BAD`.

The numbers were chosen for no reason other than having a relatively nice distribution and telling a good story ('2357 died, ate bad food at a bad cafe. Dead too bad').

## Round diagram & Reference implementation

A basic visual display of how the round works can be found [here](https://github.com/samuel-crypto/addrix/raw/master/Addrix%20Round(2).png).

This repository also contains a reference implementation (`addrix.c` and `addrix.h`) + example use (`main.c`).
The compiler will scream at you because I didn't clean the code but I don't care because it's a challenge (pls don't be mad).

## Test vectors

Encrypting an all 0s plaintext with an all 0s key (**5 rounds and 5 spiciness**) should yield the following sequence of bytes (in decimal): 
```
99, 22, 130, 119, 210, 7, 150, 25, 223, 97, 224, 115, 2, 91, 81, 2
```

Same as above except with the last (rightmost) bit of the plaintext set to 1 will yield:
```
106, 245, 178, 108, 103, 56, 86, 0, 26, 19, 50, 17, 189, 218, 210, 153
```

## Overview of different parameters
Basic recommendation: **32** rounds and **8** key-scheduling rounds (**spiciness**).

### Outlandish figures, useful for attackers:
 - **5 rounds + 5 spiciness**: The absolute minimum required for every input bit to influence every output bit and for tiny changes to distribute properly to the entire ciphertext, less than this and you will encounter basic statistical problems
 - **8 rounds + 8 spiciness**: Smallest round count considered even remotely reasonable
 - **12 rounds + 8 spiciness**: An attack on this would be considered a good achievement
 - **16 rounds + 8 spiciness**: An attack on this is quite impressive
 - **24 rounds + 8 spiciness**: An attack on this is very good
 - **32 rounds + 8 spiciness**: An attack on this means you broke the recommended parameters, I didn't think this would be possible! Brilliant job!
 
 ## Cryptoanalysis ideas / suggestions
 ### Constants
 The rotation constants in this cipher have an enormous impact on the quality of the avalanche effect. Even slight tweaks to these parameters often result in significantly worse arrangements (e.g. if you set the first to 7, nothing on the second, 11 on the third and 13 on the fourth it will take at least 9 rounds to achieve statistical equilibrium). Is there any way you can exploit the existing parameters? Close rotations (e.g. (A << 11) + (B << 13)) can often be reduced to simpler cases (M) + (N - M) << 2).
 
 ### Differential cryptoanalysis
 Because there are so few operations in the cipher and addition is only mod 32-bits, there is a fair chance differential cryptoanalysis could work excellently for a large number of rounds. Also interesting - if you consider the difference function as addition instead of XOR, does this improve the situation here?
 
 ### Rotational Cryptoanalysis
 Rotational cryptoanalysis becomes obselete for this cipher at around 30/31 rounds if you implement it as it is currently displayed, but is there any way you can implement the round function in less than 4 additions? If you can manage to do this (at least in some cases) you may be able to extend rotational cryptoanalysis far enough to work on the full 32 rounds.
 
 ## Expectations
 - 5 rounds is probably a trivial exercise in differential cryptoanalysis and I expect a very fast attack on this, I deem this very easy.
 - 8 rounds is probably quite challenging but can be unquestionably broken (e.g. brute-force time reduced to `2**64` which would be stunning victory)
 - 12 rounds is probably where effort would outweigh motivation and I will be unlikely to receive anyone at or above this round count
 - 24 rounds is probably strong enough to hold to at least `2**80` or so
 - 32 rounds is probably strong enough to hold to at least `2**100` or so
