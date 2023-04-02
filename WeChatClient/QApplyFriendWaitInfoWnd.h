#pragma once

//设置编码格式为utf-8
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
	//窗口主要布局
	QVBoxLayout* m_vLayout;

	// 文本显示
	QLabel* m_infoLabel;
};