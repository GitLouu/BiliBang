#include "mainwindow.h"
#include "windesktop.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QSharedMemory sm;
    sm.setKey("BiliBang-fsmxm-666");

    if (sm.attach())
    {
        return 0;
    }

    if (sm.create(1))
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        setParentDesktop(&w);
        return a.exec();
    }

    return 0;
}
