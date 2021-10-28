#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
/**
 * @brief http 请求，包括获取请求内容，下载文件
 */
class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject* parent = nullptr);

    /**
     * @brief 获取请求内容，发射信号 handle(QNetworkReply::NetworkError err, const QByteArray& bytes)
     * @param url
     */
    void Get(const QString& url);
    /**
     * @brief 下载文件，发射信号 handle(QNetworkReply::NetworkError err, const QByteArray& bytes, const QString& pPath, const QString& fname)
     * @param url 下载url
     * @param pPath 保存路径
     * @param fname 文件名
     */
    void Get(const QString& url, const QString& pPath, const QString& fname);

signals:
    void handle(QNetworkReply::NetworkError err, const QByteArray& bytes);
    void handle(QNetworkReply::NetworkError err, const QByteArray& bytes, const QString& pPath, const QString& fname);

private:
    QNetworkAccessManager* manager = nullptr;
    QSslConfiguration conf;

};

#endif // HTTPREQUEST_H
