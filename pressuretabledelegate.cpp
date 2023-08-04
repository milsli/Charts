#include "pressuretabledelegate.h"
#include <QTimeEdit>

PressureTableDelegate::PressureTableDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *PressureTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = new QTimeEdit(parent);
    timeEdit->setAlignment(Qt::AlignCenter);
    return timeEdit;
}

void PressureTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QTime time = index.model()->data(index, Qt::DisplayRole).toTime();    
    QTimeEdit *timeEdit = static_cast<QTimeEdit *>(editor);
    timeEdit->setTime(time);
}

void PressureTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    model->setData(index, timeEdit->time().toString(QString("hh:mm")), Qt::EditRole);
}

void PressureTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
