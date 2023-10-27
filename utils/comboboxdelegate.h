#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include "QItemDelegate"

// Written primarily by ChatGPT

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

#endif // COMBOBOXDELEGATE_H
