#include "QSimpleSplit.h"
#include "QFindFriendItemWnd.h"
#include "QFindFriendOrGroupWnd.h"
#include "QWSClientMgr.h"
#include "./json/CJsonObject.hpp"

#include <QMessageBox>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QScrollBar>

QFindFriendOrGroupWnd::QFindFriendOrGroupWnd(QWidget* p /*= nullptr*/) 
	: QWidget(p)
{
	m_vLayout = new QVBoxLayout(this);
	m_vLayout->setContentsMargins(5, 5, 5, 5);
	setLayout(m_vLayout);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(600, 400);
	
	m_vLayout->addSpacing(5);

	m_hLayout1 = new QHBoxLayout();
	m_titleLabel = new QLabel(this);
	m_minBtn = new QPushButton(this);
	m_closeBtn = new QPushButton(this);

	m_hLayout1->addWidget(m_titleLabel);
	m_titleLabel->setText("查找");
	m_hLayout1->addStretch();
	m_hLayout1->addWidget(m_minBtn);
	m_hLayout1->addWidget(m_closeBtn);

	m_minBtn->setIcon(QPixmap("./img/minWnd.png"));
	m_minBtn->setIconSize(QSize(20, 20));
	m_minBtn->setFixedSize(20, 20);

	m_closeBtn->setIcon(QPixmap("./img/closeWnd.png"));
	m_closeBtn->setIconSize(QSize(20, 20));
	m_closeBtn->setFixedSize(20, 20);
	m_vLayout->addLayout(m_hLayout1);

	QSimpleSplit* pSplitWnd1 = new QSimpleSplit(this);
	m_vLayout->addWidget(pSplitWnd1);

	m_hLayout2 = new QHBoxLayout();
	m_searchEdit = new QLineEdit();
	m_findPersonChx = new QCheckBox();
	m_findPersonChx->setText("找人");
	m_findGroupChx = new QCheckBox();
	m_findGroupChx->setText("找群");
	m_searchBtn = new QPushButton();

	m_searchEdit->setFixedSize(350, 30);

	m_hLayout2->addWidget(m_searchEdit);

	m_hLayout2->addWidget(m_findPersonChx);
	m_hLayout2->addWidget(m_findGroupChx);

	m_hLayout2->addSpacing(15);

	m_hLayout2->addWidget(m_searchBtn);

	m_hLayout2->addStretch();

	m_searchBtn->setText("查找");
	m_searchBtn->setFixedSize(100, 30);
	m_searchBtn->setStyleSheet("background-color:#1aad19;border-style: none;");

	m_vLayout->addLayout(m_hLayout2);

	QSimpleSplit* pSplitWnd2 = new QSimpleSplit(this);
	m_vLayout->addWidget(pSplitWnd2);
	
	m_listWidget = new QListWidget(this);
	m_listWidget->setFixedHeight(330);
	m_listWidget->setStyleSheet("border:0px;");
	m_vLayout->addWidget(m_listWidget);

	m_vLayout->addStretch();


	connect(m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
	connect(m_searchBtn, SIGNAL(clicked()), this, SLOT(slot_onSearchBtnClicked()));
}

void QFindFriendOrGroupWnd::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bPress)
	{
		move(event->pos() - m_poPress + pos());
	}
}

void QFindFriendOrGroupWnd::mousePressEvent(QMouseEvent* event)
{
	m_bPress = true;
	m_poPress = event->pos();
}

void QFindFriendOrGroupWnd::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_bPress = false;
}

void QFindFriendOrGroupWnd::addFriendItem(const char* headUrl, const char* name, int64_t userid)
{
	//添加好友
	QFindFriendItemWnd* pMsgItem = new QFindFriendItemWnd(m_listWidget, headUrl, name);
	pMsgItem->m_friendid = userid;

	QListWidgetItem* pListItem = new QListWidgetItem(m_listWidget);
	pMsgItem->setFixedWidth(this->width() - 5);
	pListItem->setSizeHint(QSize(this->width() - 5, 60));
	m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void QFindFriendOrGroupWnd::closeWnd()
{
	hide();
}

void QFindFriendOrGroupWnd::minWnd()
{
	showMinimized();
}

void QFindFriendOrGroupWnd::slot_onSearchBtnClicked()
{
	//点击了查找按钮向远端服务器发送查找请求
	if (m_findPersonChx->isChecked()) {
		//如果选择查找好友；
		std::string findstr =  m_searchEdit->text().toStdString();
		neb::CJsonObject json;
		json.Add("findstr",findstr);


		//清空listwidget内容
		while (m_listWidget->count() > 0)
		{
			QListWidgetItem* pitem = m_listWidget->takeItem(0);
			delete pitem;
		}

		QWSClientMgr::getInstance()->request("cs_msg_find_user", json, [this](neb::CJsonObject& msg) {

				   int state;
				   if (!msg.Get("state", state))
				   {
					   return;
				   }

				   ////解析注册消息数组；
				   auto data = msg["data"];
				   if (!data.IsArray())
				   {
					   return;
				   }

				   for (int i = 0; i < data.GetArraySize(); i++)
				   {
					   neb::CJsonObject jsonelem;
					   data.Get(i, jsonelem);
					  
					   int64_t userid;
					   if (!jsonelem.Get("userid", userid))
					   {
						   return;
					   }

					   std::string username;
					   if (!jsonelem.Get("username", username))
					   {
				   			return;
					   }

					   addFriendItem("./img/head2.png", username.c_str(),userid);

				   }

			});
	}
}

