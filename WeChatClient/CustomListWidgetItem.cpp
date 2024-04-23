#include "QPch.h"
#include "CustomListWidgetItem.h"

CustomListWidgetItem::CustomListWidgetItem(QListWidget* view /*= nullptr*/,
                                             int type /*= Type*/)
    : QListWidgetItem(view, type)
{
    LogFunc;
    m_sesId = 0;
}

CustomListWidgetItem::~CustomListWidgetItem()
{
    LogFunc;
}

int CustomListWidgetItem::sesId() const
{
    return m_sesId;
}

void CustomListWidgetItem::setSesId(int sesid)
{
    m_sesId = sesid;
}
