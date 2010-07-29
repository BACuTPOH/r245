#ifndef READ_TRANS_THREAD_H
#define READ_TRANS_THREAD_H

#include <QThread>

class ReadTransThread : public QThread
{
//    Q_OBJECT
public:
    ReadTransThread( QObject * parent = 0 );
    //~ReadTransThread();
    void run();
/*signals:
    void transReady();
private slots:*/
};

#endif // READ_TRANS_THREAD_H
