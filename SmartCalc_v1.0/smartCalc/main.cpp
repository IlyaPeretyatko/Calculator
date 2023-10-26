#include <QApplication>

#include "smartcalc.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  smartcalc w;
  w.setFixedSize(427, 793);
  w.show();
  return a.exec();
}
