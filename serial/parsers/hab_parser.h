#ifndef HAB_PARSER_H
#define HAB_PARSER_H

#include "../../data.h"

#define MAX_READ 4000
const static unsigned char HAB_APRS_HEADER[] = {0x82, 0xa0, 0xa4, 0xa6, 0x40, 0x40,
                                                0x60, 0x96, 0x9c, 0x68, 0xaa, 0x92,
                                                0xa4, 0x76, 0xae, 0x92, 0x88, 0x8a,
                                                0x62, 0x40, 0x62, 0xae, 0x92, 0x88,
                                                0x8a, 0x64, 0x40, 0x63, 0x03, 0xf0};
const static unsigned int HAB_APRS_HEADER_LEN = 30;

#define TO_RAD 0.01745329252
#define TO_METERS 0.3048
#define TO_MPS 0.514444

class habParser
{
public:
    habParser();
    static void parse_aprs_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status);
    static void parse_lora_message(char* read_buf, int read_len, SensorData* sens, GpsData* gps, StatusData* status);

private:
    static void process_aprs(int* ridx, char* read_buf, int* read_len, SensorData* sens, GpsData* gps, StatusData* status);

};

#endif // HAB_PARSER_H
