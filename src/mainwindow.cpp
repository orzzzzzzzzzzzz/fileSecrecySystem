#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "./headers/rc4.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
//#include "./headers/rsa.h"
#include <string>
#include <iostream>
#include "./src/playfair.cpp"
#include "./src/casear.cpp"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionopen_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("open file"),"",tr("text(*.txt)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly|QFile::Text)){
            QMessageBox::warning(this,tr("Error"),tr("open file error").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        this->ui->inputEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
    }else{
        qDebug()<<"取消";
    }
}

void MainWindow::on_actionsave_as_triggered()
{
    QString fileName;

    fileName = QFileDialog::getSaveFileName(this,tr("save file"),".txt",tr("text(*.txt)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if(!file.open(QFile::ReadWrite|QFile::Text)){
            QMessageBox::warning(this,tr("Error"),tr("open file error").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        in<<this->ui->outputEdit->toPlainText();
    }else{
        qDebug()<<"取消";
    }
}

//RSA MainWindow::rsa(char* pstr){
//    RSA rsa;
//    rsa.TestRSA(pstr);
//    return rsa;
//}

//void MainWindow::rsa(char* pstr){
//          RSA rsa;
//          rsa.TestRSA(pstr);
//}

void MainWindow::playfair_en(string pstr,string key){
        string output=encode(pstr,key);
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(QString::fromStdString(output));
}
void MainWindow::playfair_de(string pstr,string key){
    string output=decode(pstr,key);
    ui->outputEdit->clear();
    ui->outputEdit->setPlainText(QString::fromStdString(output));
}

void MainWindow::on_encryptBtn_clicked(){
    QString qstr=ui->inputEdit->toPlainText();
    QString pstr=ui->keyEdit->text();
    string key=pstr.toStdString();
    char * oData;
    QByteArray da=qstr.toLatin1();
    oData=da.data();
    if(ui->encryptionMethod->currentText() == "RC4"){
        RC4 rc4(key);
        rc4.KSA();
        for ( size_t i = 0; i < strlen(oData); i++ )
            oData[i] = rc4.encrypt_decrypt((unsigned char)oData[i]);
        QString str1=QString(QLatin1String(oData));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(str1);
    }else if(ui->encryptionMethod->currentText() == "RSA"){
//        RSA rsa;
//        char *cstr;
//        QByteArray ba=qstr.toLatin1();
//        cstr=ba.data();
//        rsa.TestRSA(cstr);
//        unsigned __int64 * st=rsa.getencode();
//        ui->outputEdit->clear();
//        ui->outputEdit->setPlainText(mess);
//        rsa(oData);
    }else if(ui->encryptionMethod->currentText() == "Blowfish"){
        blowfish.calcSubKey(pstr);
        QByteArray BfEncyptedData = blowfish.encrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(BfEncyptedData.toBase64());
    }else if(ui->encryptionMethod->currentText() == "XOR"){
        xorCipher.setKey(pstr);
        QByteArray XorEncyptedData = xorCipher.encrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(XorEncyptedData);
    }else if(ui->encryptionMethod->currentText() == "Playfair"){
        string plainmes=qstr.toStdString();
        playfair_en(plainmes,key);
    }else if(ui->encryptionMethod->currentText() == "Casear"){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,keylength);
        ui->outputEdit->setPlainText(out);
    }
    ui->statusBar->showMessage("Data encrypted!");
}

void MainWindow::on_decryptBtn_clicked(){
    QString qstr=ui->outputEdit->toPlainText();
    QString pstr=ui->keyEdit->text();
    qstr=qstr.simplified();
    string key=pstr.toStdString();
    char * oData;
    QByteArray da=qstr.toLatin1();
    oData=da.data();
    if(ui->encryptionMethod->currentText() == "RC4"){
            RC4 rc4(key);
            rc4.KSA();
            for ( size_t i = 0; i < strlen(oData); i++ )
                oData[i] = rc4.encrypt_decrypt((unsigned char)oData[i]);
            QString str1=QString(QLatin1String(oData));
            ui->outputEdit->clear();
            ui->outputEdit->setPlainText(str1);
    }else if(ui->encryptionMethod->currentText() == "Blowfish"){
        blowfish.calcSubKey(pstr);
        QByteArray BfEncyptedData = blowfish.decrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(BfEncyptedData.toBase64());
    }else if(ui->encryptionMethod->currentText() == "XOR"){
        xorCipher.setKey(pstr);
        QByteArray XorEncyptedData = xorCipher.decrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(XorEncyptedData);
    }else if(ui->encryptionMethod->currentText() == "Casear"){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,-keylength);
        ui->outputEdit->setPlainText(out);
    }else if(ui->encryptionMethod->currentText() == "Playfair"){
        string plainmes=qstr.toStdString();
        playfair_de(plainmes,key);
    }
    ui->statusBar->showMessage("Data decrypted!");
}
