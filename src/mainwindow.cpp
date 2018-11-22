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

void MainWindow::rsa(char* pstr){
          RSA rsa;
          rsa.TestRSA(pstr);
}

QString MainWindow::removeAccents(const QString s)
{
    QString diacriticLetters;
    QStringList noDiacriticLetters;
    if (diacriticLetters.isEmpty()) {
            diacriticLetters = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿẑẐŶŷŜŝĜĝĤĥĴĵŴŵĈĉ");
            /*append this letters for inter-OS compatibility*/
            diacriticLetters.append("éèçàù");
            noDiacriticLetters << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y"<<"z"<<"Z"<<"Y"<<"y"<<"S"<<"s"<<"G"<<"g"<<"H"<<"h"<<"J"<<"j"<<"W"<<"w"<<"C"<<"c"<<"e"<<"e"<<"c"<<"a"<<"u";
        }

        QString output = "";
        for (int i = 0; i < s.length(); i++)
        {
            QChar c = s[i];
            int dIndex = diacriticLetters.indexOf(c);
            if (dIndex < 0)
            {
                output.append(c);
            }
            else
            {
                QString replacement = noDiacriticLetters[dIndex];
                output.append(replacement);
            }
        }
        return output;
 }

QString MainWindow::textCleaner(const QString TextToClean)
{
    QString CleanedText(TextToClean);
    //delete spaces at the beginning and at the end
    CleanedText=TextToClean.trimmed();

    //remove accents
    CleanedText=removeAccents(CleanedText);

    //remove spaces
    CleanedText.replace(" ","");
    //remove tabs
    CleanedText.replace('\t',"");
    //remove returns
    CleanedText.replace('\n',"");

    //remove simple and double quotes
    CleanedText.replace('\"',"");
    CleanedText.replace('\'',"");

    //remove brackets
    CleanedText.replace('(',"");
    CleanedText.replace(')',"");

    //to upper case
    CleanedText=CleanedText.toUpper();

    return CleanedText;
}

void MainWindow::on_encryptBtn_clicked(){
    QString qstr=ui->inputEdit->toPlainText();
    QString pstr=ui->keyEdit->text();
    string key=pstr.toStdString();
    char * oData;
    QByteArray da=qstr.toLatin1();
    oData=da.data();
    if(ui->encryptionMethod->currentText() == "RC4"){       
       rc4 rc(qstr.toStdString(),pstr.toStdString(),"");
        QString input = textCleaner(qstr);
        string input_str = input.toStdString();
        QString input_qhex=QString::fromStdString(rc.string_to_hex(input_str));
        for(int i=0;i<input_qhex.length();i++){
            static const char* const lut="0123456789ABCDEF";
            string input_test=input_qhex.toStdString();
            char a = input_test[i];
            const char* p = std::lower_bound(lut, lut + 16, a);
        }
        rc.cipher(input_str,key,false);
        string output=rc.getEncoded();
        QByteArray data=QByteArray::fromStdString(output);
        ui->outputEdit->setPlainText(data.toHex());
    }else if(ui->encryptionMethod->currentText() == "RSA"){
        rsa(oData);
    }else if(ui->encryptionMethod->currentText() == "Blowfish"){
 /*      blowfish.calcSubKey(pstr);
        QByteArray BfEncyptedData = blowfish.encrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(BfEncyptedData.toBase64());*/
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
        QByteArray da=qstr.toLatin1();
        QString in=QString::fromLatin1(da);
        QString input=textCleaner(in);
        rc4 rc("","","");
        string input_str=input.toStdString();
        input=QString::fromStdString(input_str);
        rc.decipher(input_str,key);
        string output=rc.getDecoded();
        QByteArray data=QByteArray::fromStdString(output);
        QString out=QString::fromLocal8Bit(data).toLower();
        ui->outputEdit->setPlainText(out);
    }else if(ui->encryptionMethod->currentText() == "Blowfish"){
 /*       blowfish.calcSubKey(pstr);
        QByteArray BfEncyptedData = blowfish.decrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(BfEncyptedData.toBase64());*/
    }else if(ui->encryptionMethod->currentText() == "XOR"){
        xorCipher.setKey(pstr);
        QByteArray XorEncyptedData = xorCipher.decrypt(QByteArray(qstr.toUtf8()));
        ui->outputEdit->clear();
        ui->outputEdit->setPlainText(XorEncyptedData);
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
    }
    ui->statusBar->showMessage("Data decrypted!");
}
