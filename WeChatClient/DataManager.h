#pragma once

#include <QPixmap>
#include <QFile>
#include <QStringList>

typedef std::map<int64_t, QPixmap> QUserId2HeadImgMap;
typedef std::map<int64_t, QPixmap> QGroupId2HeadImgMap;
typedef std::map<int64_t, QString> QFriendId2NameMap;

class DataManager
{
  private:
    explicit DataManager();

  public:
    static DataManager* getMgr();
    static void initMgr()
    {
        getMgr();
    }
    static void exitMgr();
    void setUserIdAndName(int64_t userId, QString userName);
    void setUserHeadImg(int64_t userId, QPixmap pixmap);

  private:
    static DataManager* m_dataManager;

  public:
    QUserId2HeadImgMap m_UserId2HeadImgMap;
    QFriendId2NameMap m_FriendId2NameMap;
    QGroupId2HeadImgMap m_GroupId2ImgMap;
    // 本地文件保存的目录
    QString m_localRecvFileDir = "./recvFile/";
    QStringList m_emoijStrList;
    int64_t m_userid = -1;
    QString m_username = "";
};