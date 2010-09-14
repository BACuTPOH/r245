#ifndef COMBO_BOX_DELEGATE_H
#define COMBO_BOX_DELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QLineEdit>
#include <QColorDialog>
#include "settings_obj.h"

class EventDelegate : public QItemDelegate
{
    Q_OBJECT
private:
    QList<QString> * event_list;
    QList<QString> * react_list;
    QList<QString> * chanell_list;
public:
    EventDelegate(QList<QString> * event, QList<QString> * react, QList<QString> * chanell, QObject *parent = 0);

    QWidget * createEditor(QWidget * parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
    void setEditorData(QWidget * editor, const QModelIndex &index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // COMBO_BOX_DELEGATE_H
