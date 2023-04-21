#pragma once
#include <QTextEdit>
#include <QMimeData>

class QSelfTextEdit : public QTextEdit
{
public:
    QSelfTextEdit(QWidget *parent = nullptr);

    void insertFromMimeData(const QMimeData *source);
};
