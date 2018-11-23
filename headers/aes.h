// aes.h

#ifndef AES_H
#define AES_H

#include <cstring>
#include <iostream>
#include <stdio.h>
using namespace std;

class AES {
public:
    // public functions
    void InputData(string inputtext, string inputkey);
    string Encrypt();
    string Decrypt();

private:
    // private members
    unsigned char state[ 16 ];
    unsigned char key[ 11 ][ 16 ];
    std::string text;

    // private functions
    void SubBytes();
    void SubBytesInverse();
    void ShiftRows();
    void ShiftRowsInverse();
    void MixColumns();
    void MixColumnsInverse();
    void AddRoundKey( unsigned char *k );
    void GenKeys();
    std::string GetResult();
    std::string GetResultText();
};

#endif // AES_H
