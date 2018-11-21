#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./headers/blowfish.h"
#include "./headers/xorcipher.h"
#include "./headers/hexpi.h"
#include <qmath.h>
//#include "./src/rsa.cpp"
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

//    RSA rsa(char* pstr);
//   void rsa(char*pstr);
     void playfair_en(std::string pstr,std::string key);
     void playfair_de(std::string pstr,std::string key);

     void on_actionsave_as_triggered();

     void on_encryptBtn_clicked();

     void on_decryptBtn_clicked();

private:
    Ui::MainWindow *ui;
    QBlowfish blowfish;
    QXORCipher xorCipher;
};

#endif // MAINWINDOW_H
