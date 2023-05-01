#pragma once

#include "QPch.h"

#include <Qwidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class QCommContactInfoWnd : public QWidget
{
    Q_OBJECT
public:
    QCommContactInfoWnd(QWidget* p = nullptr);
signals:
    void signalSendMsgBtnClick(QMap<QString, QString> map);
public slots:
    void slotContactInfoChange(QMap<QString, QString> map);
    void slotSendMsgBtnClick();

public:
    void showBgPng();
    void hideBgPng();

public:
    QVBoxLayout* m_vLayout;

    QWidget* m_contentWnd;
    QVBoxLayout* m_vLayout1;

    QHBoxLayout* m_hLayout0;  // ��������󻯣���С����ť��
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
    QLabel* m_rolenameLabel;  // ��ɫ��
    QLabel* m_channelLabel;

    QLabel* m_markNameFillLabel;
    QLabel* m_rolenameFillLabel;
    QLabel* m_channelFillLabel;

    //������Ϣ����
    QPushButton* m_sendMsgBtn;
};
