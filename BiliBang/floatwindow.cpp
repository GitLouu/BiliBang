#include "floatwindow.h"

#include <QBoxLayout>

FloatWindow::FloatWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    picLabel = new QLabel(this);
    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
    boxLayout->addWidget(picLabel);
    boxLayout->setSpacing(0);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(boxLayout);
}

void FloatWindow::setPic(const QString &fname)
{
    QPixmap pix(fname);
    pix.setDevicePixelRatio(devicePixelRatio());
    picLabel->setPixmap(pix.scaledToWidth(winScaleWidth, Qt::SmoothTransformation));
}
