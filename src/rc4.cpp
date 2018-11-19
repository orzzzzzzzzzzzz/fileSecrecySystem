/*#include "./headers/rc4.h"
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
*/
#include "./headers/rc4.h"
#include<iostream>
using namespace std;
RC4::RC4(){
   this->i=this->j=0;
}

RC4::RC4(std::string key) {
    if(key.length() >N){
        std::cerr<<"Key's Length must less or equal to "<<N;
        exit(-1);
    }
    this->i=this->j=0;
    this->key = key;
}

void RC4::KSA(){
    if(this->key.empty() || this->key.length() >  N){
        exit(-1);
    }
    for (int k = 0; k < N ;++k)
        this->s[k] =(unsigned char) k;

    this->i=this->j=0;
    for (;i < N; ++i){
        j = (j+s[i]+key[i%key.length()])%N;
        swap(i,j);
    }
    this->i=this->j=0;
}

void RC4::swap(unsigned int a, unsigned int b) {
    unsigned char tmp = this->s[a];
    this->s[a] = this->s[b];
    this->s[b] = tmp;
}

unsigned char RC4::GenerateNextKeyByte() {
    this->i = (this->i+1)%N;
    this->j = (this->j + this->s[i])%N;
    swap(i,j);
    return this->s[(this->s[i]+this->s[j])%N];
}
unsigned char RC4::xorByte(unsigned char b, unsigned char c) {
    return b^c;
}

unsigned char RC4::encrypt_decrypt(unsigned char p){
    return xorByte(p,this->GenerateNextKeyByte());
}



std::string RC4::encrypt_decrypt(std::string plainText) {
    std::string cipherText = "";
    for (int k = 0; k < plainText.length(); ++k) {
        cipherText +=encrypt_decrypt((unsigned char )plainText[k]);
    }
    return cipherText;
}
void RC4::resetRC4(std::string newKey){
    this->key = "";
    this->i = this->j=0;
    resetKey(newKey);
}

void RC4::resetKey(std::string newKey) {
    this->key = newKey;
}
