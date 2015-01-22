#include "Console.h"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    setTabChangesFocus(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Horizontal, Vertical

    setFont(QFont("Courier"));
    QFontMetrics fm(font());
    setCursorWidth(fm.width(QLatin1Char('x')));


//    setMinimumHeight(sizeHint().height());

    bashOutputEnd = new QTextCursor(document());
}

void Console::keyPressEvent(QKeyEvent *event)
{
    // User can only modify text past last prompt
    int curPos = textCursor().position();
    if ((curPos == bashEndPos && event->key() == Qt::Key_Backspace) || (curPos < bashEndPos))
        return;

    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && event->modifiers() == Qt::SHIFT){
        QKeyEvent *e = new QKeyEvent(event->type(), event->key(), Qt::NoModifier);
        QTextEdit::keyPressEvent(e);
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
        QTextEdit::keyPressEvent(event);
        emit returnPressed();
    }
    else
        QTextEdit::keyPressEvent(event);
}

void Console::insertBashOutput(QString text)
{
    insertPlainText(text);

    // Save cursor to end of bash output
    bashOutputEnd->movePosition(QTextCursor::End);
    bashEndPos = bashOutputEnd->position();

    ensureCursorVisible();
}

QSize Console::sizeHint()
{
    QFontMetrics fm(font());
    int h = fm.height() * 1.8;
    int w = fm.width(QLatin1Char('x')) * 17 + 4;
    return QSize(w,h);
}
