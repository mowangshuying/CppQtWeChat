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

class QSwitchButton : public QWidget
{    
	Q_OBJECT
public:    
	QSwitchButton(QWidget *parent = nullptr);
    ~QSwitchButton();
protected:
	void paintEvent(QPaintEvent *e);    
	void resizeEvent(QResizeEvent *e);    
	void mousePressEvent(QMouseEvent *e);    
	void mouseMoveEvent(QMouseEvent *e);    
	void mouseReleaseEvent(QMouseEvent *e);
public:
	void drawBg(QPainter& painter);        //绘制背景    
	void drawSlidBlock(QPainter& painter); //绘制滑块    
	void drawText(QPainter& painter);      //绘制文字
signals:    
	void stateChange(bool state);

public:
	QColor bgColorOn;                 //开状态时的背景颜色    
	QColor bgColorOff;                //关状态时的背景颜色    
	QColor sliderColor;
	QColor sliderColorOn;             //开状态时滑块的背景颜色    
	QColor sliderColorOff;            //关状态时滑块的背景颜色    
	QColor textColorOn;               //开状态时文字颜色    
	QColor textColorOff;              //关状态时文字颜色      
	QPoint startPoint;                //滑块移动的起始点    
	QPoint endPoint;                  //滑块移动的终点    
	QPoint centerPoint;               //滑块移动的中间某点  

	QString strText;                  //文字内容  

	int mouseX;

	bool bPress;                      //左键是否按下
	bool bSwitch;                     //开关状态:开true，关false
};