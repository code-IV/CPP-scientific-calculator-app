#include <QApplication>
#include "headers/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Scientific Calculator");
    window.resize(400, 500);
    window.show();
    return app.exec();
}