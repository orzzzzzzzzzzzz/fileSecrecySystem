#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./src/rsa.cpp"
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
    void on_encrypt_clicked();

    void on_actionopen_triggered();

    void on_actionsave_as_2_triggered();

    void on_decrypt_clicked();
    RSA rsa(char* pstr);
    void playfair_en(char* pstr,std::string key);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
