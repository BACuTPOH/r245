#ifndef TRANS_PARSER_H
#define TRANS_PARSER_H

#include <QXmlDefaultHandler>
#include <QDebug>
#include "monitor.h"
#include "r245_types.h"
#include "settings_obj.h"

class TransParser : public QXmlDefaultHandler
{
public:
    TransParser(Monitor *monitor, SettingsObj *set);
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool characters(const QString &ch);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool fatalError(const QXmlParseException &exception);

private:
    R245_TRANSACT trans;
    QString dev_num;
    QString el_name;
    Monitor *monitor_obj;
    SettingsObj *set_obj;
    bool transact;
};

#endif // TRANS_PARSER_H
