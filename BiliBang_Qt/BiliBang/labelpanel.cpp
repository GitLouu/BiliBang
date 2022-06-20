#include "labelpanel.h"

#include <QGridLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>

LabelPanel::LabelPanel(QWidget *parent) : QWidget(parent), floatWin(nullptr)
{

    QGridLayout* layout = new QGridLayout(this);

    timeLabel.setText("00:00");
    titleLabel.setText("xxxxxxxx");
    pubIndexLabel.setText("第xx话");

    layout->addWidget(&timeLabel, 0, 0);
    layout->addWidget(&titleLabel, 0, 1);
    layout->addWidget(&pubIndexLabel, 1, 1);

    setLayout(layout);

}

void LabelPanel::enterEvent(QEvent *event)
{
    if(floatWin)
    {
        QFile f(fname);
        if (f.exists()) {
            floatWin->setPic(fname);
            floatWin->show();
            QSize fSize = floatWin->size();
            QList<QScreen *> ss = QGuiApplication::screens();
            QSize sSize = ss[0]->availableGeometry().size();
            QPoint mPoint = QCursor::pos();
            int posX = mPoint.x() + SPACE;
            int posY = mPoint.y();
            if (mPoint.x() + fSize.width() + SPACE > sSize.width())
            {
                posX = mPoint.x() - fSize.width() - SPACE;
            }
            if (mPoint.y() + fSize.height() > sSize.height())
            {
                posY = sSize.height() - fSize.height();
            }
            floatWin->move(posX, posY);
        }
    }
    QWidget::enterEvent(event);
}
void LabelPanel::leaveEvent(QEvent *event)
{
    if(floatWin)
    {
        floatWin->hide();
    }
    QWidget::leaveEvent(event);
}

void LabelPanel::mouseReleaseEvent(QMouseEvent *event)
{
    mouseMoved = false;
    QWidget::mouseReleaseEvent(event);
}

void LabelPanel::mouseMoveEvent(QMouseEvent *event)
{
    mouseMoved = true;
    QWidget::mouseMoveEvent(event);
}

void LabelPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!mouseMoved && event->button() == Qt::LeftButton)
        QDesktopServices::openUrl(QUrl(url));
    QWidget::mouseDoubleClickEvent(event);
}

void LabelPanel::setTitle(const QString &title)
{
    titleLabel.setText(title);
    titleLabel.setToolTip(title);
}

void LabelPanel::setTime(const QString &time)
{
    timeLabel.setText(time);
}

void LabelPanel::setPubIndex(const QString &pubIndex)
{
    pubIndexLabel.setText(pubIndex);
}

void LabelPanel::setFname(const QString& fname)
{
    this->fname = fname;
}

void LabelPanel::setUrl(const QString &url)
{
    this->url = url;
}

void LabelPanel::setFloatWin(FloatWindow* floatWin)
{
    this->floatWin = floatWin;
}
