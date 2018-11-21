#ifndef QXORCIPHER_H
#define QXORCIPHER_H


#include <QByteArray>
#include <QString>
#include <QDebug>

class QXORCipher
{
public:
    QXORCipher(QString key = "ABCD");

    void setKey(QString key);
    void setKey(QByteArray key);

    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray data);

private:
    QByteArray key;

};

#endif // QXORCIPHER_H
