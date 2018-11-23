// aes.cpp

#include "./headers/aes.h"
#include "aes_lookup_tables.cpp"


void AES::InputData(string textReceived, string inputkey)
{
    int i;
    for (i = inputkey.size(); i < 16; i++) {
        inputkey += '\0';
    }
    for (i = textReceived.size() % 16; i < 16; i++) {
        textReceived += '\0';
    }
    text = textReceived;
    memcpy(key, inputkey.c_str(), 16);

}

string AES::Encrypt()
{

    int i;
    int j;
    string result;
//    text = plainText;
//    memcpy(key, inputKey.c_str(), 16);
    for (j = 0; j < (int)text.size()/16; j++) {
        // copy 16 bytes into state
        memcpy(state, text.c_str() + 16*j, 16);

        // algorithm
        GenKeys();

        AddRoundKey( key[ 0 ] );

        for ( i = 0; i < 10; i++ ) {
            SubBytes();
            ShiftRows();
            if ( i != 9 ) {
                MixColumns();
            }
            AddRoundKey( key[ i + 1 ] );
        }

        // receive from state here!!!
        result += GetResult();
    }

    return result;
}


string AES::Decrypt()
{
    unsigned int i;
    unsigned int j;
    string result;
//    text = ciphertext;
//    memcpy(key, inputKey.c_str(), 16);
    for (j = 0; j < text.size()/32; j++) {
        // copy to state
        for (i = 0; i < 16; i++) {
            // copy 16 bytes into state
            // --- left char ---
            // if letter
            if ( text.c_str()[ i*2 + j*16*2 + 1 ] <= 'F' && text.c_str()[ i*2 + j*16*2 + 1 ] >= 'A' ) {
                state[ i ] = text.c_str()[ i*2 + j*16*2 + 1 ] - 'A' + 10;
            }
            // if number
            else if ( text.c_str()[ i*2 + j*16*2 + 1 ] <= '9' && text.c_str()[ i*2 + j*16*2 + 1 ] >= '0' ) {
                state[ i ] = text.c_str()[ i*2 + j*16*2 + 1 ] - '0';
            }

            // --- right char ---
            // if letter
            if ( text.c_str()[ i*2 + j*16*2 ] <= 'F' && text.c_str()[ i*2 + j*16*2 ] >= 'A' ) {
                state[ i ] += 16 * (text.c_str()[ i*2 + j*16*2 ] - 'A' + 10);
            }
            // if number
            else if ( text.c_str()[ i*2 + j*16*2 ] <= '9' && text.c_str()[ i*2 + j*16*2 ] >= '0' ) {
                state[ i ] += 16 * (text.c_str()[ i*2 + j*16*2 ] - '0');
            }
        }

        // algorithm
        GenKeys();

        for (i = 10; i > 0; i--)
        {
            AddRoundKey( key[ i ] );
            if( i != 10 ) {
                MixColumnsInverse();
            }
            ShiftRowsInverse();
            SubBytesInverse();
        }

        AddRoundKey( key[ i ] );

        // receive from state here!!!
        result += GetResultText();
    }

    // find first 0x00
    for ( i = 0; i < result.size(); i++ ) {
        if ( result.c_str()[ i ] == '\0' ) {
            break;
        }
    }

    // delete 0x00's at the end
    result.erase(result.size() - (j*16 - i));

    return result;
}


string AES::GetResult()
{
    char converted[100];
    int i;

    for( i = 0; i < 16; i++) {
        sprintf(&converted[ i * 2 ], "%02X", state[ i ]);
    }
    converted[ i * 2 ] = '\0';

    string result(converted);
    return result;
}


string AES::GetResultText()
{
    string result(state, state + 16);
    return result;
}


void AES::SubBytes()
{
    unsigned char x, y;
    int i;
    for ( i = 0; i < 16; i++ ) {
        x = state[ i ] >> 4;
        y = state[ i ] & 0x0F;
        state[ i ] = sbox[ y + x * 16 ];
    }
}


void AES::SubBytesInverse()
{
    unsigned char x, y;
    int i;
    for ( i = 0; i < 16; i++ ) {
        x = state[ i ] >> 4;
        y = state[ i ] & 0x0F;
        state[ i ] = sboxinverse[ y + x * 16 ];
    }
}


void AES::ShiftRows()
{
    unsigned char temp;

    temp = state[ 4 ];
    state[ 4 ] = state[ 5 ];
    state[ 5 ] = state[ 6 ];
    state[ 6 ] = state[ 7 ];
    state[ 7 ] = temp;


    temp = state[ 8 ];
    state[ 8 ] = state[ 10 ];
    state[ 10 ] = temp;
    temp = state[ 9 ];
    state[ 9 ] = state[ 11 ];
    state[ 11 ] = temp;

    temp = state[ 12 ];
    state[ 12 ] = state[ 15 ];
    state[ 15 ] = state[ 14 ];
    state[ 14 ] = state[ 13 ];
    state[ 13 ] = temp;
}


void AES::ShiftRowsInverse()
{
    unsigned char temp;


    temp = state[ 7 ];
    state[ 7 ] = state[ 6 ];
    state[ 6 ] = state[ 5 ];
    state[ 5 ] = state[ 4 ];
    state[ 4 ] = temp;

    temp = state[ 8 ];
    state[ 8 ] = state[ 10 ];
    state[ 10 ] = temp;
    temp = state[ 9 ];
    state[ 9 ] = state[ 11 ];
    state[ 11 ] = temp;


    temp = state[ 12 ];
    state[ 12 ] = state[ 13 ];
    state[ 13 ] = state[ 14 ];
    state[ 14 ] = state[ 15 ];
    state[ 15 ] = temp;
}


void AES::MixColumns()
{
    int j;
    unsigned char temp[ 16 ];
    for ( j = 0; j < 4; j++ ) { // j is COLUMN
        temp[0 + j] =
                table_2[state[0 + j]] ^
                table_3[state[4 + j]] ^
                state[8 + j] ^
                state[12 + j];
        temp[4 + j] =
                state[0 + j] ^
                table_2[state[4 + j]] ^
                table_3[state[8 + j]] ^
                state[12 + j];
        temp[8 + j] =
                state[0 + j] ^
                state[4 + j] ^
                table_2[state[8 + j]] ^
                table_3[state[12 + j]];
        temp[12 + j] =
                table_3[state[0 + j]] ^
                state[4 + j] ^
                state[8 + j] ^
                table_2[state[12 + j]];
    }
    memcpy(state, temp, 16);
}


void AES::MixColumnsInverse()
{
    int j;
    unsigned char temp[ 16 ];
    for ( j = 0; j < 4; j++ ) { // j is COLUMN
        temp[0 + j] =
                table_14[state[0 + j]] ^
                table_11[state[4 + j]] ^
                table_13[state[8 + j]] ^
                table_9[state[12 + j]];
        temp[4 + j] =
                table_9[state[0 + j]] ^
                table_14[state[4 + j]] ^
                table_11[state[8 + j]] ^
                table_13[state[12 + j]];
        temp[8 + j] =
                table_13[state[0 + j]] ^
                table_9[state[4 + j]] ^
                table_14[state[8 + j]] ^
                table_11[state[12 + j]];
        temp[12 + j] =
                table_11[state[0 + j]] ^
                table_13[state[4 + j]] ^
                table_9[state[8 + j]] ^
                table_14[state[12 + j]];
    }
    memcpy(state, temp, 16);
}

void AES::AddRoundKey( unsigned char * k )
{
    int i;
    for ( i = 0; i < 16; i++ ) {
        state[ i ] = state[ i ] ^ k[ i ];
    }
}


void AES::GenKeys()
{
    // column0
    unsigned char t;
    unsigned char temp[ 4 ];
    unsigned char x, y;
    int i, j;

    for ( j = 0; j < 10; j++ ) {
        // copy
        temp[ 0 ] = key[ j ][ 3 ];
        temp[ 1 ] = key[ j ][ 7 ];
        temp[ 2 ] = key[ j ][ 11 ];
        temp[ 3 ] = key[ j ][ 15 ];

        // rotword
        t = temp[ 0 ];
        temp[ 0 ] = temp[ 1 ];
        temp[ 1 ] = temp[ 2 ];
        temp[ 2 ] = temp[ 3 ];
        temp[ 3 ] = t;

        // subbytes
        for ( i = 0; i < 4; i++ ) {
            x = temp[ i ] >> 4;
            y = temp[ i ] & 0x0F;
            temp[ i ] = sbox[ y + x * 16 ];
        }

        // xor with 2 others
        temp[ 0 ] = temp[ 0 ] ^ key[ j ][ 0 ] ^ rcon[ j + 0 ];
        temp[ 1 ] = temp[ 1 ] ^ key[ j ][ 4 ];
        temp[ 2 ] = temp[ 2 ] ^ key[ j ][ 8 ];
        temp[ 3 ] = temp[ 3 ] ^ key[ j ][ 12 ];

        // write temp to column
        key[ j + 1 ][ 0 ] = temp[ 0 ];
        key[ j + 1 ][ 4 ] = temp[ 1 ];
        key[ j + 1 ][ 8 ] = temp[ 2 ];
        key[ j + 1 ][ 12 ] = temp[ 3 ];

        /*
         *  0  1  2  3
         *  4  5  6  7
         *  8  9 10 11
         * 12 13 14 15
         */
        // columns 1 to 3
        for ( i = 0; i < 3; i++ ) {
            key[ j + 1 ][ i + 1 ]  = key[ j ][ i + 0 + 1 ] ^ key[ j + 1 ][ i + 0 ];
            key[ j + 1 ][ i + 5 ]  = key[ j ][ i + 4 + 1 ] ^ key[ j + 1 ][ i + 4 ];
            key[ j + 1 ][ i + 9 ]  = key[ j ][ i + 8 + 1 ] ^ key[ j + 1 ][ i + 8 ];
            key[ j + 1 ][ i + 13 ] = key[ j ][ i + 12+ 1 ] ^ key[ j + 1 ][ i + 12 ];
        }
    }
}
