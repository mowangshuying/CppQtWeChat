#pragma once

#include "QPch.h"
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QMessageLogContext>
#include <qlogging.h>
#include <QtMsgHandler>

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

    static QString getTimeStr();

    static QString getFileName(const char* file);

    static void initLog();
    static void exitLog();

    static void myMessageOutput(QtMsgType type,
                                const QMessageLogContext& context,
                                const QString& msg);

  public:
    static QFile* gFileLog;
    static QtMessageHandler gDefaultHandler;
};

#define LogFunc LogDebug << "called.";

#define getLogTimeStr QSelfLog::getTimeStr().toStdString().c_str()
#define getLogFileStr QSelfLog::getFileName(__FILE__).toStdString().c_str()

#define LogDebug                                                             \
    qDebug() << "[" << getLogTimeStr << "]"                                  \
             << "["                                                          \
             << "debug"                                                      \
             << "]"                                                          \
             << "[" << getLogFileStr                                         \
             << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogInfo                                                              \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]"   \
             << "["                                                          \
             << "info"                                                       \
             << "]"                                                          \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() \
             << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogWarn                                                              \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]"   \
             << "["                                                          \
             << "warn"                                                       \
             << "]"                                                          \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() \
             << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogErr                                                               \
    qDebug() << "[" << QSelfLog::getTimeStr().toStdString().c_str() << "]"   \
             << "["                                                          \
             << "err"                                                        \
             << "]"                                                          \
             << "[" << QSelfLog::getFileName(__FILE__).toStdString().c_str() \
             << "." << __FUNCTION__ << "." << __LINE__ << "]"
