#include <QApplication>

#include "ruler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Ruler ruler;
    ruler.show();
    return app.exec();
}
