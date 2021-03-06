/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include "callout.h"
#include <QtGui/QMouseEvent>

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_chart(0),
      m_tooltip(0)
{
    // chart
    m_chart = new QChart;
    m_chart->setMinimumSize(640, 480);
    m_chart->legend()->hide();

    // status label
    m_label = new QLabel();
    m_label->setAttribute(Qt::WA_TranslucentBackground);
    scene()->addWidget(m_label);

    //lines
    m_temperature = new QSplineSeries;
    m_humidity = new QSplineSeries;
    connect(m_temperature, &QSplineSeries::hovered, this, &View::tooltipTemperature);
    connect(m_humidity, &QSplineSeries::hovered, this, &View::tooltipHumidity);

    // database
    m_databaseName = "data.csv";
    loadDatabase();

    m_chart->addSeries(m_temperature);
    m_chart->addSeries(m_humidity);

    // axis

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm dd.MM.yyyy");
    axisX->setTitleText("Date");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_temperature->attachAxis(axisX);
    m_humidity->attachAxis(axisX);

    QValueAxis *axisYtemp = new QValueAxis;
    axisYtemp->setLabelFormat("%02.1f");
    axisYtemp->setTitleText("Temperature");
    m_chart->addAxis(axisYtemp, Qt::AlignLeft);
    m_temperature->attachAxis(axisYtemp);

    QValueAxis *axisYhum = new QValueAxis;
    axisYhum->setLabelFormat("%02.1f");
    axisYhum->setTitleText("Humidity");
    m_chart->addAxis(axisYhum, Qt::AlignRight);
    m_humidity->attachAxis(axisYhum);


    m_chart->setAcceptHoverEvents(true);
    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    this->setMouseTracking(true);

    // read token from config file
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("yadisk");
    QString token = settings.value("token","").toString();

    // Yandex disk
    m_disk = new QYandexDisk(token);
    connect(m_disk, SIGNAL(signalDownloaded(QByteArray)), this, SLOT(slotDownloaded(QByteArray)));
    connect(m_disk, SIGNAL(signalList(QList<QYandexDisk::FileInfo*>)), this, SLOT(slotList(QList<QYandexDisk::FileInfo*>)));

    m_label->setText("Downloading new data");
    m_disk->list("/test/test/");
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
    }
    m_label->setGeometry(
                QRect(
                    mapToScene(
                        0,
                        event->size().height() - 12).toPoint(),
                    mapToScene(
                        event->size().width(),
                        event->size().height()).toPoint()));
    QGraphicsView::resizeEvent(event);
}

void View::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (numDegrees.y() < 0)
        m_chart->zoomOut();
    else if (numDegrees.y() > 0)
        m_chart->zoomIn();
    event->accept();
}

void View::tooltipTemperature(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {

        QString text = QString::asprintf("%s\n%s\n%02.1f??C",
                      "Temperature",
                      QDateTime::fromMSecsSinceEpoch(point.x()).toString("hh:mm dd.MM.yyyy").toLocal8Bit().data(),
                      point.y()
                      );
        m_tooltip->setText(text);
        m_tooltip->setSeries(m_temperature);
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void View::tooltipHumidity(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        QString text = QString::asprintf("%s\n%s\n%02.1f???",
                      "Humidity",
                      QDateTime::fromMSecsSinceEpoch(point.x()).toString("hh:mm dd.MM.yyyy").toLocal8Bit().data(),
                      point.y()
                      );
        m_tooltip->setText(text);
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->setSeries(m_humidity);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void View::loadDatabase()
{
    m_temperature->clear();
    m_humidity->clear();

    QFile file(m_databaseName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList items = line.split(";");

        if (items.count() < 3)
            continue;

        // substract timezone correction
        qreal epochTime = (items[0].toULongLong() * 1000) - (4 * 60 * 60 * 1000);
        float temperature = items[1].toFloat();
        float humidity = items[2].toFloat();

        m_temperature->append(epochTime, temperature);
        m_humidity->append(epochTime, humidity);
    }

    // repaint graph
    m_label->setText("Repainting graph");
    m_chart->removeSeries(m_temperature);
    m_chart->removeSeries(m_humidity);
    m_chart->addSeries(m_temperature);
    m_chart->addSeries(m_humidity);
    m_chart->update();
    repaint();
    m_label->setText("");
    return;
}

void View::downloadNextFile()
{
    if (m_fileList.count() > 0){
        QYandexDisk::FileInfo* f = m_fileList.at(0);
        m_currentFilename = f->path;
        m_label->setText("Downloading " + f->name);
        m_disk->download(f->path);
        m_fileList.remove(0);
        delete f;
    }
    else{
        loadDatabase();
    }
}

void View::slotDownloaded(QByteArray data)
{
    // extract base datetime from filename
    int slashIndex = m_currentFilename.lastIndexOf('/');
    int dotIndex = m_currentFilename.lastIndexOf('.');
    qint64 datetime = m_currentFilename.mid(slashIndex + 1, dotIndex - slashIndex - 1).toInt(0);

    // remove file
    m_disk->remove(m_currentFilename);

    QFile file(m_databaseName);
    if (!file.open(QFile::Append | QFile::Text))
        return;
    // process data
    QString dataStr(data);
    QStringList list = dataStr.split('\n');
    int i = list.count();
    for (QString s: list){
        QStringList items = s.split(";");
        if (items.count() < 2){
            i--;
            continue;
        }
        float temperature = items[0].toUInt() / 100.0;
        float humidity = items[1].toUInt() / 100.0;
        qint64 time = datetime - (60 * 60 * i);
        QString str = QString("%1;%2;%3\n").arg(QString::number(time), QString::number(temperature), QString::number(humidity));
        file.write(str.toLocal8Bit());
        i--;
    }
    file.close();

    // download next file
    downloadNextFile();
}
void View::slotList(QList<QYandexDisk::FileInfo*> list)
{
    m_label->setText(QString("New reports: %1").arg(QString::number(list.count())));
    m_fileList = list;
    downloadNextFile();
}
