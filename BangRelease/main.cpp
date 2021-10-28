#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QDebug>

QString md5(QString filePath)
{
    QFile file(filePath);
    QString calMd5;
    if (file.open(QIODevice::ReadOnly)) {
        calMd5 = (QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex());
        file.close();
    }
    return calMd5.toUpper();
}

QString compress(QString filePath, QString releasePath)
{
    QString fname = filePath.mid(filePath.lastIndexOf("/") + 1);
    QString qzName = releasePath + fname + ".qz";
    QFile file2(filePath);
    if (file2.open(QIODevice::ReadOnly)) {
        QByteArray comByte = qCompress(file2.readAll());
        QFile file3(qzName);
        if (file3.open(QIODevice::WriteOnly)) {
            file3.write(comByte);
            file3.close();
        }
        file2.close();
    }
    return qzName;
}

void releaseData()
{
    QString basePath("D:/QtProjects/release-BiliBang-template/");
    QString releasePath("D:/QtProjects/release/");
    QDir dir(releasePath);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QString version("1.1.2.3");

    QStringList list;
    list.append("BiliBang.exe"); // 更新的文件

    qDebug() << "\"version\": " << version << ",";
    for(int i = 0; i < list.size(); i++)
    {
        QString path = list.at(i);
        qDebug() << "\"path\": " << path << ",";
        QString url = compress(basePath + path, releasePath);
        qDebug() << "\"url\": " << url << ",";
        qDebug() << "\"md5\": " << md5(url) << "";
    }

}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    //return a.exec();

    releaseData();
    return 0;
}

