#include "mainwindow.h"

#include <QMenu>
#include <QApplication>
#include <QSizePolicy>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDir>
#include <QDialog>
#include <QGuiApplication>
#include <QScreen>
#include <QPalette>
#include <QPainter>
#include <QSettings>
#include <QLineEdit>
#include <QIntValidator>
#include <QTimer>
#include "windesktop.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    resize(mainWindowWidth, mainWindowHeigth);
    setPalette(QPalette(PRESET_COLOR[0]));
    setAutoFillBackground(true);

    moveCenter();

    initHttpRequest();

    initTrayMenu();

    initTopWidget();

    initPanelWidget();

    floatWin = new FloatWindow;

    initConfigs();

    readTimeLine();
}

MainWindow::~MainWindow()
{
    delete floatWin;
    delete initPanel;
    clearBangumis();
    clearVector(&panels);
    clearVector(&colorActions);
    delete shutdownTimer;
}

void MainWindow::initTrayMenu()
{
    shutdownTimer = new QTimer(this);
    connect(shutdownTimer, &QTimer::timeout, [this] {
        if (!shutdownSecsEnabled)
            return;
        if (--shutdownSecs > 0)
            return;
        shutdownSystem();
    });

    tray = new QSystemTrayIcon(this);
    QFile iFile(QCoreApplication::applicationDirPath().append(CONF_PATH).append(ICON));
    QImage image(20, 20, QImage::Format_ARGB32);
    if (!iFile.exists())
    {
        memset(image.bits(), 0x00, image.sizeInBytes());
        QPainter painter(&image);
        painter.setPen(Qt::red);
        QFont font = painter.font();
        font.setPixelSize(23);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(QRect(3, 0, 20, 20), Qt::AlignJustify, "B");
    }
    else
    {
        if (iFile.open(QIODevice::ReadOnly))
        {
            image.loadFromData(iFile.readAll());
            iFile.close();
        }
    }
    QIcon icon(QPixmap::fromImage(image));
    tray->setIcon(icon);
    tray->setToolTip("bilibili新番时间线");
    tray->show();

    QMenu* menu = new QMenu(this);

    QAction* about = new QAction(menu);
    about->setText("关于/捐助");
    hr->Get("https://gitee.com/fsmxm/pub/raw/master/donate/donate.png", QCoreApplication::applicationDirPath().append(CONF_PATH), DONATE);
    connect(about, &QAction::triggered, [this, icon] () {
        if (dia)
            dia->close();
        dia = new QDialog(this, Qt::WindowStaysOnTopHint);
        dia->setWindowTitle("关于/捐助");
        QGridLayout* glay = new QGridLayout;
        dia->setLayout(glay);

        QLabel* ver = new QLabel(dia);
        QString verTxt = "版本：";
        ver->setText(verTxt.append(version));
        ver->setOpenExternalLinks(true);
        glay->addWidget(ver, 0, 0);

        QLabel* qtLabel = new QLabel(dia);
        qtLabel->setText("<a href='https://www.qt.io'>Qt</a>");
        qtLabel->setToolTip("访问Qt官网");
        qtLabel->setOpenExternalLinks(true);
        glay->addWidget(qtLabel, 1, 0);

        QLabel* lgplLabel = new QLabel(dia);
        lgplLabel->setText("<a href='https://www.gnu.org/licenses/lgpl-3.0.html'>LGPL-V3</a>");
        lgplLabel->setOpenExternalLinks(true);
        lgplLabel->setToolTip("访问LGPL开源协议");
        glay->addWidget(lgplLabel, 1, 1);

        QLabel* tipLabel = new QLabel(dia);
        tipLabel->setText("❤ 喜欢的话可以捐助哦");
        tipLabel->setStyleSheet("color: green;");
        glay->addWidget(tipLabel, 2, 0, 1, 2);

        QLabel* donateLabel = new QLabel(dia);
        donateLabel->setPixmap(QPixmap(QCoreApplication::applicationDirPath().append(CONF_PATH).append(DONATE)).scaledToWidth(200));
        glay->addWidget(donateLabel, 3, 0, 1, 2);

        dia->open();

        connect(dia, &QDialog::finished, [this](){
            delete dia;
            dia = nullptr;
        });
    });
    menu->addAction(about);

    QAction* ckUpd = new QAction(menu);
    ckUpd->setText("检查更新");
    connect(ckUpd, &QAction::triggered, [this](){
        hr->chkUpd("https://gitee.com/fsmxm/pub/raw/master/BiliBang/pub.json");
    });
    menu->addAction(ckUpd);

    QMenu* colorMenu = new QMenu(menu);
    colorMenu->setTitle("设置颜色");
    colorMenu->setToolTip("设置挂件面板颜色");
    menu->addMenu(colorMenu);

    for (int i = 0; i < PRESET_COLOR_COUNT; i++)
    {
        QAction* color = new QAction(PRESET_COLOR_NAME[i], colorMenu);
        color->setCheckable(true);
        connect(color, &QAction::triggered, [this, color, i](){
            if (this->colorAct == color)
            {
                this->colorAct->setChecked(true);
                return;
            }
            this->setPalette(QPalette(PRESET_COLOR[i]));
            this->colorAct->setChecked(false);
            this->colorAct = color;
            this->colorIndex = i;
            this->saveConf();
        });
        colorMenu->addAction(color);
        colorActions.append(color);
    }

    colorAct = colorActions.at(0);
    colorAct->setChecked(true);


    QAction* center = new QAction(menu);
    center->setText("复位");
    connect(center, &QAction::triggered, this, &MainWindow::moveCenter);
    menu->addAction(center);

    QSettings* reg = new
            QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                      QSettings::NativeFormat, this);
    QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    QString appName = QFileInfo(appPath).baseName();
    autoStarted = reg->value(appName).toString() == appPath;

    QAction* autoRun = new QAction(menu);
    autoRun->setText("开机启动");
    autoRun->setCheckable(true);
    autoRun->setChecked(autoStarted);
    connect(autoRun, &QAction::triggered, [reg, this, appPath, appName](){
        if (autoStarted)
            reg->remove(appName);
        else
            reg->setValue(appName, appPath);
        autoStarted = !autoStarted;
    });
    menu->addAction(autoRun);

    QAction* shutdown = new QAction(menu);
    shutdown->setText("定时关机");
    connect(shutdown, &QAction::triggered, [this](){
        if (dia)
            dia->close();
        dia = new QDialog(this, Qt::WindowStaysOnTopHint);
        dia->setWindowTitle("定时关机");
        QGridLayout* glay = new QGridLayout;
        dia->setLayout(glay);

        QLineEdit* hourEdit = new QLineEdit(dia);
        hourEdit->setText("0");
        hourEdit->setValidator(new QIntValidator(0, 23, hourEdit));
        glay->addWidget(hourEdit, 0, 0);


        QLabel* lgplLabel = new QLabel(dia);
        lgplLabel->setText("小时");
        glay->addWidget(lgplLabel, 0, 1);

        QLineEdit* minEdit = new QLineEdit(dia);
        minEdit->setText("0");
        minEdit->setValidator(new QIntValidator(0, 59, minEdit));
        glay->addWidget(minEdit, 0, 2);

        QLabel* tipLabel = new QLabel(dia);
        tipLabel->setText("分钟后关机");
        glay->addWidget(tipLabel, 0, 3);

        QPushButton* yesBut = new QPushButton("定时关机", dia);
        glay->addWidget(yesBut, 1, 0, 1, 2);

        QPushButton* cancelBut = new QPushButton("取消关机", dia);
        glay->addWidget(cancelBut, 1, 2, 1, 2);

        if (shutdownSecsEnabled)
        {
            hourEdit->setText(QString::number(shutdownSecs / 3600));
            minEdit->setText(QString::number((shutdownSecs % 3600) / 60));
        }

        connect(yesBut, &QPushButton::released, [this, hourEdit, minEdit]{
            int hour = hourEdit->text().toInt();
            int min = minEdit->text().toInt();
            if (hour * 60 + min > 0)
            {
                shutdownSecs = (hour * 60 + min) * 60;
                shutdownSecsEnabled = true;
                shutdownTimer->start(1000);
                dia->close();
            }
        });
        connect(cancelBut, &QPushButton::released, [this]{
            shutdownSecs = -1;
            shutdownSecsEnabled = false;
            shutdownTimer->stop();
            dia->close();
        });


        dia->open();

        connect(dia, &QDialog::finished, [this](){
            delete dia;
            dia = nullptr;
        });
    });
    menu->addAction(shutdown);

    QAction* quit = new QAction(menu);
    quit->setText("退出");
    connect(quit, &QAction::triggered, this, &MainWindow::quit);
    menu->addAction(quit);

    tray->setContextMenu(menu);

}

void MainWindow::initTopWidget()
{
    QWidget* topWidget = new QWidget(this);
    QGridLayout* topLayout = new QGridLayout(topWidget);

    QPushButton* leftButton = new QPushButton("<", this);
    leftButton->setFlat(true);
    leftButton->setFixedWidth(topButtonWidth);
    leftButton->setToolTip("上一页");
    topLayout->addWidget(leftButton, 0, 0);
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::goLeft);

    dateLabel = new QLabel("00-00(周x)", topWidget);
    topLayout->addWidget(dateLabel, 0, 1, 1, 2, Qt::AlignHCenter);

    QPushButton* rightButton = new QPushButton(">", topWidget);
    rightButton->setFlat(true);
    rightButton->setFixedWidth(topButtonWidth);
    rightButton->setToolTip("下一页");
    topLayout->addWidget(rightButton, 0, 3);
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::goRight);

    QPushButton* bButton = new QPushButton("⇵", topWidget);
    bButton->setFlat(true);
    bButton->setFixedWidth(topButtonWidth);
    bButton->setToolTip("切换日番/国创时间线");
    topLayout->addWidget(bButton, 0, 4);
    connect(bButton, &QPushButton::clicked, this, &MainWindow::switchLine);

    topWidget->resize(mainWindowWidth, topWidgetHeight);
    topWidget->show();
}

void MainWindow::initPanelWidget()
{
    panelWidget = new QWidget(this);
    panelWidget->resize(mainWindowWidth, mainWindowHeigth - topWidgetHeight);
    panelWidget->move(0, topWidgetHeight);

    initPanel = new LabelPanel(panelWidget);
    initPanel->setTime("     ");
    initPanel->setTitle("正在读取...");
    initPanel->setPubIndex("请稍后");
    initPanel->setFname("");
    initPanel->setUrl("");
    initPanel->move(0, 0);
    initPanel->show();
    panelHeight = initPanel->size().height();

    resize(mainWindowWidth, panelHeight + topWidgetHeight);
    panelWidget->resize(mainWindowWidth, panelHeight);
}

void MainWindow::initHttpRequest()
{
    hr = new HttpRequest(this);

    void (HttpRequest::*handleJson)(QNetworkReply::NetworkError, const QByteArray&) = &HttpRequest::handle;
    void (HttpRequest::*handleFile)(QNetworkReply::NetworkError, const QByteArray&, const QString&, const QString&) = &HttpRequest::handle;
    connect(hr, handleJson, this, &MainWindow::handleResponse);
    connect(hr, handleFile, this, &MainWindow::handleDownload);

    connect(hr, &HttpRequest::hndUpd, this, &MainWindow::handleUpdate);
}

void MainWindow::initConfigs()
{
    urlLines.append("https://bangumi.bilibili.com/web_api/timeline_global");
    urlLines.append("https://bangumi.bilibili.com/web_api/timeline_cn");
    currLine = 0;
    QFile cFile(QCoreApplication::applicationDirPath().append(CONF_PATH).append(CONF));
    if (!cFile.exists())
    {
        return;
    }
    if (cFile.open(QIODevice::ReadOnly))
    {
        QJsonDocument jDoc = QJsonDocument::fromJson(cFile.readAll());

        cFile.close();
        QJsonArray jArr = jDoc["position"].toArray();
        if (!jArr.isEmpty())
        {
            this->move(jArr.at(0).toInt(), jArr.at(1).toInt());
        }
        QJsonArray urlArr = jDoc["lineUrls"].toArray();
        if (!urlArr.isEmpty())
        {
            urlLines.clear();
            for (int i = 0; i < urlArr.size(); i++) {
                urlLines.append(urlArr[i].toString());
            }
        }
        currLine = jDoc["currLine"].toInt(0);
        if (currLine < 0)
            currLine = 0;

        colorIndex = jDoc["colorIndex"].toInt(0);
        QAction* color = colorActions.at(colorIndex);
        color->trigger();
        colorAct = color;
    }
}

template<typename T>
void MainWindow::clearVector(QVector<T*>* vec)
{
    for (int i = vec->size() - 1; i >=0; i--) {
        delete vec->at(i);
    }
    vec->clear();
}

void MainWindow::clearBangumis()
{
    for (int i = bangumis.size() - 1; i >= 0; i--) {
        QVector<Bang*> bangs = bangumis.at(i)->bangs;
        for (int j = bangs.size() - 1; j >= 0; j--) {
            delete bangs.at(j);
        }
        delete bangumis.at(i);
    }
    bangumis.clear();
}

void MainWindow::drawPanels(int index)
{
    clearVector(&panels);
    if (!bangumis.isEmpty())
    {
        if (initPanel)
        {
            delete initPanel;
            initPanel = nullptr;
        }
        Bangumi* b = bangumis.at(index);
        QString date = b->date;
        dateLabel->setText(date.append("(").append(WEEK_STR[b->dayOfWeek]).append(")"));
        QVector<Bang*> bangs = b->bangs;
        if (!bangs.isEmpty())
        {
            QVector<Bang*>::iterator iter = bangs.begin();

            QFile f(this);

            resize(mainWindowWidth, panelHeight * bangs.size() + topWidgetHeight);
            panelWidget->resize(mainWindowWidth, panelHeight * bangs.size());

            for (int i = 0; iter != bangs.end(); iter++, i++) {
                Bang* bang = (*iter);

                QString fname = QString::number(bang->seasonId).append(".").append(bang->cover.split(".").last());
                QString fPathName = QCoreApplication::applicationDirPath().append(COVER_PATH).append(fname);

                LabelPanel* panel = new LabelPanel(panelWidget);
                panel->setTime(bang->pubTime);
                panel->setTitle(bang->title);
                panel->setPubIndex(bang->pubIndex);
                panel->setFname(fPathName);
                panel->setUrl(bang->url);
                panel->setFloatWin(floatWin);
                panel->move(0, panelHeight * i);
                panel->show();
                panels.append(panel);

                f.setFileName(fPathName);
                if (!f.exists())
                    hr->Get(bang->cover, QCoreApplication::applicationDirPath().append(COVER_PATH), fname);
            }
        }
        else
        {
            LabelPanel* panel = new LabelPanel(panelWidget);
            panel->setTime("     ");
            int ranIndex = rand() % PRESET_ST_LEN;
            panel->setTitle(PRESET_ST[ranIndex]);
            panel->setPubIndex(PRESET_ST_FACE[ranIndex]);
            panel->setFname("");
            panel->setUrl("");
            panel->move(0, 0);
            panel->show();
            panels.append(panel);
            resize(mainWindowWidth, panelHeight + topWidgetHeight);
            panelWidget->resize(mainWindowWidth, panelHeight);
        }
    }
}

void MainWindow::handleResponse(QNetworkReply::NetworkError err, const QByteArray &response)
{
    if (err != QNetworkReply::NoError && timerId == -1)
    {
        timerId = startTimer(REREQUEST_TIMER);
        return;
    }
    clearBangumis();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonArray jsonArr = jsonDoc["result"].toArray();

    for (int i = 0; i < jsonArr.size(); i++)
    {
        QJsonValue jsonVal = jsonArr[i];
        Bangumi* bangumi = new Bangumi;
        bangumi->date = jsonVal["date"].toString();
        bangumi->dayOfWeek = jsonVal["day_of_week"].toInt();
        if (jsonVal["is_today"].toInt() == 1)
            todayIndex = i;

        QJsonArray seaArr = jsonVal["seasons"].toArray();
        for (int j = 0; j < seaArr.size(); j++) {
            QJsonValue val = seaArr[j];
            Bang* b = new Bang;
            b->cover = val["cover"].toString();
            b->pubIndex = val["pub_index"].toString();
            if (b->pubIndex == "")
                b->pubIndex = val["delay_index"].toString().append("(").append(val["delay_reason"].toString()).append(")");
            b->pubTime = val["pub_time"].toString();
            b->seasonId = val["season_id"].toInt();
            b->title = val["title"].toString();
            b->url = val["url"].toString();
            bangumi->bangs.append(b);
        }
        bangumis.append(bangumi);
    }

//    qDebug() << "-----------------------------------------------------------------------";

//    for (int i = 0; i < bangumis.size(); i++) {
//        qDebug() << bangumis[i]->date << ", " << bangumis[i]->dayOfWeek << "===========";
//        QVector<Bang*> bangs = bangumis[i]->bangs;
//        for (int j = 0; j < bangs.size(); j++)
//        {
//            qDebug() << bangs[j]->title << ", " << bangs[j]->seasonId;
//        }
//    }

//    qDebug() << todayIndex;
    currIndex = todayIndex;

    drawPanels(todayIndex);
}

void MainWindow::handleDownload(QNetworkReply::NetworkError err, const QByteArray &response, const QString& pPath, const QString &fname)
{
    if (err != QNetworkReply::NoError)
    {
        return;
    }
    QString path = pPath;
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());
    QFile f(path.append(fname), this);
    if (f.open(QIODevice::WriteOnly))
    {
        f.write(response);
        f.close();
    }
}

void MainWindow::handleUpdate(const QByteArray &response)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QString newVersion = jsonDoc["version"].toString();

    if (dia)
        dia->close();

    dia = new QDialog(this, Qt::WindowStaysOnTopHint);
    dia->setWindowTitle("检查更新");
    QGridLayout* glay = new QGridLayout;
    dia->setLayout(glay);

    QLabel* verLabel = new QLabel(dia);
    QString verTxt = "最新版本：";
    verLabel->setText(verTxt.append(newVersion));
    glay->addWidget(verLabel, 0, 0);

    if (newVersion == version)
    {
        QLabel* newLabel = new QLabel(dia);
        newLabel->setText("已经是最新版本！");
        glay->addWidget(newLabel, 1, 0);
    }
    else
    {
        QJsonArray conArr = jsonDoc["content"].toArray();
        if (!conArr.isEmpty())
        {
            QLabel* newLabel = new QLabel(dia);
            newLabel->setText("更新内容：");
            glay->addWidget(newLabel, 2, 0);
            int i = 0;
            for (; i < conArr.size(); i++) {
                newLabel = new QLabel(dia);
                newLabel->setText(conArr.at(i).toString());
                glay->addWidget(newLabel, i + 3, 0);
            }
            QLabel* distLabel = new QLabel(dia);
            QString distLink = "<a href='";
            distLink.append(jsonDoc["dist"].toString())
                    .append("'>").append("点此下载").append("</a>");
            distLabel->setText(distLink);
            distLabel->setOpenExternalLinks(true);
            glay->addWidget(distLabel, i + 3, 0);
        }
    }

    dia->open();

    connect(dia, &QDialog::finished, [this](){
        delete  dia;
        dia = nullptr;
    });
}

void MainWindow::goLeft()
{
    if (currIndex > 0) {
        drawPanels(--currIndex);
    }
}

void MainWindow::goRight()
{
    if (currIndex < bangumis.size() - 1) {
        drawPanels(++currIndex);
    }
}

void MainWindow::switchLine()
{
    if (++currLine >= urlLines.size())
        currLine = 0;
    readTimeLine();
    saveConf();
}

void MainWindow::readTimeLine()
{
    hr->Get(urlLines.at(currLine));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressed = true;
    windowMoved = false;
    beginPos = event->pos();
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed)
    {
        QPoint movePos(QCursor::pos() - beginPos);
        move(movePos);
        windowMoved = true;
    }
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
    if (windowMoved)
        saveConf();
    QWidget::mouseReleaseEvent(event);
}

void MainWindow::saveConf()
{
    QJsonDocument doc;
    QJsonArray posArr;
    posArr.append(pos().x());
    posArr.append(pos().y());

    QJsonObject obj;
    obj.insert("position", posArr);

    QJsonArray urlArr;
    for (int i = 0; i < urlLines.size(); i++) {
        urlArr.append(urlLines[i]);
    }
    obj.insert("lineUrls", urlArr);

    obj.insert("currLine", currLine);

    obj.insert("colorIndex", colorIndex);

    doc.setObject(obj);

    QFile cFile(QCoreApplication::applicationDirPath().append(CONF_PATH).append(CONF));
    if (cFile.open(QIODevice::WriteOnly))
    {
        cFile.write(doc.toJson());
        cFile.close();
    }
}

void MainWindow::moveCenter()
{
    QList<QScreen *> ss = QGuiApplication::screens();
    QSize sSize = ss[0]->availableGeometry().size();
    this->move(sSize.width() / 2 - this->width() / 2, sSize.height() / 2 - this->height() / 2);
}

void MainWindow::quit()
{
    qApp->quit();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId)
    {
        readTimeLine();
        killTimer(timerId);
        timerId = -1;
    }
}
