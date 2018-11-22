#include "./headers/rc4.h"
#include <iostream>
#include <vector>
#include <algorithm>
void rc4::cipher(const std::string plaintext, const std::string key, const bool isHex)
{

        if(plaintext!="")
        {
            int stream_generated[255]={};
            int keystream[255]={};
            std::string keystream_str;
            std::string keystream_hex;
            std::string encoded_message;
            std::string plaintext_str;

            //stream and keystream generation
            stream_generation(key, stream_generated);
            keystream_generation(stream_generated, keystream);

            //int[] to std::string for the keystream

            for(int i=0;i<255;i++)
            {
                keystream_str+=keystream[i];
            }

            //keystream is stocked as an hexadecimal value
            keystream_hex = string_to_hex(keystream_str);
            setKeystream(keystream_hex);

            if(!isHex)
            {
                for(unsigned int i=0;i<plaintext.length();i++)
                {
                    encoded_message += plaintext[i] ^  keystream_str[i];
                }
            }
            else
            {
                plaintext_str = hex_to_string(plaintext);
                for(unsigned int i=0;i<plaintext_str.length();i++)
                {
                    encoded_message += plaintext_str[i] ^  keystream_str[i];
                }
            }
            setEncoded(encoded_message);
        }
}

void rc4::decipher(const std::string ciphered, const std::string key)
{
      if(ciphered!="")
        {
            int stream_generated[255]={};
            int keystream[255]={};
            std::string keystream_str;
            std::string keystream_hex;
            std::string decoded_message;
            std::string ciphered_str = hex_to_string(ciphered);

            //stream and keystream generation
            stream_generation(key, stream_generated);
            keystream_generation(stream_generated, keystream);

            //int[] to std::string for the keystream

            for(int i=0;i<255;i++)
            {
                keystream_str+=keystream[i];
            }
            keystream_hex = string_to_hex(keystream_str);

            for(unsigned int i=0;i<ciphered_str.length();i++)
            {
                decoded_message += ciphered_str[i] ^ keystream[i];
            }
            setDecoded(decoded_message);
        }
}

std::string rc4::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
    if(len!=0)
    {
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i)
        {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }
    }
    return output;
}

std::string rc4::hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
        if (len & 1)
        {
            output.clear();
        }
        else
        {
            output.reserve(len / 2);
            for (size_t i = 0; i < len; i += 2)
            {
                char a = input[i];
                const char* p = std::lower_bound(lut, lut + 16, a);
                char b = input[i + 1];
                const char* q = std::lower_bound(lut, lut + 16, b);
                output.push_back(((p - lut) << 4) | (q - lut));
            }
        }
    return output;
}

void rc4::stream_generation(const std::string key, int* Stream)
{
    int Swapper=0;
    int j=0;

    //Initialization of the Stream array
    for(int i=0;i<=254;i++)
    {
        Stream[i]=i;
    }
        if(key.length()!=0)
        {
            for(int i=0;i<=254;i++)
            {
                j = (j + Stream[i] + (key[i%key.length()])) % 255;
                Swapper = Stream[i];
                Stream[i] = Stream[j];
                Stream[j] = Swapper;
            }
        }
}

void rc4::keystream_generation(int* const stream, int* keystream)
{
    int i=0;
    int j=0;
    int Swapper=0;

    for(int k=0;k<255;k++)
    {
        i=(i+1)%255;
        j=(j+stream[i])%255;
        Swapper=stream[i];
        stream[i]=stream[j];
        stream[j]=Swapper;
        keystream[k]=stream[(stream[i]+stream[j])%255];
    }
}

