#include "rc4.h"
#include <string.h>

void RC4::ksa(string key, int keylength)
{
    for (int i = 0; i < 256; i++)
    {
        S[i] = i;
    }

    int j = 0, temp;

    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + key[i % keylength]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

string RC4::prga(string in, int len)
{
    int i = 0, j = 0, x = 0, temp;
    for (x = 0; x < len; x++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        k[x] = in[x] ^ S[(S[i] + S[j]) % 256];
    }

    return k;
}

string RC4::doRC4(string in, string key)
{
    int key_size = (int) key.size();
    int str_size = (int) in.size();

    // Create Key Stream
    ksa(key, key_size);
    // Encrypt or Decrypt input (plaintext)
    return prga(in, str_size);
}
