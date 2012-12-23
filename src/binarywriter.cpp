#include "binarywriter.h"

BinaryWriter::BinaryWriter()
{
}

BinaryWriter::BinaryWriter(QString fileName)
{
    QFile * file = new QFile(fileName);
    if (file->open(QIODevice::WriteOnly)) {
        stream.setDevice(file);
    }
}

BinaryWriter::~BinaryWriter()
{
    QIODevice * file = stream.device();
    if (file != NULL) {
        file->close();
        delete file;
    }
}

bool BinaryWriter::isAlive() const
{
    return (stream.device() != NULL);
}

void BinaryWriter::writeU4(quint32 value)
{
    if (isAlive()) {
        stream << value;
    }
}

void BinaryWriter::writeU2(quint16 value)
{
    if (isAlive()) {
        stream << value;
    }
}

void BinaryWriter::writeU1(quint8 value)
{
    if (isAlive()) {
        stream << value;
    }
}

void BinaryWriter::writeF4(float value)
{
    if (isAlive()) {
        void * data = &value;
        char * lol = (char*)data;
        stream.writeRawData(&lol[3], 1);
        stream.writeRawData(&lol[2], 1);
        stream.writeRawData(&lol[1], 1);
        stream.writeRawData(&lol[0], 1);
    }
}

void BinaryWriter::writeByteArray(QByteArray bytes)
{
    if (isAlive()) {
        foreach (unsigned char byte, bytes) {
            stream << byte;
        }
    }
}
