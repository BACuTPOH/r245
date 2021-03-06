#ifndef SETTINGS_OBJ_H
#define SETTINGS_OBJ_H

#include <QFile>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QtXml>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include "global.h"
#include "monitor.h"

// ��������� ��� ��������� �������
// ���� ��� ������ �������, �� ���� HEAD_DEV_INFO_CHANNEL
// � dev_model ����� �������� (CHANNEL_ACT_1 | CHANNEL_ACT_2)
#define CHANNEL_ACT_1 1
#define CHANNEL_ACT_2 2

class SettingsObj : public QObject
{
    Q_OBJECT
public:

    enum TypeModel {
        TagModel,
        DevNameModel,
        DevModel,
        EventModel,
        TagModelProxy,
        DevNameModelProxy,
        EventModelProxy
    };

    enum AliasTable
    {
        AliasId,
        AliasName
    };

    enum DevInfoAttr
    {
       // Num,
        Type,
        Id,
        LocId,
        Desc
    };

    enum EventAttr
    {
        EvNameDev,
        EvName,
        EvChanell,
        EvNameTag,
        EvEvent,
        EvReact,
        EvIdDev,
        EvIdTag
    };

    SettingsObj();
    ~SettingsObj();

    bool openSettingFile(QString file_name);
    bool openLogFile(QString file_name, Monitor * monitor);
    QAbstractItemModel * getModel(TypeModel type_model);
    void setFilterWildCard(QString ex, TypeModel type_model);
    void addTagToModel(QString id = "", QString name = "");
    void addDevNameToModel(QString id = "", QString name = "");
    void addEventToModel(QString id_dev = "",
                         QString name = "",
                         QString chanell = "",
                         QString id_tag = "",
                         QString event = "",
                         QString react = "",
                         int red = 255, int green = 255, int blue = 255);
    void addDevInfoToModel(/*QString num = "",*/
                           QString type = "",
                           QString id = "",
                           QString loc_id = "",
                           QString desc = "");
    short int setActiveDev(int row, bool active);
    void setChannelDev(int row, short int channel);
    void setTimeDev(int row, short int time, bool time1);
    void setDistDev(int row, short int dist, bool dist1);
    DEV_INFO * getDevSettings(unsigned int id);
    void readDevInfo();
    void saveSetings();
    void addLogNode(QString dev_num, R245_TRANSACT * trans);
private:
    QFile * fsettings;
    QFile * flog;
    QStandardItemModel * tag_model;
    QSortFilterProxyModel * tag_model_proxy;
    QStandardItemModel * dev_name_model;
    QSortFilterProxyModel * dev_name_model_proxy;
    QStandardItemModel * dev_model;
    QStandardItemModel * event_model;
    QSortFilterProxyModel * event_model_proxy;
    QList<DEV_INFO> dev_settings;
    QTextStream * log_stream;

    bool openFile(QFile * file, QFlags <QIODevice::OpenModeFlag> mode);
    bool closeFile(QFile * file);
    void readSettingNodes(const QDomNode &node);
    QDomElement makeElement(QDomDocument  & dom_doc,
                            const QString & name,
                            const QString & attr,
                            const QString & text);
    QDomElement addTagToDom(QDomDocument dom_doc,
                       const QString & id,
                       const QString & name);
    QDomElement addDevNameToDom(QDomDocument dom_doc,
                                const QString & id,
                                const QString & name);
    QDomElement addDevToDom(QDomDocument dom_doc, const DEV_INFO &id);
    QDomElement addEventToDom(QDomDocument dom_doc, int row);
    void initSetModels();
public slots:
};
#endif // SETTINGS_OBJ_H
