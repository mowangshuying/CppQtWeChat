#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include<QWidget>
#include<QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "json/CJsonObject.hpp"

class QSessionTopWnd : public QWidget 
{
	//�����Ϣӳ��֧��
	Q_OBJECT
public:
	//�Ự����
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