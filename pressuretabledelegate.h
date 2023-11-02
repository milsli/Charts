#ifndef PRESSURETABLEDELEGATE_H
#define PRESSURETABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class TimeColumnDelegate : public QStyledItemDelegate
{
public:
    explicit TimeColumnDelegate(QObject *parent = nullptr);

    // Create Editor when we construct MyDelegate
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Then, we set the Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // When we modify data, this model reflect the change
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    // Give the SpinBox the info on size and location
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // displays text while not edit
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;
};

//class NumberColumnDelegate :  public QStyledItemDelegate
//{
//public:
//    explicit NumberColumnDelegate(QObject *parent = nullptr);

//    // Create Editor when we construct MyDelegate
//    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

//    // Then, we set the Editor
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;

//    // When we modify data, this model reflect the change
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

//    // Give the SpinBox the info on size and location
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//};

//class ComboColumnDelegate :  public QStyledItemDelegate
//{
//    Q_OBJECT
//public:
//    explicit ComboColumnDelegate(QObject *parent = nullptr);

//    // Create Editor when we construct MyDelegate
//    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

//    // Then, we set the Editor
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;

//    // When we modify data, this model reflect the change
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

//    // Give the SpinBox the info on size and location
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//};

#endif // PRESSURETABLEDELEGATE_H
