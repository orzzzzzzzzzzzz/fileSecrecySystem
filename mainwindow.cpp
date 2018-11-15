#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "rc4.h"
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
    ;
}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    if(checked){
        ;
    }
}

void MainWindow::on_actionopen_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("文件"),"",tr("text(*.txt)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly|QFile::Text)){
            QMessageBox::warning(this,tr("Error"),tr("read file error:&1").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
//        dialog = new Dialog(this);
//        dialog->setModal(false);
        this->ui->textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
//        dialog->show();
    }else{
        qDebug()<<"取消";
    }
}
