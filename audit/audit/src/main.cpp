#include <QApplication>
#include "main_window.h"
#include "utils.h"

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	MainWindow win;
	win.show();
    /*Utils u;
    u.loadLibrary("libr245dll.dll"); 
    u.testFunc();
    u.unloadLibrary();   */
    
	//app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    //return 0;
	return app.exec();
}
