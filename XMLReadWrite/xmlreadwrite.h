#ifndef XMLREADWRITE_H
#define XMLREADWRITE_H

#include <QObject>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomText>
#include <QtXml/QDomNode>
#include <QtXml/QXmlStreamReader>
#include <QFile>
#include <QMessageBox>

class XMLReadWrite : public QObject
{
    Q_OBJECT
public:
    explicit XMLReadWrite(QObject *parent = 0);

    void read(QString fileName);

    void write(QString fileName);

private:
    QString fileName;

signals:

public slots:
};

#endif // XMLREADWRITE_H
