#pragma once

#include <QPixmap>
#include <QFile>
#include <QStringList>

typedef std::map<int64_t, QPixmap> QUserId2HeadImgMap;
typedef std::map<int64_t, QPixmap> QGroupId2HeadImgMap;
typedef std::map<int64_t, QString> QFriendId2NameMap;

class QDataManager
{
private:
    QDataManager();

public:
    static QDataManager* getMgr();
    static void freeMgr();
    void setUserIdAndName(int64_t userId, QString userName);

private:
    static QDataManager* m_dataManager;

public:
    QUserId2HeadImgMap m_UserId2HeadImgMap;
    QFriendId2NameMap m_FriendId2NameMap;
    QGroupId2HeadImgMap m_GroupId2ImgMap;
    //本地文件保存的目录
    QString m_localRecvFileDir = "./recvFile/";
    QStringList m_emoijStrList;
    int64_t m_userid = -1;
    QString m_username = "";
};