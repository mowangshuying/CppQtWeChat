#pragma once


#include "QPch.h"

#include <QWidget>

#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class QCreateGroupWnd : public QWidget {
	Q_OBJECT
public:
	QCreateGroupWnd(QWidget* p = nullptr);

	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void addListWnd1Item(const char* headUrl,int64_t friendid,const char* nickname,const char* rolename);
	void addListWnd2Item(const char* headUrl, int64_t friendid, const char* nickname, const char* rolename);

	//ͨ��rolename�ж�listwnd2���Ƿ��Ѿ���Ӵ˴���
	bool hasThisWndByRolename(QString rolename,QListWidget* listWnd);
	//ͨ��rolename�Ƴ�listwnd2���д˴���
	void delThisWndByRolename(QString rolename, QListWidget* listWnd);

	//��Ҫ�Ƕ�ȡ��ǰ�м������Ѳ���ʾ
	void updateData();
public slots:
	void closeWnd();
	void minWnd();

	//���յ����Ե�ѡ�İ�ť��ѡ��ʱ��
	void slot_selRBtnClick(QMap<QString, QString> map);
	//���ȷ�ϰ�ť����Ӧ
	void slot_comfirmBtnClick();
public:
	QVBoxLayout* m_vLayout;

	QHBoxLayout* m_hLayout1;
	
	//�ṩ����֧�֣�title,�رպ���С��ť
	QHBoxLayout* m_hLayout2;
	QLabel* m_titleLabel;
	QPushButton* m_minBtn;
	QPushButton* m_closeBtn;

	QLineEdit* m_serchEdit;
	QListWidget* m_listWnd1;
	QVBoxLayout* m_vLayout1;

	QLabel* m_hasSelLabel;
	QListWidget* m_listWnd2;
	QLineEdit* m_groupNameEdit;
	QHBoxLayout* m_hLayout3;
	QPushButton* m_comfirmBtn;
	QVBoxLayout* m_vLayout2;

	bool m_bPress = false;
	QPoint m_poPress;

};