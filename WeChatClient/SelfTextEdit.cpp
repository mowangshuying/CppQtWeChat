#include "QPch.h"
#include "SelfTextEdit.h"
#include <QApplication>
#include <QClipboard>
#include <QTextCursor>
#include <QImage>

SelfTextEdit::SelfTextEdit(QWidget *parent /*= nullptr*/) : QTextEdit(parent)
{
    LogFunc;
    setAcceptRichText(true);
}

void SelfTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        const QMimeData *mime = QApplication::clipboard()->mimeData();
        if (mime->hasImage())
        {
            QImage image = qvariant_cast<QImage>(mime->imageData());
            textCursor().insertImage(image);
            return;
        }
    }

    QTextEdit::insertFromMimeData(source);
}
