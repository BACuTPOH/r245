#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include <QMainWindow>
#include "monitor_window.h"
#include "settings_window.h"
#include "settings_obj.h"
#include "monitor.h"
#include "ui_main_window.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
	MainWindow( QWidget * parent = 0, Qt::WFlags f = 0 );
        ~MainWindow();
private:
        SettingsWindow * swindow;
        MonitorWindow  * mwindow;
        SettingsObj    * set_obj;
        Monitor        * monitor;
private slots:
};
#endif




