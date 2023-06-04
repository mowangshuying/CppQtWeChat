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
    //���һЩ��������ݣ���Ựid��
    //����Ĭ��ֵΪ-1
    int m_sesId = -1;
};