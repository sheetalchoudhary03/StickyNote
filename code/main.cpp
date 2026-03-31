#include <QApplication>
#include "sticky_note.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StickyNote note;
    note.show();
    return app.exec();
}

