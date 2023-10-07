#include "QFindFriendItemWnd.h"
#include "QStyleSheetMgr.h"

QFindFriendItemWnd::QFindFriendItemWnd(QWidget* p) : QWidget(p)
{
    LogFunc;
    setObjectName("QFindFriendItemWnd");
}

QFindFriendItemWnd::QFindFriendItemWnd(QWidget* p,
                                       const char* headUrl,
                                       const char* name)
    : QWidget(p), m_username(name)
{
    LogFunc;
    setObjectName("QFindFriendItemWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + objectName() + ".qss";
    object.m_widget = this;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    m_hLayout = new QHBoxLayout(this);

    m_hLayout->setSpacing(0);
    setLayout(m_hLayout);
    m_headUrl = new QLabel();

    m_headUrl->setPixmap(QPixmap(headUrl));
    m_headUrl->setFixedSize(40, 40);
    m_name = new QLabel();

    m_name->setText(name);

    m_hLayout->addSpacing(50);
    m_hLayout->addWidget(m_headUrl);
    m_hLayout->addWidget(m_name);

    m_hLayout->addStretch();
    m_addFriendBtn = new QPushButton();
    m_addFriendBtn->setText("Ìí¼ÓºÃÓÑ");
    // m_addFriendBtn->setFixedSize(60, 30);
    m_hLayout->addWidget(m_addFriendBtn);
    m_hLayout->addSpacing(80);

    // m_addFriendBtn->setStyleSheet("background-color:#1aad19;border-style:
    // none;");

    setFixedHeight(60);
    //  setObjectName("QCommContactItemWnd");
    //  setStyleSheet("#QCommContactItemWnd{border-bottom:1px solid red;}");

    connect(m_addFriendBtn,
            SIGNAL(clicked()),
            this,
            SLOT(slotAddFriendBtnClick()));
}

void QFindFriendItemWnd::slotAddFriendBtnClick()
{
    m_nextWnd = new QApplyFriendNextWnd(nullptr, m_friendid, m_username);
    m_nextWnd->show();
}
