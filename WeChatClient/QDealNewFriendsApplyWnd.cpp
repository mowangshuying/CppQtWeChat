#include <QMouseEvent>
#include <QScrollBar>
#include "QDealNewFriendsApplyWnd.h"
#include "QDealNewFriendsApplyItemWnd.h"
#include "QSimpleSplit.h"
#include "QWSClientMgr.h"
#include "QMainWnd.h"
#include "./json/CJsonObject.hpp"
#include "QStyleSheetMgr.h"

QDealNewFriendsApplyWnd::QDealNewFriendsApplyWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QDealNewFriendsApplyWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->setContentsMargins(10, 10, 10, 10);
    m_vLayout->setSpacing(0);

    m_hLayout = new QHBoxLayout();

    m_minBtn = new QPushButton(this);
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);
    //m_minBtn->setStyleSheet("border:0px;");

    m_maxBtn = new QPushButton(this);
    m_maxBtn->setIcon(QPixmap("./img/normalBtn_.png"));
    m_maxBtn->setIconSize(QSize(20, 20));
    m_maxBtn->setFixedSize(20, 20);
    //m_maxBtn->setStyleSheet("border:0px;");

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);
    //m_closeBtn->setStyleSheet("border:0px;");

    //m_hLayout->addSpacing(20);
    m_hLayout->addStretch();
    m_hLayout->setSpacing(5);
    m_hLayout->addWidget(m_minBtn);
    m_hLayout->addWidget(m_maxBtn);
    m_hLayout->addWidget(m_closeBtn);
    m_vLayout->addLayout(m_hLayout);

    // m_sLayout = new QStackedLayout();
    m_listWnd1 = new QListWidget();
    m_listWnd2 = new QListWidget();

    m_tabWnd = new QTabWidget();
    m_tabWnd->addTab(m_listWnd1, "好友验证");
    m_tabWnd->addTab(m_listWnd2, "群系统消息");

    m_tabWnd->setStyleSheet("QTabBar::tab{height:40;width:100}");

    m_vLayout->addSpacing(15);
    m_vLayout->addWidget(m_tabWnd);

    setMinimumSize(640, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    m_listWnd1->setAttribute(Qt::WA_StyledBackground);
    m_listWnd2->setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_StyledBackground);

   // m_listWnd1->setStyleSheet("border:0px;");
   // m_listWnd2->setStyleSheet("border:0px;");

    if (objectName().isEmpty())
        setObjectName("QDealNewFriendsApplyWnd");
   // setStyleSheet("QWidget#QDealNewFriendsApplyWnd{border-top-right-radius:4px;border-bottom-right-radius:4px; background-color:white;border:none;}");
}

void QDealNewFriendsApplyWnd::addListItem(const char* headurl, const char* name, const char* msg, int state, int id, bool isApplyer, int userid)
{
    QDealNewFriendsApplyItemWnd* pMsgItem = new QDealNewFriendsApplyItemWnd(m_listWnd1, headurl, name, msg, state, id, isApplyer, userid);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd1);

    pMsgItem->setFixedWidth(this->width() - 5);
    pListItem->setSizeHint(QSize(this->width() - 5, 65));
    m_listWnd1->setItemWidget(pListItem, pMsgItem);
}

void QDealNewFriendsApplyWnd::setFriendApplyList()
{
    //
    neb::CJsonObject json;
    json.Add("ownerid", QMainWnd::getMainWnd()->m_userid);
    QWSClientMgr::getMgr()->request("cs_msg_get_applyadduserlist", json, [this](neb::CJsonObject& msg) {
        LogDebug << msg.ToString().c_str();
        for (int i = 0; i < msg["data"].GetArraySize(); i++)
        {
            neb::CJsonObject tempJsonObj;
            if (!msg["data"].Get(i, tempJsonObj))
            {
                continue;
            }

            //
            int64_t ownerid;
            if (!tempJsonObj.Get("ownerid", ownerid))
            {
                continue;
            }

            int64_t friendid;
            if (!tempJsonObj.Get("friendid", friendid))
            {
                continue;
            }

            if (ownerid == QMainWnd::getMainWnd()->m_userid)
            {
                std::string username;
                if (!tempJsonObj.Get("friendusername", username))
                {
                    continue;
                }

                std::string applymsg;
                if (!tempJsonObj.Get("applymsg", applymsg))
                {
                    continue;
                }

                int applystate = -1;
                if (!tempJsonObj.Get("applystate", applystate))
                {
                    continue;
                }

                int id = -1;
                if (!tempJsonObj.Get("id", id))
                {
                    continue;
                }

                if (hasThisFriendApplyById(id))
                {
                    continue;
                }

                addListItem("./img/head2.png", username.c_str(), applymsg.c_str(), applystate, id, true, friendid);
            }
            else
            {
                std::string username;
                if (!tempJsonObj.Get("ownerusername", username))
                {
                    continue;
                }

                std::string applymsg;
                if (!tempJsonObj.Get("applymsg", applymsg))
                {
                    continue;
                }

                int applystate = -1;
                if (!tempJsonObj.Get("applystate", applystate))
                {
                    continue;
                }

                int id = -1;
                if (!tempJsonObj.Get("id", id))
                {
                    continue;
                }

                if (hasThisFriendApplyById(id))
                {
                    continue;
                }

                addListItem("./img/head2.png", username.c_str(), applymsg.c_str(), applystate, id, false, ownerid);
            }
        }
    });
}

bool QDealNewFriendsApplyWnd::hasThisFriendApplyById(int id)
{
    bool bHas = false;
    for (int i = 0; i < m_listWnd1->count(); i++)
    {
        QListWidgetItem* pitem = m_listWnd1->item(i);
        QDealNewFriendsApplyItemWnd* pApplyItemWnd = dynamic_cast<QDealNewFriendsApplyItemWnd*>(m_listWnd1->itemWidget(pitem));
        if (pApplyItemWnd->m_id == id)
        {
            bHas = true;
            break;
        }
    }

    return bHas;
}
