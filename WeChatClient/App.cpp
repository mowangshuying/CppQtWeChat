//#include <vld.h>

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "LoginRegWnd.h"
#include "WSClientMgr.h"
#include "DealNewFriendsApplyWnd.h"
#include <QDir>
#include "StyleSheetMgr.h"
#include "DataManager.h"
#include "Log.h"
#include <QThread>

int main(int argc, char** argv)
{
    LogFunc;
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));

    LogDebug << "start";
    Log::initLog();
    StyleSheetMgr::initMgr();
    WSClientMgr::initMgr();
    DataManager::initMgr();

    //加载本地的样式表
    QFile qss("./stylesheet/wechat.qss");
    if (qss.open(QFile::ReadOnly))
    {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }

    //登录窗口
    LoginRegWnd lgw;
    lgw.show();

    app.exec();

    // 最后退出日志
    DataManager::exitMgr();
    WSClientMgr::exitMgr();
    StyleSheetMgr::exitMgr();
    Log::exitLog();
}