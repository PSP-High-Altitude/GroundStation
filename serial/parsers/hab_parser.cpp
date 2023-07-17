#include "hab_parser.h"
#include <QString>
#include <QStringList>
#include "math.h"
#include "qdebug.h"

habParser::habParser() {}

void habParser::process_aprs(int* ridx, char* read_buf, int* read_len, SensorData* sens, GpsData* gps, StatusData* status)
{
    while((unsigned char) read_buf[*ridx] != HAB_APRS_HEADER[0])
    {
        (*ridx)++;
        (*read_len)--;
        if(*read_len < 0)
        {
            return;
        }
    }
    (*ridx)++;

    for(int i = 1; i < HAB_APRS_HEADER_LEN; i++)
    {
        if(*read_len == 0)
        {
            return;
        }
        if((unsigned char) read_buf[*ridx] != HAB_APRS_HEADER[i])
        {
            return;
        }
        (*ridx)++;
    }

    (*ridx)++;
    (*read_len)--;
    unsigned int msg_len = 0;

    for(int i = *ridx; *read_len > 0; i++)
    {
        if((unsigned char) read_buf[i] == '\n')
        {
            *read_len += i - *ridx;
            break;
        }
        msg_len++;
        (*read_len)--;
    }

    if(*read_len < 1)
    {
        return;
    }

    char* msg = (char*) malloc(msg_len + 1);
    memcpy(msg, read_buf + *ridx, msg_len);
    msg[msg_len] = '\0';

    QString msg_str = QString(msg);
    *ridx += msg_len + 2;
    *read_len -= msg_len + 2;
    free(msg);

    gps->hour = msg_str.mid(0, 2).toInt();
    gps->min = msg_str.mid(2, 4).toInt();
    gps->sec = msg_str.mid(4, 6).toInt();
    gps->lat = msg_str.mid(7, 2).toFloat();
    gps->lat += msg_str.mid(9, 5).toFloat() / 60.0F;
    gps->lat *= msg_str.at(14).toLatin1() == 'N' ? 1.0F : -1.0F;
    gps->lon = msg_str.mid(16, 3).toFloat();
    gps->lon += msg_str.mid(19, 5).toFloat() / 60.0F;
    gps->lon *= msg_str.at(24).toLatin1() == 'E' ? 1.0F : -1.0F;

    float hdg = msg_str.mid(26, 3).toFloat();
    float gnd_spd = msg_str.mid(30, 3).toFloat();
    gps->vel_north = gnd_spd * cos(hdg * TO_DEG);
    gps->vel_east = gnd_spd * sin(hdg * TO_DEG);

    gps->height_msl = msg_str.mid(36, 6).toFloat();

    gps->fix_valid = 1;
}

void habParser::parse_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status)
{
    if(!(sens && gps && status))
    {
        return;
    }

    int read_len_aprs = read_len;
    int ridx_aprs = 0;

    while(read_len_aprs > 0)
    {
        process_aprs(&ridx_aprs, read_buf, &read_len_aprs, sens, gps, status);
    }
}
