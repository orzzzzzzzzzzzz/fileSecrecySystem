#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"

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
    void on_pushButton_2_clicked();

    void on_radioButton_2_clicked(bool checked);

    void on_actionopen_triggered();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
};

#endif // MAINWINDOW_H
