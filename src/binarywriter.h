#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <QFile>
#include <QDataStream>

class BinaryWriter
{
private:
    QDataStream stream;

public:
    BinaryWriter();
    BinaryWriter(QString fileName);
    ~BinaryWriter();

    bool isAlive() const;

    void writeU4(quint32 value);
    void writeU2(quint16 value);
    void writeU1(quint8 value);

};

#endif // BINARYWRITER_H
