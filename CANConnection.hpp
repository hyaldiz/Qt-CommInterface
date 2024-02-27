#ifndef CANCONNECTION_HPP
#define CANCONNECTION_HPP

#include <QObject>
#include <QVariant>
#include <QCanBusDevice>

class CANConnection : public QObject
{
    Q_OBJECT

public:
    using ConfigurationItem_t = QPair<QCanBusDevice::ConfigurationKey,QVariant>;

    CANConnection(QObject* parent = nullptr);

    void connectDevice();
    void disconnectDevice();

    QCanBusDevice* device()const;

private:
    QList<ConfigurationItem_t> _configuration;

    std::unique_ptr<QCanBusDevice> _device;
};

#endif
