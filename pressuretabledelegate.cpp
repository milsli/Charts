#include "pressuretabledelegate.h"
#include <QTimeEdit>

PressureTableDelegate::PressureTableDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *PressureTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTimeEdit *editor = new QTimeEdit(parent);
    // editor->setTime(QTime::fromString("00:00", "hh:mm"));
    // editor->setTimeRange(100);
    return editor;
}

void PressureTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // Get the value via index of the Model
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QString formattedTime = QString(value / 60) + ":" + QString(value%60);
    // Put the value into the TimeEdit
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    timeEdit->setTime(QTime::fromString(formattedTime, "hh:mm"));
}

void PressureTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    //timeEdit->interpretText();
    int value = timeEdit->time().hour();
    value *= 60;
    value += timeEdit->time().minute();
    model->setData(index, value, Qt::EditRole);
}

void PressureTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
