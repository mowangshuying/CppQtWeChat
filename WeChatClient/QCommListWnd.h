#pragma once


#include "QPch.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMap>
#include <QString>

#include "QSelectWnd1.h"

class QCommListWnd : public QWidget
{
	//�����Ϣӳ��֧��
	Q_OBJECT

public:
	enum QCommListWndEnum
	{
		QCommMsgItemWnd_Tpye		=	0,
		QCommContactItemWnd_Type	=	1,
		QCommGroupItemWnd_Type		=	2,
	};

	QCommListWnd(QWidget* p = nullptr, QCommListWndEnum wndType = QCommMsgItemWnd_Tpye);

signals:
	void commListChanged(int num);
	void signal_contactInfoChange(QMap<QString,QString> infoMap);
public slots:
	void onCurrentItemClicked(QListWidgetItem* item);
	//�������Ǹ���ť�Ļ�
	void onStartGroupBtnClicked();
public:
	QListWidgetItem* addMsgItem(const char* headUrl, const char* name, const char* msg,qint64 sesid, int64_t userid,bool isGroupMsg);
	void addContactsItem(const char* headUrl, const char* name,bool isNewFriend = false,int friendid = -1);
	void addGroupItem(const char* headUrl, const char* name, int groupid);
	

	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;

	//������
	QLineEdit* m_searchEdit;
	//��ʼ���Ǹ���ť
	QPushButton* m_startGroupBtn;

	QSelectWnd1* m_selectWnd1;

	QListWidget* m_listWidget;
	//��������
	QCommListWndEnum m_WndType;
};