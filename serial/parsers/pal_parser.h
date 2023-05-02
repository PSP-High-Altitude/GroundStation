#ifndef PAL_PARSER_H
#define PAL_PARSER_H

#include "../../data.h"

#define MAX_READ 4000

class palParser
{
public:
    palParser();
    static void parse_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status);
};

#endif // PAL_PARSER_H
