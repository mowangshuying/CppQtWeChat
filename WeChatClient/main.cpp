
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
    QSelfLog::logSysInit();

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    LogDebug << "start";

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));
    //��������ͨ�ŵĹ���
    QWSClientMgr::getMgr();
    // ��ʽ�����
    QStyleSheetMgr::getMgr()->init();

    //���ر��ص���ʽ��
    QFile qss("./stylesheet/wechat.qss");
    if (qss.open(QFile::ReadOnly))
    {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }

    //��¼����
    QLoginAndRegWnd lgw;
    lgw.show();

    app.exec();
}