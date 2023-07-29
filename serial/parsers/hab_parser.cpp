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
    gps->min = msg_str.mid(2, 2).toInt();
    gps->sec = msg_str.mid(4, 2).toInt();
    gps->lat = msg_str.mid(7, 2).toFloat();
    gps->lat += msg_str.mid(9, 5).toFloat() / 60.0F;
    gps->lat *= msg_str.at(14).toLatin1() == 'N' ? 1.0F : -1.0F;
    gps->lon = msg_str.mid(16, 3).toFloat();
    gps->lon += msg_str.mid(19, 5).toFloat() / 60.0F;
    gps->lon *= msg_str.at(24).toLatin1() == 'E' ? 1.0F : -1.0F;

    float hdg = msg_str.mid(26, 3).toFloat();
    float gnd_spd = msg_str.mid(30, 3).toFloat() * TO_MPS;
    gps->vel_north = gnd_spd * cos(hdg * TO_RAD);
    gps->vel_east = gnd_spd * sin(hdg * TO_RAD);

    gps->height_msl = msg_str.mid(36, 6).toFloat() * TO_METERS;

    gps->fix_valid = 1;

    sens->baro.temperature = msg_str.mid(42, 3).toFloat();
    sens->temp.temperature = msg_str.mid(46, 3).toFloat();
    sens->baro.pressure = msg_str.mid(50, 5).toFloat();
}

void habParser::parse_aprs_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status)
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

void habParser::parse_lora_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status)
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

        if(read_len > 0 && read_buf[ridx] == '$' && read_buf[ridx + 1] == '@')
        {
            char len_str[4] = "000";
            memcpy(len_str, read_buf + ridx + 2, 3);
            unsigned int msg_len = (new QString(len_str))->toUInt() + 7;    // Must add rssi length
            ridx += 5;
            read_len -= 5;

            unsigned char* msg = (unsigned char*) malloc(msg_len + 1);
            memcpy(msg, read_buf + ridx, msg_len);
            msg[msg_len] = '\0';

            gps->num_sats = (uint8_t) msg[23];
            gps->fix_valid = (uint8_t) msg[24];

            gps->year = 1980 + (uint8_t) msg[0];
            gps->month = (uint8_t) msg[1];
            gps->day = (uint8_t) msg[2];

            gps->hour = (uint8_t) msg[3];
            gps->min = (uint8_t) msg[4];
            gps->sec = (uint8_t) msg[5];

            if(gps->fix_valid && gps->num_sats > 0)
            {
                gps->lat = (int32_t) (((uint32_t) msg[6]) + (((uint32_t) msg[7]) << 8) + (((uint32_t) msg[8]) << 16) + (((uint32_t) msg[9]) << 24));
                gps->lat /= 10000000.0;
                gps->lon = (int32_t) (((uint32_t) msg[10]) + (((uint32_t) msg[11]) << 8) + (((uint32_t) msg[12]) << 16) + (((uint32_t) msg[13]) << 24));
                gps->lon /= 10000000.0;
                gps->height_msl = (int32_t) (((uint32_t) msg[14]) + (((uint32_t) msg[15]) << 8) + (((uint32_t) msg[16]) << 16));
                gps->height_msl /= 10.0;

                gps->vel_north = (int16_t) (((uint32_t) msg[17]) + (((uint32_t) msg[18]) << 8));
                gps->vel_north /= 10.0;
                gps->vel_east = (int16_t) (((uint32_t) msg[19]) + (((uint32_t) msg[20]) << 8));
                gps->vel_east /= 10.0;
                gps->vel_down = (int16_t) (((uint32_t) msg[21]) + (((uint32_t) msg[22]) << 8));
                gps->vel_down /= 10.0;
            }

            status->rssi = QString((char*) (msg + 25)).toFloat();

            ridx += msg_len;
            read_len -= msg_len;

            free(msg);
        }
    }
}
