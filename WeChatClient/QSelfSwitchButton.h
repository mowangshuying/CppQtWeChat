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
    void drawBg(QPainter &painter);         //���Ʊ���
    void drawSlidBlock(QPainter &painter);  //���ƻ���
    void drawText(QPainter &painter);       //��������
  signals:
    void stateChange(bool state);

  public:
    QColor m_bgColorOn;   //��״̬ʱ�ı�����ɫ
    QColor m_bgColorOff;  //��״̬ʱ�ı�����ɫ
    QColor m_sliderColor;
    QColor m_sliderColorOn;   //��״̬ʱ����ı�����ɫ
    QColor m_sliderColorOff;  //��״̬ʱ����ı�����ɫ
    QColor m_textColorOn;     //��״̬ʱ������ɫ
    QColor m_textColorOff;    //��״̬ʱ������ɫ

    QPoint m_startPoint;   //�����ƶ�����ʼ��
    QPoint m_endPoint;     //�����ƶ����յ�
    QPoint m_centerPoint;  //�����ƶ����м�ĳ��

    QString m_strText;  //��������

    int m_mouseX;

    bool m_bPress;   //����Ƿ���
    bool m_bSwitch;  //����״̬:��true����false
};