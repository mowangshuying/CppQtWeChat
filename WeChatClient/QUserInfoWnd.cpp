#include "QUserInfoWnd.h"
#include "QSelfSplit.h"
#include <QApplication>
#include <QPixmap>
#include "QDataManager.h"
#include "QMainWnd.h"
#include <QGraphicsDropShadowEffect>
#include "QStyleSheetMgr.h"

QUserInfoWnd::QUserInfoWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    m_centerWnd = new QWidget(this);
    m_centerWnd->setObjectName("QUserInfoWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    // 设置为无边框及背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedSize(300, 240);

    m_vLayout = new QVBoxLayout();
    m_centerWnd->setLayout(m_vLayout);

    m_vLayout->addSpacing(25);

    //
    m_hLayout1 = new QHBoxLayout();
    m_vLayout1 = new QVBoxLayout();
    m_vLayout2 = new QVBoxLayout();

    QFont ft;
    ft.setPointSize(15);

    m_usernameLabel = new QLabel();
    m_userIdLabel = new QLabel();

    m_usernameLabel->setFont(ft);
    ft.setPointSize(12);
    m_userIdLabel->setFont(ft);

    m_usernameLabel->setText("xxxxxx");
    m_userIdLabel->setText("用户id:xxxxxx");

    m_vLayout1->addWidget(m_usernameLabel);
    m_vLayout1->addWidget(m_userIdLabel);

    //
    m_hLayout1->addLayout(m_vLayout1);

    m_headLabel = new QLabel(this);
    m_headLabel->setFixedSize(40, 40);

    m_headLabel->setPixmap(QPixmap("./img/head2.png"));

    m_changeHeadImgBtn = new QPushButton();
    m_changeHeadImgBtn->setText("修改头像");
    // m_changeHeadImgBtn->setFixedWidth(50);
    // m_changeHeadImgBtn->setFixedHeight(20);

    m_vLayout2->addWidget(m_headLabel);
    m_vLayout2->addWidget(m_changeHeadImgBtn);

    m_hLayout1->addLayout(m_vLayout2);
    m_vLayout->addLayout(m_hLayout1);

    ///* {
    //     QSimpleSplit* sp = new QSimpleSplit();
    //     m_vLayout->addWidget(sp);
    // }*/

    m_vLayout->addStretch();
}

bool QUserInfoWnd::event(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if (QApplication::activeWindow() != this)
        {
            this->hide();
        }
    }

    return QWidget::event(event);
}
