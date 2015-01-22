#include "Launcher.h"

# define BUFFER_SIZE    1024

Launcher::Launcher(int bashInputFD, int bashOutputFD, QWidget *parent) : QDialog(parent)
{
    bashIn = bashInputFD;
    bashOut = bashOutputFD;
    skipNextLine = false;

    bashOutputNotifier = new QSocketNotifier(bashOut, QSocketNotifier::Read);
    connect(bashOutputNotifier, SIGNAL(activated(int)), this, SLOT(printBashStdout(int)));

    // Console
    cons = new Console(this);
    connect(cons, SIGNAL(returnPressed()), this, SLOT(consoleToBash()));

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(cons);
    setLayout(layout);
}

void Launcher::consoleToBash(){
    cons->bashOutputEnd->setPosition(cons->bashEndPos, QTextCursor::MoveAnchor);
    cons->bashOutputEnd->movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    QString consInput = cons->bashOutputEnd->selection().toPlainText();

    printf("Console input was: %s\n", consInput.toStdString().c_str());
    fflush(stdout);

    const char *out = consInput.toStdString().c_str();

    skipNextLine = true; // Avoid duplication of command

    write(bashIn, out, consInput.length() + 1);
}

void Launcher::printBashStdout(int bashStdout){
    char buf[BUFFER_SIZE];
    int nbytes;

    if ((nbytes = read(bashStdout, buf, BUFFER_SIZE)) > 0){
        buf[nbytes] = '\0';

        char *newLinePos = NULL;
        if (skipNextLine && (newLinePos = strchr(buf, '\n')))
            skipNextLine = false;
        else if (skipNextLine)
            return;

        printf("Bash output was: %s\n", buf);
        printf("Bash output size was: %d\n", bashStdout);
        fflush(stdout);
        if (newLinePos)
            cons->insertBashOutput(QString(newLinePos + 1));
        else
            cons->insertBashOutput(QString(buf));
    }
    else if (nbytes < 0){
        perror("Read error");
    }
}

Launcher::~Launcher()
{
}
