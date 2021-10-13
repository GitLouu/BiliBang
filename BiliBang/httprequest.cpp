#include "httprequest.h"

#include <QTextCodec>
#include <QFile>


HttpRequest::HttpRequest(QObject *parent) : QObject(parent)
{

    manager = new QNetworkAccessManager(this);

    QSslConfiguration conf;
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(conf);
}


void HttpRequest::Get(const QString& url, const QString& pPath, const QString& fname)
{
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [reply, pPath, fname, this] () {
        emit handle(reply->error(), reply->readAll(), pPath, fname);
        reply->deleteLater();
    });
}

void HttpRequest::Get(const QString& url)
{
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [reply, this] () {
        emit handle(reply->error(), reply->readAll());
        reply->deleteLater();
    });
}

void HttpRequest::chkUpd(const QString &url)
{
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [reply, this] () {
        emit hndUpd(reply->readAll());
        reply->deleteLater();
    });
}
