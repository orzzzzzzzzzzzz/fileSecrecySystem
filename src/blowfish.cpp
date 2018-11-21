/*
Copyright (c) 2014 Raivis Strogonovs
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "./headers/blowfish.h"

QBlowfish::QBlowfish(QString key)
{
    calcSubKey(key);
}

QByteArray QBlowfish::encrypt(QByteArray data)
{
    QByteArray cryptedData;

    for(int i = 0; i < data.length(); i+=8)
    {
        quint32 L = get32Batch(data, i);
        quint32 R = get32Batch(data, i+4);
        _encyrpt(L, R);
        cryptedData.append( convertToChar(L, R) );
    }

    return cryptedData;
}

QByteArray QBlowfish::decrypt(QByteArray cryptedData)
{
    QByteArray data;
    for(int i = 0; i < cryptedData.length(); i+=8)
    {
        quint32 L = get32Batch(cryptedData, i);
        quint32 R = get32Batch(cryptedData, i+4);
        _decrypt(L, R);
        data.append( convertToChar(L, R) );
    }


    return data;
}



quint32 QBlowfish::get32Batch(QByteArray data, uint startVal)
{
    quint32 result = 0;
    for(int i=startVal; i<startVal+4; i++)
    {
        result <<= 8;
        if(i < data.length())
            result |= data[i]&0xFF;
    }

    return result;
}


QByteArray QBlowfish::convertToChar(quint32 L, quint32 R)
{
    QByteArray result;

    result.append( char( (L >> 24) & 0xFF ) );
    result.append( char( (L >> 16) & 0xFF)  );
    result.append( char( (L >> 8) & 0xFF) );
    result.append( char( L & 0xFF ) );

    result.append( char((R >> 24) & 0xFF)  ) ;
    result.append( char((R >> 16) & 0xFF ) );
    result.append( char((R >> 8) & 0xFF ) );
    result.append(char( R & 0xFF ) );

    return result;
}

void QBlowfish::calcSubKey(QString keyStr)
{
    if(keyStr.length() < 4)
    {
        qDebug() << "Key must be at least 32 bits long";
        return;
    }

    initBoxes();

    int keyLength = qCeil(keyStr.length()/4.0);

    quint32 key[keyLength];
    quint32 *ptr = &key[0];
    for(int i=0; i<keyStr.length(); i+=4)
    {
        quint32 tempKey = 0;
        tempKey |= (quint8(keyStr[i].unicode()) << 24);
        if(i+1 < keyStr.length()) tempKey |= (quint8(keyStr[i+1].unicode()) << 16);
        if(i+2 < keyStr.length()) tempKey |= (quint8(keyStr[i+2].unicode()) << 8);
        if(i+3 < keyStr.length()) tempKey |= quint8(keyStr[i+3].unicode());
        *ptr = tempKey;
        qDebug() << *ptr;
        ptr++;
    }


    for(int i=0; i<18; i++)
    {
        P[i] ^= key[i%keyLength];
    }
    quint32 L = 0, R = 0;
    for(int i=0; i<18; i+=2)
    {
        _encyrpt(L, R);
        P[i] = L;
        P[i+1] = R;
    }

    for(int i=0; i<4; i++)
        for(int j=0; j<256; j+=2)
        {
            _encyrpt(L, R);
            S[i][j] = L;
            S[i][j+1] = R;
        }
}

quint32 QBlowfish::f(quint32 x)
{
    quint32 h = S[0][x>>24] + S[1][x >> 16 & 0xff];
    return ( h ^ S[2][x >> 8 & 0xff]) + S[3][x & 0xff];
}

void QBlowfish::_encyrpt(quint32 &L, quint32 &R)
{
    for(int i=0; i<16; i+=2)
    {
        L ^= P[i];
        R ^= f(L);
        R ^= P[i+1];
        L ^= f(R);
    }

    L ^= P[16];
    R ^= P[17];

    qSwap( L, R );
}

void QBlowfish::_decrypt(quint32 &L, quint32 &R)
{
    for(int i=16; i>0; i-=2)
    {
        L ^= P[i+1];
        R ^= f(L);
        R ^= P[i];
        L ^= f(R);
    }

    L ^= P[1];
    R ^= P[0];

    qSwap( L, R);
}

void QBlowfish::initBoxes()
{
    //Initialize P boxes
    for(int i=0; i<18; i++)
        P[i] = hexPi.Pi[i];

    //Initialize S boxes
    int i = 18;
    for(int b=0; b<4; b++)
        for(int j=0; j<256; j++)
            S[b][j] = hexPi.Pi[i++];

}
