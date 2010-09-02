#ifndef TRANS_PARSER_H
#define TRANS_PARSER_H

#include <QXmlDefaultHandler>
#include <QDebug>
#include "monitor.h"
#include "r245_types.h"

class TransParser : public QXmlDefaultHandler
{
public:
    TransParser(Monitor *monitor);
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool characters(const QString &ch);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool fatalError(const QXmlParseException &exception);

private:
    R245_TRANSACT trans;
    QString dev_num;
    QString el_name;
    Monitor *monitor_obj;
    bool transact;
};

#endif // TRANS_PARSER_H
