
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "QLoginAndRegWnd.h"
#include "QWSClientMgr.h"
#include "QDealNewFriendsApplyWnd.h"
#include <QDir>

int main(int argc, char** argv)
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));
    //创建网络通信的管理
    QWSClientMgr::getInstance();

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
}