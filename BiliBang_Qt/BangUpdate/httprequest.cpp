#include "httprequest.h"

#include <QTextCodec>
#include <QFile>


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
