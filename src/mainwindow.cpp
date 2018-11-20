#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "./headers/rc4.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "./headers/rsa.h"
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


//RSA MainWindow::rsa(char* pstr){
//    RSA rsa;
//    rsa.TestRSA(pstr);
//    return rsa;
//}
/*void MainWindow::rsa(char* pstr){
          RSA rsa;
          rsa.TestRSA(pstr);
}*/

void MainWindow::playfair_en(string pstr,string key){
        string output=encode(pstr,key);
        ui->plainTextEdit->clear();
        ui->plainTextEdit->setPlainText(QString::fromStdString(output));
}
void MainWindow::playfair_de(string pstr,string key){
    string output=decode(pstr,key);
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(QString::fromStdString(output));
}

void MainWindow::on_encrypt_clicked()
{
    QString qstr=ui->plainTextEdit->toPlainText();
    QString pstr=ui->lineEdit->text();
    string key=pstr.toStdString();
    char * oData;
    QByteArray da=qstr.toLatin1();
    oData=da.data();
    if(ui->RC4->isChecked()){
        RC4 rc4(key);
        rc4.KSA();
        for ( size_t i = 0; i < strlen(oData); i++ )
            oData[i] = rc4.encrypt_decrypt((unsigned char)oData[i]);
        QString str1=QString(QLatin1String(oData));
        ui->plainTextEdit->clear();
        ui->plainTextEdit->setPlainText(str1);
        }
        else if(ui->RSA->isChecked()){
 /*            RSA rsa;
             char *cstr;
             QByteArray ba=qstr.toLatin1();
             cstr=ba.data();
             rsa.TestRSA(cstr);
             unsigned __int64 * st=rsa.getencode();
             ui->plainTextEdit->clear();
 //            ui->plainTextEdit->setPlainText(mess);*/
  //      rsa(oData);
        }
    if(ui->Playfair->isChecked()){
        string plainmes=qstr.toStdString();
        playfair_en(plainmes,key);
    }
    if(ui->Casear->isChecked()){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,keylength);
        ui->plainTextEdit->setPlainText(out);
    }
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
        this->ui->plainTextEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
    }else{
        qDebug()<<"取消";
    }
}

void MainWindow::on_actionsave_as_2_triggered()
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
        in<<this->ui->plainTextEdit->toPlainText();
    }else{
        qDebug()<<"取消";
    }
}


void MainWindow::on_decrypt_clicked()
{
    QString qstr=ui->plainTextEdit->toPlainText();
    QString pstr=ui->lineEdit->text();
    qstr=qstr.simplified();
    string key=pstr.toStdString();
    char * oData;
    QByteArray da=qstr.toLatin1();
    oData=da.data();
    if(ui->RC4->isChecked()){
            RC4 rc4(key);
            rc4.KSA();
            for ( size_t i = 0; i < strlen(oData); i++ )
                oData[i] = rc4.encrypt_decrypt((unsigned char)oData[i]);
            QString str1=QString(QLatin1String(oData));
            ui->plainTextEdit->clear();
            ui->plainTextEdit->setPlainText(str1);
    }
    if(ui->Casear->isChecked()){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,-keylength);
        ui->plainTextEdit->setPlainText(out);
    }
    if(ui->Playfair->isChecked()){
        string plainmes=qstr.toStdString();
        playfair_de(plainmes,key);

    }
}
