#include "QGroupInfoWnd.h"
#include <QApplication>
#include "QSimpleSplit.h"
#include "QGroupFriendsItemWnd.h"
#include <QScrollArea>

#include "QStyleSheetMgr.h"

QGroupInfoWnd::QGroupInfoWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QGroupInfoWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    // setFixedSize(250, 535);
    setFixedWidth(265);
    setMinimumHeight(535);

    // ��������
    m_scrollArea = new QScrollArea(this);
    m_centerWnd = new QWidget(this);

    m_centerWnd->setObjectName("QGroupInfoCenterWnd");
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_scrollArea->setGeometry(0, 0, width(), height());

    m_vLayout = new QVBoxLayout();
    m_centerWnd->setLayout(m_vLayout);

    m_vLayout->setContentsMargins(0, 0, 0, 0);

    //��ʼ��װ
    m_groupfriendsWnd = new QGroupFriendsWnd(this);
    m_groupfriendsWnd->setObjectName("QGroupFriendsWnd");
    m_groupfriendsWnd->setFixedHeight(249);
    m_vLayout->addWidget(m_groupfriendsWnd);

    m_simpleGroupInfoWnd = new QWidget(this);
    m_simpleGroupInfoWnd->setObjectName("QSimpleGroupInfoWnd");

    object.m_qssFileName = "./stylesheet/" + m_simpleGroupInfoWnd->objectName() + ".qss";
    object.m_widget = m_simpleGroupInfoWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout1 = new QVBoxLayout();
    m_simpleGroupInfoWnd->setLayout(m_vLayout1);
    m_vLayout->addWidget(m_simpleGroupInfoWnd);

    m_groupName1 = new QLabel();
    m_groupName1->setText("Ⱥ������");
    m_vLayout1->addWidget(m_groupName1);

    m_groupName2 = new QEditLabel();
    m_groupName2->setFixedSize(200, 25);
    m_groupName2->setText("���ڴ˴����Ⱥ������...");
    m_vLayout1->addWidget(m_groupName2);

    m_groupNotice1 = new QLabel();
    m_groupNotice1->setText("Ⱥ����");
    m_vLayout1->addWidget(m_groupNotice1);

    m_groupNotice2 = new QEditLabel();
    m_groupNotice2->setText("���ڴ˴�����Ⱥ����...");
    m_groupNotice2->setFixedSize(200, 25);
    m_vLayout1->addWidget(m_groupNotice2);

    // �ṩ�ָ���
    m_vLayout1->addSpacing(5);

    m_groupRemarks1 = new QLabel();
    m_groupRemarks1->setText("��ע");
    m_vLayout1->addWidget(m_groupRemarks1);

    // �ṩ�ָ���
    m_vLayout1->addSpacing(5);

    m_groupRemarks2 = new QEditLabel();
    m_groupRemarks2->setText("���ڴ˴�����Ⱥ��ע...");
    m_groupRemarks2->setFixedSize(200, 25);
    m_vLayout1->addWidget(m_groupRemarks2);

    // �ṩ�ָ���
    m_vLayout1->addSpacing(5);

    m_groupRoleName1 = new QLabel();
    m_groupRoleName1->setText("���ڱ�Ⱥ���ǳ�");
    m_vLayout1->addWidget(m_groupRoleName1);

    // �ṩ�ָ���
    m_vLayout1->addSpacing(5);

    m_groupRoleName2 = new QEditLabel();
    m_groupRoleName2->setText("���ڴ˴����ñ�ע...");
    m_groupRoleName2->setFixedSize(200, 25);
    ;
    m_vLayout1->addWidget(m_groupRoleName2);

    m_simpleGroupSettingWnd = new QWidget(this);
    m_simpleGroupSettingWnd->setObjectName("QSimpleGroupSettingWnd");
    object.m_qssFileName = "./stylesheet/" + m_simpleGroupSettingWnd->objectName() + ".qss";
    object.m_widget = m_simpleGroupSettingWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout->addWidget(m_simpleGroupSettingWnd);
    m_vLayout2 = new QVBoxLayout();

    m_simpleGroupSettingWnd->setLayout(m_vLayout2);

    m_hLayout21 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout21);

    m_showGroupFriendsRoleNameLabel = new QLabel();
    m_showGroupFriendsRoleNameLabel->setText("��ʾȺ��Ա�ǳ�");
    m_hLayout21->addWidget(m_showGroupFriendsRoleNameLabel);

    m_showGroupFriendsRoleNameBtn = new QSwitchButton();
    m_hLayout21->addWidget(m_showGroupFriendsRoleNameBtn);
    m_hLayout21->addSpacing(40);

    //
    m_hLayout22 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout22);
    m_MsgFreeLabel = new QLabel();
    m_MsgFreeLabel->setText("��Ϣ�����");
    m_hLayout22->addWidget(m_MsgFreeLabel);

    m_MsgFreeBtn = new QSwitchButton();
    m_hLayout22->addWidget(m_MsgFreeBtn);
    m_hLayout22->addSpacing(40);

    m_hLayout23 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout23);
    m_topWndLabel = new QLabel();
    m_topWndLabel->setText("�ö�����");
    m_hLayout23->addWidget(m_topWndLabel);

    m_topWndBtn = new QSwitchButton();
    m_hLayout23->addWidget(m_topWndBtn);
    m_hLayout23->addSpacing(40);

    m_hLayout24 = new QHBoxLayout();
    m_vLayout2->addLayout(m_hLayout24);

    m_save2AddrBookLabel = new QLabel();
    m_save2AddrBookLabel->setText("���浽ͨѶ¼");
    m_hLayout24->addWidget(m_save2AddrBookLabel);

    m_save2AddrBookBtn = new QSwitchButton();
    m_hLayout24->addWidget(m_save2AddrBookBtn);
    m_hLayout24->addSpacing(40);

    m_delexitWnd = new QWidget(this);
    m_delexitWnd->setObjectName("QGroupInfoExitWnd");
    object.m_qssFileName = "./stylesheet/" + m_delexitWnd->objectName() + ".qss";
    object.m_widget = m_delexitWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_vLayout->addWidget(m_delexitWnd);
    m_vLayout3 = new QVBoxLayout();
    m_delexitWnd->setLayout(m_vLayout3);

    m_delexitBtn = new QPushButton();
    m_delexitBtn->setText("ɾ�����˳�");

    m_delexitBtn->setFixedSize(220, 30);
    m_vLayout3->addWidget(m_delexitBtn);

    m_vLayout->addStretch();

    //�����ޱ߿�
   // setAttribute(Qt::WA_StyledBackground);
   setWindowFlags(Qt::FramelessWindowHint);

    m_scrollArea->setWidget(m_centerWnd);
}

bool QGroupInfoWnd::event(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if (QApplication::activeWindow() != this)
        {
            this->hide();
        }
    }
    return QWidget::event(event);
}
