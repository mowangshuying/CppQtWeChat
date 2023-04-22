#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

#include "QSimpleSplit.h"

// ��Ӻ���ʱ������չʾ�Ĵ��ڣ���дһЩ��֤��Ϣ
class QApplyFriendInputInfoWnd : public QWidget
{
    Q_OBJECT
public:
    QApplyFriendInputInfoWnd(QWidget* p = nullptr);

public:
    QWidget* m_centerWnd;
    //��Ҫ����
    QHBoxLayout* m_hLayout;
    QVBoxLayout* m_vLayout;

    //��ϵ��ͷ��
    QLabel* m_headUrl;

    //��ɫ��
    QLabel* m_userName;
    //������Ϣ
    QLabel* m_moreInfo;
    //�û���Ϣ����
    QWidget* m_userInfoWnd;
    QVBoxLayout* m_vUserInfoLayout;

    // �󲿴������Ҳ����ڵķָ���
    QSimpleSplit* m_split;

    // ������֤��Ϣ�ı�ǩ
    QLabel* m_inputMsgLabel;
    // ������֤��Ϣ���ı���
    QTextEdit* m_inputMsgEdit;
};