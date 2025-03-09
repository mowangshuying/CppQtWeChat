#pragma once

#include "def.h"
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QMessageLogContext>
#include <qlogging.h>

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

    static void init();
    static void deinit();
    static QString getTimeStr();
    static QString getFileName(const char* file);
    static void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
  
public:
    static QFile* gFileLog;
    static QtMessageHandler gDefaultHandler;
};

#define LogFunc LogD << "called.";

#define getLogTimeStr Log::getTimeStr().toStdString().c_str()
#define getLogFileStr Log::getFileName(__FILE__).toStdString().c_str()

#define LogD                            \
    qDebug().nospace() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "debug"                     \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "] "

#define LogI                             \
    qDebug().nospace() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "info"                      \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "] "

#define LogW                             \
    qDebug().nospace() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "warn"                      \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "] "

#define LogE                              \
    qDebug().nospace() << "[" << getLogTimeStr << "]" \
             << "["                         \
             << "err"                       \
             << "]"                         \
             << "[" << getLogFileStr << "." << __FUNCTION__ << "." << __LINE__ << "] "

#define SplitStr "---------- ---------- ---------- ---------- ----------"