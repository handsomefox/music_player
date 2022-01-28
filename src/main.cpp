#include <QApplication>
#include <QFile>
#include "UI/MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Q_INIT_RESOURCE(breeze_themes);
  QFile file(":/dark-purple/stylesheet.qss");
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream(&file);
  a.setStyleSheet(stream.readAll());
  MainWindow w;
  w.show();
  return QApplication::exec();
}
