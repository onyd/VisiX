

#include <QApplication>

#include <MainWindow.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // Creating main window
    MainWindow w;
    w.show();

    return QApplication::exec();
}
