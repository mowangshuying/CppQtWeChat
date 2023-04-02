#pragma once

/*
* ���ڲο���ԭ�ĵ�ַ������Ӵ˴��ݻ���
https://blog.csdn.net/weixin_39572794/article/details/110516773
* ���ڲ�����Դ��Ƚ��ң���������Ϊ����Ӧ����Ŀ���Լ��Դ˴���������Ӧ���޸ģ�
* �ش˱�ע���ش˸�л�˲������ߵĹ���
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
	void drawBg(QPainter& painter);        //���Ʊ���    
	void drawSlidBlock(QPainter& painter); //���ƻ���    
	void drawText(QPainter& painter);      //��������
signals:    
	void stateChange(bool state);

public:
	QColor bgColorOn;                 //��״̬ʱ�ı�����ɫ    
	QColor bgColorOff;                //��״̬ʱ�ı�����ɫ    
	QColor sliderColor;
	QColor sliderColorOn;             //��״̬ʱ����ı�����ɫ    
	QColor sliderColorOff;            //��״̬ʱ����ı�����ɫ    
	QColor textColorOn;               //��״̬ʱ������ɫ    
	QColor textColorOff;              //��״̬ʱ������ɫ      
	QPoint startPoint;                //�����ƶ�����ʼ��    
	QPoint endPoint;                  //�����ƶ����յ�    
	QPoint centerPoint;               //�����ƶ����м�ĳ��  

	QString strText;                  //��������  

	int mouseX;

	bool bPress;                      //����Ƿ���
	bool bSwitch;                     //����״̬:��true����false
};