#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject* parent = nullptr);

    void replyFinished(QNetworkReply* reply);

    void Get(const QString& url);
    void Get(const QString& url, const QString& pPath, const QString& fname);

    void chkUpd(const QString& url);

signals:
    void handle(QNetworkReply::NetworkError err, const QByteArray& bytes);
    void handle(QNetworkReply::NetworkError err, const QByteArray& bytes, const QString& pPath, const QString& fname);

    void hndUpd(const QByteArray& bytes);

private:
    QNetworkAccessManager* manager;
    QNetworkRequest request;

};

#endif // HTTPREQUEST_H
