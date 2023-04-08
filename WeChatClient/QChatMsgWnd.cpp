#include <QPainter>
#include <QDateTime>

#include "QMainWnd.h"
#include "QChatMsgWnd.h"
#include "QChatMsgWnd.h"
#include "QDataManager.h"

QChatMsgWnd::QChatMsgWnd(QWidget* p /*= nullptr*/, int64_t sendid, int64_t recvid) : QWidget(p), m_recvid(recvid), m_sendid(sendid)
{
    //设置窗体的字体大小
    QFont font = this->font();
    font.setFamily("MicrosoftYaHei");
    font.setPointSize(10);
    setFont(font);
    m_leftPixmap = QPixmap("./img/owner.png");
    m_rightPixmap = QPixmap("./img/other.png");
    m_loadingMovie = new QMovie(this);
    m_loadingMovie->setFileName("./img/loading.gif");

    m_loadingLable = new QLabel(this);
    m_loadingLable->setMovie(m_loadingMovie);
    m_loadingLable->resize(16, 16);
    //设置窗口背景：透明
    m_loadingLable->setAttribute(Qt::WA_TranslucentBackground, true);
    //关闭自动填充窗口背景
    m_loadingLable->setAutoFillBackground(false);
    setAttribute(Qt::WA_StyledBackground);
}

QSize QChatMsgWnd::fontRect(QString str)
{
    m_msg = str;

    int minH = 30;    //消息的最小高度为30
    int iconWH = 30;  //头像的大小
    int iconSpaceW = 20;
    int iconRectW = 5;
    int iconTMPH = 10;
    int triangleW = 6;
    int outerFrameW = 20;
    int textSpaceRect = 12;  //

    // int width = this->width(); 注释无效变量

    m_outerFrameWidth = this->width() - outerFrameW - 2 * (iconWH + iconTMPH + iconSpaceW);
    m_textWidth = m_outerFrameWidth - 2 * textSpaceRect;

    m_spaceWidth = this->width() - textSpaceRect;

    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    QSize msgSize = getRealStringSize(str);
    LogDebug << "fontRect size = " << msgSize;

    int height = msgSize.height() > minH ? msgSize.height() : minH;

    LogDebug << "QChatMsgWnd::fontRect height = " << height;
    m_triangleLeftRect = QRect(iconWH + iconSpaceW + iconRectW, m_lineHeight / 2, triangleW, 5);

    m_triangleRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - triangleW, m_lineHeight / 2, triangleW, 5);

    if (msgSize.width() < (m_textWidth + m_spaceWidth))
    {
        m_outerFrameLeftRect.setRect(m_triangleLeftRect.x() + m_triangleLeftRect.width(),
                                     m_lineHeight / 4 * 3,
                                     msgSize.width() - m_spaceWidth + 2 * textSpaceRect,
                                     height - m_lineHeight);

        m_outerFrameRightRect.setRect(this->width() - msgSize.width() + m_spaceWidth - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - triangleW,
                                      m_lineHeight / 4 * 3,
                                      msgSize.width() - m_spaceWidth + 2 * textSpaceRect,
                                      height - m_lineHeight);
    }
    else
    {
        m_outerFrameLeftRect.setRect(m_triangleLeftRect.x() + m_triangleLeftRect.width(), m_lineHeight / 4 * 3, m_outerFrameWidth, height - m_lineHeight);

        m_outerFrameRightRect.setRect(m_triangleRightRect.x() - m_outerFrameWidth, m_lineHeight / 4 * 3, m_outerFrameWidth, height - m_lineHeight);
    }

    m_textLeftRect.setRect(m_outerFrameLeftRect.x() + textSpaceRect,
                           m_outerFrameLeftRect.y() + iconTMPH,
                           m_outerFrameLeftRect.width() - 2 * textSpaceRect,
                           m_outerFrameLeftRect.height() - 2 * iconTMPH + 2);

    m_textRigtRect.setRect(m_outerFrameRightRect.x() + textSpaceRect,
                           m_outerFrameRightRect.y() + iconTMPH,
                           m_outerFrameRightRect.width() - 2 * textSpaceRect,
                           m_outerFrameRightRect.height() - 2 * iconTMPH + 2);

    return QSize(msgSize.width(), height);
}

QSize QChatMsgWnd::getRealStringSize(QString str)
{
    // LogDebug() << "getRealStringSize start------------------------------";
    QFontMetricsF fm(this->font());
    // m_lineHeight = fm.lineSpacing();
    // int fontHeight = fm.height();
    int fontw = fm.width(" ");
    m_lineHeight = 20;
    int nCount = str.count("\n");
    int nLineNum = 0;
    int nMaxWidth = 0;

    int nEmoijCount = 0;
    // for(int i=0;i<QMainWnd::getSinletonInstance()->m_toolWnd->m_)

    for (int i = 0; i < QDataManager::getInstance()->m_emoijStrList.size(); i++)
    {
        QString tempStr = QDataManager::getInstance()->m_emoijStrList[i];
        int tempCount = str.count(tempStr);
        nEmoijCount += tempCount;
    }

    // int nLength = str.length();

    int nAllTextLen = fm.width(str) + nEmoijCount;
    if (nCount == 0)
    {
        nMaxWidth = nAllTextLen;
        // nMaxWidth = 10*str.length();

        QString value = str;
        if (nMaxWidth > m_textWidth)
        {
            nMaxWidth = m_textWidth;
            //获取每行的最大存放字符数量
            // int size = m_textWidth/fm.width(" ");
            // int size = m_textWidth /10;

            //计算含有几行
            // int num = fm.width(value)/m_textWidth;
            int num = nAllTextLen / m_textWidth;

            nLineNum = num;
            /*if( fm.width(value)/m_textWidth-num>0) {
                    nLineNum += 1;
            }*/

            if (nAllTextLen / m_textWidth != 0)
            {
                nLineNum += 1;
            }
        }
        else
        {
            //如果一行可以放下
            nLineNum += 1;
        }
    }
    else
    {
        for (int i = 0; i <= nCount; i++)
        {
            QString tempSplitStr = str.split("\n").at(i);
            int nEmoijCount = 0;
            for (int i = 0; i < QDataManager::getInstance()->m_emoijStrList.size(); i++)
            {
                QString tempStr = QDataManager::getInstance()->m_emoijStrList[i];
                int tempCount = tempSplitStr.count(tempStr);
                nEmoijCount += tempCount;
            }

            int nAllTextLen = fm.width(tempSplitStr) + nEmoijCount;

            // int valueWidth = fm.width(value);
            if (nAllTextLen > nMaxWidth)
            {
                nMaxWidth = nAllTextLen;
            }

            if (nMaxWidth > m_textWidth)
            {
                nMaxWidth = m_textWidth;
                //获取每行的最大存放字符数量
                // int size = m_textWidth / fm.width(" ");
                //计算含有几行
                int num = nAllTextLen / m_textWidth;
                nLineNum = num;
                if (nAllTextLen % m_textWidth != 0)
                {
                    nLineNum += 1;
                }
            }
            else
            {
                nLineNum++;
            }
        }
    }

    return QSize(nMaxWidth + m_spaceWidth, (nLineNum + 2) * m_lineHeight);
}

void QChatMsgWnd::setText(QString text, QString time, QSize allSize, ChatMsgTypeEnum chatMsgType)
{
    m_msg = text;
    m_chatMsgType = chatMsgType;
    m_time = time;
    m_curTime = QDateTime::fromTime_t(time.toInt()).toString("dd:hh:mm");
    m_allSize = allSize;
    this->update();
}

void QChatMsgWnd::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_Other)
    {
        m_leftPixmap = QDataManager::getInstance()->m_UserId2HeadImgMap[m_recvid];
        m_leftPixmap = m_leftPixmap.scaled(30, 30);
        painter.drawPixmap(m_iconLeftRect, m_leftPixmap);
        //绘制外部边框
        QColor color = QColor(158, 234, 106);
        painter.setBrush(color);
        painter.drawRoundedRect(m_outerFrameLeftRect, 4, 4);

        QPointF points[3] = {QPointF(m_triangleLeftRect.x(), 25),
                             QPointF(m_triangleLeftRect.x() + m_triangleLeftRect.width(), 20),
                             QPointF(m_triangleLeftRect.x() + m_triangleLeftRect.width(), 30)};

        painter.drawPolygon(points, 3);

        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textLeftRect, m_msg, option);
    }

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_Owner)
    {
        m_rightPixmap = QMainWnd::getInstance()->m_toolWnd->m_headImg;
        m_rightPixmap = m_rightPixmap.scaled(30, 30);

        //绘制玩家头像
        painter.drawPixmap(m_iconRightRect, m_rightPixmap);

        //绘制外部边框
        QColor color = QColor(158, 234, 106);
        painter.setBrush(color);
        painter.drawRoundedRect(m_outerFrameRightRect, 4, 4);

        //
        QPointF points[3] = {QPointF(m_triangleRightRect.x() + m_triangleRightRect.width(), 25),
                             QPointF(m_triangleRightRect.x(), 20),
                             QPointF(m_triangleRightRect.x(), 30)};

        painter.drawPolygon(points, 3);

        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textRigtRect, m_msg, option);
    }

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_Time)
    {
    }

    // update();
}
