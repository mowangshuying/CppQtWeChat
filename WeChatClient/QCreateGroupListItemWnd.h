#pragma once


#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMap>

class QCreateGroupListItemWnd : public QWidget 
{
	Q_OBJECT
public:
	QCreateGroupListItemWnd(QWidget* p = nullptr);
	QCreateGroupListItemWnd(QWidget* p, const char* headimg, int64_t friendid, const char* nickname, const char* rolename);
//signals:
//	void signal_selRBtnClick(QMap<QString,QString> map);
//public slots:
//	void slot_clickedSelBtn(bool isSel = false);
public:
	QHBoxLayout* m_hLayout;
	QLabel* m_headImage;
	QLabel* m_nickName;
	QLabel* m_roleName;
	QString m_headImgStr;
	int64_t m_friendid;
	
	//QRadioButton* m_selRBtn;
};