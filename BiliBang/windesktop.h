#ifndef WINDESKTOP_H
#define WINDESKTOP_H

#include <QWidget>

#include "windows.h"

bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
void setParentDesktop(QWidget *pWidget);
QString GetProductVer(const QString pFilePath);

bool shutdownSystem();

#endif // WINDESKTOP_H
