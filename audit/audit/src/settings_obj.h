#ifndef SETTINGS_OBJ_H
#define SETTINGS_OBJ_H

#include <QFile>
#include <QStandardItemModel>
#include <QtXml>
#include <QIODevice>
#include "global.h"

//  онстанты дл€ активации каналов
// ≈сли оба канала активны, то поле HEAD_DEV_INFO_CHANNEL
// в dev_model имеет значение (CHANNEL_ACT_1 | CHANNEL_ACT_2)
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
        EventModel
    };

    enum DevInfoAttr
    {
       // Num,
        Type,
        Id,
        LocId,
        Desc
    };

    SettingsObj();
    ~SettingsObj();

    bool openSettingFile(QString file_name);
    QStandardItemModel * getModel(TypeModel type_model);
    void addTagToModel(QString id = "", QString name = "");
    void addDevNameToModel(QString id = "", QString name = "");
    void addEventToModel(QString id = "",
                         QString name = "",
                         QString chanell = "",
                         QString time = "",
                         QString id_tag = "",
                         QString react = "");
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
private:
    QFile * fsettings;
    QFile * flog;
    QStandardItemModel * tag_model;
    QStandardItemModel * dev_name_model;
    QStandardItemModel * dev_model;
    QStandardItemModel * event_model;
    QList<DEV_INFO> dev_settings;

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
public slots:
};
#endif // SETTINGS_OBJ_H
