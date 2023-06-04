#pragma once

#include "QPch.h"

#include <QListWidget>

class QCustomListWidgetItem : public QListWidgetItem
{
public:
    explicit QCustomListWidgetItem(QListWidget* view = nullptr, int type = 0);
    ~QCustomListWidgetItem();

    int sesId() const;
    void setSesId(int sesid);

private:
    //添加一些额外的数据，如会话id；
    //设置默认值为-1
    int m_sesId = -1;
};