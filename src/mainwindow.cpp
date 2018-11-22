#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "./headers/rc4.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
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

//void MainWindow::rsa(char* pstr){
//          RSA rsa;
//          rsa.TestRSA(pstr);
//}

void MainWindow::speedTest(){
    ui->executionTime->setText(QString::number(time.elapsed())+"ms");
}

void MainWindow::on_encryptBtn_clicked(){
    time.start();
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
        QStringList list=ui->inputEdit->toPlainText().split("\n");
        ui->outputEdit->clear();
        for(int i=0;i<list.size();i++){
        QString mes=list.at(i);
        string plainmes=mes.toStdString();
        string output=encode(plainmes,key);
        ui->outputEdit->appendPlainText(QString::fromStdString(output));
        }
    }else if(ui->encryptionMethod->currentText() == "Casear"){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,keylength);
        ui->outputEdit->setPlainText(out);
    }else if(ui->encryptionMethod->currentText() == "AES"){
        int returnCode = aes.InputForEncrypt(qstr.toStdString(),pstr.toStdString());
        if (returnCode == 1) {
            ui->statusBar->showMessage(tr("没有输入明文"));
        }else if (returnCode == 2) {
            ui->statusBar->showMessage(tr("秘钥最多１６位"));
        }else if (returnCode == 3) {
            ui->statusBar->showMessage(tr("秘钥为空"));
        }else {
            ui->statusBar->clearMessage();
            string str = aes.Encrypt();
            ui->outputEdit->clear();
            ui->outputEdit->setPlainText(QString::fromUtf8(str.data(),str.size()));
        }
    }
    ui->statusBar->showMessage("Data encrypted!");
    speedTest();
}

void MainWindow::on_decryptBtn_clicked(){
    time.start();
    QString istr=ui->inputEdit->toPlainText();
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
        QByteArray BfDecyptedData = blowfish.decrypt(QByteArray::fromBase64(QByteArray(qstr.toUtf8())));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(BfDecyptedData);
    }else if(ui->encryptionMethod->currentText() == "XOR"){
        xorCipher.setKey(pstr);
        QByteArray XorDecyptedData = xorCipher.decrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(XorDecyptedData);
    }else if(ui->encryptionMethod->currentText() == "Casear"){
        int keylength=pstr.toInt();
        QStringList list=ui->outputEdit->toPlainText().split("\n");
        ui->outputEdit->clear();
        for(int i=0;i<list.size();i++){
            QString mes=list.at(i);
            char* data;
            QByteArray dat=mes.toLatin1();
            data=dat.data();
            char* out=encrypt(data,-keylength);
            ui->outputEdit->appendPlainText(out);
        }
    }else if(ui->encryptionMethod->currentText() == "Playfair"){
        QStringList list=ui->outputEdit->toPlainText().split("\n");
        ui->outputEdit->clear();
        for(int i=0;i<list.size();i++){
            QString mes=list.at(i);
            string plainmes=mes.toStdString();
            string output=decode(plainmes,key);
            ui->outputEdit->appendPlainText(QString::fromStdString(output));
        }
    }else if(ui->encryptionMethod->currentText() == "AES"){
        int returnCode = aes.InputForEncrypt(qstr.toStdString(),pstr.toStdString());
        if (returnCode == 1) {
            ui->statusBar->showMessage(tr("没有输入明文"));
        }else if (returnCode == 2) {
            ui->statusBar->showMessage(tr("秘钥最多１６位"));
        }else if (returnCode == 3) {
            ui->statusBar->showMessage(tr("秘钥为空"));
        }else {
            ui->statusBar->clearMessage();
            string str = aes.Decrypt();
            ui->outputEdit->clear();
            ui->outputEdit->setPlainText(QString::fromUtf8(str.data(),str.size()));
        }
    }
    ui->statusBar->showMessage("Data decrypted!");
    speedTest();
}
