//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//
//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DBG_NEW
//#endif
//
//#endif  //

#include <vld.h>

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "QLoginAndRegWnd.h"
#include "QWSClientMgr.h"
#include "QDealNewFriendsApplyWnd.h"
#include <QDir>
#include "QStyleSheetMgr.h"

int main(int argc, char** argv)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));

    LogDebug << "start";
    QSelfLog::initLog();
    QStyleSheetMgr::initMgr();
    //创建网络通信的管理
    QWSClientMgr::initMgr();

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

    QWSClientMgr::ExitMgr();
    QStyleSheetMgr::exitMgr();
    QSelfLog::exitLog();
}