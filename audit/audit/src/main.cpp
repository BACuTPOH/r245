#include <QApplication>
#include <QTextCodec>
#include "main_window.h"
#include "utils.h"
#include "global.h"

Utils utils;

int main(int argc, char ** argv)
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForCStrings(codec);

    QApplication app( argc, argv );
    MainWindow win;
    win.show();
    //utils.testFunc();
    //utils.unloadLibrary();
    
    //return 0;
    return app.exec();
}
