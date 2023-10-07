#pragma once

#include "QPch.h"

#include <QWidget>

class QSelfSplit : public QWidget
{
    Q_OBJECT
  public:
    enum QSimpleSplitDirection
    {
        Direction_V = 0,  //水平方向
        Direction_H,
    };

    QSelfSplit(QWidget* p = nullptr,
               QSimpleSplitDirection direct = Direction_H);
};