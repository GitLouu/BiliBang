#include "mainwindow.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint); // 去掉窗口最大化
    resize(mainWindowWidth, mainWindowHeigth);

    QVBoxLayout* layout = new QVBoxLayout(this);

    progress = new QProgressBar(this);

    label = new QLabel(this);

    layout->addWidget(progress);
    layout->addWidget(label);

    label->setText("正在更新...");
}

MainWindow::MainWindow(const QString & parentPid, const QString & updJsonUrl, const QString & currVer)
    : MainWindow(nullptr)
{
    this->parentPid = parentPid;
    this->updJsonUrl = updJsonUrl;
    this->currVer = currVer;

    hr = new HttpRequest(this);

    hr->Get(this->updJsonUrl);

    // 函数指针，handle指向HttpRequest::handle函数
    void (HttpRequest::*handleJson)(QNetworkReply::NetworkError, const QByteArray&) = &HttpRequest::handle;
    void (HttpRequest::*handleDownload)(QNetworkReply::NetworkError, const QByteArray&, const QString&, const QString&) = &HttpRequest::handle;

    connect(hr, handleJson, this, &MainWindow::handleJson);
    connect(hr, handleDownload, this, &MainWindow::download);
}

MainWindow::~MainWindow()
{
}

void MainWindow::handleJson(QNetworkReply::NetworkError err, const QByteArray &response)
{
    if (err != QNetworkReply::NoError)
    {
        label->setText("更新失败，网络错误");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonArray all = doc["all"].toArray();
    for (int i = 0; i < all.size(); i++) {
        QJsonValue val = all[i];
        QString version = val["version"].toString();
        if (currVer.compare(version) < 0)
        {
            QJsonArray parts = val["parts"].toArray();
            for (int j = 0; j < parts.size(); j++) {
                QJsonValue pVal = parts[j];
                QString fileName = pVal["path"].toString();
                if (setupFileList.contains(fileName))
                    continue;
                setupFileList.append(fileName);
                downUrlList.append(pVal["url"].toString());
                downMd5List.append(pVal["md5"].toString());
            }
        }
    }
    progress->setMaximum(setupFileList.size() * 2);
    chkDownload();
}

void MainWindow::chkDownload()
{
    progress->setValue(downCount);
    if (downCount >= downUrlList.size()) {
        uncompressAll();
        return;
    }
    QString url = downUrlList.at(downCount);
    hr->Get(url, QCoreApplication::applicationDirPath() + UPD_PATH, url.mid(url.lastIndexOf("/") + 1));
}

void MainWindow::uncompressAll()
{
    // 关闭主程序
    killProcess(parentPid);
    // 让程序等待1秒，等主程序完全退出
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000)
        QCoreApplication::processEvents();
    //

    // TODO 备份旧文件
    // 开始更新
    for (int i = 0; i < setupFileList.size(); i++) {
        QString fileName = setupFileList.at(i);
        int index = fileName.indexOf("/");
        if (index != -1) {
            createDir(QCoreApplication::applicationDirPath() + "/" + fileName.left(index + 1));
        }
        QFile read(downFileList.at(i));
        if (read.open(QIODevice::ReadOnly)) {
            QByteArray unComByte = qUncompress(read.readAll());
            QString setupFullName = QCoreApplication::applicationDirPath() + "/" + fileName;
            QFile write(setupFullName);
            if (write.open(QIODevice::WriteOnly))
            {
                write.write(unComByte);
                write.close();
            }
            read.close();
        }

        progress->setValue(downCount + i + 1);
    }
    // 启动主程序
//    QProcess* process = new QProcess(this);
//    process->start("BiliBang.exe", QStringList());
    QProcess::startDetached("BiliBang.exe", QStringList());
    // 关闭更新程序
    qApp->quit();
    //killProcess(QString::number(QCoreApplication::applicationPid()));
}

void MainWindow::download(QNetworkReply::NetworkError err, const QByteArray& bytes,
                                const QString& pPath, const QString& fname)
{
    if (err != QNetworkReply::NoError) {
        label->setText("更新失败，网络错误");
        return;
    }

    QString calMd5(QCryptographicHash::hash(bytes, QCryptographicHash::Md5).toHex());
    if (calMd5.toUpper() != downMd5List.at(downCount).toUpper()) {
        label->setText("更新失败，文件校验出错");
        return;
    }

    createDir(pPath);
    QString fullName = pPath + fname;
    QFile file(fullName);
    if (file.open(QIODevice::WriteOnly))
    {
        qint64 ret = file.write(bytes);
        file.close();
        if (ret == -1) {
            label->setText("更新失败，文件保存出错");
            return;
        }
    } else {
        label->setText("更新失败，无保存权限");
        return;
    }

    downFileList.append(fullName);
    downCount++;
    chkDownload();
}

bool MainWindow::createDir(const QString & dir)
{
    QDir d(dir);
    if (!d.exists())
        return d.mkpath(d.absolutePath());
    return true;
}

void MainWindow::killProcess(QString pid)
{
    // 杀死进程pid，不杀其开启的子进程
    QStringList arg;
    arg.append("/PID");
    arg.append(pid);
    arg.append("/F");
    QProcess::execute("TASKKILL", arg);
}
