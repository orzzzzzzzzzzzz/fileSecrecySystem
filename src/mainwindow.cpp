#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "./headers/rc4.h"
#include "rc4.h"
#include "rsa.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>


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

void MainWindow::on_pushButton_2_clicked()
{
    QString qstr=ui->textEdit->toPlainText();
        if(ui->RC4->isChecked()){
            string key="justfortest123";
            RC4 rc4;
            string cstr=string((const char*)qstr.toLocal8Bit());
            string data=string(rc4.doRC4(cstr,key));
            qDebug()<<QString::fromLocal8Bit(data.c_str());
            ui->textEdit->clear();
            ui->textEdit->setPlainText(QString::fromLocal8Bit(data.c_str()));

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
        this->ui->textEdit->setPlainText(in.readAll());
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
        in<<this->ui->textEdit->toPlainText();
    }else{
        qDebug()<<"取消";
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString qstr=ui->textEdit->toPlainText();
        if(ui->RC4->isChecked()){
            RC4 rc4;
            string key="justfortest123";
            string cstr=string((const char*)qstr.toLocal8Bit());
            string data=rc4.doRC4(cstr,key);
            ui->textEdit->clear();
            ui->textEdit->setPlainText(QString::fromLocal8Bit(data.c_str()));
        }
}
