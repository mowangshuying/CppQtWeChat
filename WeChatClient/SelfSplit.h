#pragma once

#include "def.h"

#include <QWidget>

class SelfSplit : public QWidget
{
    Q_OBJECT
  public:
    enum QSimpleSplitDirection
    {
        Direction_V = 0,  // 水平方向
        Direction_H,
    };

    SelfSplit(QWidget* p = nullptr, QSimpleSplitDirection direct = Direction_H);
};