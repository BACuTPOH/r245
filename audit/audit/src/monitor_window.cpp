#include "monitor_window.h"
#include "global.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QFileDialog>
#include <QProgressDialog>



MonitorWindow::MonitorWindow(SettingsObj * set, Monitor * mon, QWidget *parent):
    QDialog(parent)
{
    setupUi(this);

    set_obj = set;
    monitor = mon;

    monitor_view->setModel(monitor->getModel(true));
    monitor_view->hideColumn(Monitor::DevNumAttr);
    monitor_view->hideColumn(Monitor::TagIdAttr);
    monitor_view->hideColumn(Monitor::TransCodeAttr);

    connect(&timer, SIGNAL(timeout()), SLOT(slotUpdateTrans()));
    connect(resetFilterBtn, SIGNAL(clicked()), SLOT(slotResetFilter()));
    connect(mw_tabs, SIGNAL(currentChanged(int)), SLOT(slotTabChanged()));
    connect(tag_check, SIGNAL(stateChanged(int)), SLOT(slotTagInform()));
    connect(print_button, SIGNAL(clicked()), SLOT(slotPrintClick()));
    connect(save_file_button, SIGNAL(clicked()), SLOT(slotSaveFile()));

    tag_check->setChecked(true);

    timer.start(2000);
}

/*MyThread::MyThread(QPrinter * pr, QTextDocument * d)
{
    printer = pr;
    qdoc = d;
}
MyThread::~MyThread()
{
    qDebug("exit Thread");
}

void MyThread::run()
{
    qDebug("Run");
    qdoc->print(printer);
    exec();

}*/


void MonitorWindow::printMonitor(QPrinter * printer)
{
    QTextDocument qdoc;
    QString text;
    QAbstractItemModel * model = monitor->getModel(true);

    int count_row = model->rowCount();

    text = "<table bgcolor='#000000' cellpadding=3 cellspacing=2>";

    text += "<tr bgcolor='#e5e5e5'><td>Время</td><td>Дата</td><td>Имя устройства</td>";
    text += "<td>id устройства</td><td>Канал</td><td>Имя метки</td>";
    text += "<td>id метки</td><td>Тип события</td><td>Код событя</td></tr>";


    //QProgressDialog progress("Идет обработка событий монитора", "", 0, count_row);
    //progress.setWindowTitle("Пожалуйста подождите...");
    //progress.setMinimumDuration(0);

    for(int row = 0; row < count_row; row++)
    {
        //progress.setValue(row);
        //qApp->processEvents();

        text += "<tr>";
        for(int column = 0; column <= Monitor::TransCodeAttr; column++)
        {
            text += "<td bgcolor='#ffffff'>" + model->index(row, column).data().toString() + "</td>";
        }

        text += "</td>";
    }

    text += "</table>";

    qdoc.setHtml(text);

    //MyThread t(printer, &qdoc);
    //t.run();
    qdoc.print(printer);
}

void MonitorWindow::slotSaveFile()
{
    QString file_path = QFileDialog::getSaveFileName(0, "Сохранение отчета", "", "*.pdf");

    if(!file_path.isEmpty())
    {
        QPrinter printer;

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(file_path);

        printMonitor(&printer);
    }
}

void MonitorWindow::slotPrintClick()
{
    QPrinter printer;

    QPrintDialog dialog(&printer);

    if(dialog.exec() == QDialog::Accepted)
    {
        printMonitor(&printer);
    }

}

void MonitorWindow::slotTagInform()
{
    monitor->onlyTagInf(tag_check->isChecked());
}

void MonitorWindow::slotTabChanged()
{
    if(monitor_tab->isVisible())
    {
        qDebug("apply filter");

        monitor->setFilter(numChanEdt->text(), numDevEdt->text(), numTagEdt->text(), sinceDateSpn->date(),
                           toDateSpn->date(), sinceTimeSpn->time(), toTimeSpn->time());
    }
}

void MonitorWindow::slotResetFilter()
{
    qDebug("reset filter");
    numChanEdt->setText("");
    numDevEdt->setText("");
    numTagEdt->setText("");
    sinceDateSpn->setDate(QDate().fromString("01.09.2010", Qt::LocalDate));
    toDateSpn->setDate(QDate().currentDate());
    sinceTimeSpn->setTime(QTime().fromString("00:00:00"));
    toTimeSpn->setTime(QTime().fromString("23:59:59"));
}

void MonitorWindow::slotUpdateTrans()
{
    if(this->isActiveWindow())
    {
        R245_TRANSACT trans;
        short int status = 0;
        int dev_count = set_obj->getModel(SettingsObj::DevModel)->rowCount();


        // !!! Исправить цикл идет по всем устройствам, надо только по подключенным и активным
        for(int dev_num = 0; dev_num < dev_count; dev_num++)
        {
            while(!(status = utils.R245_GetTransact(dev_num, &trans)))
            {
                QString tag_name = "", dev_name = "";
                QAbstractItemModel * tag_model = set_obj->getModel(SettingsObj::TagModel);
                QAbstractItemModel * dev_name_model = set_obj->getModel(SettingsObj::DevNameModel);

                for(int i = 0; i < tag_model->rowCount(); ++i)
                {
                    if((unsigned long)tag_model->index(i, 0).data().toInt() == trans.tid)
                    {
                        tag_name = tag_model->index(i, 1).data().toString();
                        break;
                    }
                }

                for(int i = 0; i < dev_name_model->rowCount(); ++i)
                {
                    if(dev_name_model->index(i, 0).data().toInt() == dev_num)
                    {
                        dev_name = dev_name_model->index(i, 1).data().toString();
                        break;
                    }
                }
                monitor->addTransToModel(QString().setNum(dev_num), &trans, tag_name, dev_name);
                set_obj->addLogNode(QString().setNum(dev_num), &trans); // add node to log file
                eventHandler(QString().setNum(dev_num), &trans);
            }
        }
    }
}

void MonitorWindow::eventHandler(QString dev_num, R245_TRANSACT *trans)
{
    QStandardItemModel * event_model = (QStandardItemModel *)set_obj->getModel(SettingsObj::EventModel);

    int row_count = event_model->rowCount();
    QMap <int, QString> * state = monitor->getState();

    for(int row = 0; row < row_count; ++row)
    {
        QString event_name = event_model->data(event_model->index(row, SettingsObj::EvEvent)).toString();
        if(event_name == (*state)[trans->code])
        {
            QString id_dev = event_model->data(event_model->index(row, SettingsObj::EvIdDev)).toString();
            QString chanell = event_model->data(event_model->index(row, SettingsObj::EvChanell)).toString();
            QString id_tag = event_model->data(event_model->index(row, SettingsObj::EvIdTag)).toString();

            if(
                 (dev_num == id_dev) &&
                 (QString().setNum(trans->channel) == chanell) &&
                 (QString().setNum(trans->tid) == id_tag)
              )
            {
                QStandardItem * event_react = event_model->item(row, SettingsObj::EvReact);
                QString react = event_react->text();
                QStandardItemModel * monitor_model = (QStandardItemModel *) monitor->getModel(false);
                if(react == "выделить цветом")
                {
                    for(int i = 0; i < monitor_model->columnCount(); ++i)
                    {
                        monitor_model->item(row, i)->setBackground(event_react->background());
                    }
                } else if(react == "показать сообщение")
                {
                    QString msg = "<table><tr><td>Таг: </td><td>" + id_tag + "</td></tr>" +
                                  "<tr><td>Устройство: </td><td>" + id_dev + "</td></tr>" +
                                  "<tr><td>Канал: </td><td>" + chanell + "</td></tr></table>";


                    QMessageBox* pmbx = new QMessageBox(
                            QMessageBox::Information,
                            event_name,
                            msg);
                    pmbx->exec();
                    delete pmbx;
                }
            }
        }
    }
}

MonitorWindow::~MonitorWindow()
{
    utils.R245_CloseAllDev();
}
