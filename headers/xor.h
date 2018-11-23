#ifndef XOR_H
#define XOR_H

#include <QByteArray>
#include <QString>
#include <QDebug>

class XOR{
public:
    XOR(QString key = "ABCD");
    void setKey(QString key);
    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray data);
private:
    QByteArray key;

};

#endif // XOR_H
