#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

class QApplyFriendWaitInfoWnd : public QWidget 
{
	Q_OBJECT
public:
	QApplyFriendWaitInfoWnd(QWidget* p = nullptr);

public:
	//������Ҫ����
	QVBoxLayout* m_vLayout;

	// �ı���ʾ
	QLabel* m_infoLabel;
};