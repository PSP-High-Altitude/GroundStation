#ifndef PSPCOM_DECODER_H
#define PSPCOM_DECODER_H

#include "pspcommsg.h"
#include <QObject>
#include "stdlib.h"

#pragma pack(push, 1)

struct GPSPos {
    Q_GADGET
    Q_PROPERTY(uint8_t num_sats MEMBER m_num_sats)
    Q_PROPERTY(float lat MEMBER m_lat)
    Q_PROPERTY(float lon MEMBER m_lon)
    Q_PROPERTY(float height_msl MEMBER m_height_msl)
public:
    uint8_t m_num_sats;
    float m_lat;
    float m_lon;
    float m_height_msl;
};
typedef struct GPSPos GPSPos;

struct GPSVel {
    Q_GADGET
    Q_PROPERTY(float vel_n MEMBER m_vel_n)
    Q_PROPERTY(float vel_e MEMBER m_vel_e)
    Q_PROPERTY(float vel_d MEMBER m_vel_d)
public:
    float m_vel_n;
    float m_vel_e;
    float m_vel_d;
};
typedef struct GPSVel GPSVel;

struct Pres {
    Q_GADGET
    Q_PROPERTY(uint8_t id MEMBER m_id)
    Q_PROPERTY(float pres MEMBER m_pres)
public:
    uint8_t m_id;
    float m_pres;
};
typedef struct Pres Pres;

struct PyroStat {
    Q_GADGET
    Q_PROPERTY(uint8_t p0_arm  MEMBER m_p0_arm)
    Q_PROPERTY(uint8_t p0_cont MEMBER m_p0_cont)
    Q_PROPERTY(uint8_t p1_arm  MEMBER m_p1_arm)
    Q_PROPERTY(uint8_t p1_cont MEMBER m_p1_cont)
    Q_PROPERTY(uint8_t p2_arm  MEMBER m_p2_arm)
    Q_PROPERTY(uint8_t p2_cont MEMBER m_p2_cont)
    Q_PROPERTY(uint8_t p3_arm  MEMBER m_p3_arm)
    Q_PROPERTY(uint8_t p3_cont MEMBER m_p3_cont)
public:
    uint8_t m_p0_arm : 1;
    uint8_t m_p0_cont : 1;
    uint8_t m_p1_arm : 1;
    uint8_t m_p1_cont : 1;
    uint8_t m_p2_arm : 1;
    uint8_t m_p2_cont : 1;
    uint8_t m_p3_arm : 1;
    uint8_t m_p3_cont : 1;

    int operator==(const PyroStat& r) const{
        if(memcmp(this, &r, sizeof(PyroStat)) == 0) {
            return true;
        }
        return false;
    }

    int operator!=(const PyroStat& r) const{
        if(memcmp(this, &r, sizeof(PyroStat)) == 0) {
            return false;
        }
        return true;
    }
};
typedef struct PyroStat PyroStat;

struct PyroStat2 {
    Q_GADGET
    Q_PROPERTY(int num_pyro_stat MEMBER m_num_pyro_stat)
    Q_PROPERTY(PyroStat pyro_stat_0 MEMBER m_pyro_stat_0)
    Q_PROPERTY(PyroStat pyro_stat_1 MEMBER m_pyro_stat_1)
public:
    int m_num_pyro_stat;
    PyroStat m_pyro_stat_0;
    PyroStat m_pyro_stat_1;
};
typedef struct PyroStat2 PyroStat2;

typedef struct GPSVel GPSVel;

struct SysStat {
    Q_GADGET
    Q_PROPERTY(uint8_t gps_fix_ok  MEMBER m_gps_fix_ok)
    Q_PROPERTY(uint8_t storage_ok MEMBER m_storage_ok)
    Q_PROPERTY(uint8_t ins_ok  MEMBER m_ins_ok)
    Q_PROPERTY(uint8_t flight_phase MEMBER m_flight_phase)
    Q_PROPERTY(uint8_t error_flag  MEMBER m_error_flag)
public:
    uint8_t m_gps_fix_ok : 1;
    uint8_t m_storage_ok : 1;
    uint8_t m_ins_ok : 1;
    uint8_t m_flight_phase : 4;
    uint8_t m_error_flag : 1;
};
typedef struct SysStat SysStat;

#pragma pack(pop)

class PSPCOMDecoder : public QObject
{
    Q_OBJECT
public: 
    explicit PSPCOMDecoder(QObject *parent = 0);

    Q_INVOKABLE void processMessage(PspcomMsg msg);

signals:
    void rssiReceived(double rssi);
    void gpsPosReceived(GPSPos gps_pos);
    void gpsVelReceived(GPSVel gps_vel);
    void presReceived(Pres pres);
    void pyroStatusReceived(PyroStat2);
    void sysStatusReceived(SysStat sys_stat);
};

#endif // PSPCOM_DECODER_H
