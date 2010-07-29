#include "read_trans_thread.h"

ReadTransThread::ReadTransThread( QObject * parent){}


void ReadTransThread::run()
{
    /*quint8 ft_status = 0;
    quint16 num_trans = 0;
    quint16 dev_addr = 1;

    ft_status = utils.R245_GetNumTrans(ft_handle, dev_addr, &num_trans);
    if(!ft_status && num_trans > 0)
        emit transReady();*/
    while(true)
    {
        //emit transReady();
        //qDebug("in thread");
    }
    //exec();
}


