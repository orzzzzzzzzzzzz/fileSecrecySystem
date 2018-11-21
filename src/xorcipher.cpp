#include "./headers/xorcipher.h"



QXORCipher::QXORCipher(QString key)
{
    setKey(key);
}

void QXORCipher::setKey(QByteArray key)
{
    if(key.isEmpty())
    {
        qDebug() << "failed to set new key. Key can't be empty";
        return;
    }

    this->key = key;
}

void QXORCipher::setKey(QString key)
{
    this->key = key.toUtf8();
}

QByteArray QXORCipher::encrypt(QByteArray data)
{
    QByteArray result;

    int keyIndex = 0;
    for(int i = 0; i<data.length(); i++)
    {
        result.append( data[i]^key[keyIndex++] );
        if(keyIndex == key.length()) keyIndex = 0;
    }

    return result;
}

QByteArray QXORCipher::decrypt(QByteArray data)
{
    //Rember that decryption is the same as encryption regarding
    //XOR cipher. So we can just reuse encrypt method and have a
    //decrypt method just so it is easier to read.

    return encrypt(data);
}
