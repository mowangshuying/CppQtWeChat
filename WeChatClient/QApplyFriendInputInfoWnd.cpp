#include "QApplyFriendInputInfoWnd.h"
#include "QSelfLabel.h"
#include "QStyleSheetMgr.h"

QApplyFriendInputInfoWnd::QApplyFriendInputInfoWnd(QWidget* p /*= nullptr*/)
    : QWidget(p)
{
    LogFunc;
    setObjectName("QApplyFriendInputInfoWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    setContentsMargins(0, 0, 0, 0);
    setFixedSize(460, 280);

    // ������Ҫ����Ϊˮƽ����
    m_hLayout = new QHBoxLayout();
    setLayout(m_hLayout);

    m_userInfoWnd = new QWidget();
    m_vUserInfoLayout = new QVBoxLayout();

    m_headUrl = new QLabel();
    m_userName = new QLabel();
    m_moreInfo = new QLabel();

    // ����ͷ��
    m_headUrl->setFixedSize(80, 80);
    QPixmap headImg = QPixmap("./img/head2.png");
    headImg =
        headImg.scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_headUrl->setPixmap(headImg);

    // �û���
    m_userName->setText("��ҵı��");
    m_moreInfo->setText("�������Ϣ...");

    m_vUserInfoLayout->addWidget(m_headUrl);
    m_vUserInfoLayout->addSpacing(20);
    m_vUserInfoLayout->addWidget(m_userName);
    m_vUserInfoLayout->addSpacing(20);
    m_vUserInfoLayout->addWidget(m_moreInfo);
    m_vUserInfoLayout->addStretch();

    m_userInfoWnd->setLayout(m_vUserInfoLayout);
    m_userInfoWnd->setFixedWidth(130);
    // m_userInfoWnd->setWindowFlags(Qt::FramelessWindowHint);
    // m_userInfoWnd->setStyleSheet("background-color:#aaa;border:0px;");
    m_hLayout->addWidget(m_userInfoWnd);

    // ���һ���ָ���
    m_split = new QSelfSplit(nullptr, QSelfSplit::Direction_V);
    m_hLayout->addWidget(m_split);

    m_vLayout = new QVBoxLayout();
    m_inputMsgLabel = new QLabel();
    m_inputMsgEdit = new QTextEdit();

    m_inputMsgLabel->setText("��������֤��Ϣ:");
    m_inputMsgEdit->setFixedWidth(280);
    m_inputMsgEdit->setFixedHeight(150);

    m_vLayout->addWidget(m_inputMsgLabel);
    m_vLayout->addWidget(m_inputMsgEdit);
    m_vLayout->addStretch();

    m_hLayout->addLayout(m_vLayout);
}
