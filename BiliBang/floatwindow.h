#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QWidget>
#include <QLabel>

class FloatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FloatWindow(QWidget *parent = nullptr);

    void setPic(const QString& fname);

signals:

private:
    QLabel* picLabel;
    int winScaleWidth = 400;

};

#endif // FLOATWINDOW_H
