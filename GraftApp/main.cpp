#include "MainWindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
#ifdef OSX
    QString exePath = (argv[0]);
    QString exeDirectory = exePath.left(exePath.lastIndexOf("/"));

    QStringList pluginsPaths;
    pluginsPaths.push_back(exeDirectory + "/../PlugIns");
    QCoreApplication::setLibraryPaths(pluginsPaths);

    QDir::setCurrent(exeDirectory);
#endif
    //std::cout << "wd" << QDir::currentPath().toStdString() << std::endl;
    QApplication a(argc, argv);
    //QLibraryInfo::location(QLibraryInfo::PluginsPath),
    MainWindow w;
    w.show();

    return a.exec();
}
