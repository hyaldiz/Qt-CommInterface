#include "CANComm.hpp"
#include <QCanBusFrame>
#include <QCanBusDevice>

CANComm::CANComm(QObject *parent) : CommInterface(parent)
    ,_connection(std::make_unique<CANConnection>())
{
    _connection->connectDevice();

    connect(_connection->device(),&QCanBusDevice::framesReceived,this,&CANComm::readyRead);
    connect(_connection->device(),&QCanBusDevice::errorOccurred,this,&CANComm::processError);
    connect(_connection->device(),&QCanBusDevice::framesWritten,this,&CANComm::processFramesWritten);
}

void CANComm::status()
{
    if (!_connection->device() || !_connection->device()->hasBusStatus()) {
        qWarning() << tr("No CAN bus status available.");
        return;
    }

    switch (_connection->device()->busStatus()) {
    case QCanBusDevice::CanBusStatus::Good:
        qInfo() << tr("CAN bus status: Good.");
        break;
    case QCanBusDevice::CanBusStatus::Warning:
        qInfo() << tr("CAN bus status: Warning.");
        break;
    case QCanBusDevice::CanBusStatus::Error:
        qInfo() << tr("CAN bus status: Error.");
        break;
    case QCanBusDevice::CanBusStatus::BusOff:
        qInfo() << tr("CAN bus status: Bus Off.");
        break;
    default:
        qInfo() << tr("CAN bus status: Unknown.");
        break;
    }
}

void CANComm::processError(QCanBusDevice::CanBusError error)
{
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        qWarning() << _connection->device()->errorString();
        break;
    default:
        break;
    }
}

void CANComm::processFramesWritten(qint64 count)
{
    static qint64 numberFramesWritten = 0;

    numberFramesWritten += count;

    qInfo() << tr("%1 frames written").arg(numberFramesWritten);
}

void CANComm::write(const QVariant& frame)
{
    if(frame.canConvert<QCanBusFrame>()) {
        QCanBusFrame canFrame = frame.value<QCanBusFrame>();

        if(canFrame.isValid()) {
            _connection->device()->writeFrame(canFrame);
            return;
        }

        qWarning() << tr("Invalid QCanBusFrame");
    }
}

QVariantMap CANComm::read()
{
    QVariantMap canValues;
    if(_connection->device()->framesAvailable()) {
        auto frame = _connection->device()->readFrame();

        canValues["ID"] = frame.frameId();
        canValues["FrameType"] = frame.frameType();
        canValues["Payload"] = frame.payload();
    }

    return canValues;
}
