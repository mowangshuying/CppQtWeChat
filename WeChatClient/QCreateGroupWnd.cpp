#include <QDebug>
#include <QScrollBar>
#include <QMouseEvent>
#include <QMessageBox>

#include "QSelfSplit.h"
#include "QCreateGroupWnd.h"
#include "QCreateGroupListItemWithSelBtnWnd.h"
#include "QCreateGroupListItemWnd.h"
#include "QDataManager.h"
#include "QWSClientMgr.h"
#include "QMainWnd.h"
#include "QStyleSheetMgr.h"

QCreateGroupWnd::QCreateGroupWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    // setObjectName("QCreateGroupWnd");

    m_centerWnd = new QWidget(this);
    // m_centerWnd->setFixedSize(460, 360);
    m_centerWnd->setObjectName("QCreateGroupWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_vLayout->setContentsMargins(10, 10, 10, 10);

    m_centerWnd->setLayout(m_vLayout);
    m_centerWnd->setFixedSize(640, 420);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_titleLabel = new QLabel(m_centerWnd);
    m_minBtn = new QPushButton(m_centerWnd);
    m_closeBtn = new QPushButton(m_centerWnd);

    m_titleLabel->setText("����Ⱥ��");
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);
    // m_minBtn->setStyleSheet("border:0px;");

    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);
    //   m_closeBtn->setStyleSheet("border:0px");

    // ������
    m_hLayout1 = new QHBoxLayout();
    m_hLayout1->addWidget(m_titleLabel);
    m_hLayout1->addStretch();
    m_hLayout1->addWidget(m_minBtn);
    m_hLayout1->addWidget(m_closeBtn);

    m_vLayout->addLayout(m_hLayout1);

    m_hLayout2 = new QHBoxLayout();
    m_vLayout1 = new QVBoxLayout();
    m_serchEdit = new QLineEdit();
    m_listWnd1 = new QListWidget();

    m_listWnd1->setContentsMargins(0, 0, 0, 0);
    m_listWnd1->setFixedHeight(300);

    m_serchEdit->setFixedWidth(280);
    m_serchEdit->setPlaceholderText("������ҹؼ���:");

    m_vLayout1->addWidget(m_serchEdit);
    m_vLayout1->addWidget(m_listWnd1);
    m_hLayout2->addLayout(m_vLayout1);

    QSelfSplit* sp = new QSelfSplit(nullptr, QSelfSplit::Direction_H);
    m_hLayout2->addWidget(sp);

    m_vLayout2 = new QVBoxLayout();
    m_hasSelLabel = new QLabel();
    m_listWnd2 = new QListWidget();
    m_groupNameEdit = new QLineEdit();

    m_hasSelLabel->setText("��ѡ��ϵ��:0");
    m_listWnd2->setFixedHeight(280);

    m_groupNameEdit->setPlaceholderText("������Ⱥ����:");

    m_hLayout3 = new QHBoxLayout();
    m_comfirmBtn = new QPushButton();

    m_comfirmBtn->setText("ȷ��");

    m_hLayout3->addStretch();
    m_hLayout3->addWidget(m_comfirmBtn);

    m_vLayout2->addWidget(m_hasSelLabel);
    m_vLayout2->addWidget(m_listWnd2);
    m_vLayout2->addStretch();
    m_vLayout2->addWidget(m_groupNameEdit);
    m_vLayout2->addLayout(m_hLayout3);

    m_hLayout2->addLayout(m_vLayout2);
    m_vLayout->addLayout(m_hLayout2);

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
    connect(m_comfirmBtn, SIGNAL(clicked()), this, SLOT(slotComfirmBtnClick()));
}

void QCreateGroupWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void QCreateGroupWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void QCreateGroupWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void QCreateGroupWnd::addListWnd1Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    QCreateGroupListItemWithSelBtnWnd* pListwnd1Item = new QCreateGroupListItemWithSelBtnWnd(m_listWnd1, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd1);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd1->setItemWidget(pListItem, pListwnd1Item);

    connect(pListwnd1Item, SIGNAL(signalSelRBtnClick(QMap<QString, QString>)), this, SLOT(slotSelRBtnClick(QMap<QString, QString>)));
}

void QCreateGroupWnd::addListWnd2Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename)
{
    QCreateGroupListItemWnd* pListwnd2Item = new QCreateGroupListItemWnd(m_listWnd2, headUrl, friendid, nickname, rolename);
    QListWidgetItem* pListItem = new QListWidgetItem(m_listWnd2);
    pListItem->setSizeHint(QSize(275, 35));
    m_listWnd2->setItemWidget(pListItem, pListwnd2Item);
}

bool QCreateGroupWnd::hasThisWndByRolename(QString rolename, QListWidget* listWnd)
{
    bool bHas = false;
    if (listWnd == nullptr)
    {
        return bHas;
    }

    for (int i = 0; i < listWnd->count(); i++)
    {
        QListWidgetItem* item = listWnd->item(i);
        if (listWnd == m_listWnd1)
        {
            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                bHas = true;
                break;
            }
        }
    }
    return bHas;
}

void QCreateGroupWnd::delThisWndByRolename(QString rolename, QListWidget* listWnd)
{
    for (int i = 0; i < listWnd->count(); i++)
    {
        QListWidgetItem* item = listWnd->item(i);
        if (listWnd == m_listWnd1)
        {
            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                listWnd->takeItem(i);
                break;
            }
        }

        if (listWnd == m_listWnd2)
        {
            QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(listWnd->itemWidget(item));
            if (wnd->m_roleName->text() == rolename)
            {
                // listWnd->removeItemWidget(item);
                listWnd->takeItem(i);
                break;
            }
        }
    }
}

void QCreateGroupWnd::updateData()
{
    //���Ҳ�����������Ӧ����
    auto itMap = QDataManager::getMgr()->m_FriendId2NameMap.begin();
    auto itMapEnd = QDataManager::getMgr()->m_FriendId2NameMap.end();
    for (; itMap != itMapEnd; itMap++)
    {
        if (hasThisWndByRolename(itMap->second, m_listWnd1) == false)
        {
            addListWnd1Item("./img/head2.png", itMap->first, QString::number(itMap->first).toStdString().c_str(), itMap->second.toStdString().c_str());
        }
    }

    //�Ҳ�������ѡ�е�״̬
    {
        int count = m_listWnd1->count();
        for (int index = 0; index < count; index++)
        {
            QListWidgetItem* item = m_listWnd1->item(index);
            if (item == nullptr)
            {
                continue;
            }

            QCreateGroupListItemWithSelBtnWnd* wnd = dynamic_cast<QCreateGroupListItemWithSelBtnWnd*>(m_listWnd1->itemWidget(item));
            if (wnd == nullptr)
            {
                continue;
            }

            //���ѡ�е�״̬
            wnd->m_selRBtn->setChecked(false);
        }
    }

    //�Ƴ��������е�����
    {
        QListWidgetItem* item;
        int count = m_listWnd2->count();
        for (int index = 0; index < count; index++)
        {
            item = m_listWnd2->takeItem(0);
            delete item;
        }
    }

    m_hasSelLabel->setText("��ѡ��ϵ��:0");
    m_serchEdit->setPlaceholderText("������ҹؼ���`-`");
    m_groupNameEdit->setPlaceholderText("������Ⱥ����`-`");
    m_serchEdit->setText("");
    m_groupNameEdit->setText("");
}

void QCreateGroupWnd::closeWnd()
{
    hide();
}

void QCreateGroupWnd::minWnd()
{
    showMinimized();
}

void QCreateGroupWnd::slotSelRBtnClick(QMap<QString, QString> map)
{
    // LogDebug << "map" << map;
    if (map["isSel"] == "true")
    {
        if (hasThisWndByRolename(map["rolename"], m_listWnd2) == false)
        {
            addListWnd2Item(map["headimg"].toLocal8Bit().data(),
                            map["friendid"].toInt(),
                            map["nickname"].toLocal8Bit().data(),
                            map["rolename"].toLocal8Bit().data());
        }
    }
    else
    {
        delThisWndByRolename(map["rolename"], m_listWnd2);
    }

    //��ȡm_listwnd2�к��е���Ŀ�������޸�lable�е�����
    QString str = QString("��ѡ��ϵ��:%1").arg(m_listWnd2->count());
    m_hasSelLabel->setText(str);
}

void QCreateGroupWnd::slotComfirmBtnClick()
{
    //��ֱ�����ش���Ⱥ�ĵĶԻ���
    hide();

    //��ȡ��ǰ���id����ȡȺ������
    int ownerid = QDataManager::getMgr()->m_userid;
    QString groupname = m_groupNameEdit->text();
    // LogDebug << "ownerid:" << ownerid << "groupname:" << groupname;
    if (groupname == "")
    {
        // LogDebug << "groupname is empty()";
        QMessageBox::information(nullptr, "info", "��������ȷ��Ⱥ������");
        return;
    }
    //��ȡȺ�ĳ�Ա��Ϣ
    std::vector<int64_t> groupfriendIdVct;
    for (int i = 0; i < m_listWnd2->count(); i++)
    {
        QListWidgetItem* item = m_listWnd2->item(i);
        if (item == NULL)
        {
            continue;
        }

        QCreateGroupListItemWnd* wnd = dynamic_cast<QCreateGroupListItemWnd*>(m_listWnd2->itemWidget(item));
        if (wnd == NULL)
        {
            continue;
        }
        LogDebug << "groupfrindIdVct add " << wnd->m_friendid;
        groupfriendIdVct.push_back(wnd->m_friendid);
    }
    //�������
    neb::CJsonObject json;
    json.Add("createid", ownerid);
    json.Add("groupname", groupname.toStdString());
    json.AddEmptySubArray("groupfriends");
    for (int i = 0; i < groupfriendIdVct.size(); i++)
    {
        json["groupfriends"].Add(groupfriendIdVct[i]);
    }
    //��Զ�˷�������������
    QWSClientMgr::getMgr()->request("cs_msg_create_group", json, [this, groupname](neb::CJsonObject& msg) {
        LogDebug << "msg:" << msg.ToString().c_str();
        //���ڷ���˽������� createid��groupname,groupid,groupfriends
        int groupid = -1;
        if (!msg["data"].Get("groupid", groupid))
        {
            LogDebug << "msg[\"data\"] can not find groupid!";
            return;
        }
        //���յ�����Ⱥ��ɹ���ʱ�����б���Ƕ��һ������
        QMainWnd::getMainWnd()->m_commGroupsListWnd->addGroupItem("./img/groupHead.png", groupname.toStdString().c_str(), groupid);
    });
}
