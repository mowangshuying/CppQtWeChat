#pragma once

#include "QPch.h"

#include <Qwidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class QCommContactInfo : public QWidget
{
    Q_OBJECT
public:
    QCommContactInfo(QWidget* p = nullptr);
signals:
    void signal_sendMsgBtnClick(QMap<QString, QString> map);
public slots:
    void slot_contactInfoChange(QMap<QString, QString> map);
    void slot_sendMsgBtnClick();

public:
    void showBgPng();
    void hideBgPng();

public:
    QVBoxLayout* m_vLayout;
    QVBoxLayout* m_vLayout1;

    //ͷ���Ĺرմ򿪵Ȱ�ť
    QHBoxLayout* m_hLayout0;
    QHBoxLayout* m_hLayout1;
    QHBoxLayout* m_hLayout2;
    QHBoxLayout* m_hLayout3;
    QHBoxLayout* m_hLayout4;
    QHBoxLayout* m_hLayout5;

    QPushButton* m_minBtn;
    QPushButton* m_maxBtn;
    QPushButton* m_closeBtn;

    QLabel* m_nickNameLabel;
    QLabel* m_headimgLabel;

    QLabel* m_markNameLabel;
    QLabel* m_rolenameLabel;  //��ɫ��
    QLabel* m_channelLabel;

    QLabel* m_markNameFillLabel;
    QLabel* m_rolenameFillLabel;
    QLabel* m_channelFillLabel;

    //������Ϣ����
    QPushButton* m_sendMsgBtn;
};