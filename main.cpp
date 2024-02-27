#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "CommInterface.hpp"
#include "CANComm.hpp"

/*
    ip link set up/down can0
    ip -details link show can0
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CommInterface *ci = new CANComm();

    QTimer *tmr = new QTimer();
    tmr->setInterval(1000);

    QObject::connect(tmr,&QTimer::timeout,[=](){
        qDebug() << "Write...";

        QCanBusFrame::FrameId id = 0x65D;
        QByteArray payload;
        payload += 0x70;
        payload += 0x80;
        payload += 0x90;

        QCanBusFrame frame(id,payload);

        ci->write(QVariant::fromValue(frame));

        ci->status();
    });

    QObject::connect(ci,&CommInterface::readyRead,[=](){

        qDebug() << "Read --> " << ci->read();

        ci->status();
    });

    tmr->start();

    return a.exec();
}
