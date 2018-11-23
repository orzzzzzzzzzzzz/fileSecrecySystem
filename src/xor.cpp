#include "./headers/xor.h"

XOR::XOR(QString key){
    setKey(key);
}

void XOR::setKey(QString key){
    this->key = key.toUtf8();
}

QByteArray XOR::encrypt(QByteArray data){
    QByteArray result;
     int keyIndex = 0;
    for(int i = 0; i<data.length(); i++){
        result.append(data[i]^key[keyIndex++]);
         if(keyIndex == key.length()) keyIndex = 0;
    }
    return result;
}

QByteArray XOR::decrypt(QByteArray data){
    return encrypt(data);
}
