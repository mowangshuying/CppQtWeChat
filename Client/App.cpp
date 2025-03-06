#include <QApplication>
#include <QMessageBox>

#include "LoginRegWnd.h"
#include "NetClientUtils.h"
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

    LogD << "start";
    Log::init();
    StyleSheetMgr::initMgr();
    NetClientUtils::init();
    DataManager::initMgr();

    // 加载本地的样式表
    QFile qss("./stylesheet/wechat.qss");
    if (qss.open(QFile::ReadOnly))
    {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }

    // 登录窗口
    LoginRegWnd lgw;
    lgw.show();

    app.exec();

    // 最后退出日志
    DataManager::exitMgr();
    NetClientUtils::deinit();
    StyleSheetMgr::exitMgr();
    Log::deinit();
}