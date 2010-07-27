#ifndef __MONITOR_WINDOW_H__
#define __MONITOR_WINDOW_H__

#include <QDialog>
#include "ui_monitor_window.h"

class MonitorWindow : public QDialog, public Ui::MonitorWindow
{
    Q_OBJECT
    
public:
     MonitorWindow(QWidget *parent = 0);
private slots:
};

#endif // __MONITOR_WINDOW_H__
