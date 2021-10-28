#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QWidget>
#include <QLabel>

/**
 * @brief 悬浮图片框
 */
class FloatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FloatWindow(QWidget *parent = nullptr);
    /**
     * @brief 设置图片地址，并缩放到固定宽度（图片宽高比例不变）
     * @param fname 图片地址
     */
    void setPic(const QString& fname);

signals:

private:
    QLabel* picLabel = nullptr;
    /**
     * @brief 悬浮框缩放宽度，固定值400
     */
    int winScaleWidth = 400;

};

#endif // FLOATWINDOW_H
