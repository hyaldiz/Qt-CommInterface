#include "CANConnection.hpp"
#include <QCanBus>
#include <QCanBusFrame>

#include <QDebug>

CANConnection::CANConnection(QObject *parent) : QObject(parent)
{
    _configuration.clear();
    _configuration.append({QCanBusDevice::LoopbackKey,QVariant::fromValue(true)});
    _configuration.append({QCanBusDevice::ReceiveOwnKey,QVariant::fromValue(true)});
    _configuration.append({QCanBusDevice::ErrorFilterKey,QVariant::fromValue(QCanBusFrame::AnyError)});
    _configuration.append({QCanBusDevice::BitRateKey,QVariant::fromValue(500000)});
    //_configuration.append({QCanBusDevice::CanFdKey,QVariant::fromValue(false)});
    //_configuration.append({QCanBusDevice::DataBitRateKey,QVariant::fromValue(500000)});
}

void CANConnection::connectDevice()
{
    const char* pluginName =  "socketcan";//"virtualcan";"socketcan";
    const char* interfaceName = "can0";
    QString errStr;

    _device.reset(QCanBus::instance()->createDevice(pluginName,interfaceName,&errStr));

    if (!_device) {
        qDebug() << tr("Error creating device '%1', reason: '%2'").arg(pluginName, errStr);
        return;
    }

    for(const ConfigurationItem_t &item : _configuration)
        _device->setConfigurationParameter(item.first,item.second);

    if(!_device->connectDevice()) {
        qWarning() << tr("Connection error: %1").arg(_device->errorString());
        _device.reset();
    }
}

void CANConnection::disconnectDevice()
{
    if(!_device)
        return;

    _device->disconnectDevice();
}

QCanBusDevice *CANConnection::device() const
{
    return _device.get();
}
