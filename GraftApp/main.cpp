#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QString exePath = (argv[0]);
    QString exeDirectory = exePath.left(exePath.lastIndexOf("/"));

    QStringList pluginsPaths;
    pluginsPaths.push_back(exeDirectory + "/../PlugIns");
    QCoreApplication::setLibraryPaths(pluginsPaths);

    QApplication a(argc, argv);
    //QLibraryInfo::location(QLibraryInfo::PluginsPath),
    MainWindow w;
    w.show();

    return a.exec();
}
