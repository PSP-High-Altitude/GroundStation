#include "fileio.h"
#include <QTextStream>

FileIO::FileIO(QObject *parent) :
    QObject(parent), mWrite(false), mAppend(false)
{
    file = new QFile();
}

bool FileIO::open() {
    file->setFileName(mSource);
    if(mWrite && mAppend) {
        return file->open(QIODevice::ReadWrite | QIODevice::Append);
    } else if (mWrite) {
        return file->open(QIODevice::ReadWrite);
    } else {
        return file->open(QIODevice::ReadOnly);
    }
}

bool FileIO::close() {
    file->close();
    return true;
}

QString FileIO::read(qint64 num)
{
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }

    if (file->isOpen()) {
        QString contents;
        contents = file->read(num);

        return contents;
    } else {
        emit error("Unable to open the file");
        return QString();
    }
}

QString FileIO::readLine()
{
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }

    if (file->isOpen()) {
        QString line;
        line = file->readLine();

        return line;
    } else {
        emit error("Unable to open the file");
        return QString();
    }
}


bool FileIO::write(const QString& data)
{
    if (mSource.isEmpty())
        return false;

    if (file->isOpen())
        return false;

    QTextStream out(file);
    out << data;

    return true;
}
