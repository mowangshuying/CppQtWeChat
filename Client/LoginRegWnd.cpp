#include "LoginRegWnd.h"
#include "SelfSplit.h"
#include <QMouseEvent>

#include <QDebug>
#include "WSClientMgr.h"
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
    m_settingBtn = new QPushButton(m_centerWnd);

    m_settingBtn->setIconSize(QSize(20, 20));
    m_settingBtn->setIcon(QPixmap("./img/settingBtn_.png").scaled(20, 20));
    m_settingBtn->setFixedSize(20, 20);

    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png").scaled(20, 20));
    m_minBtn->setFixedSize(20, 20);

    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png").scaled(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    m_settingBtn->setObjectName("settingBtn");
    m_closeBtn->setObjectName("closeBtn");
    m_minBtn->setObjectName("minBtn");

    m_hTopLayout->addWidget(m_titleLabel);
    m_hTopLayout->addWidget(m_settingBtn);
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

    m_accuntEdit->setText("myfirstAccount");
    m_pwdEdit->setText("mypassword");

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
    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(slotMinWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(slotCloseWnd()));
    connect(m_regLoginCB, SIGNAL(clicked(bool)), this, SLOT(slotRegLoginSel(bool)));
    connect(m_regOrLoginBtn, SIGNAL(clicked()), this, SLOT(slotRegLoginBtnClicked()));
    connect(m_pwdEdit, SIGNAL(returnPressed()), this, SLOT(slotRegLoginBtnClicked()));
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

void LoginRegWnd::slotCloseWnd()
{
    close();
}

void LoginRegWnd::slotMinWnd()
{
    showMinimized();
}

void LoginRegWnd::slotRegLoginSel(bool isSel /*= false*/)
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

void LoginRegWnd::slotRegLoginBtnClicked()
{
    // 提供注册功能，点击注册按钮向远端服务发送消息
    if (m_bReg)
    {
        neb::CJsonObject json;

        std::string username = m_accuntEdit->text().toStdString().c_str();  // 用户名
        std::string password = m_pwdEdit->text().toStdString().c_str();     // 密码
        std::string nickname = m_accuntEdit->text().toStdString().c_str();  // 角色名

        // 性别标识
        int sex = 0;
        json.Add("username", username);
        json.Add("password", password);
        json.Add("nickname", nickname);
        json.Add("sex", sex);

        WSClientMgr::getMgr()->request("cs_msg_register", json, [this](neb::CJsonObject& msg) {
            int state = 0;

            if (!msg.Get("state", state))
                return;

            std::string infoStr = "注册失败:" + msg["data"].ToString();
            if (state != 0)
            {
                QMessageBox::information(nullptr, "info", infoStr.c_str());
                return;
            }

            int userId = -1;
            if (!msg["data"].Get("userId", userId))
            {
                return;
            }

            // 上传默认的头像.
            QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
            QNetworkRequest request;
            request.setUrl(QUrl(HTTP_FILE_SERVER_ADDR));
            QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"headimg\";filename=\"%1.png\"").arg(userId));
            part.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
            QFile* file = new QFile("./img/default.png");
            file->open(QFile::ReadOnly);
            part.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(part);
            QNetworkReply* reply = pManager->post(request, multiPart);

            // 注册成功后，弹出窗口
            infoStr = "注册成功";
            QMessageBox::information(nullptr, "info", infoStr.c_str());
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

        WSClientMgr::getMgr()->request("cs_msg_login", json, [this](neb::CJsonObject& msg) {
            int state = 0;
            if (!msg.Get("state", state))
                return;

            int64 userid = 0;
            if (!msg["data"].Get("userId", userid))
                return;

            std::string token;
            if (!msg["data"].Get("token", token))
                return;

            std::string username;
            if (!msg["data"].Get("username", username))
                return;

            LogDebug << msg.ToString().c_str();
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
