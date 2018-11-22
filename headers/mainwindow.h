#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <qmath.h>
#include "./headers/blowfish.h"
#include "./headers/xor.h"
#include "./headers/hexpi.h"
#include "./headers/aes.h"
//#include "./headers/rsa.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionopen_triggered();

//    void rsa(char*pstr);

    void on_actionsave_as_triggered();

    void on_encryptBtn_clicked();

    void on_decryptBtn_clicked();

    void speedTest();

private:
    Ui::MainWindow *ui;
    QBlowfish blowfish;
    XOR xorCipher;
    QTime time;
    AES aes;
};

#endif // MAINWINDOW_H
