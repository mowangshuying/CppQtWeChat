
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
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./img/wechat.ico"));

    LogDebug << "start";
    QSelfLog::init();
    //��������ͨ�ŵĹ���
    QWSClientMgr::init();

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