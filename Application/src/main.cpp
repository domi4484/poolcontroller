
// Project includes ------------------------
#include "gui/MainWindow.h"

// Qt includes -----------------------------
#include <QApplication>

const QString EMULATE("--emulate");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool emulate = false;
    for(int i=0; i!=argc; i++)
    {
        if(argv[i] == EMULATE)
            emulate = true;
    }

    try
    {
        MainWindow w(emulate);
        w.show();
        return a.exec();
    }
    catch(...)
    {}
}
