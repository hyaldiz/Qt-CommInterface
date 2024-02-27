#ifndef COMMINTERFACE_HPP
#define COMMINTERFACE_HPP

#include <QObject>
#include <QVariantMap>

class CommInterface : public QObject
{
    Q_OBJECT

public:
    CommInterface(QObject* parent = nullptr) : QObject(parent) { }

    virtual void status() = 0;

signals:
    void readyRead();

public slots:
    virtual void write(const QVariant&) = 0;
    virtual QVariantMap read() = 0;
};

#endif
