#pragma once

#include "def.h"
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QMessageLogContext>
#include <qlogging.h>
#include <QtMsgHandler>

// 日志格式统一规范
// [时间] [等级] [文件名] [函数名] [行数] [内容]
class Log
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
    Log()
    {
    }

    static QString getTimeStr();

    static QString getFileName(const char* file);

    static void init();
    static void deinit();
    static void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

  public:
    static QFile* gFileLog;
    static QtMessageHandler gDefaultHandler;
};

#define LogFunc LogDebug << "called.";

#define getLogTimeStr Log::getTimeStr().toStdString().c_str()
#define getLogFileStr Log::getFileName(__FILE__).toStdString().c_str()

#define LogDebug                            \
    qDebug() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "debug"                     \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogInfo                             \
    qDebug() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "info"                      \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogWarn                             \
    qDebug() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "warn"                      \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "]"

#define LogErr                              \
    qDebug() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "err"                       \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "]"
