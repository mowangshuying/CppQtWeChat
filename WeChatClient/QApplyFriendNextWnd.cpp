#include "QApplyFriendNextWnd.h"
#include "QSelfSplit.h"
#include "QApplyFriendInputInfoWnd.h"
#include "QApplyFriendWaitInfoWnd.h"

#include <QMouseEvent>
#include <QMessageBox>

#include "QWSClientMgr.h"
#include "QMainWnd.h"
#include "QStyleSheetMgr.h"

QApplyFriendNextWnd::QApplyFriendNextWnd(QWidget* p /*= nullptr*/,
                                         int64_t friendid /*= -1*/,
                                         QString username /*= ""*/)
    : QWidget(p), m_friendid(friendid), m_username(username)
{
    LogFunc;
    setFixedSize(460, 360);
    m_centerWnd = new QWidget(this);
    m_centerWnd->setFixedSize(460, 360);
    m_centerWnd->setObjectName("QApplyFriendNextWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_state = PushBtnState::PBS_Next;

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_vLayout->setContentsMargins(10, 10, 10, 10);
    m_centerWnd->setLayout(m_vLayout);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_hLayout1 = new QHBoxLayout(m_centerWnd);

    m_titleLabel = new QLabel();
    m_minBtn = new QPushButton();
    m_closeBtn = new QPushButton();

    m_titleLabel->setText("添加好友");

    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);

    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    m_hLayout1->addWidget(m_titleLabel);
    m_hLayout1->addWidget(m_minBtn);
    m_hLayout1->addWidget(m_closeBtn);
    m_vLayout->addLayout(m_hLayout1);

    ////////////////////////////////////////
    m_sLayout = new QStackedLayout(m_centerWnd);
    m_sLayout->setContentsMargins(0, 0, 0, 0);

    m_wnd1 = new QApplyFriendInputInfoWnd();
    m_sLayout->addWidget(m_wnd1);

    m_wnd2 = new QApplyFriendWaitInfoWnd();
    m_sLayout->addWidget(m_wnd2);
    ////////////////////////////////////////

    m_vLayout->addLayout(m_sLayout);

    m_hLayout2 = new QHBoxLayout(m_centerWnd);
    m_pushBtn = new QPushButton();
    m_pushBtn->setText("下一步");

    m_hLayout2->addStretch();
    m_hLayout2->addWidget(m_pushBtn);
    m_vLayout->addLayout(m_hLayout2);

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_pushBtn, SIGNAL(clicked()), this, SLOT(slotPushBtnClick()));
}

void QApplyFriendNextWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void QApplyFriendNextWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void QApplyFriendNextWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void QApplyFriendNextWnd::slotPushBtnClick()
{
    if (m_state == PushBtnState::PBS_Next)
    {
        //设置m_sLayout为第2个窗口
        m_sLayout->setCurrentIndex(1);
        //设置按钮字体为关闭
        m_pushBtn->setText("关闭");
        m_state = PushBtnState::PBS_Close;

        //向远端服务器发送添加好友申请
        neb::CJsonObject json;
        json.Add("ownerid", QMainWnd::getMainWnd()->m_userid);
        json.Add("friendid", m_friendid);

        //获取验证的消息
        auto pApplyWnd1 = dynamic_cast<QApplyFriendInputInfoWnd*>(m_wnd1);
        json.Add(
            "applymsg",
            pApplyWnd1->m_inputMsgEdit->toPlainText().toStdString().c_str());

        QWSClientMgr::getMgr()->request("cs_msg_apply_add_user",
                                        json,
                                        [](neb::CJsonObject& msg) {
                                            int state = 0;
                                            if (!msg.Get("state", state))
                                            {
                                                return;
                                            }
                                            LogDebug << "recv apply add user";
                                        });

        return;
    }

    if (m_state == PushBtnState::PBS_Close)
    {
        close();
        return;
    }
}

void QApplyFriendNextWnd::closeWnd()
{
    close();
}

void QApplyFriendNextWnd::minWnd()
{
    showMinimized();
}
