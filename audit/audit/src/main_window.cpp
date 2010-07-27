#include "main_window.h"
#include "monitor_window.h"
#include "settings_window.h"
//
MainWindow::MainWindow( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
    SettingsWindow * sw = new SettingsWindow(this);// надо удалить потом объект
    MonitorWindow  * mw = new MonitorWindow(this); // надо удалить потом объект
    monitor_button->setIcon(QIcon("icon/monitor.png"));
    settings_button->setIcon(QIcon("icon/settings.png"));
    connect(settings_button, SIGNAL(clicked()), sw, SLOT(show()));
    connect(monitor_button, SIGNAL(clicked()), mw, SLOT(show()));
}
//
