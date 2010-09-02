#include <QDebug>
#include <QTextStream>
#include "settings_obj.h"
#include "global.h"

SettingsObj::SettingsObj()
{

    fsettings = NULL;
    flog = NULL;
    log_stream = NULL;

    QStringList tag_header;
    tag_header << "id" << "имя";
    tag_model = new QStandardItemModel();
    tag_model->setColumnCount(tag_header.count());
    tag_model->setHorizontalHeaderLabels(tag_header);

    tag_model_proxy = new QSortFilterProxyModel();
    tag_model_proxy->setSourceModel(tag_model);
    tag_model_proxy->setFilterKeyColumn(-1); // filter all column
    tag_model_proxy->setFilterWildcard("*");

    dev_name_model = new QStandardItemModel();
    dev_name_model->setColumnCount(tag_header.count());
    dev_name_model->setHorizontalHeaderLabels(tag_header);

    dev_name_model_proxy = new QSortFilterProxyModel();
    dev_name_model_proxy->setSourceModel(dev_name_model);
    dev_name_model_proxy->setFilterKeyColumn(-1); // filter all column
    dev_name_model_proxy->setFilterWildcard("*");

    QStringList event_header;
    event_header << "id/имя устройства" << "имя" << "канал" << "время" << "id/имя метки" << "реакция";
    event_model = new QStandardItemModel();
    event_model->setColumnCount(event_header.count());
    event_model->setHorizontalHeaderLabels(event_header);

    dev_model = new QStandardItemModel();
}

void SettingsObj::setFilterWildCard(QString ex, TypeModel type_model)
{
    switch(type_model)
    {
        case TagModelProxy:
            tag_model_proxy->setFilterWildcard(ex);
            break;
        case DevNameModelProxy:
            dev_name_model_proxy->setFilterWildcard(ex);
            break;
        default:
            break;
    }
}

bool SettingsObj::openSettingFile(QString file_name)
{
    if(closeFile(fsettings))
    {
        delete fsettings;
        fsettings = NULL;
    }
    qDebug("Open Settings");
    fsettings = new QFile(file_name);

    QDomDocument dom_doc;

    if(openFile(fsettings, QIODevice::ReadOnly))
    {
        if(dom_doc.setContent(fsettings))
        {
            QDomElement dom_el = dom_doc.documentElement();
            readSettingNodes(dom_el);
        }
        closeFile(fsettings);
        return true;
    }

    return false;
}

bool SettingsObj::openLogFile(QString file_name, Monitor *monitor)
{
    if(closeFile(flog))
    {
        delete flog;
        flog = NULL;
    }
    qDebug("Open Log");

    TransParser tparser(monitor);
    flog = new QFile(file_name);
    QXmlInputSource source(flog);
    QXmlSimpleReader reader;

    reader.setContentHandler(&tparser);
    reader.parse(source);

    if(closeFile(flog))
    {
        qDebug("Close flog");
    }

    if(openFile(flog, QIODevice::Append))
    {
        qDebug("Open flog OK");
        flog->seek(flog->size() - QString("</log>\n").size());
        log_stream = new QTextStream(flog);
    }

    return true;
}

void SettingsObj::addLogNode(QString dev_num, R245_TRANSACT * trans)
{
    if(log_stream != NULL)
    {
        *log_stream << "    <transact>\n";
        *log_stream << "        <code>"    << trans->code    << "</code>\n";
        *log_stream << "        <channel>" << trans->channel << "</channel>\n";
        *log_stream << "        <tid>"     << trans->tid     << "</tid>\n";
        *log_stream << "        <day>"     << trans->day     << "</day>\n";
        *log_stream << "        <month>"   << trans->month   << "</month>\n";
        *log_stream << "        <year>"    << trans->year    << "</year>\n";
        *log_stream << "        <hour>"    << trans->hour    << "</hour>\n";
        *log_stream << "        <min>"     << trans->min     << "</min>\n";
        *log_stream << "        <sec>"     << trans->sec     << "</sec>\n";
        *log_stream << "        <dow>"     << trans->dow     << "</dow>\n";
        *log_stream << "        <dev_num>" << dev_num        << "</dev_num>\n";
        *log_stream << "    </transact>\n";
    }
}

void SettingsObj::readDevInfo()
{
    R245_DEV_INFO info;
    short int dev_ctr = 0;

    dev_model->clear();
    utils.R245_CloseAllDev();

    QStringList dev_header;
    dev_header << /*"номер" <<*/ "тип" << "id" << "locId" << "описание";
    dev_model->setColumnCount(dev_header.count());
    dev_model->setHorizontalHeaderLabels(dev_header);

    while(!utils.R245_GetDevInfo(dev_ctr, &info))
    {
        //QString num = info.serial_number;
        QString type = QString().setNum(info.type);
        QString id = QString().setNum(info.id);
        QString loc_id = QString().setNum(info.loc_id);
        QString desc = info.desc;

        addDevInfoToModel(/*num,*/ type, id, loc_id, desc);
        dev_ctr++;
    }
}

void SettingsObj::readSettingNodes(const QDomNode &node)
{
    QDomNode dom_node = node.firstChild();
    while(!dom_node.isNull())
    {
        if(dom_node.isElement())
        {
            QDomElement dom_el = dom_node.toElement();
            if(!dom_el.isNull())
            {
                if(dom_el.tagName() == "tag")
                {
                    QDomElement child_el = dom_node.firstChildElement();
                    addTagToModel(dom_el.attribute("id", ""), child_el.text());
                } else if(dom_el.tagName() == "dev_name")
                {
                    QDomElement child_el = dom_node.firstChildElement();
                    addDevNameToModel(dom_el.attribute("id", ""), child_el.text());
                } else if(dom_el.tagName() == "dev")
                {
                    DEV_INFO dev;

                    dev.id = dom_el.attribute("id", "").toInt();

                    QDomElement child_el = dom_node.firstChildElement();
                    while(!child_el.isNull())
                    {
                        if(child_el.tagName() == "channel")
                        {
                            dev.channel = child_el.text().toInt();
                        } else if(child_el.tagName() == "active")
                        {
                            dev.active = child_el.text().toInt();
                        } else if(child_el.tagName() == "dist1")
                        {
                            dev.dist1 = child_el.text().toInt();
                        } else if(child_el.tagName() == "time1")
                        {
                            dev.time1 = child_el.text().toInt();
                        } else if(child_el.tagName() == "dist2")
                        {
                            dev.dist2 = child_el.text().toInt();
                        } else if(child_el.tagName() == "time2")
                        {
                            dev.time2 = child_el.text().toInt();
                        }
                        child_el = child_el.nextSiblingElement();
                    }

                    if(getDevSettings(dev.id) == NULL)
                        dev_settings.append(dev);

                }
            }
        }
        readSettingNodes(dom_node);
        dom_node = dom_node.nextSibling();
    }
}

QAbstractItemModel * SettingsObj::getModel(TypeModel type_model)
{
    switch(type_model)
    {
        case TagModel:
            return tag_model;
        case DevNameModel:
            return dev_name_model;
        case DevModel:
            return dev_model;
        case EventModel:
            return event_model;
        case TagModelProxy:
            return tag_model_proxy;
        case DevNameModelProxy:
            return dev_name_model_proxy;
    }
    return NULL;
}

QDomElement SettingsObj::addTagToDom(QDomDocument dom_doc,
                                const QString &id,
                                const QString &name)
{
    QDomElement dom_element = makeElement(dom_doc, "tag", id, "");

    dom_element.appendChild(makeElement(dom_doc, "name", "", name));

    return dom_element;
}

QDomElement SettingsObj::addDevNameToDom(QDomDocument dom_doc, const QString &id, const QString &name)
{
    QDomElement dom_element = makeElement(dom_doc, "dev_name", id, "");

    dom_element.appendChild(makeElement(dom_doc, "name", "", name));

    return dom_element;
}

QDomElement SettingsObj::addDevToDom(QDomDocument dom_doc, const DEV_INFO &dev)
{
    QDomElement dom_element = makeElement(dom_doc, "dev", QString().setNum(dev.id), "");

    dom_element.appendChild(makeElement(dom_doc, "channel", "", QString().setNum(dev.channel)));
    dom_element.appendChild(makeElement(dom_doc, "active", "", QString().setNum(dev.active)));
    dom_element.appendChild(makeElement(dom_doc, "dist1", "", QString().setNum(dev.dist1)));
    dom_element.appendChild(makeElement(dom_doc, "time1", "", QString().setNum(dev.time1)));
    dom_element.appendChild(makeElement(dom_doc, "dist2", "", QString().setNum(dev.dist2)));
    dom_element.appendChild(makeElement(dom_doc, "time2", "", QString().setNum(dev.time2)));

    return dom_element;
}

bool SettingsObj::openFile(QFile * file, QFlags<QIODevice::OpenModeFlag> mode)
{
    if(file == NULL)
        return false;

    if(!file->open(mode))
    {
        qDebug() << "Error: open file";
        return false;
    }

    return true;
}

bool SettingsObj::closeFile(QFile *file)
{
    if(file != NULL)
    {
        if(file->isOpen())
        {
            file->close();
        }
        return true;
    }

    return false;
}

short int SettingsObj::setActiveDev(int row, bool active)
{
    QBrush color;
    qint8 ft_status = 1;
    DEV_INFO * dev = getDevSettings(dev_model->data(dev_model->index(row, Id)).toInt());

    if(dev != NULL)
    {
        if(active)
        {
            color = Qt::green;
            utils.R245_InitDev(row);
            ft_status = utils.R245_AuditEn(row, active);
        } else
        {
            color = Qt::white;
            ft_status = utils.R245_CloseDev(row);
        }

        if(!ft_status)
        {
            dev->active = active;
            qDebug("Active ok");
            for(int i = 0; i < dev_model->columnCount(); ++i)
            {
                dev_model->item(row, i)->setBackground(color);
            }
        } else
        {
            utils.R245_CloseDev(row);
        }
    }

    return ft_status;
}


void SettingsObj::setChannelDev(int row, short int channel)
{
    qint8 ft_status = 0;
    DEV_INFO * dev = getDevSettings(dev_model->data(dev_model->index(row, Id)).toInt());

    if(dev != NULL)
    {
        if(!dev->active)
            utils.R245_InitDev(row);

        if(channel & CHANNEL_ACT_1)
            ft_status = utils.R245_SetChan(row, 1, 1);
        else
            ft_status = utils.R245_SetChan(row, 1, 0);

        if(!ft_status)
        {
            if(channel & CHANNEL_ACT_2)
                ft_status = utils.R245_SetChan(row, 2, 1);
            else
                ft_status = utils.R245_SetChan(row, 2, 0);

            if(!ft_status)
            {
                dev->channel = channel;
            }
        }
        if(!dev->active)
            utils.R245_CloseDev(row);
    }
}

void SettingsObj::setDistDev(int row, short int dist, bool dist1)
{
    qint8 ft_status = 0;
    DEV_INFO * dev = getDevSettings(dev_model->data(dev_model->index(row, Id)).toInt());
    unsigned char channel = (dist1)? 1: 2;

    if(dev != NULL)
    {
        if(!dev->active)
            utils.R245_InitDev(row);
        ft_status = utils.R245_SetDamp(row, channel, dist);

        if(!ft_status)
        {
            if(dist1)
                dev->dist1 = dist;
            else
                dev->dist2 = dist;
        }
        if(!dev->active)
            utils.R245_CloseDev(row);
    } else
    {
        qDebug() << "link to dev is null";
    }
}

void SettingsObj::setTimeDev(int row, short int time, bool time1)
{
    qint8 ft_status = 0;
    DEV_INFO * dev = getDevSettings(dev_model->data(dev_model->index(row, Id)).toInt());
    unsigned char channel = (time1)? 1: 2;

    if(dev != NULL)
    {

        if(!dev->active)
            utils.R245_InitDev(row);

        ft_status = utils.R245_SetTime(row, channel, time);

        if(!ft_status)
        {
            if(time1)
                dev->time1 = time;
            else
                dev->time2 = time;
        }

        if(!dev->active)
            utils.R245_CloseDev(row);
    }
}

void SettingsObj::saveSetings()
{
    QString id, name;

    QDomDocument doc("settings");
    QDomElement root_el = doc.createElement("settings");

    doc.appendChild(root_el);

    QDomElement tag_dom = makeElement(doc, "tags", "", "");

    root_el.appendChild(tag_dom);

    for(int row = 0; row < tag_model->rowCount(); ++row)
    {
        id = tag_model->data(tag_model->index(row, 0)).toString();
        name = tag_model->data(tag_model->index(row, 1)).toString();

        tag_dom.appendChild(addTagToDom(doc, id, name));
    }

    QDomElement dev_name_dom = makeElement(doc, "dev_names", "", "");

    root_el.appendChild(dev_name_dom);

    for(int row = 0; row < dev_name_model->rowCount(); ++row)
    {
        id = dev_name_model->data(dev_name_model->index(row, 0)).toString();
        name = dev_name_model->data(dev_name_model->index(row, 1)).toString();

        dev_name_dom.appendChild(addDevNameToDom(doc, id, name));
    }

    QDomElement dev_dom = makeElement(doc, "devices", "", "");

    root_el.appendChild(dev_dom);

    QList<DEV_INFO>::iterator it = dev_settings.begin();

    for(; it != dev_settings.end(); ++it)
    {
        dev_dom.appendChild(addDevToDom(doc, *it));
        //qDebug() << "DEVICE";
    }

    if(openFile(fsettings, QIODevice::WriteOnly))
    {
        QTextStream(fsettings) << doc.toString();
        closeFile(fsettings);
    }

}

QDomElement SettingsObj::makeElement(QDomDocument &dom_doc,
                                     const QString &name,
                                     const QString &attr,
                                     const QString &text)
{
    QDomElement dom_element = dom_doc.createElement(name);

    if(!attr.isEmpty())
    {
        QDomAttr dom_attr = dom_doc.createAttribute("id");
        dom_attr.setValue(attr);
        dom_element.setAttributeNode(dom_attr);
    }

    if(!text.isEmpty())
    {
        QDomText dom_text = dom_doc.createTextNode(text);
        dom_element.appendChild(dom_text);
    }

    return dom_element;
}

void SettingsObj::addDevNameToModel(QString id, QString name)
{
    int row = 0/*dev_name_model->rowCount()*/;
    dev_name_model->insertRow(row);
    dev_name_model->setItem(row, 0, new QStandardItem(id));
    dev_name_model->setItem(row, 1, new QStandardItem(name));
}

void SettingsObj::addTagToModel(QString id, QString name)
{
    int row = 0/*tag_model->rowCount()*/;
    tag_model->insertRow(row);
    tag_model->setItem(row, 0, new QStandardItem(id));
    tag_model->setItem(row, 1, new QStandardItem(name));
}

void SettingsObj::addEventToModel(QString id, QString name,
                     QString chanell, QString time,
                     QString id_tag, QString react)
{
    int row = event_model->rowCount();

    event_model->insertRow(row);
    event_model->setItem(row, 0, new QStandardItem(id));
    event_model->setItem(row, 1, new QStandardItem(name));
    event_model->setItem(row, 2, new QStandardItem(chanell));
    event_model->setItem(row, 3, new QStandardItem(time));
    event_model->setItem(row, 4, new QStandardItem(id_tag));
    event_model->setItem(row, 5, new QStandardItem(react));
}

DEV_INFO * SettingsObj::getDevSettings(unsigned int id)
{
    QList<DEV_INFO>::iterator it = dev_settings.begin();

    for(; it != dev_settings.end(); ++it)
    {
        if(it->id == id)
        {
            return &(*it);
        }
    }

    return NULL;
}

void SettingsObj::addDevInfoToModel(/*QString num,*/ QString type, QString id,
                       QString loc_id, QString desc)
{
    int row = dev_model->rowCount();

    dev_model->insertRow(row);
    //dev_model->setItem(row, Num, new QStandardItem(num));
    dev_model->setItem(row, Type, new QStandardItem(type));
    dev_model->setItem(row, Id, new QStandardItem(id));
    dev_model->setItem(row, LocId, new QStandardItem(loc_id));
    dev_model->setItem(row, Desc, new QStandardItem(desc));

    DEV_INFO * dev = getDevSettings(id.toInt());

    if(dev == NULL)
    {
        DEV_INFO dev_new;

        dev_new.active  = 0;
        dev_new.id      = id.toInt();

        utils.R245_InitDev(row);

        utils.R245_GetChan(row, &dev_new.channel);
        utils.R245_GetDamp(row, 1, &dev_new.dist1);
        utils.R245_GetDamp(row, 2, &dev_new.dist2);
        utils.R245_GetTime(row, 1, &dev_new.time1);
        utils.R245_GetTime(row, 2, &dev_new.time2);

        utils.R245_CloseDev(row);

        dev_settings.append(dev_new);
    } else
    {
        setActiveDev(row, dev->active);
        setChannelDev(row, dev->channel);
        setTimeDev(row, dev->time1, true);
        setTimeDev(row, dev->time2, false);
        setDistDev(row, dev->dist1, true);
        setDistDev(row, dev->dist2, false);
    }
}

SettingsObj::~SettingsObj()
{
    if(log_stream != NULL)
    {
        *log_stream << "</log>\n";
    }

    if(closeFile(fsettings))
        delete fsettings;
    if(closeFile(flog))
        delete flog;

    delete log_stream;
    delete tag_model;
    delete tag_model_proxy;
    delete dev_name_model;
    delete dev_name_model_proxy;
    delete dev_model;
    delete event_model;
}
