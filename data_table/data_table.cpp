#include "data_table.h"
#include "hab_table.h"
#include "pal_table.h"
#include <QHeaderView>
#include <QGridLayout>
#include <math.h>
#include <QMainWindow>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QVBoxLayout>
#include <QPalette>
#include <qdebug.h>

GenericTable::GenericTable(SensorTable* table, QObject *parent)
{
    this->table = table;
}

GenericTable::~GenericTable() {}

void GenericTable::update_table(SensorData* sens, GpsData* gps, StatusData* status) {}

TableLabel::TableLabel(const QString text, Qt::Alignment alignment)
{
    this->setText(text);
    this->setAlignment(alignment);
}

SensorTable::SensorTable(QWidget* window, QObject *parent)
{
    this->table = nullptr;
    QTableWidget *placeholder = window->findChild<QTableWidget*>("table_sensor");
    QVBoxLayout *layout = window->findChild<QVBoxLayout*>("table_layout");
    layout->replaceWidget(placeholder, this);
    this->setMinimumHeight(placeholder->minimumHeight());
    QPalette p = placeholder->palette();
    p.setColor(QPalette::Inactive, QPalette::Base, p.color(QPalette::Active, QPalette::Base));
    p.setColor(QPalette::Inactive, QPalette::Text, p.color(QPalette::Active, QPalette::Text));
    this->setPalette(p);
    placeholder->deleteLater();
    this->setColumnCount(4);
    this->setRowCount(14);
    this->setHorizontalHeaderLabels({"Name", "Value", "Name", "Value"});
    this->verticalHeader()->setVisible(false);
    this->setColumnWidth(0, 150);
    this->setColumnWidth(2, 150);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    current_uid = 0;
}

void SensorTable::make_table(int uid)
{
    if(uid == current_uid)
    {
        return;
    }
    else
    {
        current_uid = uid;
    }
    while(!table.isNull())
    {
        table.clear();
    }
    for(int i = 0; i < this->rowCount(); i++)
    {
        for(int j = 0; j < this->columnCount(); j++)
        {
            this->removeCellWidget(i, j);
        }
    }
    GenericTable* table_ptr;
    switch(uid)
    {
    case 2:
        table_ptr = new PalTable(this);
        table = QSharedPointer<GenericTable>(table_ptr);
        break;
    case 3:
        table_ptr = new HabTable(this);
        table = QSharedPointer<GenericTable>(table_ptr);
        break;
    }
}

void SensorTable::update_table(SensorData* sens, GpsData* gps, StatusData* status)
{
    if(!table.isNull())
    {
        table.get()->update_table(sens, gps, status);
    }
}

void SensorTable::resize_columns()
{
    this->setColumnWidth(1, (this->width()-302)/2);
    this->setColumnWidth(3, (this->width()-302)/2);
}

void SensorTable::showEvent(QShowEvent *event)
{
    this->resize_columns();
}
