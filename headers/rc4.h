/*#include <iostream>
using namespace::std;

#ifndef _RC4_H
#define _RC4_H

class RC4
{
public:
    void ksa(string key, int keylength);
    string prga(string in, int len);
    string doRC4(string in, string key);

private:
    int S[256];
    char k[256];

};

#endif // _RC4_H
*/
#ifndef CHATRC4_RC4_H
#define CHATRC4_RC4_H

#include <iostream>
#define N 1024



class RC4 {
private:
    std::string key;
    unsigned char s[N];
    unsigned int i,j;
    void swap(unsigned int,unsigned int);
    unsigned char GenerateNextKeyByte();
    unsigned char xorByte(unsigned char, unsigned char);


public:
    RC4();
    RC4(std::string);
    void resetRC4(std::string newKey);
    void resetKey(std::string);
    void KSA();
    unsigned char encrypt_decrypt(unsigned char);
    std::string encrypt_decrypt(std::string);





};

#endif //CHATRC4_RC4_H
