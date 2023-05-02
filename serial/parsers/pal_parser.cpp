
#include "pal_parser.h"
#include <QString>
#include <QStringList>

palParser::palParser() {}

void palParser::parse_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status)
{
    if(!(sens && gps && status))
    {
        return;
    }

    int ridx = 0;

    while(read_len > 0)
    {
        while(read_buf[ridx] != '$' && read_len > 0)
        {
            ridx++;
            read_len--;
        }

        if(read_len > 0 && read_buf[ridx] == '$' && read_buf[ridx + 1] == '#')
        {
            char len_str[4] = "000";
            memcpy(len_str, read_buf + ridx + 2, 3);
            unsigned int msg_len = (new QString(len_str))->toUInt();
            ridx += 5;
            read_len -= 5;

            char* msg = (char*) malloc(msg_len + 1);
            memcpy(msg, read_buf + ridx, msg_len);
            msg[msg_len] = '\0';

            QString* msg_str = new QString(msg);
            QStringList value_list = msg_str->replace('\n', ',').split(u',');
            sens->accel.accelX = value_list.value(0).toFloat();
            sens->accel.accelY = value_list.value(1).toFloat();
            sens->accel.accelZ = value_list.value(2).toFloat();

            sens->gyro.gyroX = value_list.value(3).toFloat();
            sens->gyro.gyroY = value_list.value(4).toFloat();
            sens->gyro.gyroZ = value_list.value(5).toFloat();

            sens->baro.temperature = value_list.value(6).toFloat();
            sens->baro.pressure = value_list.value(7).toFloat();

            sens->mag.magX = value_list.value(8).toFloat();
            sens->mag.magY = value_list.value(9).toFloat();
            sens->mag.magZ = value_list.value(10).toFloat();

            sens->acch.accelX = value_list.value(11).toFloat();
            sens->acch.accelY = value_list.value(12).toFloat();
            sens->acch.accelZ = value_list.value(13).toFloat();

            ridx += msg_len;
            read_len -= msg_len;

            free(msg);
        }

        if(read_len > 0 && read_buf[ridx] == '$' && read_buf[ridx + 1] == '@')
        {
            char len_str[4] = "000";
            memcpy(len_str, read_buf + ridx + 2, 3);
            unsigned int msg_len = (new QString(len_str))->toUInt();
            ridx += 5;
            read_len -= 5;

            char* msg = (char*) malloc(msg_len + 1);
            memcpy(msg, read_buf + ridx, msg_len);
            msg[msg_len] = '\0';

            QString* msg_str = new QString(msg);
            QStringList value_list = msg_str->replace('\n', ',').split(u',');
            gps->fix_valid = value_list.value(15).toUInt();
            gps->num_sats = value_list.value(14).toUInt();

            gps->year = value_list.value(0).toUInt();
            gps->month = value_list.value(1).toUInt();
            gps->day = value_list.value(2).toUInt();

            gps->hour = value_list.value(3).toUInt();
            gps->min = value_list.value(4).toUInt();
            gps->sec = value_list.value(5).toUInt();

            if(gps->fix_valid && gps->num_sats > 0)
            {
                gps->lat = value_list.value(6).toFloat();
                gps->lon = value_list.value(7).toFloat();
                gps->height_msl = value_list.value(8).toFloat();

                gps->accuracy_horiz = value_list.value(9).toFloat();
                gps->accuracy_vertical = value_list.value(10).toFloat();

                gps->vel_north = value_list.value(11).toFloat();
                gps->vel_east = value_list.value(12).toFloat();
                gps->vel_down = value_list.value(13).toFloat();
            }

            ridx += msg_len;
            read_len -= msg_len;

            free(msg);
        }
    }
}
