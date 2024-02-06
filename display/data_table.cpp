#include "data_table.h"
#include "data_table_row.h"
#include "mainwindow.h"
#include "qtablewidget.h"
#include "utils/misc.h"

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
        if(row.get_name() == type /*&& row.get_pspcom_id() == dev_id*/)
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
        if(row.get_name() == type /*&& row.get_pspcom_id() == dev_id*/)
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
    this->last_data_s = 0;

    age_timer.setInterval(1000);
    connect(&age_timer, &QTimer::timeout, this, [this]{
        if(this != ((MainWindow*) this->mw)->get_active_table())
        {
            return;
        }
        QTableWidget *table_widget = this->mw->findChild<QTableWidget*>("data_table");
        fill_cell(table_widget, this, "age", 0, ++last_data_s); // Set data age
    });
    age_timer.start();
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
    age_timer.stop();
    last_data_s = 0;
    age_timer.start();
    QTableWidget *table_widget = mw->findChild<QTableWidget*>("data_table");
    switch(msg.msg_id)
    {
    case 0x84:
    {
        // Read x
        float x = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, this, "acc_x", 0, x);

        // Read y
        float y = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, this, "acc_y", 0, y);

        // Read z
        float z = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, this, "acc_z", 0, z);

        break;
    }
    case 0x85:
    {
        // Read x
        float x = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, this, "rot_x", 0, x);

        // Read y
        float y = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, this, "rot_y", 0, y);

        // Read z
        float z = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, this, "rot_z", 0, z);

        break;
    }
    case 0x86:
    {
        // Read temp
        float temp = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, this, "temp", 0, temp);

        break;
    }
    case 0x87:
    {
        // Read pres
        float pres = bytes_to_float(msg.payload + 1);
        float baro_alt = 44330 * (1 - pow(pres/1013.25, 1/5.255));
        fill_cell(table_widget, this, "pres", 0, pres);
        fill_cell(table_widget, this, "baro_alt", 0, baro_alt);

        break;
    }
    case 0x8A:
    {
        // Read number of sats
        int num_sats = msg.payload[0];
        fill_cell(table_widget, this, "num_sats", 0, num_sats);

        // Read lat
        float lat = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, this, "lat", 0, lat);

        // Read lon
        float lon = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, this, "lon", 0, lon);

        // Read alt
        float alt = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, this, "alt_msl", 0, alt);

        break;
    }
    case 0x8B:
    {
        // Read velocity north
        float vel_n = bytes_to_float(msg.payload);
        fill_cell(table_widget, this, "vel_n", 0, vel_n);

        // Read velocity east
        float vel_e = bytes_to_float(msg.payload + 4);
        fill_cell(table_widget, this, "vel_e", 0, vel_e);

        // Read velocity down
        float vel_d = bytes_to_float(msg.payload + 8);
        fill_cell(table_widget, this, "vel_d", 0, vel_d);

        break;
    }
    case 0x8E:
    {
        // Read RSSI
        int16_t rssi = msg.payload[0] | (((int16_t)msg.payload[1]) << 8);
        fill_cell(table_widget, this, "rssi", 0, (float)rssi*0.01);

        break;
    }
    case 0xE0:
    {
        // Read number of sats
        int num_sats = msg.payload[0];
        fill_cell(table_widget, this, "num_sats", 0, num_sats);

        // Read lat
        float lat = bytes_to_float(msg.payload + 1);
        fill_cell(table_widget, this, "lat", 0, lat);

        // Read lon
        float lon = bytes_to_float(msg.payload + 5);
        fill_cell(table_widget, this, "lon", 0, lon);

        // Read alt
        float alt = bytes_to_float(msg.payload + 9);
        fill_cell(table_widget, this, "alt_msl", 0, alt);

        // Read velocity north
        float vel_n = bytes_to_float(msg.payload + 13);
        fill_cell(table_widget, this, "vel_n", 0, vel_n);

        // Read velocity east
        float vel_e = bytes_to_float(msg.payload + 17);
        fill_cell(table_widget, this, "vel_e", 0, vel_e);

        // Read velocity down
        float vel_d = bytes_to_float(msg.payload + 21);
        fill_cell(table_widget, this, "vel_d", 0, vel_d);

        // Read pres
        float pres = bytes_to_float(msg.payload + 26);
        float baro_alt = 44330 * (1 - pow(pres/1013.25, 1/5.255));
        fill_cell(table_widget, this, "pres", 0, pres);
        fill_cell(table_widget, this, "baro_alt", 0, baro_alt);

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
