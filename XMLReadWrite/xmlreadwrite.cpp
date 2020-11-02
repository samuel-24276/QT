#include "xmlreadwrite.h"

XMLReadWrite::XMLReadWrite(QObject *parent) : QObject(parent)
{

}

void XMLReadWrite::read(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QXmlStreamReader reader(&file);
    while(!reader.atEnd())
    {
//        if(reader.name() == "Demo")
            qDebug()<<reader.text();
    }
    if (reader.hasError())
    {
        QMessageBox::critical(0, tr("Error"),
                              tr("Failed to parse file %1").arg(fileName));
        return;
    }
    else if (file.error() != QFile::NoError)
    {
        QMessageBox::critical(0, tr("Error"),
                              tr("Cannot read file %1").arg(fileName));
        return;
    }
    file.close();
}

void XMLReadWrite::write(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
}
