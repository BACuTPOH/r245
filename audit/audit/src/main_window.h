#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QMainWindow>
#include "ui_main_window.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
	MainWindow( QWidget * parent = 0, Qt::WFlags f = 0 );
        ~MainWindow();
private slots:
};
#endif




