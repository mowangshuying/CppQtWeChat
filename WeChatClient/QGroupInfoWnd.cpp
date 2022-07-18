#include "QGroupInfoWnd.h"
#include <QApplication>
#include "QSimpleSplit.h"
#include "QGroupFriendsItemWnd.h"
#include <QScrollArea>

QGroupInfoWnd::QGroupInfoWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
	//
	setFixedSize(250, 535);

	// 滚动区域
	m_scrollArea = new QScrollArea(this);
	m_centerWnd = new QWidget(this);
	m_scrollArea->setGeometry(0, 0, 250, 535);

	m_vLayout = new QVBoxLayout();
	m_centerWnd->setLayout(m_vLayout);

	m_vLayout->setContentsMargins(0, 0, 0, 0);

	//开始组装
	m_groupfriendsWnd = new QGroupFriendsWnd();
	m_groupfriendsWnd->setFixedHeight(249);
	m_groupfriendsWnd->setStyleSheet("border:0px;");
	m_vLayout->addWidget(m_groupfriendsWnd);


	{
		QSimpleSplit* sp = new QSimpleSplit();
		m_vLayout->addWidget(sp);
	}

	m_simpleGroupInfoWnd = new QWidget();
	m_simpleGroupInfoWnd->setStyleSheet("border:0px");
	m_vLayout1 = new QVBoxLayout();
	m_simpleGroupInfoWnd->setLayout(m_vLayout1);
	m_vLayout->addWidget(m_simpleGroupInfoWnd);
	
	m_groupName1 = new QLabel();
	m_groupName1->setText("群聊名称");
	//m_groupName1->setStyleSheet("font-size:14px;border:0px");
	m_vLayout1->addWidget(m_groupName1);

	// 提供分割线
	//m_vLayout1->addSpacing(5);

	m_groupName2 = new QEditLabel();
	//m_groupName2->setFixedHeight(25);
	m_groupName2->setStyleSheet("border:1px solid #828790;");
	m_groupName2->setFixedSize(200,25);
	m_vLayout1->addWidget(m_groupName2);

	// 提供分割线
	//m_vLayout1->addSpacing(5);

	m_groupNotice1 = new QLabel();
	m_groupNotice1->setText("群公告");
	m_groupNotice1->setStyleSheet("font-size:14px");
	//m_groupNotice1->setFixedHeight(25);
	m_vLayout1->addWidget(m_groupNotice1);

	// 提供分割线
	//m_vLayout1->addSpacing(5);

	m_groupNotice2 = new QEditLabel();
	//m_groupNotice2->setText("未设置");
	m_groupNotice2->setFixedSize(200,25);
	m_groupNotice2->setStyleSheet("border:1px solid #828790;");
	m_vLayout1->addWidget(m_groupNotice2);

	// 提供分割线
	m_vLayout1->addSpacing(5);

	m_groupRemarks1 = new QLabel();
	m_groupRemarks1->setText("备注");
	m_groupRemarks1->setStyleSheet("font-size:14px");
	m_vLayout1->addWidget(m_groupRemarks1);

	// 提供分割线
	m_vLayout1->addSpacing(5);

	m_groupRemarks2 = new QEditLabel();
	//m_groupRemarks2->setText("群聊的备仅自己可见");
	m_groupRemarks2->setFixedSize(200, 25);
	m_groupRemarks2->setStyleSheet("border:1px solid #828790;");
	m_vLayout1->addWidget(m_groupRemarks2);

	// 提供分割线
	m_vLayout1->addSpacing(5);

	m_groupRoleName1 = new QLabel();
	m_groupRoleName1->setText("我在本群的昵称");
	m_groupRoleName1->setStyleSheet("font-size:14px");
	m_vLayout1->addWidget(m_groupRoleName1);

	// 提供分割线
	m_vLayout1->addSpacing(5);

	m_groupRoleName2 = new QEditLabel();
	m_groupRoleName2->setText("白衣沽酒");
	m_groupRoleName2->setFixedSize(200, 25);;
	m_groupRoleName2->setStyleSheet("border:1px solid #828790");
	m_vLayout1->addWidget(m_groupRoleName2);

	{
		QSimpleSplit* sp = new QSimpleSplit();
		m_vLayout->addWidget(sp);
	}

	m_simpleGroupSettingWnd = new QWidget();
	m_simpleGroupSettingWnd->setStyleSheet("border:0px;");
	m_vLayout->addWidget(m_simpleGroupSettingWnd);
	m_vLayout2 = new QVBoxLayout();
	//m_vLayout2->setSpacing(5);

	m_simpleGroupSettingWnd->setLayout(m_vLayout2);


	m_hLayout21 = new QHBoxLayout();
	m_vLayout2->addLayout(m_hLayout21);

	m_showGroupFriendsRoleNameLabel = new QLabel();
	m_showGroupFriendsRoleNameLabel->setText("显示群成员昵称");
	m_showGroupFriendsRoleNameLabel->setStyleSheet("font-size:14px");
	m_hLayout21->addWidget(m_showGroupFriendsRoleNameLabel);

	m_showGroupFriendsRoleNameBtn = new QSwitchButton();
	m_hLayout21->addWidget(m_showGroupFriendsRoleNameBtn);
	m_hLayout21->addSpacing(40);

	//
	m_hLayout22 = new QHBoxLayout();
	m_vLayout2->addLayout(m_hLayout22);
	m_MsgFreeLabel = new QLabel();
	m_MsgFreeLabel->setText("消息免打扰");
	m_MsgFreeLabel->setStyleSheet("font-size:14px");
	//m_vLayout2->addWidget(m_MsgFreeLabel);
	m_hLayout22->addWidget(m_MsgFreeLabel);

	m_MsgFreeBtn = new QSwitchButton();
	m_hLayout22->addWidget(m_MsgFreeBtn);
	m_hLayout22->addSpacing(40);

	m_hLayout23 = new QHBoxLayout();
	m_vLayout2->addLayout(m_hLayout23);
	m_topWndLabel = new QLabel();
	m_topWndLabel->setText("置顶聊天");
	m_topWndLabel->setStyleSheet("font-size:14px");
	//m_vLayout2->addWidget(m_topWndLabel);
	m_hLayout23->addWidget(m_topWndLabel);

	m_topWndBtn = new QSwitchButton();
	m_hLayout23->addWidget(m_topWndBtn);
	m_hLayout23->addSpacing(40);

	m_hLayout24 = new QHBoxLayout();
	m_vLayout2->addLayout(m_hLayout24);

	m_save2AddrBookLabel = new QLabel();
	m_save2AddrBookLabel->setText("保存到通讯录");
	m_save2AddrBookLabel->setStyleSheet("font-size:14px");
	//m_vLayout2->addWidget(m_save2AddrBookLabel);
	m_hLayout24->addWidget(m_save2AddrBookLabel);

	m_save2AddrBookBtn = new QSwitchButton();
	m_hLayout24->addWidget(m_save2AddrBookBtn);
	m_hLayout24->addSpacing(40);

	{
		QSimpleSplit* sp = new QSimpleSplit();
		m_vLayout->addWidget(sp);
	}

	m_delexitWnd = new QWidget();
	m_delexitWnd->setStyleSheet("border:0px;");
	m_vLayout->addWidget(m_delexitWnd);
	m_vLayout3 = new QVBoxLayout();
	m_delexitWnd->setLayout(m_vLayout3);

	m_delexitBtn = new QPushButton();
	m_delexitBtn->setText("删除并退出");
	m_delexitBtn->setStyleSheet("border:0px;background-color:#f00;font-size:14px;height:30px;");
	m_delexitBtn->setFixedSize(220, 30);
	m_vLayout3->addWidget(m_delexitBtn);


	m_vLayout->addStretch();

	//setMinimumHeight(700);
	//设置无边框
	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	
	setStyleSheet("background-color:#fff;");
	m_centerWnd->setStyleSheet("border:1px;");	//setContentsMargins(0, 0, 10, 0);
	m_scrollArea->setWidget(m_centerWnd);
	m_scrollArea->setStyleSheet("border-top:1px solid #828790;border-bottom:0px;border-right:0px;border-left:1px solid #828790;");
}

bool QGroupInfoWnd::event(QEvent* event) {
	if (event->type() == QEvent::ActivationChange)
	{
		if (QApplication::activeWindow() != this)
		{
			this->hide();
		}
	}
	return QWidget::event(event);
}

