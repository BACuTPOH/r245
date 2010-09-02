#include "main_window.h"
#include "global.h"

MainWindow::MainWindow( QWidget * parent, Qt::WFlags f) 
        : QMainWindow(parent, f)
{
    setupUi(this);
    monitor_button->setIcon(QIcon("icon/monitor.png"));
    settings_button->setIcon(QIcon("icon/settings.png"));

    if(utils.loadLibrary("libr245dll.dll"))
    {

        set_obj = new SettingsObj();
        monitor = new Monitor();

        swindow = new SettingsWindow(set_obj, monitor, this);
        mwindow = new MonitorWindow(set_obj, monitor, this);

        connect(settings_button, SIGNAL(clicked()), swindow, SLOT(show()));
        connect(monitor_button, SIGNAL(clicked()), mwindow, SLOT(show()));
    } else
    {
        swindow = NULL;
        set_obj = NULL;
        monitor = NULL;
        mwindow = NULL;
    }

}

MainWindow::~MainWindow()
{
    delete swindow;
    delete mwindow;
    delete set_obj;
    delete monitor;

    utils.unloadLibrary();
}
