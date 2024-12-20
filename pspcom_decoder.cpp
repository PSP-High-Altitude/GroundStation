#include "pspcom_decoder.h"

PSPCOMDecoder::PSPCOMDecoder(QObject *parent) {

}

void PSPCOMDecoder::processMessage(PspcomMsg msg) {
    switch(msg.m_msg_id) {
    case 0x8E:
        int16_t rssi_raw = ((int16_t)msg.m_payload.at(1)) << 8;
        rssi_raw |= (uint8_t)msg.m_payload.at(0);
        double rssi = rssi_raw * 0.01;
        emit rssiReceived(rssi);
        break;
    }
}
