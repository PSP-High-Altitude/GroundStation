#ifndef PSPCOM_READER_H
#define PSPCOM_READER_H

#include <QObject>
#include <pspcommsg.h>

class PSPCOMReader : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(PspcomMsg lastMessage
                   READ getLastMessage)

    PspcomMsg getLastMessage() { return mLastMessage; }

    explicit PSPCOMReader(QObject *parent = 0);

public slots:
    Q_INVOKABLE void reset();
    Q_INVOKABLE void write(QByteArray data);

signals:
    void messageReceived(PspcomMsg msg);

private:
    PspcomMsg msg;
    PspcomMsg mLastMessage;
    int state;
    int checksum;
    void feedFSM(const char byte);
};

#endif // PSPCOM_READER_H
