#include "QSelfSplit.h"

QSelfSplit::QSelfSplit(QWidget* p /*= nullptr*/, QSimpleSplitDirection direct /* Direction_H */)
{
    setObjectName("QSimpleSplit");
    setAttribute(Qt::WA_StyledBackground);
    //����һЩ��ʽ, �޽���, �ޱ߿�
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint);

    if (direct == Direction_H)
    {
        setFixedHeight(1);
    }
    else
    {
        setFixedWidth(1);
    }

    //���÷ָ��ߵ���ɫ
    // ��ɫ�ķָ���
    // setStyleSheet("background-color:#E7E7E;border:none");

    // ��ɫ�ķָ���
    setStyleSheet("background-color:#828790;border:0px");
}
