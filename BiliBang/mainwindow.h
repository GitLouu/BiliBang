#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QLineEdit>

#include "labelpanel.h"
#include "httprequest.h"
#include "bang.h"
#include "floatwindow.h"
// 主窗口
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    // 鼠标拖拽移动事件
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    /**
     * @brief 定时器事件 继承自 QObject
     * @param event
     */
    void timerEvent(QTimerEvent *event);

private:
    /**
     * @brief http request
     */
    HttpRequest* hr = nullptr;
    /**
     * @brief 自动关机Timer
     */
    QTimer* powerOffTimer = nullptr;
    /**
     * @brief 系统托盘图标
     */
    QSystemTrayIcon* tray = nullptr;
    /**
     * @brief 悬浮图片框
     */
    FloatWindow* floatWin = nullptr;
    /**
     * @brief 初始显示的内容（主要提示用户，读取中稍后）
     */
    LabelPanel* initPanel = nullptr;
    /**
     * @brief 日期标签
     */
    QLabel* dateLabel = nullptr;
    /**
     * @brief 主面板，放置动画panel
     */
    QWidget* panelWidget = nullptr;
    /**
     * @brief 弹出对话框，包括关于，定时关机，检查更新
     */
    QDialog* dia = nullptr;
    /**
     * @brief 托盘菜单 - 设置颜色
     */
    QAction* colorAct = nullptr;
    /**
     * @brief 当前选择的面板颜色
     */
    int colorIndex = 0;
    /**
     * @brief vector 存放所有动画panel
     */
    QVector<LabelPanel*> panels;
    /**
     * @brief vector 存放所有设置颜色
     */
    QVector<QAction*> colorActions;
    /**
     * @brief url lines
     */
    QVector<QString> urlLines;
    /**
     * @brief vector bangumis
     */
    QVector<Bangumi*> bangumis;
    // 拖拽移动相关
    bool mousePressed;
    bool windowMoved;
    QPoint beginPos;
    /**
     * @brief 是否开机自启
     */
    bool autoStarted = false;
    // 窗体，面板等宽度
    int mainWindowWidth = 300;
    int mainWindowHeigth = 400;
    int topButtonWidth = 30;
    int topWidgetHeight = 50;
    int panelHeight = 30;
    // 读取动画索引
    int todayIndex = 0;
    int currIndex = 0;
    int currLine = 0;
    /**
     * @brief 定时关机剩余秒数
     */
    int powerOffSecsLeft = -1;
    /**
     * @brief 软件版本
     */
    QString version = "1.1.2.0";
    /**
     * @brief timerId -1，无自动访问timer，网络正常；其他，有自动访问timer，网络不正常。
     */
    int timerId = -1;
    /**
     * @brief 网络有问题时，自动重新访问的时间，2秒
     */
    const static int REREQUEST_TIME = 2000;
    // 周几
    const QString WEEK_STR[8] {"", "周一", "周二", "周三", "周四", "周五", "周六", "周日"};
    // 地址 -
    const QString TEMP_PATH = "/temp/";
    const QString COVER_PATH = TEMP_PATH + "cover/";
    const QString CONF_PATH = "/conf/";
    const QString CONF = "conf.json";
    const QString ICON = "icon.png";
    const QString DONATE = "donate.png";
    const QString DONATE_PIC_URL = "https://gitee.com/GiteeLou/pub/raw/master/donate/donate.png";
    const QString CHK_UPD_URL = "https://gitee.com/GiteeLou/pub/raw/master/BiliBang/pub.json";
    const QString BANGUMI_URL_GLOLBAL = "https://bangumi.bilibili.com/web_api/timeline_global";
    const QString BANGUMI_URL_CN = "https://bangumi.bilibili.com/web_api/timeline_cn";
    // 设置颜色相关
    const static int PRESET_COLOR_COUNT = 10;
    const QString PRESET_COLOR_NAME[PRESET_COLOR_COUNT] {"预设", "浅粉", "柠檬", "米黄",
                                        "沙褐", "天蓝", "卡其", "木槿",
                                        "透明", "透反"};
    const QColor PRESET_COLOR[PRESET_COLOR_COUNT] {QColor(255,240,245), QColor(255,217,230),
                                QColor(204,255,0), QColor(245,245,220),
                                QColor(244,164,96), QColor(135,206,250),
                                QColor(153,107,31), QColor(102,64,255),
                                QColor(255,255,255,0), QColor(0,0,0,0)};
    // 无动画信息时显示内容
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

    // init functions
    void initTrayMenu();
    void initTopWidget();
    void initPanelWidget();
    void initHttpRequest();
    void initConfigs();
    void initFloatWindow();
    //
    QString getWeek(int week);
    void drawPanels(int index);
    void clearBangumis();
    template<typename T>
    void clearVector(QVector<T*>* vec);
    /**
     * @brief 处理网络访问内容
     * @param err
     * @param response
     */
    void handleResponse(QNetworkReply::NetworkError err, const QByteArray& response);
    /**
     * @brief 处理网络下载文件
     * @param err
     * @param response
     * @param pPath
     * @param fname
     */
    void handleDownload(QNetworkReply::NetworkError err, const QByteArray& response, const QString& pPath, const QString& fname);
    /**
     * @brief 处理检查更新
     * @param response
     */
    void handleUpdate(const QByteArray& response);
    void quit();
    void goLeft();
    void goRight();
    void switchLine();
    void readTimeLine();
    void saveConf();
    // 居中显示（居于屏幕中间）
    void moveCenter();
    // //
    void handlePowerOff();

};
#endif // MAINWINDOW_H
