#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

class QApplyFriendWnd2 : public QWidget {
	Q_OBJECT
public:
	QApplyFriendWnd2(QWidget* p = nullptr);

public:
	//��Ҫ����
	QHBoxLayout* m_hLayout;

	//��߲���
	//QVBoxLayout* m_vLayout1;
	//�ұ߲���
	QVBoxLayout* m_vLayout;

	//��ϵ��ͷ��
	QLabel* m_headUrl;
	//��ɫ��
	QLabel* m_roleName;
	//������Ϣ
	QLabel* m_moreInfo;
	//�û���Ϣ����
	QWidget* m_userInfoWnd;
	QVBoxLayout* m_vUserInfoLayout;
	//
	QLabel* m_inMsgLabel;
	//QTextEdit* m_inMsgEdit;
};