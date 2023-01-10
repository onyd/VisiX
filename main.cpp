

#include <QApplication>

#include <MainWindow.hpp>

#include "QTreeSitter.h"

int main(int argc, char** argv)
{
    
    // Build a syntax tree based on source code stored in a string.
    QFile file("..\\libs\\tree-sitter-qmljs\\test\\highlight\\components.qml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    QTextStream in(&file);

    QTreeSitter parser = QTreeSitter::QML();
    QString source = in.readAll();
    parser.setSource(source);
    parser.update();

    QApplication app(argc, argv);

    // Creating main window
    MainWindow w;
    w.show();

    return QApplication::exec();
}
