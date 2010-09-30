#ifndef __MONITOR_WINDOW_H__
#define __MONITOR_WINDOW_H__

#include <QDialog>
#include <QTimer>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QFileDialog>
#include <QProgressDialog>
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
    
    /*class TransThread : QThread
    {
    public:
        TransThread(MonitorWindow * mon_win, SettingsObj * set);

        void run();

    private:
        SettingsObj * set_obj;
        MonitorWindow * monitor_win;
        Monitor * monitor;
    };*/

public:
    MonitorWindow(SettingsObj * set, Monitor * mon, QWidget *parent = 0);
    ~MonitorWindow();

    Monitor * getMonitor();
    void eventHandler(QString dev_num, R245_TRANSACT * trans);
private:
    QTimer timer;
    SettingsObj * set_obj;
    Monitor * monitor;
    //TransThread * trans_thread;

    void initMas();
    void printMonitor(QPrinter * printer);
private slots:
    void slotUpdateTrans();
    void slotResetFilter();
    void slotTabChanged();
    void slotTagInform();
    void slotPrintClick();
    void slotSaveFile();
    void slotClearMonitor();
};

#endif // __MONITOR_WINDOW_H__
