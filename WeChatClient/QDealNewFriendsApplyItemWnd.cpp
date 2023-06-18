#include "QDealNewFriendsApplyItemWnd.h"
#include "QWSClientMgr.h"
#include <QMessageBox>
#include "QDataManager.h"

QDealNewFriendsApplyItemWnd::QDealNewFriendsApplyItemWnd(QWidget* p /*= nullptr*/,
                                                         const char* headurl /*= ""*/,
                                                         const char* name /*= ""*/,
                                                         const char* msg /*= ""*/,
                                                         int state /*= 0*/,
                                                         int id /* = 0*/,
                                                         bool isApplyer /* = false*/,
                                                         int userid /*=0*/)
    : QWidget(p), m_state(state), m_id(id), m_userid(userid), m_isApplyer(isApplyer)

{
    LogFunc;
    setObjectName("QDealNewFriendsApplyItemWnd");

    m_vLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout(this);
    m_name = new QLabel(this);
    m_msg = new QLabel(this);
    m_AgreeBtn = new QPushButton(this);
    m_refuseBtn = new QPushButton(this);

    setAttribute(Qt::WA_StyledBackground);

    m_name->setText(name);
    m_msg->setText(msg);
    m_msg->setWordWrap(true);
    m_AgreeBtn->setText("同意");
    m_refuseBtn->setText("拒绝");

    m_vLayout->addSpacing(10);
    m_vLayout->addWidget(m_name);
    m_vLayout->addWidget(m_msg);
    m_vLayout->addSpacing(10);

    m_headurl = new QLabel();
    m_headurl->setFixedSize(40, 40);
    m_headurl->setPixmap(QPixmap(headurl));

    m_hLayout->addSpacing(50);
    m_hLayout->addWidget(m_headurl);
    m_hLayout->addSpacing(25);
    m_hLayout->addLayout(m_vLayout);
    m_hLayout->addStretch();
    m_hLayout->addWidget(m_AgreeBtn);
    m_hLayout->addWidget(m_refuseBtn);
    m_hLayout->addSpacing(40);

    m_vLayout->setContentsMargins(5, 0, 0, 0);
    m_vLayout->setSpacing(0);

    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->setSpacing(0);

    if (m_isApplyer)
    {
        if (state == 0)
        {
            m_AgreeBtn->setText("等待同意");
        }
        else if (state == 1)
        {
            m_AgreeBtn->setText("已同意");
        }
        else
        {
            m_AgreeBtn->setText("已拒绝");
        }

        m_AgreeBtn->setEnabled(false);
        m_refuseBtn->hide();
    }
    else
    {
        if (state == 0)
        {
            m_refuseBtn->show();
        }
        else if (state == 1)
        {
            m_AgreeBtn->setText("已同意");
            m_AgreeBtn->setEnabled(false);
            m_refuseBtn->hide();
        }
        else
        {
            m_AgreeBtn->setText("已拒绝");
            m_AgreeBtn->setEnabled(false);
            m_refuseBtn->hide();
        }
    }

    auto itf = QDataManager::getMgr()->m_UserId2HeadImgMap.find(m_userid);
    if (itf != QDataManager::getMgr()->m_UserId2HeadImgMap.end())
    {
        m_headurl->setPixmap(itf->second);
    }

    setLayout(m_hLayout);
    setFixedHeight(60);

    connect(m_AgreeBtn, SIGNAL(clicked()), this, SLOT(slotOnAgreeBtnClick()));
    connect(m_refuseBtn, SIGNAL(clicked()), this, SLOT(slotOnRefuseBtnClick()));
}

void QDealNewFriendsApplyItemWnd::slotOnAgreeBtnClick()
{
    //向远端发送同意好友申请
    neb::CJsonObject json;
    json.Add("applyid", m_id);
    json.Add("applystate", 1);
    QWSClientMgr::getMgr()->request("cs_msg_do_apply_add_user", json, [this](neb::CJsonObject& msg) {
        m_AgreeBtn->setText("已同意");
        m_AgreeBtn->setEnabled(false);
        m_refuseBtn->hide();
    });
}

void QDealNewFriendsApplyItemWnd::slotOnRefuseBtnClick()
{
    //向远端发送拒绝好友申请
    neb::CJsonObject json;
    json.Add("applyid", m_id);
    json.Add("applystate", 2);
    QWSClientMgr::getMgr()->request("cs_msg_do_apply_add_user", json, [this](neb::CJsonObject& msg) {
        m_AgreeBtn->setText("已拒绝");
        m_AgreeBtn->setEnabled(false);
        m_refuseBtn->hide();
    });
}
