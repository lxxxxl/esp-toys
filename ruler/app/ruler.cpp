#include <QLabel>
#include <QPushButton>
#include <QUdpSocket>
#include <QVBoxLayout>

#include "ruler.h"

Ruler::Ruler(QWidget *parent)
    : QWidget(parent)
{

    label = new QLabel(tr("Waiting for data..."));
    label->setWordWrap(true);

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(8811, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead,
            this, &Ruler::processPendingDatagrams);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(label, 0, Qt::AlignCenter);

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->addLayout(hboxLayout);
    setLayout(vboxLayout);

    setWindowTitle(tr("Ruler client"));
}

void Ruler::processPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream ds(datagram);
        ds.setByteOrder(QDataStream::LittleEndian);
        unsigned int distance;
        ds >> distance;

        label->setText(tr("Distance, cm: %1")
                             .arg(distance));
    }
}
