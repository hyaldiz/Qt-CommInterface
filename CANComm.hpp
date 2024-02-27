#ifndef CANCOMM_HPP
#define CANCOMM_HPP

#include "CommInterface.hpp"
#include "CANConnection.hpp"

class CANComm : public CommInterface
{
    Q_OBJECT
public:
    CANComm(QObject* parent = nullptr);

    void status()override;

private slots:
    void processError(QCanBusDevice::CanBusError error);
    void processFramesWritten(qint64 count);

public slots:
    void write(const QVariant& frame)override;
    QVariantMap read()override;

private:
    std::unique_ptr<CANConnection> _connection;
};

#endif
