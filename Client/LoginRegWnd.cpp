#include "LoginRegWnd.h"
#include "SelfSplit.h"
#include <QMouseEvent>

#include <QDebug>
#include "NetClientUtils.h"
#include <QMessageBox>
#include <QBuffer>
#include <QHttpMultiPart>
#include "MainWnd.h"
#include "DataManager.h"

#include "./json/CJsonObject.hpp"
#include "StyleSheetMgr.h"
#include <QStyleOption>
#include <QPainter>

LoginRegWnd::LoginRegWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    LogFunc;
    m_centerWnd = new QWidget(this);
    m_centerWnd->setObjectName("centerWnd");
    // 设置固定大小
    setFixedSize(430, 330);
    m_centerWnd->setFixedSize(430, 330);
    setContentsMargins(10, 10, 10, 10);

    m_vLayout = new QVBoxLayout(m_centerWnd);
    m_vLayout->setSpacing(0);
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_centerWnd->setLayout(m_vLayout);

    m_topWnd = new QWidget(m_centerWnd);
    m_topWnd->setContentsMargins(0, 0, 0, 0);

    // 顶部的窗口的布局
    m_vTopLayout = new QVBoxLayout(m_centerWnd);
    m_vTopLayout->setContentsMargins(0, 0, 0, 0);
    m_hTopLayout = new QHBoxLayout(m_centerWnd);

    m_topWnd->setLayout(m_vTopLayout);

    m_titleLabel = new QLabel(m_centerWnd);
    m_minBtn = new QPushButton(m_centerWnd);
    m_closeBtn = new QPushButton(m_centerWnd);

    m_minBtn->setIconSize(QSize(25, 25));
    m_minBtn->setIcon(QPixmap("./img/ChromeMinimize.png"));
    m_minBtn->setFixedSize(25, 25);

    m_closeBtn->setIconSize(QSize(25, 25));
    m_closeBtn->setIcon(QPixmap("./img/ChromeClose.png"));
    m_closeBtn->setFixedSize(25, 25);

    m_closeBtn->setObjectName("closeBtn");
    m_minBtn->setObjectName("minBtn");

    m_hTopLayout->addWidget(m_titleLabel);
    //m_hTopLayout->addWidget(m_settingBtn);
    m_hTopLayout->addWidget(m_minBtn);
    m_hTopLayout->addWidget(m_closeBtn);

    m_vTopLayout->addLayout(m_hTopLayout);

    m_vTopLayout->addStretch();
    m_vLayout->addWidget(m_topWnd);

    m_bottomWnd = new QWidget(m_centerWnd);
    m_bottomWnd->setFixedHeight(200);

    m_hBottomLayout1 = new QHBoxLayout(m_centerWnd);
    m_bottomWnd->setLayout(m_hBottomLayout1);

    m_vBottomLayout = new QVBoxLayout(m_centerWnd);
    m_accuntEdit = new QLineEdit(m_centerWnd);
    m_accuntEdit->setFixedSize(240, 40);
    m_pwdEdit = new QLineEdit(m_centerWnd);
    m_pwdEdit->setFixedSize(240, 40);

    m_accuntEdit->setPlaceholderText("请输入账号");
    m_pwdEdit->setPlaceholderText("请输入密码");

    m_accuntEdit->setText("");
    m_pwdEdit->setText("");

    m_regOrLoginBtn = new QPushButton();
    m_regOrLoginBtn->setFixedSize(240, 40);

    m_regOrLoginBtn->setText("登录");

    m_hBottomLayout2 = new QHBoxLayout(m_centerWnd);
    m_regLoginCB = new QCheckBox(m_centerWnd);
    m_remmerPwdCB = new QCheckBox(m_centerWnd);

    m_regLoginCB->setText("注册");
    m_remmerPwdCB->setText("记住密码");

    m_hBottomLayout2->addWidget(m_regLoginCB);
    m_hBottomLayout2->addWidget(m_remmerPwdCB);

    m_vBottomLayout->addWidget(m_accuntEdit);
    m_vBottomLayout->addWidget(m_pwdEdit);
    m_vBottomLayout->addLayout(m_hBottomLayout2);
    m_vBottomLayout->addWidget(m_regOrLoginBtn);

    m_hBottomLayout1->addStretch();
    m_hBottomLayout1->addLayout(m_vBottomLayout);
    m_hBottomLayout1->addStretch();

    m_vLayout->addWidget(m_bottomWnd);

    regSignalSlot();
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 注册样式表
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/LoginRegWnd.qss";
    object.m_widget = m_centerWnd;
    StyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);
}

LoginRegWnd::~LoginRegWnd()
{
    if (m_mainWnd != nullptr)
        delete m_mainWnd;
}

void LoginRegWnd::regSignalSlot()
{
    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(onMinWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(onCloseWnd()));
    connect(m_regLoginCB, SIGNAL(clicked(bool)), this, SLOT(onRegLoginSel(bool)));
    connect(m_regOrLoginBtn, SIGNAL(clicked()), this, SLOT(onRegLoginBtnClicked()));
    connect(m_pwdEdit, SIGNAL(returnPressed()), this, SLOT(onRegLoginBtnClicked()));
}

void LoginRegWnd::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void LoginRegWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void LoginRegWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void LoginRegWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void LoginRegWnd::onCloseWnd()
{
    close();
}

void LoginRegWnd::onMinWnd()
{
    showMinimized();
}

void LoginRegWnd::onRegLoginSel(bool isSel /*= false*/)
{
    if (isSel)
    {
        m_bReg = true;
        m_regOrLoginBtn->setText("注册");
    }
    else
    {
        m_bReg = false;
        m_regOrLoginBtn->setText("登录");
    }
}

void LoginRegWnd::onRegLoginBtnClicked()
{
    
    if (m_bReg)
    {
        // 用户注册-----------------------------------------------------------------
        neb::CJsonObject json;

        std::string username = m_accuntEdit->text().toStdString().c_str();
        std::string password = m_pwdEdit->text().toStdString().c_str();
        std::string nickname = m_accuntEdit->text().toStdString().c_str();

        // 性别标识
        int sex = 0;
        json.Add("username", username);
        json.Add("password", password);
        json.Add("nickname", nickname);
        json.Add("sex", sex);

        NetClientUtils::request("regUser", json, [this](neb::CJsonObject& msg) {
            int64 userId = 0;
            if (!msg["data"].Get("userId", userId))
            {
                return;
            }

            if (userId > 0)
            {
                QMessageBox::information(nullptr, "info", "reg Suc");
            }
            else
            {
                QMessageBox::information(nullptr, "info", "reg Err");
            }
        });
        return;
    }

    // 不是注册就是登录
    if (!m_bReg)
    {
        std::string username = m_accuntEdit->text().toStdString().c_str();
        std::string password = m_pwdEdit->text().toStdString().c_str();
        neb::CJsonObject json;
        json.Add("username", username);
        json.Add("password", password);

        NetClientUtils::request("login", json, [this](neb::CJsonObject& msg) {
            int64 userid = 0;
            if (!msg["data"].Get("userId", userid))
                return;

            std::string username;
            if (!msg["data"].Get("username", username))
                return;

            // 登录成功打开主窗口并所有用户数据；
            m_mainWnd = MainWnd::getMainWnd();
            if (m_mainWnd == nullptr)
                return;

            m_mainWnd->setUserIdAndName(userid, username.c_str());
            m_mainWnd->request();
            m_mainWnd->show();

            DataManager::getMgr()->setUserIdAndName(userid, username.c_str());
            this->hide();
        });
    }
}
