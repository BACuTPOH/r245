#ifndef __MONITOR_WINDOW_H__
#define __MONITOR_WINDOW_H__

#include <QDialog>
#include <QTimer>
#include <QStandardItemModel>
#include <QMessageBox>
#include "settings_obj.h"
#include "monitor.h"
#include "ui_monitor_window.h"

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
private slots:
    void slotUpdateTrans();
    void slotResetFilter();
    void slotTabChanged();
};

#endif // __MONITOR_WINDOW_H__
