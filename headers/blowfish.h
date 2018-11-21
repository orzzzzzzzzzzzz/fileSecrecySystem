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

#ifndef QBLOWFISH_H
#define QBLOWFISH_H

#include <qmath.h>
#include "hexpi.h"
#include <QDebug>

class QBlowfish
{
public:
    QBlowfish(QString key = "ABCD");

    void initBoxes();
    void calcSubKey(QString keyStr);
    QByteArray encrypt(QByteArray data);
    QByteArray decrypt(QByteArray cryptedData);


private:
    void _decrypt(quint32 &L, quint32 &R);
    void _encyrpt(quint32 &L, quint32 &R);
    quint32 f(quint32 x);
    quint32 get32Batch(QByteArray data, uint startVal);
    quint32 get32FromChars(QByteArray data, uint startVal);
    QByteArray convertToChar(quint32 L, quint32 R);

    HexPi hexPi;

    quint32 P[18];
    quint32 S[4][256];

};

#endif // QBLOWFISH_H
