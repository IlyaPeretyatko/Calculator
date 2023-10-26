#ifndef SMARTCALC_H
#define SMARTCALC_H

#include <QMainWindow>
#include <QVector>

extern "C" {
#include "../s21_smartCalc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class smartcalc;
}
QT_END_NAMESPACE

class smartcalc : public QMainWindow {
  Q_OBJECT

 public:
  smartcalc(QWidget *parent = nullptr);
  ~smartcalc();
  void from_QStr_to_string(char *str, QString qstr);
  double x, xmax, xmin, ymax, ymin;

 private:
  Ui::smartcalc *ui;
  QVector<double> x_graph, y_graph;

 private slots:
  void digits_numbers();
  void on_pushButton_dot_clicked();
  void on_pushButton_plus_clicked();
  void on_pushButton_minus_clicked();
  void on_pushButton_mult_clicked();
  void on_pushButton_left_clicked();
  void on_pushButton_right_clicked();
  void on_pushButton_del_clicked();
  void on_pushButton_mod_clicked();
  void on_pushButton_pow_clicked();
  void on_pushButton_sqrt_clicked();
  void on_pushButton_log_clicked();
  void on_pushButton_sin_clicked();
  void on_pushButton_cos_clicked();
  void on_pushButton_asin_clicked();
  void on_pushButton_acos_clicked();
  void on_pushButton_tan_clicked();
  void on_pushButton_atan_clicked();
  void on_pushButton_ln_clicked();
  void on_pushButton_x_clicked();
  void on_pushButton_AC_clicked();
  void on_pushButton_calculate_clicked();
  void on_lineEdit_enterx_textChanged(const QString &arg1);
  void on_lineEdit_xmax_textChanged(const QString &arg1);
  void on_lineEdit_xmin_textChanged(const QString &arg1);
  void on_lineEdit_ymax_textChanged(const QString &arg1);
  void on_lineEdit_ymin_textChanged(const QString &arg1);
  void on_pushButton_graph_clicked();
};
#endif  // SMARTCALC_H
