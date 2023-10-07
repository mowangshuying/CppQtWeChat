#pragma once

#include "QPch.h"

#include <QLabel>

// �Զ���Label�ؼ���Label�����ʱ�򷢳��ź�
class QSelfLabel : public QLabel
{
    Q_OBJECT
    // ���캯������������
  public:
    explicit QSelfLabel(QWidget* parent = nullptr,
                        Qt::WindowFlags wndFlags = Qt::WindowFlags());
    explicit QSelfLabel(const QString& text,
                        QWidget* parent = nullptr,
                        Qt::WindowFlags wndFlags = Qt::WindowFlags());
  signals:
    void clicked();

  protected:
    void mousePressEvent(QMouseEvent* ev) override;
};
