#ifndef __MONITOR_WINDOW_H__
#define __MONITOR_WINDOW_H__

#include <QDialog>
#include <QTimer>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTextDocument>
#include "settings_obj.h"
#include "monitor.h"
#include "ui_monitor_window.h"



/*class MyThread: QThread
{
private:
    QTextDocument * qdoc;
    QPrinter * printer;

public:
    MyThread(QPrinter * pr, QTextDocument * d);
    ~MyThread();

    void run();

};*/


class MonitorWindow : public QDialog, public Ui::MonitorWindow
{
    Q_OBJECT
    
public:
    MonitorWindow(SettingsObj * set, Monitor * mon, QWidget *parent = 0);
    ~MonitorWindow();
private:
    QTimer timer;
    SettingsObj * set_obj;
    Monitor * monitor;

    void initMas();
    void eventHandler(QString dev_num, R245_TRANSACT * trans);
    void printMonitor(QPrinter * printer);
private slots:
    void slotUpdateTrans();
    void slotResetFilter();
    void slotTabChanged();
    void slotTagInform();
    void slotPrintClick();
    void slotSaveFile();
};

#endif // __MONITOR_WINDOW_H__
