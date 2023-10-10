#pragma once

/*
* 关于参考，原文地址，代码从此处演化：
https://blog.csdn.net/weixin_39572794/article/details/110516773
* 由于博客内源码比较乱，重新整理，为了适应本项目，自己对此代码做出相应的修改，
* 特此标注：特此感谢此博文作者的贡献
*/

#include "QPch.h"

#include <QWidget>
#include <QMouseEvent>

class QSelfSwitchButton : public QWidget
{
    Q_OBJECT
  public:
    QSelfSwitchButton(QWidget *parent = nullptr);
    ~QSelfSwitchButton();

  protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

  public:
    void drawBg(QPainter &painter);         //绘制背景
    void drawSlidBlock(QPainter &painter);  //绘制滑块
    void drawText(QPainter &painter);       //绘制文字
  signals:
    void stateChange(bool state);

  public:
    QColor m_bgColorOn;   //开状态时的背景颜色
    QColor m_bgColorOff;  //关状态时的背景颜色
    QColor m_sliderColor;
    QColor m_sliderColorOn;   //开状态时滑块的背景颜色
    QColor m_sliderColorOff;  //关状态时滑块的背景颜色
    QColor m_textColorOn;     //开状态时文字颜色
    QColor m_textColorOff;    //关状态时文字颜色

    QPoint m_startPoint;   //滑块移动的起始点
    QPoint m_endPoint;     //滑块移动的终点
    QPoint m_centerPoint;  //滑块移动的中间某点

    QString m_strText;  //文字内容

    int m_mouseX;

    bool m_bPress;   //左键是否按下
    bool m_bSwitch;  //开关状态:开true，关false
};