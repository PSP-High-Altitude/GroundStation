#include "data_table.h"
#include "data_table_row.h"
#include "mainwindow.h"
#include "qtablewidget.h"

float bytes_to_float(unsigned char *buf)
{
    union {
        unsigned char bytes[4];
        float floatValue;
    } float_conv;

    if(Q_BYTE_ORDER == Q_BIG_ENDIAN)
    {
        for (int i = 0; i < 4; i++) {
            float_conv.bytes[3 - i] = buf[i];
        }
        return float_conv.floatValue;
    }
    else
    {
        for (int i = 0; i < 4; i++) {
            float_conv.bytes[i] = buf[i];
        }
        return float_conv.floatValue;
    }
}

float conv_unit(QString type, QString to, float val)
{
    if(!type.compare("acc_x") || !type.compare("acc_y") || !type.compare("acc_z"))
    {
        if(!to.compare("m/s^2"))
        {
            return val * 9.80665;
        }
    }
    if(!type.compare("rot_x") || !type.compare("rot_y") || !type.compare("rot_z"))
    {
        if(!to.compare("rad/s"))
        {
            return val * 0.017453;
        }
    }
    if(!type.compare("temp"))
    {
        if(!to.compare("°F"))
        {
            return (val * 9.0/5.0) + 32.0;
        }
        if(!to.compare("K"))
        {
            return val + 273.15;
        }
    }
    if(!type.compare("pres"))
    {
        if(!to.compare("inHg"))
        {
            return val * 0.029529983;
        }
        if(!to.compare("atm"))
        {
            return val * 0.000986923;
        }
    }
    if(!type.compare("alt_gnd") || !type.compare("alt_msl"))
    {
        if(!to.compare("ft"))
        {
            return val * 3.28084;
        }
    }
    return val;
}

void fill_cell(QTableWidget *tw, DataTable *t, QString type, int dev_id, QString value) {
    for(int i = 0; i < t->get_rows()->count(); i++)
    {
        DataTableRow row = t->get_rows()->at(i);
        if(row.get_name() == type && row.get_pspcom_id() == dev_id)
        {
            tw->setItem(i, 1, new QTableWidgetItem(value));
            tw->model()->setData(tw->model()->index(i, 1),Qt::AlignCenter,Qt::TextAlignmentRole);
        }
    }
}

void fill_cell(QTableWidget *tw, DataTable *t, QString type, int dev_id, float value) {
    for(int i = 0; i < t->get_rows()->count(); i++)
    {
        DataTableRow row = t->get_rows()->at(i);
        if(row.get_name() == type && row.get_pspcom_id() == dev_id)
        {
            value = conv_unit(type, row.get_units(), value);
            tw->setItem(i, 1, new QTableWidgetItem(QString::number(value, 'f', row.get_decimal_places())));
            tw->model()->setData(tw->model()->index(i, 1),Qt::AlignCenter,Qt::TextAlignmentRole);
        }
    }
}

DataTable::DataTable(QWidget *mw, QString name, QList<DataTableRow> *rows)
{
    this->mw = mw;
    this->name = name;
    this->rows = rows;
}

QString DataTable::get_name()
{
    return this->name;
}

void DataTable::set_name(QString name)
{
    this->name = name;
}

QList<DataTableRow>* DataTable::get_rows()
{
    return this->rows;
}

void DataTable::update_data(pspcommsg msg)
{
    QTableWidget *table_widget = mw->findChild<QTableWidget*>("data_table");
    DataTable *table = ((MainWindow*) mw)->get_active_table();
    switch(msg.msg_id)
    {
    case 0x84:
    {
        int dev_id = msg.payload[0];

        // Read x
        float x = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, table, "acc_x", dev_id, x);

        // Read y
        float y = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, table, "acc_y", dev_id, y);

        // Read z
        float z = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, table, "acc_z", dev_id, z);

        break;
    }
    case 0x85:
    {
        int dev_id = msg.payload[0];

        // Read x
        float x = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, table, "rot_x", dev_id, x);

        // Read y
        float y = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, table, "rot_y", dev_id, y);

        // Read z
        float z = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, table, "rot_z", dev_id, z);

        break;
    }
    case 0x86:
    {
        int dev_id = msg.payload[0];

        // Read temp
        float temp = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, table, "temp", dev_id, temp);

        break;
    }
    case 0x87:
    {
        int dev_id = msg.payload[0];

        // Read pres
        float pres = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, table, "pres", dev_id, pres);

        break;
    }
    }
}

bool DataTable::operator== (const DataTable& other) const
{
    return (this->name.compare(other.name) == 0);
}

bool DataTable::operator!= (const DataTable& other) const
{
    return (this->name.compare(other.name));
}
