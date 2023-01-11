

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

    QTSQuery query = parser.build("(ui_import source: (identifier) @source)");
    QTSMatches matches = parser.match(query);

    while (matches.next()) {
        auto match = matches.current();
        for (const auto& capture : match) {
            qDebug() << parser.text(capture);
        }
    }

    QApplication app(argc, argv);

    // Creating main window
    MainWindow w;
    w.show();

    return QApplication::exec();
}
