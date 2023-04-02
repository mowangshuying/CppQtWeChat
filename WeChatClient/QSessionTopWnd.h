#pragma once


#include "QPch.h"

#include<QWidget>
#include<QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "json/CJsonObject.hpp"

class QSessionTopWnd : public QWidget 
{
	//添加消息映射支持
	Q_OBJECT
public:
	//会话窗口
	QSessionTopWnd(QWidget* p = nullptr);

public:
	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout1;
	QHBoxLayout* m_hLayout2;

	QPushButton* m_minBtn;
	QPushButton* m_maxBtn;
	QPushButton* m_closeBtn;

	QLabel* m_titleLabel;
	QPushButton* m_moreBtn;
};