#include "QLoginAndRegWnd.h"
#include "QSimpleSplit.h"
#include <QMouseEvent>

#include <QDebug>
#include "QWSClientMgr.h"
#include <QMessageBox>
#include <QBuffer>
#include <QHttpMultiPart>
#include "QMainWnd.h"
#include "QDataManager.h"

#include "./json/CJsonObject.hpp"

QLoginAndRegWnd::QLoginAndRegWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    setObjectName("QLoginAndRegWnd");

    setFixedSize(430, 330);
    setContentsMargins(0, 0, 0, 0);
    m_vLayout = new QVBoxLayout();
    m_vLayout->setContentsMargins(0, 0, 0, 0);
    m_vLayout->setSpacing(0);
    setLayout(m_vLayout);
    setWindowFlags(Qt::FramelessWindowHint);

    m_topWnd = new QWidget();
    m_topWnd->setFixedSize(430, 130);
    m_topWnd->setStyleSheet("background-color:#1aad19;border-style: none;");
    m_topWnd->setContentsMargins(0, 0, 0, 0);

    //�����Ĵ��ڵĲ���
    m_vTopLayout = new QVBoxLayout();
    m_vTopLayout->setContentsMargins(0, 0, 0, 0);
    m_hTopLayout = new QHBoxLayout();

    m_topWnd->setLayout(m_vTopLayout);

    m_titleLabel = new QLabel();
    m_minBtn = new QPushButton();
    m_closeBtn = new QPushButton();

    m_titleLabel->setText("Ī����Ӯ/����");
    m_minBtn->setIcon(QPixmap("./img/minWnd1.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);

    m_closeBtn->setIcon(QPixmap("./img/closeWnd1.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    m_hTopLayout->addWidget(m_titleLabel);
    m_hTopLayout->addWidget(m_minBtn);
    m_hTopLayout->addWidget(m_closeBtn);

    m_vTopLayout->addLayout(m_hTopLayout);

    {
        QSimpleSplit* sp = new QSimpleSplit();
        sp->setStyleSheet("background-color:gray;border:0px;");
        m_vTopLayout->addWidget(sp);
    }

    m_vTopLayout->addStretch();

    m_vLayout->addWidget(m_topWnd);

    {
        QSimpleSplit* sp = new QSimpleSplit();
        m_vLayout->addWidget(sp);
    }

    m_bottomWnd = new QWidget();
    m_bottomWnd->setFixedHeight(200);

    m_hBottomLayout1 = new QHBoxLayout();
    m_bottomWnd->setLayout(m_hBottomLayout1);

    m_vBottomLayout = new QVBoxLayout();
    m_accuntEdit = new QLineEdit();
    m_accuntEdit->setFixedSize(240, 40);
    m_pwdEdit = new QLineEdit();
    m_pwdEdit->setFixedSize(240, 40);

    m_accuntEdit->setPlaceholderText("�������˺�");
    m_pwdEdit->setPlaceholderText("����������");

    m_accuntEdit->setText("myfirstAccount");
    m_pwdEdit->setText("mypassword");

    m_regOrLoginBtn = new QPushButton();
    m_regOrLoginBtn->setFixedSize(240, 40);
    m_regOrLoginBtn->setStyleSheet("background-color:#1aad19;border-style: none;");
    m_regOrLoginBtn->setText("��¼");

    m_hBottomLayout2 = new QHBoxLayout();
    m_regOrLoginChx = new QCheckBox();
    m_remmerPwdChx = new QCheckBox();

    m_regOrLoginChx->setText("ע��");
    m_remmerPwdChx->setText("�һ�����");

    m_hBottomLayout2->addWidget(m_regOrLoginChx);
    m_hBottomLayout2->addWidget(m_remmerPwdChx);

    m_vBottomLayout->addWidget(m_accuntEdit);
    m_vBottomLayout->addWidget(m_pwdEdit);
    m_vBottomLayout->addLayout(m_hBottomLayout2);
    m_vBottomLayout->addWidget(m_regOrLoginBtn);

    m_hBottomLayout1->addStretch();
    m_hBottomLayout1->addLayout(m_vBottomLayout);
    m_hBottomLayout1->addStretch();

    m_vLayout->addWidget(m_bottomWnd);

    regSignalSlot();
}

void QLoginAndRegWnd::regSignalSlot()
{
    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(slotMinWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(slotCloseWnd()));
    connect(m_regOrLoginChx, SIGNAL(clicked(bool)), this, SLOT(slotRegOrLoginSel(bool)));
    connect(m_regOrLoginBtn, SIGNAL(clicked()), this, SLOT(slotRegOrLoginBtn()));
    connect(m_pwdEdit, SIGNAL(returnPressed()), this, SLOT(slotRegOrLoginBtn()));
}

void QLoginAndRegWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bPress)
    {
        move(event->pos() - m_poPress + pos());
    }
}

void QLoginAndRegWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void QLoginAndRegWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void QLoginAndRegWnd::slotCloseWnd()
{
    close();
}

void QLoginAndRegWnd::slotMinWnd()
{
    showMinimized();
}

void QLoginAndRegWnd::slotRegOrLoginSel(bool isSel /*= false*/)
{
    if (isSel)
    {
        m_bReg = true;
        m_regOrLoginBtn->setText("ע��");
    }
    else
    {
        m_bReg = false;
        m_regOrLoginBtn->setText("��¼");
    }
}

void QLoginAndRegWnd::slotRegOrLoginBtn()
{
    if (m_bReg)
    {
        //�ṩע�Ṧ�ܣ����ע�ᰴť��Զ�˷�������Ϣ
        neb::CJsonObject json;

        std::string username = m_accuntEdit->text().toStdString().c_str();  //�û���
        std::string password = m_pwdEdit->text().toStdString().c_str();     //����
        std::string nickname = m_accuntEdit->text().toStdString().c_str();  //��ɫ��

        //�Ա��ʶ
        int sex = 0;
        json.Add("username", username);
        json.Add("password", password);
        json.Add("nickname", nickname);
        json.Add("sex", sex);

        QWSClientMgr::getInstance()->request("cs_msg_register", json, [this](neb::CJsonObject& msg) {
            int state = 0;
            if (!msg.Get("state", state))
            {
                return;
            }

            int userId = -1;
            if (!msg["data"].Get("userId", userId))
            {
                return;
            }

            std::string infoStr = "ע��ʧ��";
            if (state == 0)
            {
                infoStr = "ע��ɹ�";
            }

            QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
            QNetworkRequest request;
            request.setUrl(QUrl("http://49.232.169.205:80/UploadDemo/UploadServlet"));
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

            //ע��ɹ��󣬵�������
            QMessageBox::information(nullptr, "info", infoStr.c_str());
        });
        return;
    }

    if (!m_bReg)
    {
        std::string username = m_accuntEdit->text().toStdString().c_str();
        std::string password = m_pwdEdit->text().toStdString().c_str();

        neb::CJsonObject json;
        json.Add("username", username);
        json.Add("password", password);

        QWSClientMgr::getInstance()->request("cs_msg_login", json, [this](neb::CJsonObject& msg) {
            //
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

            m_mainWnd = QMainWnd::getInstance();
            if (m_mainWnd == nullptr)
                return;

            m_mainWnd->setUserIdAndName(userid, username.c_str());
            m_mainWnd->request();
            m_mainWnd->show();

            QDataManager::getInstance()->setUserIdAndName(userid, username.c_str());
            this->hide();
        });
    }
}
