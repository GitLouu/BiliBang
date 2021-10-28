#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = a.arguments();

    if (args.size() < 4) {
        return -1;
    }
    QString parentPid = args.at(1); // 父进程id，at(0)是本程序路径
    QString updateJsonUrl = args.at(2); // 升级相关json文件url
    QString currVersion = args.at(3); // 待升级程序当前版本
    MainWindow w(parentPid, updateJsonUrl, currVersion);
    w.show();
    return a.exec();
}
