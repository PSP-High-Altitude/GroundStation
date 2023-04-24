
#ifndef DATA_H
#define DATA_H

#include <cstdint>

typedef struct {
    // UTC Time
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    uint8_t fix_valid;
    uint8_t num_sats;

    float lon;                // deg
    float lat;                // deg
    float height_msl;         // m
    float accuracy_horiz;     // m
    float accuracy_vertical;  // m
    float vel_north;          // m/s
    float vel_east;           // m/s
    float vel_down;           // m/s

} GpsData;

typedef struct {
    float temperature;  // degC
    float pressure;     // mbar
} BaroData;

typedef struct {
    float accelX;  // linear acceleration sensor x-axis in g
    float accelY;  // linear acceleration sensor y-axis in g
    float accelZ;  // linear acceleration sensor z-axis in g
} Accel;

typedef struct {
    float gyroX;  // anglular rate sensor yaw x-axis in dps
    float gyroY;  // anglular rate sensor yaw y-axis in dps
    float gyroZ;  // anglular rate sensor yaw z-axis in dps
} Gyro;

typedef struct {
    float magX;  // magnetic field vector x-axis in gauss
    float magY;  // magnetic field vector y-axis in gauss
    float magZ;  // magnetic field vector z-axis in gauss
} Mag;

typedef struct {
    Accel acch;
    Accel accel;
    Gyro gyro;
    Mag mag;
    BaroData baro;
} SensorData;

#endif // DATA_H
