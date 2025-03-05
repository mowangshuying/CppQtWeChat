#include "ToolWnd.h"
#include "MainWnd.h"
#include "DataManager.h"

ToolWnd::ToolWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    setObjectName("QToolWnd");
    setFixedWidth(55);
    setWindowFlags(Qt::FramelessWindowHint);
    // setStyleSheet("background-color:#2E2E2E;");
    setMouseTracking(true);

    if (objectName().isEmpty())
        setObjectName("QToolWnd");
    setStyleSheet(
        "QWidget#QToolWnd{border-top-left-radius:4px;border-bottom-left-radius:"
        "4px; background-color:#2E2E2E;border:none;}");

    // 脱离父窗口的样式控制
    setAttribute(Qt::WA_StyledBackground);

    m_vBoxLayout = new QVBoxLayout(this);
    m_headUrlLabel = new SelfLabel("头像");
    m_headUrlLabel->setFixedSize(40, 40);

    m_msgBtn = new QPushButton(this);
    m_msgBtn->setFixedSize(30, 30);
    m_msgBtn->setIconSize(QSize(30, 30));
    m_msgBtn->setToolTip("聊天");
    m_msgBtn->setIcon(QPixmap("./img/msgBtnNormal.png"));

    m_contactsBtn = new QPushButton(this);
    m_contactsBtn->setFixedSize(30, 30);
    m_contactsBtn->setIconSize(QSize(30, 30));
    m_contactsBtn->setToolTip("联系人");
    m_contactsBtn->setIcon(QPixmap("./img/contactsBtnNomal.png"));

    m_groupsBtn = new QPushButton(this);
    m_groupsBtn->setFixedSize(30, 30);
    m_groupsBtn->setIconSize(QSize(30, 30));
    m_groupsBtn->setToolTip("群组");
    m_groupsBtn->setIcon(QPixmap("./img/groupBtnNomal.png"));

    m_moreBtn = new QPushButton(this);
    m_moreBtn->setFixedSize(30, 30);
    m_moreBtn->setIconSize(QSize(30, 30));
    m_moreBtn->setToolTip("更多");
    m_moreBtn->setIcon(QPixmap("./img/tmore.png"));

    m_vBoxLayout->setAlignment(Qt::AlignCenter);
    m_vBoxLayout->setContentsMargins(0, 20, 0, 0);
    m_vBoxLayout->addWidget(m_headUrlLabel);
    m_vBoxLayout->addSpacing(5);
    m_vBoxLayout->addWidget(m_msgBtn);
    m_vBoxLayout->addSpacing(5);
    m_vBoxLayout->addWidget(m_contactsBtn);
    m_vBoxLayout->addSpacing(5);
    m_vBoxLayout->addWidget(m_groupsBtn);
    m_vBoxLayout->addStretch();
    m_vBoxLayout->addWidget(m_moreBtn);
    m_vBoxLayout->addSpacing(20);

    setLayout(m_vBoxLayout);

    m_btnVct.push_back(m_msgBtn);
    m_btnVct.push_back(m_contactsBtn);
    m_selectIndex = 0;

    m_userInfoWnd = new UserInfoWnd();
    m_userInfoWnd->hide();

    m_pictureToolWnd = new PictureToolWnd();
    m_pictureToolWnd->hide();

    m_selectMoreWnd = new SelectMoreWnd();
    m_selectMoreWnd->hide();

    connect(m_msgBtn, SIGNAL(clicked()), this, SLOT(slotOnClickMsgBtn()));
    connect(m_contactsBtn, SIGNAL(clicked()), this, SLOT(slotOnClickContactsBtn()));
    connect(m_groupsBtn, SIGNAL(clicked()), this, SLOT(slotOnClickGroupsBtn()));
    connect(m_headUrlLabel, SIGNAL(clicked()), this, SLOT(slotOnClickHeadUrlLabel()));
    connect(m_userInfoWnd->m_changeHeadImgBtn, SIGNAL(clicked()), this, SLOT(slotOnClickChangeHeadImgBtn()));
    connect(m_moreBtn, SIGNAL(clicked()), this, SLOT(slotOnClickMoreBtn()));
}

void ToolWnd::slotOnClickMsgBtn()
{
    m_selectIndex = 0;
    m_msgBtn->setIcon(QPixmap("./img/msgBtnClicked.png"));
    m_contactsBtn->setIcon(QPixmap("./img/contactsBtnNomal.png"));
    m_groupsBtn->setIcon(QPixmap("./img/groupBtnNomal.png"));
    // 发出信号
    signalToolWndPageChanged(0);
}

void ToolWnd::slotOnClickContactsBtn()
{
    m_selectIndex = 0;
    m_msgBtn->setIcon(QPixmap("./img/msgBtnNormal.png"));
    m_contactsBtn->setIcon(QPixmap("./img/contactsBtnClicked.png"));
    m_groupsBtn->setIcon(QPixmap("./img/groupBtnNomal.png"));
    signalToolWndPageChanged(1);
}

void ToolWnd::slotOnClickHeadUrlLabel()
{
    // 点击就先隐藏
    m_userInfoWnd->hide();

    // 更新数据并显示；
    QRect rect = m_headUrlLabel->geometry();
    QPoint gPoint = m_headUrlLabel->mapToGlobal(QPoint(0, 0));
    QRect swRect = m_userInfoWnd->geometry();
    swRect.setX(gPoint.x() + m_headUrlLabel->width() / 2);
    swRect.setY(gPoint.y() + m_headUrlLabel->height() / 2);

    QPixmap pixmap = DataManager::getMgr()->m_UserId2HeadImgMap[DataManager::getMgr()->m_userid];
    pixmap = pixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_userInfoWnd->m_headLabel->setPixmap(pixmap);
    m_userInfoWnd->m_usernameLabel->setText(DataManager::getMgr()->m_username);
    m_userInfoWnd->m_userIdLabel->setText("用户id:" + QString::number(DataManager::getMgr()->m_userid));
    m_userInfoWnd->setGeometry(swRect);
    m_userInfoWnd->show();
}

void ToolWnd::slotOnClickChangeHeadImgBtn()
{
    m_pictureToolWnd->show();
}

void ToolWnd::slotOnClickGroupsBtn()
{
    // m_selectIndex = 0;
    m_msgBtn->setIcon(QPixmap("./img/msgBtnNormal.png"));
    m_contactsBtn->setIcon(QPixmap("./img/contactsBtnNomal.png"));
    m_groupsBtn->setIcon(QPixmap("./img/groupBtnClicked.png"));
    signalToolWndPageChanged(2);  // 暂时先注释，待会放出来
}

void ToolWnd::slotOnClickMoreBtn()
{
    if (m_selectMoreWnd->isHidden())
    {
        QPoint gPoint = m_moreBtn->mapToGlobal(QPoint(m_moreBtn->width(), m_moreBtn->height()));
        QRect swRect = m_selectMoreWnd->geometry();
        swRect.setX(gPoint.x());
        swRect.setY(gPoint.y() - swRect.height());
        m_selectMoreWnd->setGeometry(swRect);
        m_selectMoreWnd->show();
    }
}
