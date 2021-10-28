#include "floatwindow.h"

#include <QBoxLayout>

FloatWindow::FloatWindow(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    picLabel = new QLabel(this);
    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
    boxLayout->addWidget(picLabel);
    boxLayout->setSpacing(0);
    boxLayout->setMargin(0);
    picLabel->show();
    setLayout(boxLayout);
}

void FloatWindow::setPic(const QString &fname)
{
    QPixmap pix(fname);
    picLabel->setPixmap(pix.scaledToWidth(winScaleWidth, Qt::SmoothTransformation));
}
