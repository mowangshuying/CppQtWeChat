#pragma once


#include "QPch.h"

#include<QWidget>

class QSimpleSplit : public QWidget 
{
	Q_OBJECT
public:
	enum QSimpleSplitDirection 
	{
		Direction_V = 0,//ˮƽ����
		Direction_H,
	};

	QSimpleSplit(QWidget* p = nullptr,QSimpleSplitDirection direct = Direction_H);
};