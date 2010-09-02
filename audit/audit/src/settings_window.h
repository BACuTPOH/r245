#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include <QDialog>
#include "ui_settings_window.h"
#include "settings_obj.h"

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT
    
public:
     SettingsWindow(SettingsObj * set, Monitor * monitor, QWidget *parent = 0);

private:
     SettingsObj * set_obj;
     Monitor * monitor_obj;
     void openFile(QLineEdit * le, QString caption);
private slots:
    void slotOpenSettings();
    void slotOpenLog();
    void slotAdd();
    void slotDevActive();
    void slotDevClick(QModelIndex qmi);
    void slotReadDevInfo();
    void slotDist1(int value);
    void slotTime1(int value);
    void slotDist2(int value);
    void slotTime2(int value);
    void slotSaveSetings();
    void slotActChannel();
    void slotFindTag();
    void slotFindDevName();
    void slotSynchTime();
};


#endif // __SETTINGS_WINDOW_H__
