#include "QSelfLog.h"

QFile* QSelfLog::gFileLog = nullptr;
QtMessageHandler QSelfLog::gDefaultHandler = nullptr;

QString QSelfLog::getTimeStr()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    return timeStr;
}

QString QSelfLog::getFileName(const char* file)
{
    QString fileStr = file;
    QString fileNameStr =
        fileStr.right(fileStr.size() - fileStr.lastIndexOf("\\") - 1);
    return fileNameStr;
}

void QSelfLog::initLog()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString timeStr = dateTime.toString("[yyyy-MM-dd][hh_mm_ss_zzz]");
    QString filePath = QString("./log/log_%1.txt").arg(timeStr);

    gFileLog = new QFile(filePath);
    if (!gFileLog->open(QIODevice::WriteOnly | QIODevice::Text |
                        QIODevice::Append))
    {
        delete gFileLog;
        gFileLog = nullptr;
        return;
    }
    //初始化自定义日志处理函数myMessageOutput
    gDefaultHandler = qInstallMessageHandler(myMessageOutput);
}

void QSelfLog::exitLog()
{
    if (gFileLog != nullptr)
    {
        gFileLog->close();
        delete gFileLog;
        gFileLog = nullptr;
    }
}

void QSelfLog::myMessageOutput(QtMsgType type,
                               const QMessageLogContext& context,
                               const QString& msg)
{
    if (gFileLog)
    {
        QTextStream tWrite(gFileLog);
        tWrite << msg << "\n";
        gDefaultHandler(type, context, msg);
    }
    else
    {
        fprintf(stderr, "%s\n", msg.toStdString().c_str());
    }
}
