#pragma once

#include <QDebug>
#include <QTime>

#pragma execution_character_set("utf-8")

// 日志格式统一规范
// [时间] [等级] [文件名] [函数名] [行数] [内容]
class QSelfLog
{
public:
    enum LogLevel
    {
        Debug,
        Info,
        Waring,
        Err,
    };

public:
    QSelfLog()
    {
    }

    static QString getTimeStr()
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
        return timeStr;
    }

    static QString getFileName(const char* file)
    {
        QString fileStr = file;
        QString fileNameStr = fileStr.right(fileStr.size() - fileStr.lastIndexOf("\\") - 1);
        return fileNameStr;
    }

public:
};

#define LogDebug                                                           \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]" \
             << "["                                                        \
             << "debug"                                                    \
             << "]"                                                        \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogWarn                                                            \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]" \
             << "["                                                        \
             << "warn"                                                     \
             << "]"                                                        \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogErr                                                             \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]" \
             << "["                                                        \
             << "err"                                                      \
             << "]"                                                        \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() << "." << __FUNCTION__ << "." << __LINE__ << "]"
