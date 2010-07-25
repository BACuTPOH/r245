#ifndef __SETTINGS_WINDOW_H__
#define __SETTINGS_WINDOW_H__

#include <QDialog>
#include "ui_settings_window.h"

class SettingsWindow : public QDialog, public Ui::SettingsWindow
{
    Q_OBJECT
    
public:
     SettingsWindow(QWidget *parent = 0);
private slots:
};


#endif // __SETTINGS_WINDOW_H__
