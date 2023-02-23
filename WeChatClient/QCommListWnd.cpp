#include <QDebug>
#include <QScrollBar>
#include "QMainWnd.h"

#include "QSelectWnd1.h"
#include "QSimpleSplit.h"
#include "QCommListWnd.h"
#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include "QCustomListWidgetItem.h"
#include "QCommGroupItemWnd.h"

QCommListWnd::QCommListWnd(QWidget* p /*= nullptr*/, QCommListWndEnum wndType/*QCommMsgItemWnd_Tpye*/)
	: QWidget(p),m_WndType(wndType)
{
	m_vLayout = new QVBoxLayout();
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_vLayout->setSpacing(0);
	setLayout(m_vLayout);
	setFixedWidth(250);
	
	m_hLayout = new QHBoxLayout();
	m_hLayout->setContentsMargins(0, 0, 0, 0);
	m_hLayout->setSpacing(0);
	m_searchEdit = new QLineEdit(this);
	m_startGroupBtn = new QPushButton(this);
	m_searchEdit->setFixedHeight(25);
	m_startGroupBtn->setFixedSize(25,25);
	m_startGroupBtn->setIcon(QPixmap("./img/startGroupBtn.png"));
	m_startGroupBtn->setIconSize(QSize(25, 25));

	m_hLayout->addSpacing(10);
	m_hLayout->addWidget(m_searchEdit);
	m_hLayout->addSpacing(10);
	m_hLayout->addWidget(m_startGroupBtn);
	m_hLayout->addSpacing(10);
	m_vLayout->addSpacing(25);
	m_vLayout->addLayout(m_hLayout);
	m_vLayout->addSpacing(15);

	{
		/*添加分割线的示例代码*/
		QSimpleSplit* sp = new QSimpleSplit(this);
		m_vLayout->addWidget(sp);
	}
	
	m_listWidget = new QListWidget();
	m_listWidget->setFixedWidth(250);
	m_listWidget->setAttribute(Qt::WA_StyledBackground);
	m_listWidget->setWindowFlags(Qt::FramelessWindowHint);
	m_vLayout->addWidget(m_listWidget);

	m_selectWnd1 = new QSelectWnd1(nullptr);
	m_selectWnd1->hide();

	setFixedWidth(250);
	setObjectName("QCommListWnd");
	setStyleSheet("#QCommListWnd{background:white;border:0px;}");
	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(Qt::FramelessWindowHint);


	m_listWidget->setStyleSheet("border:0px;");
	connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onCurrentItemClicked(QListWidgetItem*)));
	connect(m_startGroupBtn, SIGNAL(clicked()), this, SLOT(onStartGroupBtnClicked()));

	


}

void QCommListWnd::onCurrentItemClicked(QListWidgetItem* item)
{
	// 自定义列表 
	// 包含如下列表信息：联系人列表、消息列表、群聊列表
	QCustomListWidgetItem* pCustItem = dynamic_cast<QCustomListWidgetItem*>(item);
	qDebug() << __FUNCTION__  << "sesid:" << pCustItem->sesId() << "\n";
	
	//当前点击的是联系人列表中的某一项目
	if (m_WndType == QCommContactItemWnd_Type) 
	{
		QCommContactItemWnd* wnd = dynamic_cast<QCommContactItemWnd*>(m_listWidget->itemWidget(pCustItem));
		if (wnd->m_bNewFriend)
		{
			QMainWnd::getInstance()->m_dealNewFriendsApplyWnd->setFriendApplyList();
			//切换窗口到新的朋友窗口
			//m_sLayout2中位置为1的窗口为新的朋友的窗口
			QMainWnd::getInstance()->m_sLayout2->setCurrentIndex(1);
		}
		else
		{
			// 设置信息
			// 该联系人的名字和该联系人的id信息
			QMap<QString, QString> infoMap;
			infoMap["name"] = wnd->m_name->text();
			infoMap["friendid"] = QString::number(wnd->m_friendId,10);
			
			//切换到展示联系人信息的页面
			QMainWnd::getInstance()->m_sLayout2->setCurrentIndex(0);

			//联系人的信息改变
			emit signal_contactInfoChange(infoMap);
		}
	}
	
	//当前点击的是消息列表中的某一项
	if(m_WndType == QCommMsgItemWnd_Tpye)
	{
		QCommMsgItemWnd* wnd = dynamic_cast<QCommMsgItemWnd*>(m_listWidget->itemWidget(pCustItem));
		qint64 sesid = wnd->m_sesId;
		commListChanged(sesid);
	}

	//当前点击的我是群列表中的某一项
	if (m_WndType == QCommGroupItemWnd_Type)
	{
		//判断当前点击是那一项
		QCommGroupItemWnd* wnd = dynamic_cast<QCommGroupItemWnd*>(m_listWidget->itemWidget(pCustItem));
		//获取到群的id
		int groupid = wnd->m_groupId;
		//右边工具栏需要跳转到msg那一栏
		QMainWnd::getInstance()->m_toolWnd->m_msgBtn->click();
		//先找到消息列表中所在的位置
		auto msgListWidget = QMainWnd::getInstance()->m_commMsgListWnd->m_listWidget;

		//int sesid = -1;
		for (int i = 0; i < msgListWidget->count(); i++)
		{
			QListWidgetItem* pitem = msgListWidget->item(i);
			QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(msgListWidget->itemWidget(pitem));
			if (pWnd->m_isGroupMsg && pWnd->m_userid == groupid)
			{
				//sesid = pWnd->m_sesId;
				msgListWidget->setCurrentItem(pitem);
				QMainWnd::getInstance()->slot_sesIdToIndex(pWnd->m_sesId);
				break;
			}
		}

		//if (sesid != -1)
		//{
		//	QMainWnd::getSinletonInstance()->slot_sesIdToIndex(sesid);
		//}
	}
}

void QCommListWnd::onStartGroupBtnClicked()
{
	//qDebug() << "onStartGroupBtnClicked()";
	QRect rect = m_startGroupBtn->geometry();
	qDebug() << "rect" << rect << endl;
	qDebug() << " the pos of startGroupBtn: " << m_startGroupBtn->pos();
	QPoint gPoint = m_startGroupBtn->mapToGlobal(QPoint(0,0));
	
	/*选择的窗口只能允许出现一个*/
	QRect swRect = m_selectWnd1->geometry();
	swRect.setX(gPoint.x() - m_selectWnd1->width()+m_startGroupBtn->width());
	swRect.setY(gPoint.y() + m_startGroupBtn->height() + 5);
	m_selectWnd1->setGeometry(swRect);
	m_selectWnd1->show();
}

QListWidgetItem* QCommListWnd::addMsgItem(const char* headUrl, const char* name, const char*  msg,qint64 sesid,int64_t userid, bool isGroupMsg)
{

	QCommMsgItemWnd* pMsgItem = new QCommMsgItemWnd(m_listWidget,headUrl,name,msg,sesid,userid,isGroupMsg);
	QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
	pMsgItem->setFixedWidth(this->width()-5);
	pListItem->setSizeHint(QSize(this->width()-5, 65));
	m_listWidget->setItemWidget(pListItem, pMsgItem);
	return pListItem;
}

void QCommListWnd::addContactsItem(const char* headUrl, const char*  name, bool isNewFriend /*= false*/, int friendid/* = -1*/)
{
	QCommContactItemWnd* pMsgItem = new QCommContactItemWnd(m_listWidget,headUrl,name,isNewFriend,friendid);
	QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
	pMsgItem->setFixedWidth(this->width()-5);
	pListItem->setSizeHint(QSize(this->width()-5, 65));
	m_listWidget->setItemWidget(pListItem, pMsgItem);
}

void QCommListWnd::addGroupItem(const char* headUrl, const char* name, int groupid)
{
	QCommGroupItemWnd* pGroupItem = new QCommGroupItemWnd(m_listWidget, headUrl, name, groupid);
	QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
	pGroupItem->setFixedWidth(this->width() - 5);
	pListItem->setSizeHint(QSize(this->width() - 5, 65));
	m_listWidget->setItemWidget(pListItem, pGroupItem);
}

