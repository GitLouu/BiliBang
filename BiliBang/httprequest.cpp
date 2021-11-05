#include "httprequest.h"

#include <QFile>
#include <QEventLoop>
#include <QThread>

HttpRequest::HttpRequest(QObject *parent) : QObject(parent)
{

    manager = new QNetworkAccessManager(this);

    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::AnyProtocol);

}


void HttpRequest::Get(const QString& url, const QString& pPath, const QString& fname)
{
    QNetworkRequest request;
    request.setSslConfiguration(conf);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, pPath, fname, this] () {
        emit handle(reply->error(), reply->readAll(), pPath, fname);
        reply->deleteLater();
    });
}

void HttpRequest::Get(const QString& url)
{
    QNetworkRequest request;
    request.setSslConfiguration(conf);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this] () {
        emit handle(reply->error(), reply->readAll());
        reply->deleteLater();
    });
}

void HttpRequest::Get(const QStringList& urls)
{
    QNetworkRequest request;
    request.setSslConfiguration(conf);
    QNetworkReply::NetworkError err = QNetworkReply::NoError;
    QList<QByteArray> byteArrays;
    for(int i = 0; i < urls.size(); i++)
    {
        request.setUrl(QUrl(urls.at(i)));
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() != QNetworkReply::NoError)
        {
            err = reply->error();
            break;
        }
        byteArrays.append(reply->readAll());
        reply->deleteLater();
    }
    emit handle(err, byteArrays);
}

void HttpRequest::chkUpd(const QString &url)
{
    QNetworkRequest request;
    request.setSslConfiguration(conf);
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this] () {
        emit hndUpd(reply->readAll());
        reply->deleteLater();
    });
}
