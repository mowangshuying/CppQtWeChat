#include "QSimpleSplit.h"

QSimpleSplit::QSimpleSplit(QWidget* p /*= nullptr*/, QSimpleSplit_direction d/* = QSimpleSplit_direction_v*/)
{
	setAttribute(Qt::WA_StyledBackground);
	//setAttribute(Qt::FramelessWindowHint);
	//����һЩ��ʽ
	//�޽���
	setFocusPolicy(Qt::NoFocus);
	setWindowFlags(Qt::FramelessWindowHint);
	if (d == QSimpleSplit_direction_v) {
		//
		setFixedHeight(1);
	}
	else {
		setFixedWidth(1);
	}
	//���÷ָ��ߵ���ɫ
	// ��ɫ�ķָ���
	//setStyleSheet("background-color:#E7E7E;border:none");
	
	// ��ɫ�ķָ���
	setStyleSheet("background-color:#828790;border:0px");
}

