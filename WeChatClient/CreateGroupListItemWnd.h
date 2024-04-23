#pragma once

#include "QPch.h"

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMap>

class CreateGroupListItemWnd : public QWidget
{
    Q_OBJECT
  public:
    CreateGroupListItemWnd(QWidget* p = nullptr);
    CreateGroupListItemWnd(QWidget* p, const char* headimg, int64_t friendid, const char* nickname, const char* rolename);

  public:
    QHBoxLayout* m_hLayout;
    QLabel* m_headImage;
    QLabel* m_nickName;
    QLabel* m_roleName;
    QString m_headImgStr;
    int64_t m_friendid;
};