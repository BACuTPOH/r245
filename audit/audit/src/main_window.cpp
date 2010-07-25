#include "main_window.h"
#include "settings_window.h"
//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
    SettingsWindow * sw = new SettingsWindow(this);// надо удалить потом объект
    connect(settings_button, SIGNAL(clicked()), sw, SLOT(show()));
}
//
