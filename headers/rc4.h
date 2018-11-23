#ifndef CHATRC4_RC4_H
#define CHATRC4_RC4_H

#include <iostream>
class rc4 {
public:

    rc4(const std::string e, const std::string d, const std::string k) : encoded_message(e),decoded_message(d),keystream(k){}

    ~rc4(){}

    std::string getEncoded(){return encoded_message;}

    std::string getDecoded(){return decoded_message;}

    std::string getKeystream(){return keystream;}

    void setEncoded(const std::string e){encoded_message=e;}

    void setDecoded(const std::string d){decoded_message=d;}

    void setKeystream(const std::string k){keystream=k;}

    void cipher(const std::string plaintext, const std::string key, const bool isHex);

    void decipher(const std::string ciphered, const std::string key);

    void stream_generation(const std::string key, int* Stream);

    void keystream_generation(int* const stream, int* keystream);

    std::string string_to_hex(const std::string& input);

    std::string hex_to_string(const std::string& input);

private:

    std::string encoded_message;

    std::string decoded_message;

    std::string keystream;
};

#endif //CHATRC4_RC4_H
