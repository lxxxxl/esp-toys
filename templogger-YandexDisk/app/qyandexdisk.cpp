#include "qyandexdisk.h"

QYandexDisk::QYandexDisk(QString token, QObject *parent) : QObject(parent)
{
    this->token = token;

    m_networkAccessManager = new QNetworkAccessManager(this);
    // allow processing HTTP redirects
    m_networkAccessManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}


void QYandexDisk::download(QString filename)
{
    QNetworkReply* reply = this->m_networkAccessManager->get(
                createRequest("resources/download", filename)
                );
    reply->setProperty("type", rtDownload1);
}

// upload file to disk
void QYandexDisk::upload(QString filename, QByteArray data)
{
    QNetworkReply* reply = this->m_networkAccessManager->get(
                createRequest("resources/upload", filename)
                );
    reply->setProperty("data", data);
    reply->setProperty("type", rtUpload1);
}

// remove file or directory from disk
void QYandexDisk::remove(QString path)
{
    QNetworkReply* reply = this->m_networkAccessManager->deleteResource(
                createRequest("resources", path)
                );
    reply->setProperty("type", rtRemove);
}

// create new directory
void QYandexDisk::mkdir(QString path)
{
    QNetworkReply* reply = this->m_networkAccessManager->put(
                createRequest("resources", path),
                QByteArray()
                );
    reply->setProperty("type", rtMkdir);
}

// list files in directory
void QYandexDisk::list(QString path)
{
    QNetworkReply* reply = this->m_networkAccessManager->get(
                createRequest("resources", path)
                );
    reply->setProperty("type", rtList);
}

// get file size
void QYandexDisk::size(QString path)
{
    QNetworkReply* reply = this->m_networkAccessManager->get(
                createRequest("resources", path)
                );
    reply->setProperty("type", rtSize);
}
// get free space count on Disk
void QYandexDisk::capacity()
{
    QNetworkReply* reply = this->m_networkAccessManager->get(
                createRequest(QString(), QString())
                );
    reply->setProperty("type", rtCapacity);
}

QNetworkRequest QYandexDisk::createRequest(QString method, QString arg)
{

    QString url = baseUrl + method;
    if (arg.length() > 0)
        url += "?path=" + arg;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", QString("OAuth " + this->token).toLocal8Bit());
    return request;
}

// slot for processing all responses
void QYandexDisk::finished(QNetworkReply *reply)
{
    RequestType type = (RequestType)reply->property("type").toUInt();
    switch (type) {
    case rtDownload1:
        finishedDownloadPhase1(reply);
        break;
    case rtDownload2:
        finishedDownloadPhase2(reply);
        break;
    case rtUpload1:
        finishedUploadPhase1(reply);
        break;
    case rtUpload2:
        finishedUploadPhase2(reply);
        break;
    case rtRemove:
        finishedRemove(reply);
        break;
    case rtMkdir:
        finishedMkdir(reply);
        break;
    case rtList:
        finishedList(reply);
        break;
    case rtSize:
        finishedSize(reply);
        break;
    case rtCapacity:
        finishedCapacity(reply);
        break;
    }
    reply->deleteLater();
}

// slot for processing "capacity" API request
void QYandexDisk::finishedCapacity(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != HTTP_OK){
        emit signalError();
        return;
    }

    QByteArray resp = reply->readAll();
    // parse JSON
    QJsonDocument replyDoc = QJsonDocument::fromJson(resp);
    QJsonObject rootObject = replyDoc.object();

    CapacityInfo info;
    info.totalSpace = rootObject.value("total_space").toDouble(0);
    info.trashSize = rootObject.value("trash_size").toDouble(0);
    info.usedSpace = rootObject.value("used_space").toDouble(0);
    emit signalCapacity(&info);

}

// slot for processing "delete" API request
void QYandexDisk::finishedRemove(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    emit signalRemoved((status == HTTP_NO_CONTENT) || (status == HTTP_ACCEPTED));
}

// slot for processing "create-folder" API request
void QYandexDisk::finishedMkdir(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    emit signalCreated(status == HTTP_CREATED);
}

// slot for processing "upload" API request phase 1
void QYandexDisk::finishedUploadPhase1(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != HTTP_OK){
        emit signalError();
        return;
    }

    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    QJsonObject obj = doc.object();

    QString href = obj.value("href").toString();
    QNetworkRequest request;
    request.setUrl(QUrl(href));

    QByteArray data = reply->property("data").toByteArray();
    QNetworkReply *reply2 = this->m_networkAccessManager->put(request, data);
    reply2->setProperty("type", rtUpload2);
}

// slot for processing "upload" API request phase 2
void QYandexDisk::finishedUploadPhase2(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    emit signalUploaded(status == HTTP_CREATED);
}

// slot for processing "download" API request phase 1
void QYandexDisk::finishedDownloadPhase1(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != HTTP_OK){
        emit signalError();
        return;
    }

    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    QJsonObject obj = doc.object();

    QString href = obj.value("href").toString();
    QNetworkRequest request;
    request.setUrl(QUrl(href));

    QNetworkReply *reply2 = this->m_networkAccessManager->get(request);
    reply2->setProperty("type", rtDownload2);
}

// slot for processing "download" API request phase 2
void QYandexDisk::finishedDownloadPhase2(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status == HTTP_OK)
        emit signalDownloaded(reply->readAll());
    else
        emit signalError();
}

// slot for processing "size" API request
void QYandexDisk::finishedSize(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != HTTP_OK){
        emit signalError();
        return;
    }

    QByteArray resp = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(resp);
    QJsonObject obj = doc.object();

    if (!obj.contains("size")){
        emit signalError();
        return;
    }

    qint64 size = obj.value("size").toDouble(0);
    emit signalSize(size);
}

// slot for processing "list" API request
void QYandexDisk::finishedList(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != HTTP_OK){
        emit signalError();
        return;
    }

    QList<FileInfo*> list;

    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    QJsonObject obj = doc.object();

    QJsonObject _embedded = obj.value("_embedded").toObject();
    QJsonArray items = _embedded.value("items").toArray();

    for (const QJsonValue& item: qAsConst(items)){
        QJsonObject object = item.toObject();
        FileInfo *info = new FileInfo;
        QString type = object.value("type").toString();
        info->isDir = (type == "dir");
        if (info->isDir){
            info->md5 = QString();
            info->size = 0;
        }
        else{
            info->md5 = object.value("md5").toString();
            info->size = object.value("size").toDouble();
        }
        info->name = object.value("name").toString();
        info->path = object.value("path").toString();
        QString created = object.value("created").toString();
        info->created = QDateTime::fromString(created, Qt::ISODate);
        info->modified = QDateTime::fromString(object.value("modified").toString(), Qt::ISODate);

        list.append(info);
    }
    emit signalList(list);
}
