#pragma once

#include "def.h"

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMap>

class CreateGroupListItemWithSelBtnWnd : public QWidget
{
    Q_OBJECT
  public:
    CreateGroupListItemWithSelBtnWnd(QWidget* p = nullptr);
    CreateGroupListItemWithSelBtnWnd(QWidget* p, const char* headimg, int64_t friendid, const char* nickname, const char* rolename);
  signals:
    void signalSelRBtnClick(QMap<QString, QString> map);
  public slots:
    void slotClickedSelBtn(bool isSel = false);

  public:
    QHBoxLayout* m_hLayout;
    QLabel* m_headImage;
    QLabel* m_nickName;
    QLabel* m_roleName;
    QString m_headImgStr;
    int64_t m_friendid;
    QRadioButton* m_selRBtn;
};