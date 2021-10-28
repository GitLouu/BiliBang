#ifndef WINDESKTOP_H
#define WINDESKTOP_H

#include <QWidget>

#include "windows.h"

bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
void setParentDesktop(QWidget *pWidget);
QString GetProductVer(const QString pFilePath);
/**
 * @brief 关闭系统（计算机）
 * @return
 */
bool powerOff();

#endif // WINDESKTOP_H
