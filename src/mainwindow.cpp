#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "./headers/rc4.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "./src/rsa.cpp"
#include <string>
#include <iostream>
#include "./src/playfair.cpp"
#include "./src/casear.cpp"
#include "./src/permutation.cpp"
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

void MainWindow::playfair_en(char* pstr,string key){
        string input = string((char*)pstr);
        cout<<input;
        string output = "";
        string temp_str = "";
        for(int i = 0 ; i < input.length() ; i++)
        {
            if(input[i] != ' ')  temp_str +=  input[i];
        }
        input = temp_str;

        //trim whitespace of key
        temp_str = "";
        for(int i = 0 ; i < key.length() ; i++)
        {
            if(key[i] != ' ')  temp_str +=  key[i];
        }
        key = temp_str;
        if((input.length() % 2) == 1) input += "x";
        vector< vector<char> > matrix = cipher_matrix(key);
        string process_pair = "";
        //   print_matrix(matrix);
        while(true)
        {
            if(input.length() == 1)
            {
                process_pair = input + "x";
            }
            else
            {
                process_pair = input.substr(0,2);
                if(process_pair[0] == process_pair[1])
                {
                    input = process_pair.substr(1,1) + input;
                    process_pair = process_pair.substr(0,1) + "x";
                    //            cout << process_pair << "..." << endl;
                }
            }
            //      cout << process_pair << '\t';
            process_pair = cipher_decipher(process_pair, matrix);
            output +=  process_pair;
            //      cout << process_pair << '\t' << output << endl;
            if(input.length() > 2)
            {
                input = input.substr(2);

            }
            else break;
        }
        cout << output;

}

RSA MainWindow::rsa(char* pstr){
    RSA rsa;
    rsa.TestRSA(pstr);
    return rsa;
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
        /*string key="justfortest123";
            RC4 rc4;
            string cstr=string((const char*)qstr.toLocal8Bit());
            string data=string(rc4.doRC4(cstr,key));
            ui->textEdit->clear();
            ui->textEdit->setPlainText(QString::fromLocal8Bit(data.c_str()));*/
        ui->lineEdit_2->isReadOnly();
        //string key = "45A01F645FC35B383552544B9BF5DD756";
        RC4 rc4(key);
        rc4.KSA();
        for ( size_t i = 0; i < strlen(oData); i++ )
            oData[i] = rc4.encrypt_decrypt((unsigned char)oData[i]);
        QString str1=QString(QLatin1String(oData));
        //ui->plainTextEdit->clear();
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
        rsa(oData);
        }
    if(ui->Playfair->isChecked()){
        playfair_en(oData,key);
    }
    if(ui->Casear->isChecked()){
        int keylength=pstr.toInt();
        char* out=encrypt(oData,keylength);
        ui->plainTextEdit->setPlainText(out);
    }
    if(ui->Permutation->ischecked()){

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
           /* RC4 rc4;
            string key="justfortest123";
            string cstr=string((const char*)qstr.toLocal8Bit());
            string data=rc4.doRC4(cstr,key);
            ui->textEdit->clear();
            ui->textEdit->setPlainText(QString::fromLocal8Bit(data.c_str()));*/
        ui->lineEdit_2->isReadOnly();
            string key=pstr.toStdString();
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
}
