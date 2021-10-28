#ifndef BANG_H
#define BANG_H
#include <QString>
#include <QVector>
/**
 * @brief The Bang struct
 */
struct Bang
{
    /**
     * @brief 标题
     */
    QString title;
    /**
     * @brief 封面url
     */
    QString cover;
    /**
     * @brief 第几话
     */
    QString pubIndex;
    /**
     * @brief 发布（更新）时间
     */
    QString pubTime;
    /**
     * @brief seasonId
     */
    int seasonId;
    /**
     * @brief 播放地址
     */
    QString url;
};

/**
 * @brief The Bangumi struct
 */
struct Bangumi
{
    /**
     * @brief bang vector
     */
    QVector<Bang*> bangs;
    /**
     * @brief 日期
     */
    QString date;
    /**
     * @brief 周几
     */
    int dayOfWeek;
};

#endif // BANG_H
