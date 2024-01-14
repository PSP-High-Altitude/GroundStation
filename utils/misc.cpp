#include "misc.h"
#include "qobject.h"

float bytes_to_float(unsigned char *buf)
{
    union {
        unsigned char bytes[4];
        float floatValue;
    } float_conv;

    if(Q_BYTE_ORDER == Q_BIG_ENDIAN)
    {
        for (int i = 0; i < 4; i++) {
            float_conv.bytes[3 - i] = buf[i];
        }
        return float_conv.floatValue;
    }
    else
    {
        for (int i = 0; i < 4; i++) {
            float_conv.bytes[i] = buf[i];
        }
        return float_conv.floatValue;
    }
}
