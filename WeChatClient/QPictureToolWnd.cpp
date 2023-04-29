#include "QPictureToolWnd.h"
#include <QFileDialog>
#include "./json/CJsonObject.hpp"
#include "QMainWnd.h"
#include <QBuffer>
#include <QByteArray>
#include "QWSClientMgr.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QNetworkReply>
#include "QDataManager.h"
#include "QSimpleSplit.h"
#include "QStyleSheetMgr.h"

QPictureToolWnd::QPictureToolWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
    m_centerWnd = new QWidget(this);
    m_centerWnd->setFixedSize(460, 360);
    m_centerWnd->setObjectName("QPictureToolWnd");
    QStyleSheetObject object;
    object.m_qssFileName = "./stylesheet/" + m_centerWnd->objectName() + ".qss";
    object.m_widget = m_centerWnd;
    QStyleSheetMgr::getMgr()->reg(object.m_qssFileName, object);

    // setObjectName("QPictureToolWnd");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_vLayout = new QVBoxLayout();
    m_centerWnd->setLayout(m_vLayout);
    //  m_vLayout->setAlignment(Qt::AlignHCenter);

    m_picLable = new QLabel();
    m_picLable->setFixedSize(249, 249);
    m_picLable->setAlignment(Qt::AlignHCenter);

    m_hLayout1 = new QHBoxLayout();
    m_minBtn = new QPushButton(this);
    m_closeBtn = new QPushButton(this);

    // 最小化按钮
    m_minBtn->setIcon(QPixmap("./img/minBtn_.png"));
    m_minBtn->setIconSize(QSize(20, 20));
    m_minBtn->setFixedSize(20, 20);

    // 关闭按钮
    m_closeBtn->setIcon(QPixmap("./img/closeBtn_.png"));
    m_closeBtn->setIconSize(QSize(20, 20));
    m_closeBtn->setFixedSize(20, 20);

    // 添加弹簧
    m_hLayout1->addStretch();
    m_hLayout1->addWidget(m_minBtn);
    m_hLayout1->addWidget(m_closeBtn);

    m_determineBtn = new QPushButton();
    m_cancelBtn = new QPushButton();
    m_uploadBtn = new QPushButton();

    m_determineBtn->setText("确定");
    m_cancelBtn->setText("取消");
    m_uploadBtn->setText("上传");

    m_hLayout2 = new QHBoxLayout();
    m_hLayout2->addWidget(m_uploadBtn);
    m_hLayout2->addWidget(m_determineBtn);
    m_hLayout2->addWidget(m_cancelBtn);

    m_vLayout->addLayout(m_hLayout1);
    {
        QSimpleSplit* sp = new QSimpleSplit();
        m_vLayout->addWidget(sp);
    }
    m_vLayout->addWidget(m_picLable, 0, Qt::AlignCenter);
    {
        QSimpleSplit* sp = new QSimpleSplit();
        m_vLayout->addWidget(sp);
    }
    m_vLayout->addLayout(m_hLayout2);

    connect(m_uploadBtn, SIGNAL(clicked()), this, SLOT(slotUploadBtnClicked()));
    connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelBtnClicked()));
    connect(m_determineBtn, SIGNAL(clicked()), this, SLOT(slotDetermineBtnClicked()));

    connect(m_minBtn, SIGNAL(clicked()), this, SLOT(SlotMinWnd()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(slotCloseWnd()));
}

void QPictureToolWnd::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bPress)
    {
        return;
    }
    move(event->pos() - m_poPress + pos());
}

void QPictureToolWnd::mousePressEvent(QMouseEvent* event)
{
    m_bPress = true;
    m_poPress = event->pos();
}

void QPictureToolWnd::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_bPress = false;
}

void QPictureToolWnd::slotUploadBtnClicked()
{
    QString filepath = QFileDialog::getOpenFileName(nullptr, "选择图片", ".", "*.png");
    if (filepath.trimmed().isEmpty())
    {
        LogErr << "file path is Empty!";
        return;
    }

    QPixmap picture;
    picture.load(filepath);
    m_filePath = filepath;
    picture = picture.scaled(m_picLable->width(), m_picLable->height());
    m_picLable->setPixmap(picture);
    picture = picture.scaled(40, 40);
    m_headImg = picture;
}

void QPictureToolWnd::slotCancelBtnClicked()
{
    hide();
}

void QPictureToolWnd::slotDetermineBtnClicked()
{
    hide();

    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://49.232.169.205:80/UploadDemo/UploadServlet"));
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"headimg\";filename=\"u%1.png\"").arg(QMainWnd::getMainWnd()->m_userid));
    part.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    QFile* file = new QFile(m_filePath);
    file->open(QFile::ReadOnly);
    part.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(part);
    QNetworkReply* reply = pManager->post(request, multiPart);
    //
    connect(reply, &QNetworkReply::readyRead, this, [=]() {
        QByteArray all = reply->readAll();

        file->close();
        file->deleteLater();
        reply->deleteLater();

        QString str = QString(all);
        LogDebug << str << endl;

        neb::CJsonObject json1(str.toStdString());
        std::string headimgstr = "";
        if (!json1.Get("headimg", headimgstr))
        {
            return;
        }

        neb::CJsonObject json2;
        json2.Add("headimg", headimgstr);
        json2.Add("userid", QMainWnd::getMainWnd()->m_userid);
        //告诉远端服务器该玩家的头像数据
        QWSClientMgr::getMgr()->request("cs_msg_updateheadimg", json2, [this](neb::CJsonObject& msg) {
            LogDebug << msg.ToString().c_str() << endl;
            QMainWnd::getMainWnd()->m_toolWnd->m_headImg = QMainWnd::getMainWnd()->m_toolWnd->m_pictureToolWnd->m_headImg;
            QMainWnd::getMainWnd()->m_toolWnd->m_headUrlLabel->setPixmap(QMainWnd::getMainWnd()->m_toolWnd->m_pictureToolWnd->m_headImg);
            QDataManager::getMgr()->m_UserId2HeadImgMap[QMainWnd::getMainWnd()->m_userid] = QMainWnd::getMainWnd()->m_toolWnd->m_pictureToolWnd->m_headImg;
        });
    });
}

void QPictureToolWnd::slotCloseWnd()
{
    hide();
}

void QPictureToolWnd::SlotMinWnd()
{
    showMinimized();
}
