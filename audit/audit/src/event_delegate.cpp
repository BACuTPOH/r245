#include "event_delegate.h"

EventDelegate::EventDelegate(QList<QString> *event, QList<QString> *react, QList<QString> *chanell, QObject *parent):
        QItemDelegate(parent)
{
    event_list = event;
    chanell_list = chanell;
    react_list = react;
}

QWidget * EventDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QWidget * editor;

    switch(index.column())
    {
        case SettingsObj::EvReact:
            editor = new QComboBox(parent);
            ((QComboBox *)editor)->addItems(*react_list);
            break;
        case SettingsObj::EvEvent:
            editor = new QComboBox(parent);
            ((QComboBox *)editor)->addItems(*event_list);
            break;
        case SettingsObj::EvChanell:
            editor = new QComboBox(parent);
            ((QComboBox *)editor)->addItems(*chanell_list);
            break;
        default:
            editor = new QLineEdit(parent);
            break;
    }

    return editor;
}

void EventDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    //qDebug("setEditorData");
    /*switch(index.column())
    {
        case SettingsObj::EvChanell:
        case SettingsObj::EvEvent:
        case SettingsObj::EvReact:
            QComboBox * cb = static_cast<QComboBox*>(editor);
            cb->setCurrentIndex(index.model()->data(index, Qt::DisplayRole).toInt());
            break;
        default:
            QLineEdit * le = static_cast<QLineEdit*>(editor);
            le->setText(index.model()->data(index, Qt::DisplayRole).toString());
            break;
    }*/
}

void EventDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug("setModelData");
    QSortFilterProxyModel * model_proxy = qobject_cast<QSortFilterProxyModel *>(model);
    QStandardItemModel * model_source = qobject_cast<QStandardItemModel *>(model_proxy->sourceModel());

    switch(index.column())
    {
        case SettingsObj::EvChanell:
        case SettingsObj::EvEvent:
            QComboBox * cb = static_cast<QComboBox*>(editor);
            model->setData(index, cb->itemText(cb->currentIndex()), Qt::EditRole);
            break;
        case SettingsObj::EvReact:
            QComboBox * cb_react = static_cast<QComboBox*>(editor);
            model->setData(index, cb_react->itemText(cb_react->currentIndex()), Qt::EditRole);

            if(cb_react->itemText(cb_react->currentIndex()) == "выделить цветом")
            {
                QColor color = QColorDialog::getColor(Qt::green);
                if(color.isValid())
                {
                    model_source->item(index.row(), index.column())->setBackground(color);
                }
            } else
            {
                model_source->item(index.row(), index.column())->setBackground(Qt::white);
            }

            break;
        default:
            QLineEdit * le = static_cast<QLineEdit*>(editor);
            model->setData(index, le->text(), Qt::EditRole);
            break;
    }
}

void EventDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
