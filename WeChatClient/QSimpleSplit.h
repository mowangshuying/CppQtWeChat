#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include<QWidget>

class QSimpleSplit : public QWidget {
	Q_OBJECT
public:
	enum QSimpleSplit_direction {
		QSimpleSplit_direction_h = 0,//ˮƽ����
		QSimpleSplit_direction_v,
	};
	QSimpleSplit(QWidget* p = nullptr,QSimpleSplit_direction d = QSimpleSplit_direction_v);
};