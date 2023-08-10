#include "pressuretabledelegate.h"
#include <QTimeEdit>
#include <QLineEdit>

TimeColumnDelegate::TimeColumnDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *TimeColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = new QTimeEdit(parent);
    timeEdit->setDisplayFormat("mm:ss");
    timeEdit->setAlignment(Qt::AlignCenter);
    return timeEdit;
}

void TimeColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QTime time = index.model()->data(index, Qt::DisplayRole).toTime();
    QTimeEdit *timeEdit = static_cast<QTimeEdit *>(editor);
    timeEdit->setTime(time);
}

void TimeColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    model->setData(index, timeEdit->time(), Qt::EditRole);
}

void TimeColumnDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QString TimeColumnDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QTime time = value.toTime();

    QString str = time.toString("mm:ss");

    return str;
}

NumberColumnDelegate::NumberColumnDelegate(QObject *parent)
{
}

QWidget *NumberColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    QIntValidator *validator = new QIntValidator(600, 950, lineEdit);
    lineEdit->setValidator(validator);
    return lineEdit;
}

void NumberColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::DisplayRole).toInt();
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    lineEdit->setText(QString::number(value));
}

void NumberColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text().toInt(), Qt::EditRole);
}

void NumberColumnDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
