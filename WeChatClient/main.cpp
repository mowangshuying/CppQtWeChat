//#include <vld.h>

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "QLoginAndRegWnd.h"
#include "QWSClientMgr.h"
#include "QDealNewFriendsApplyWnd.h"
#include <QDir>
#include "QStyleSheetMgr.h"
#include "QDataManager.h"
#include "QSelfLog.h"
#include <QThread>

int main(int argc, char** argv)
{
    LogFunc;
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));

    LogDebug << "start";
    QSelfLog::initLog();
    QStyleSheetMgr::initMgr();
    QWSClientMgr::initMgr();
    QDataManager::initMgr();

    //加载本地的样式表
    QFile qss("./stylesheet/wechat.qss");
    if (qss.open(QFile::ReadOnly))
    {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }

    //登录窗口
    QLoginAndRegWnd lgw;
    lgw.show();

    app.exec();

    // 最后退出日志
    QDataManager::exitMgr();
    QWSClientMgr::exitMgr();
    QStyleSheetMgr::exitMgr();
    QSelfLog::exitLog();
}