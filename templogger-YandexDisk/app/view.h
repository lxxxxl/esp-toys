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

#ifndef VIEW_H
#define VIEW_H
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QFile>
#include <QLabel>
#include <QSplineSeries>
#include <QSettings>
#include "qyandexdisk.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QChart;
QT_END_NAMESPACE

class Callout;

QT_USE_NAMESPACE

class View: public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);
    void loadDatabase();
    void downloadNextFile();

protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

public slots:
    void tooltipTemperature(QPointF point, bool state);
    void tooltipHumidity(QPointF point, bool state);

     void slotDownloaded(QByteArray data);
     void slotList(QList<QYandexDisk::FileInfo*> list);

private:
    QChart *m_chart;
    Callout *m_tooltip;
    QSplineSeries *m_temperature;
    QSplineSeries *m_humidity;
    QYandexDisk *m_disk;
    QString m_databaseName;
    QList<QYandexDisk::FileInfo*> m_fileList;
    QString m_currentFilename;
    QLabel *m_label;
};

#endif
