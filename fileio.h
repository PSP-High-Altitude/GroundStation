#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include <QFile>

class FileIO : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString source
                   READ getSource
                       WRITE setSource
                           NOTIFY sourceChanged)
    Q_PROPERTY(bool write
                   READ getWrite
                       WRITE setWrite
                           NOTIFY writeChanged)
    Q_PROPERTY(bool append
                   READ getAppend
                       WRITE setAppend
                           NOTIFY appendChanged)
    explicit FileIO(QObject *parent = 0);

    Q_INVOKABLE QString read(qint64 num);
    Q_INVOKABLE QString readLine();
    Q_INVOKABLE bool write(const QString& data);
    Q_INVOKABLE bool open();
    Q_INVOKABLE bool close();

    QString getSource() { return mSource; };
    bool getWrite() { return mWrite; };
    bool getAppend() { return mAppend; };

public slots:
    void setSource(const QString& source) { mSource = source; };
    void setWrite(const bool& write) { mWrite = write; };
    void setAppend(const bool& append) { mAppend = append; };

signals:
    void sourceChanged(const QString& source);
    void writeChanged(const bool& write);
    void appendChanged(const bool& append);
    void error(const QString& msg);

private:
    QString mSource;
    bool mWrite;
    bool mAppend;
    QFile *file;
};

#endif // FILEIO_H
