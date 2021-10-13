#ifndef LABELPANEL_H
#define LABELPANEL_H

#include <QWidget>
#include <QLabel>

#include "floatwindow.h"

class LabelPanel : public QWidget
{
    Q_OBJECT
public:
    explicit LabelPanel(QWidget *parent = nullptr);

    void setTime(const QString& time);
    void setTitle(const QString& title);
    void setPubIndex(const QString& pubIndex);
    void setUrl(const QString& url);
    void setFname(const QString& fname);

    void setFloatWin(FloatWindow* floatWin);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    QString url;
    QString fname;

    QLabel timeLabel;
    QLabel titleLabel;
    QLabel pubIndexLabel;

    FloatWindow* floatWin;

    bool mouseMoved = false;

    const int SPACE = 50;

};

#endif // LABELPANEL_H
