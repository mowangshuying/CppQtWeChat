#include "SelfSplit.h"

SelfSplit::SelfSplit(QWidget* p /*= nullptr*/, QSimpleSplitDirection direct /* Direction_H */)
{
    setObjectName("QSimpleSplit");
    setAttribute(Qt::WA_StyledBackground);
    // 设置一些样式, 无焦点, 无边框
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

    // 设置分割线的颜色
    //  灰色的分割线
    //  setStyleSheet("background-color:#E7E7E;border:none");

    // 红色的分割线
    setStyleSheet("background-color:#828790;border:0px");
}
