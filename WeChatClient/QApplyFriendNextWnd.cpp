#include "QApplyFriendNextWnd.h"
#include "QSimpleSplit.h"
#include "QApplyFriendInputInfoWnd.h"
#include "QApplyFriendWaitInfoWnd.h"

#include <QMouseEvent>
#include <QMessageBox>

#include "QWSClientMgr.h"
#include "QMainWnd.h"

QApplyFriendNextWnd::QApplyFriendNextWnd(QWidget* p /*= nullptr*/, int64_t friendid /*= -1*/, QString username /*= ""*/)
    : QWidget(p), m_friendid(friendid), m_username(username)
{
    setObjectName("QApplyFriendNextWnd");

    m_state = Ps_Next;

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_vLayout);
    setFixedSize(460, 360);
    setWindowFlags(Qt::FramelessWindowHint);

    m_hLayout1 = new QHBoxLayout();

    m_titleLabel = new QLabel();
    m_minBtn = new QPushButton();
    m_closeBtn = new QPushButton();

    m_titleLabel->setText("don't forget win or lost - 添加好友");

    m_minBtn->setIcon(QPixmap("./img/minWnd.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);

    m_closeBtn->setIcon(QPixmap("./img/closeWnd.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    m_hLayout1->addWidget(m_titleLabel);
    m_hLayout1->addWidget(m_minBtn);
    m_hLayout1->addWidget(m_closeBtn);

    m_vLayout->addLayout(m_hLayout1);

    QSimpleSplit* sp1 = new QSimpleSplit();
    m_vLayout->addWidget(sp1);

    m_sLayout = new QStackedLayout(this);
    m_sLayout->setContentsMargins(0, 0, 0, 0);

    m_wnd1 = new QApplyFriendInputInfoWnd();
    m_sLayout->addWidget(m_wnd1);

    m_wnd2 = new QApplyFriendWaitInfoWnd();
    m_sLayout->addWidget(m_wnd2);

    m_vLayout->addLayout(m_sLayout);

    QSimpleSplit* sp2 = new QSimpleSplit();
    m_vLayout->addWidget(sp2);

    m_hLayout2 = new QHBoxLayout();
    m_pushBtn = new QPushButton(this);
    m_pushBtn->setText("下一步");
    // m_pushBtn->raise();

    m_hLayout2->addStretch();
    m_hLayout2->addWidget(m_pushBtn);
    m_vLayout->addLayout(m_hLayout2);

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_pushBtn, SIGNAL(clicked()), this, SLOT(slot_pushBtnClick()));
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

void QApplyFriendNextWnd::slot_pushBtnClick()
{
    if (m_state == Ps_Next)
    {
        //设置m_sLayout为第2个窗口
        m_sLayout->setCurrentIndex(1);
        //设置按钮字体为关闭
        m_pushBtn->setText("关闭");
        m_state = Ps_Close;

        //向远端服务器发送添加好友申请
        neb::CJsonObject json;
        json.Add("ownerid", QMainWnd::getInstance()->m_userid);
        json.Add("friendid", m_friendid);

        //获取验证的消息
        auto pApplyWnd1 = dynamic_cast<QApplyFriendInputInfoWnd*>(m_wnd1);
        json.Add("applymsg", pApplyWnd1->m_inputMsgEdit->toPlainText().toStdString().c_str());

        QWSClientMgr::getInstance()->request("cs_msg_apply_add_user", json, [](neb::CJsonObject& msg) {
            int state = 0;
            if (!msg.Get("state", state))
            {
                return;
            }
            //添加好友申请
            // QMessageBox::information(nullptr, "cs_msg_apply_add_friend", msg.ToString().c_str());
        });

        return;
    }

    if (m_state == Ps_Close)
    {
        close();
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
