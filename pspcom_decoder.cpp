#include "pspcom_decoder.h"
#include "stdlib.h"
#include "qdebug.h"

PSPCOMDecoder::PSPCOMDecoder(QObject *parent) {

}

void PSPCOMDecoder::processMessage(PspcomMsg msg) {
    switch(msg.m_msg_id) {
    case 0x8E: {
        // RSSI
        int16_t rssi_raw = ((int16_t)msg.m_payload.at(1)) << 8;
        rssi_raw |= (uint8_t)msg.m_payload.at(0);
        double rssi = rssi_raw * 0.01;
        emit rssiReceived(rssi);
        break;
    }
    case 0xE0: {
        // STD_TELEM_1
        GPSPos gps_pos;
        GPSVel gps_vel;
        Pres pres;
        PyroStat2 pyro_stat;
        pyro_stat.m_num_pyro_stat = 1;
        SysStat sys_stat;
        int pos = 0;

        memcpy(&gps_pos, msg.m_payload.data() + pos, sizeof(GPSPos));
        pos += sizeof(GPSPos);

        memcpy(&gps_vel, msg.m_payload.data() + pos, sizeof(GPSVel));
        pos += sizeof(GPSVel);

        memcpy(&pres, msg.m_payload.data() + pos, sizeof(Pres));
        pos += sizeof(Pres);

        memcpy(&pyro_stat.m_pyro_stat_0, msg.m_payload.data() + pos, sizeof(PyroStat));
        pos += sizeof(PyroStat);

        memcpy(&sys_stat, msg.m_payload.data() + pos, sizeof(SysStat));
        pos += sizeof(SysStat);

        emit gpsPosReceived(gps_pos, sys_stat.m_gps_fix_ok);
        emit gpsVelReceived(gps_vel, sys_stat.m_gps_fix_ok);
        emit presReceived(pres);
        emit pyroStatusReceived(pyro_stat);
        emit sysStatusReceived(sys_stat);
        break;
    }
    case 0xE1: {
        // STD_TELEM_2
        GPSPos gps_pos;
        GPSVel gps_vel;
        Pres pres;
        PyroStat2 pyro_stat;
        pyro_stat.m_num_pyro_stat = 2;
        SysStat sys_stat;
        int pos = 0;

        memcpy(&gps_pos, msg.m_payload.data() + pos, sizeof(GPSPos));
        pos += sizeof(GPSPos);

        memcpy(&gps_vel, msg.m_payload.data() + pos, sizeof(GPSVel));
        pos += sizeof(GPSVel);

        memcpy(&pres, msg.m_payload.data() + pos, sizeof(Pres));
        pos += sizeof(Pres);

        memcpy(&pyro_stat.m_pyro_stat_0, msg.m_payload.data() + pos, sizeof(PyroStat));
        pos += sizeof(PyroStat);

        memcpy(&pyro_stat.m_pyro_stat_1, msg.m_payload.data() + pos, sizeof(PyroStat));
        pos += sizeof(PyroStat);

        memcpy(&sys_stat, msg.m_payload.data() + pos, sizeof(SysStat));
        pos += sizeof(SysStat);

        emit gpsPosReceived(gps_pos, sys_stat.m_gps_fix_ok);
        emit gpsVelReceived(gps_vel, sys_stat.m_gps_fix_ok);
        emit presReceived(pres);
        emit pyroStatusReceived(pyro_stat);
        emit sysStatusReceived(sys_stat);
        break;
    }
    }
}
