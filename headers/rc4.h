#include <iostream>
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

