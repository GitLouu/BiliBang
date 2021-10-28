#ifndef LABELPANEL_H
#define LABELPANEL_H

#include <QWidget>
#include <QLabel>

#include "floatwindow.h"

/**
 * @brief 标签面板（用来渲染动画名称、第几话、播出时间等）
 */
class LabelPanel : public QWidget
{
    Q_OBJECT
public:
    explicit LabelPanel(QWidget *parent = nullptr);

    void setTime(const QString& time);
    void setTitle(const QString& title);
    void setPubIndex(const QString& pubIndex);
    /**
     * @brief 设置动画播放地址
     * @param url
     */
    void setUrl(const QString& url);
    /**
     * @brief 设置动画封面的图片文件地址
     * @param fname
     */
    void setFname(const QString& fname);
    void setFloatWin(FloatWindow* floatWin);

protected:
    // 控制鼠标悬浮事件来触发悬浮框
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    /**
     * @brief 鼠标点击打开的url
     */
    QString url;
    /**
     * @brief （悬浮）图片地址
     */
    QString fname;

    QLabel timeLabel;
    QLabel titleLabel;
    QLabel pubIndexLabel;

    FloatWindow* floatWin = nullptr;

    bool mouseMoved = false;

    const int SPACE = 50;

};

#endif // LABELPANEL_H
