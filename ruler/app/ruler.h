#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QUdpSocket;
QT_END_NAMESPACE

class Ruler : public QWidget
{
    Q_OBJECT

public:
    explicit Ruler(QWidget *parent = nullptr);

private slots:
    void processPendingDatagrams();

private:
    QLabel *label = nullptr;
    QUdpSocket *udpSocket = nullptr;
};

#endif
