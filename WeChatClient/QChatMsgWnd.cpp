#include <QPainter>
#include <QDateTime>

#include "QMainWnd.h"
#include "QChatMsgWnd.h"
#include "QChatMsgWnd.h"
#include "QDataManager.h"

int QChatMsgWnd::m_tmp = 0;

QChatMsgWnd::QChatMsgWnd(QWidget* p /*= nullptr*/, int64_t sendid, QString userName, int64_t recvid)
    : QWidget(p), m_recvid(recvid), m_sendid(sendid), m_userName(userName)
{
    LogFunc;
    setObjectName("QChatMsgWnd");
    //���ô���������С
    QFont font = this->font();
    font.setFamily("MicrosoftYaHei");
    font.setPointSize(10);
    setFont(font);
    m_leftPixmap = QPixmap("./img/owner.png");
    m_rightPixmap = QPixmap("./img/other.png");
    // m_loadingMovie = new QMovie(this);
    // m_loadingMovie->setFileName("./img/loading.gif");

    // m_loadingLable = new QLabel(this);
    // m_loadingLable->setMovie(m_loadingMovie);
    // m_loadingLable->resize(16, 16);
    ////���ô��ڱ�����͸��
    // m_loadingLable->setAttribute(Qt::WA_TranslucentBackground, true);
    ////�ر��Զ���䴰�ڱ���
    // m_loadingLable->setAutoFillBackground(false);

    // setFixedWidth(640);
    setAttribute(Qt::WA_StyledBackground);

    // ���Դ���
    ///*  m_tmp++;
    //  if (m_tmp % 2 == 1)
    //  {
    //      setStyleSheet("background-color:yellow;");
    //  }
    //  else
    //  {
    //      setStyleSheet("background-color:gray;");
    //  }*/
}

QSize QChatMsgWnd::fontRect(QString str)
{
    m_msg = str;

    int minH = 30;        // ��Ϣ����С�߶�Ϊ30
    int iconWH = 30;      // ͷ��Ĵ�С
    int iconSpaceW = 20;  // ͷ�����߿��С
    int iconRectW = 5;
    int iconTMPH = 10;
    int triangleW = 6;
    int outerFrameW = 20;
    int textSpaceRect = 12;  //

    m_outerFrameWidth = this->width() - outerFrameW - 2 * (iconWH + iconTMPH + iconSpaceW);

    // ��������Ŀ�� = ��Ϣ��� - 2 * textSpaceRect
    // ��ȡ���ֵ������
    m_textWidth = m_outerFrameWidth - 2 * textSpaceRect;

    // m_spaceWidth = this->width() - textSpaceRect;

    // ���ͷ���
    // x:ͷ�������߿� iconSpaceW; y: iconTMPH��ͷ�񵽴����ϱ߿���룩
    // ��ȼ��߶ȶ�Ϊ30
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);

    // �ұ�ͷ���
    // x: ���ڿ��(this->width()) - ͷ���봰���б߿����(iconSpaceW) - ͷ����(iconWH) ; y: iconTMPH�����봰���ϱ߾��룩
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    // ��ȡ��Ϣ�����
    QSize msgSize = getRealStringSize(str);
    LogDebug << "get Real string size = " << msgSize;

    // ��Ϣ��ĸ߶ȣ���Ϣ��ĸ߶���С��ͷ��ȸ�
    int height = msgSize.height() > minH ? msgSize.height() : minH;
    LogDebug << "msg height = " << height;

    // ����������
    // ����������Rect
    // x��ͷ����봰�����߿���루iconSpaceW) + ͷ���ȣ�30��+ ���ǵ�ͷ��ľ���
    // y���еĸ߶�/2
    // w: 6
    // h: 5
    m_triangleLeftRect = QRect(iconSpaceW + iconWH + iconRectW, m_lineHeight / 2, triangleW, 5);
    m_triangleRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - triangleW, m_lineHeight / 2, triangleW, 5);

    // LogDebug << "m_textWidth + m_spaceWidth :" << (m_textWidth + m_spaceWidth);
    if (msgSize.width() < m_textWidth)
    {
        m_outerFrameLeftRect.setRect(m_triangleLeftRect.x() + m_triangleLeftRect.width(),
                                     m_lineHeight / 4 * 3,
                                     msgSize.width() + 2 * textSpaceRect,
                                     height - m_lineHeight);

        m_outerFrameRightRect.setRect(this->width() - msgSize.width() - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - triangleW,
                                      m_lineHeight / 4 * 3,
                                      msgSize.width() + 2 * textSpaceRect,
                                      height - m_lineHeight);
    }
    else
    {
        LogDebug << "msgSize.Width() >= m_textWidth + m_spaceWidth";
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
    // �Ƿ�չʾ�ǳ�
    if (m_bShowName)
    {
        // m_iconLeftRect.setRect(m_iconLeftRect.x(), m_iconLeftRect.y() + m_lineHeight, m_iconLeftRect.width(), m_iconLeftRect.height());
        // m_iconRightRect.setRect(m_iconRightRect.x(), m_iconRightRect.y() + m_lineHeight, m_iconRightRect.width(), m_iconRightRect.height());

        m_triangleLeftRect.setRect(m_triangleLeftRect.x(), m_triangleLeftRect.y() + m_lineHeight / 2, m_triangleLeftRect.width(), m_triangleLeftRect.height());
        m_triangleRightRect.setRect(m_triangleRightRect.x(),
                                    m_triangleRightRect.y() + m_lineHeight / 2,
                                    m_triangleRightRect.width(),
                                    m_triangleRightRect.height());

        m_textLeftRect.setRect(m_textLeftRect.x(), m_textLeftRect.y() + m_lineHeight / 2, m_textLeftRect.width(), m_textLeftRect.height());
        m_textRigtRect.setRect(m_textRigtRect.x(), m_textRigtRect.y() + m_lineHeight / 2, m_textRigtRect.width(), m_textRigtRect.height());

        m_outerFrameLeftRect.setRect(m_outerFrameLeftRect.x(),
                                     m_outerFrameLeftRect.y() + m_lineHeight / 2,
                                     m_outerFrameLeftRect.width(),
                                     m_outerFrameLeftRect.height());
        m_outerFrameRightRect.setRect(m_outerFrameRightRect.x(),
                                      m_outerFrameRightRect.y() + m_lineHeight / 2,
                                      m_outerFrameRightRect.width(),
                                      m_outerFrameRightRect.height());

        // �����û�������
        QFontMetricsF fm(this->font());
        int nAllTextLen = fm.width(m_userName);
        m_leftUserNameRect.setRect(m_outerFrameLeftRect.x(), m_outerFrameLeftRect.y() - m_lineHeight * 0.9, nAllTextLen, m_lineHeight);
        m_rightUserNameRect.setRect(m_outerFrameRightRect.x() + m_outerFrameRightRect.width() - nAllTextLen,
                                    m_outerFrameRightRect.y() - m_lineHeight * 0.9,
                                    nAllTextLen,
                                    m_lineHeight);

        height += m_lineHeight;
    }

    return QSize(msgSize.width(), height);
}

QSize QChatMsgWnd::getRealStringSize(QString str)
{
    QFontMetricsF fm(this->font());
    int fontw = fm.width(" ");
    m_lineHeight = 20;
    int nCount = str.count("\n");
    int nLineNum = 0;
    int nMaxWidth = 0;

    // ��ȡ��Ϣ�ܿ��
    int nAllTextLen = fm.width(str) + 1;
    // �����Ϣ��û�л��з���
    if (nCount == 0)
    {
        nMaxWidth = nAllTextLen;
        QString value = str;
        if (nMaxWidth > m_textWidth)
        {
            nMaxWidth = m_textWidth;
            int num = nAllTextLen / m_textWidth;
            nLineNum = num;
            if (nAllTextLen / m_textWidth != 0)
            {
                nLineNum += 1;
            }
        }
        else
        {
            //���һ�п��Է���
            nLineNum += 1;
        }
    }
    else
    {
        for (int i = 0; i <= nCount; i++)
        {
            QString tempSplitStr = str.split("\n").at(i);
            int nAllTextLen = fm.width(tempSplitStr) + 1;
            if (nAllTextLen > nMaxWidth)
            {
                nMaxWidth = nAllTextLen;
            }

            if (nMaxWidth > m_textWidth)
            {
                nMaxWidth = m_textWidth;
                //��ȡÿ�е�������ַ����������㺬�м���
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

    // ԭ������߶� + 2 * m_lineHeight
    return QSize(nMaxWidth /* + m_spaceWidth*/, (nLineNum + 2) * m_lineHeight);
}

void QChatMsgWnd::setText(QString text, QString time, QSize allSize, ChatMsgTypeEnum chatMsgType)
{
    m_msg = text;
    m_chatMsgType = chatMsgType;
    m_time = time;
    // m_curTime = QDateTime::fromTime_t(time.toInt()).toString("dd:hh:mm");
    m_curTime = time;
    m_allSize = allSize;
    this->update();
}

void QChatMsgWnd::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    if (ChatMsgTypeEnum::ChatMsg_OtherMsgText == m_chatMsgType || ChatMsg_OwnerMsgText == m_chatMsgType)
    {
        fontRect(m_msg);
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_OtherMsgText)
    {
        // ͷ��
        m_leftPixmap = QDataManager::getMgr()->m_UserId2HeadImgMap[m_recvid];
        m_leftPixmap = m_leftPixmap.scaled(30, 30);
        painter.drawPixmap(m_iconLeftRect, m_leftPixmap);

        // �����ⲿ�߿�
        QColor color = QColor(158, 234, 106);
        painter.setBrush(color);
        painter.drawRoundedRect(m_outerFrameLeftRect, 4, 4);

        // ����������
        QPointF points[3] = {QPointF(m_triangleLeftRect.x(), 25 + 10),
                             QPointF(m_triangleLeftRect.x() + m_triangleLeftRect.width(), 20 + 10),
                             QPointF(m_triangleLeftRect.x() + m_triangleLeftRect.width(), 30 + 10)};
        painter.drawPolygon(points, 3);

        // ��������
        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textLeftRect, m_msg, option);

        if (m_bShowName)
        {
            penText.setColor(QColor(153, 153, 153));
            painter.setPen(penText);
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            painter.setFont(this->font());
            painter.drawText(m_leftUserNameRect, m_userName, option);
        }
    }

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_OwnerMsgText)
    {
        m_rightPixmap = QMainWnd::getMainWnd()->m_toolWnd->m_headImg;
        m_rightPixmap = m_rightPixmap.scaled(30, 30);

        //�������ͷ��
        painter.drawPixmap(m_iconRightRect, m_rightPixmap);

        //�����ⲿ�߿�
        QColor color = QColor(158, 234, 106);
        painter.setBrush(color);
        painter.drawRoundedRect(m_outerFrameRightRect, 4, 4);

        // ����������
        QPointF points[3] = {QPointF(m_triangleRightRect.x() + m_triangleRightRect.width(), 25 + 10),
                             QPointF(m_triangleRightRect.x(), 20 + 10),
                             QPointF(m_triangleRightRect.x(), 30 + 10)};
        painter.drawPolygon(points, 3);

        // ��������
        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textRigtRect, m_msg, option);

        if (m_bShowName)
        {
            penText.setColor(QColor(153, 153, 153));
            painter.setPen(penText);
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            painter.setFont(this->font());
            painter.drawText(m_rightUserNameRect, m_userName, option);
        }
    }

    if (m_chatMsgType == ChatMsgTypeEnum::ChatMsg_Time)
    {
        QPen penText;
        penText.setColor(QColor(153, 153, 153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(), m_curTime, option);
    }
}
