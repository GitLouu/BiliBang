#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QMutex>

#include "httprequest.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(const QString & parentPid, const QString & updJsonUrl, const QString & currVer);
    virtual ~MainWindow();

signals:

private:
    // 进度条
    QProgressBar* progress = nullptr;
    // 文本提示
    QLabel* label = nullptr;

    HttpRequest* hr = nullptr;

    QString parentPid;
    QString updJsonUrl;
    QString currVer;

    /**
     * @brief 要升级的文件名列表(文件实际安装位置）
     */
    QStringList setupFileList;
    /**
     * @brief 下载文件url的列表
     */
    QStringList downUrlList;
    /**
     * @brief 下载文件的md5列表
     */
    QStringList downMd5List;
    /**
     * @brief 已下载文件的列表
     */
    QStringList downFileList;
    // 下载完成文件计数
    int downCount = 0;

    QMutex mutex;

    // 窗体，面板等宽度
    int mainWindowWidth = 300;
    int mainWindowHeigth = 100;

    const QString TEMP_PATH = "/temp/";
    const QString UPD_PATH = TEMP_PATH + "upd/";

    void handleJson(QNetworkReply::NetworkError err, const QByteArray &response);
    void download(QNetworkReply::NetworkError err, const QByteArray& bytes, const QString& pPath, const QString& fname);
    /**
     * @brief 下载和检查
     */
    void chkDownload();
    /**
     * @brief 解压所有文件
     */
    void uncompressAll();
    /**
     * @brief 创建目录，true 创建成功或已经存在，false创建失败
     * @return true or false
     */
    bool createDir(const QString & dir);

    void killProcess(QString pid);

};

#endif // MAINWINDOW_H
