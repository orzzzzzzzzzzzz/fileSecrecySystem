#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "./headers/blowfish.h"
#include "./headers/xor.h"
#include "./headers/hexpi.h"
#include <qmath.h>
#include "./headers/rsa.h"
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

    void rsa(char*pstr);

    void on_actionsave_as_triggered();

    void on_encryptBtn_clicked();

    void on_decryptBtn_clicked();

    QString textCleaner(const QString TextToClean);

    QString removeAccents(const QString s);

private:
    Ui::MainWindow *ui;
//    QBlowfish blowfish;
    XOR xorCipher;
};

#endif // MAINWINDOW_H
