#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QSystemTrayIcon>

#include "labelpanel.h"
#include "httprequest.h"
#include "bang.h"
#include "floatwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);

    void timerEvent(QTimerEvent *event);

private:
    HttpRequest* hr = nullptr;

    QTimer* shutdownTimer;

    QSystemTrayIcon* tray = nullptr;
    FloatWindow* floatWin = nullptr;
    QVector<LabelPanel*> panels;
    LabelPanel* initPanel = nullptr;

    QLabel* dateLabel = nullptr;
    QWidget* panelWidget = nullptr;

    QDialog* dia = nullptr;

    QAction* colorAct = nullptr;
    int colorIndex = 0;
    QVector<QAction*> colorActions;

    bool mousePressed;
    bool windowMoved;
    QPoint beginPos;

    bool autoStarted = false;

    int mainWindowWidth = 300;
    int mainWindowHeigth = 400;
    int topButtonWidth = 30;
    int topWidgetHeight = 50;
    int panelHeight = 30;

    int todayIndex = 0;
    int currIndex = 0;
    int currLine = 0;

    bool shutdownSecsEnabled = false;
    int shutdownSecs = -1;

    QString version = "1.1.1.2";

    int timerId = -1;
    const static int REREQUEST_TIMER = 2000;

    const QString WEEK_STR[8] {"", "周一", "周二", "周三", "周四", "周五", "周六", "周日"};

    const QString COVER_PATH = "/temp/cover/";

    const QString CONF_PATH = "/conf/";
    const QString CONF = "conf.json";
    const QString ICON = "icon.png";
    const QString DONATE = "donate.png";

    const static int PRESET_COLOR_COUNT = 10;
    const QString PRESET_COLOR_NAME[PRESET_COLOR_COUNT] {"预设", "浅粉", "柠檬", "米黄",
                                        "沙褐", "天蓝", "卡其", "木槿",
                                        "透明", "透反"};
    const QColor PRESET_COLOR[PRESET_COLOR_COUNT] {QColor(255,240,245), QColor(255,217,230),
                                QColor(204,255,0), QColor(245,245,220),
                                QColor(244,164,96), QColor(135,206,250),
                                QColor(153,107,31), QColor(102,64,255),
                                QColor(255,255,255,0), QColor(0,0,0,0)};

    const static int PRESET_ST_LEN = 10;
    const QString PRESET_ST[PRESET_ST_LEN] {"一切都是时辰的错", "你为什么这么熟练啊",
                                            "今天的风儿好喧嚣啊", "教练，我想打篮球",
                                            "只要微笑就可以了", "我秃了，也变强了",
                                            "但是我拒绝", "这是我爸爸在夏威夷教我的",
                                            "秘技：反复横跳", "不是假发，是桂"};
    const QString PRESET_ST_FACE[PRESET_ST_LEN] {"(′д｀σ)σ", "(ノへ￣、)",
                                                 "(￣、￣)", "○|￣|_",
                                                 "（￣︶￣）*", "(⓿_⓿)",
                                                 "(￣_,￣ )", "O(∩_∩)O",
                                                 "┏ (゜ω゜)=", "（￣。。￣）"};


    QVector<QString> urlLines;
    QVector<Bangumi*> bangumis;


    void initTrayMenu();
    void initTopWidget();
    void initPanelWidget();
    void initHttpRequest();
    void initConfigs();

    void clearBangumis();
    QString getWeek(int week);
    void drawPanels(int index);
    template<typename T>
    void clearVector(QVector<T*>* vec);

    void handleResponse(QNetworkReply::NetworkError err, const QByteArray& response);
    void handleDownload(QNetworkReply::NetworkError err, const QByteArray& response, const QString& pPath, const QString& fname);
    void handleUpdate(const QByteArray& response);
    void quit();
    void goLeft();
    void goRight();
    void switchLine();
    void readTimeLine();

    void saveConf();
    void moveCenter();

};
#endif // MAINWINDOW_H
