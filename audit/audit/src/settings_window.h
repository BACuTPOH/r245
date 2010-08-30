#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include <QDialog>
#include "ui_settings_window.h"
#include "settings_obj.h"

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT
    
public:
     SettingsWindow(SettingsObj * set, QWidget *parent = 0);

private:
     SettingsObj * set_obj;
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
};


#endif // __SETTINGS_WINDOW_H__
