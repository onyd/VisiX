

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

    auto d = parser.text(parser.root());
    qDebug() << d.toUtf8().data();
    qDebug() << parser.print();

    QFile qfile("..\\resources\\queries\\objects.scm");
    if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    auto query = qfile.readAll().toStdString();

    QTSMatches matches = parser.match(query);
    for (const auto& match : matches) {
        for (const auto& capture : match) {
            qDebug() << capture.text();
        }
    }

    QApplication app(argc, argv);

    // Creating main window
    MainWindow w;
    w.show();

    return QApplication::exec();
}
