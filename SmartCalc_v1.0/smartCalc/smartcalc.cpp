#include "smartcalc.h"

#include "../s21_smartCalc.h"
#include "ui_smartcalc.h"

smartcalc::smartcalc(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::smartcalc) {
  ui->setupUi(this);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
}

smartcalc::~smartcalc() { delete ui; }

void smartcalc::digits_numbers() {
  QPushButton *button = (QPushButton *)sender();
  ui->label->setText(ui->label->text() + button->text());
}

void smartcalc::on_pushButton_dot_clicked() {
  ui->label->setText(ui->label->text() + ".");
}

void smartcalc::on_pushButton_plus_clicked() {
  ui->label->setText(ui->label->text() + "+");
}

void smartcalc::on_pushButton_minus_clicked() {
  ui->label->setText(ui->label->text() + "-");
}

void smartcalc::on_pushButton_mult_clicked() {
  ui->label->setText(ui->label->text() + "*");
}

void smartcalc::on_pushButton_left_clicked() {
  ui->label->setText(ui->label->text() + "(");
}

void smartcalc::on_pushButton_right_clicked() {
  ui->label->setText(ui->label->text() + ")");
}

void smartcalc::on_pushButton_del_clicked() {
  ui->label->setText(ui->label->text() + "/");
}

void smartcalc::on_pushButton_mod_clicked() {
  ui->label->setText(ui->label->text() + "%");
}

void smartcalc::on_pushButton_pow_clicked() {
  ui->label->setText(ui->label->text() + "^");
}

void smartcalc::on_pushButton_sqrt_clicked() {
  ui->label->setText(ui->label->text() + "sqrt(");
}

void smartcalc::on_pushButton_log_clicked() {
  ui->label->setText(ui->label->text() + "log(");
}

void smartcalc::on_pushButton_sin_clicked() {
  ui->label->setText(ui->label->text() + "sin(");
}

void smartcalc::on_pushButton_cos_clicked() {
  ui->label->setText(ui->label->text() + "cos(");
}

void smartcalc::on_pushButton_asin_clicked() {
  ui->label->setText(ui->label->text() + "asin(");
}

void smartcalc::on_pushButton_acos_clicked() {
  ui->label->setText(ui->label->text() + "acos(");
}

void smartcalc::on_pushButton_tan_clicked() {
  ui->label->setText(ui->label->text() + "tan(");
}

void smartcalc::on_pushButton_atan_clicked() {
  ui->label->setText(ui->label->text() + "atan(");
}

void smartcalc::on_pushButton_ln_clicked() {
  ui->label->setText(ui->label->text() + "ln(");
}

void smartcalc::on_pushButton_x_clicked() {
  ui->label->setText(ui->label->text() + "x");
}

void smartcalc::on_pushButton_AC_clicked() { ui->label->setText(""); }

void smartcalc::on_pushButton_calculate_clicked() {
  char input_string[256] = {0};
  from_QStr_to_string(input_string, ui->label->text());
  char polish[256] = {0};
  double res = 0;
  if (parser(input_string, polish)) {
    ui->label->setText("Error");
  } else {
    if (calculation(polish, &res, x)) {
      ui->label->setText("Error");
    } else {
      char output_string[64] = {0};
      sprintf(output_string, "%.7lf", res);
      ui->label->setText(output_string);
    }
  }

  ui->lineEdit_xmax->setText("");
  ui->lineEdit_ymax->setText("");
  ui->lineEdit_xmin->setText("");
  ui->lineEdit_ymin->setText("");
  ui->lineEdit_enterx->setText("");
}

void smartcalc::from_QStr_to_string(char *str, QString qstr) {
  for (int i = 0; i < qstr.length(); ++i) {
    str[i] = qstr.toStdString().c_str()[i];
  }
}

void smartcalc::on_lineEdit_enterx_textChanged(const QString &arg1) {
  x = arg1.toDouble();
}

void smartcalc::on_lineEdit_xmax_textChanged(const QString &arg2) {
  xmax = arg2.toDouble();
}

void smartcalc::on_lineEdit_xmin_textChanged(const QString &arg3) {
  xmin = arg3.toDouble();
}

void smartcalc::on_lineEdit_ymax_textChanged(const QString &arg4) {
  ymax = arg4.toDouble();
}

void smartcalc::on_lineEdit_ymin_textChanged(const QString &arg5) {
  ymin = arg5.toDouble();
}

void smartcalc::on_pushButton_graph_clicked() {
  if (ui->lineEdit_xmax->text().isEmpty()) {
    xmax = 1000;
  }
  if (ui->lineEdit_xmin->text().isEmpty()) {
    xmin = -1000;
  }
  if (ui->lineEdit_ymax->text().isEmpty()) {
    ymax = 1000;
  }
  if (ui->lineEdit_ymin->text().isEmpty()) {
    ymin = -1000;
  }

  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->setInteraction(QCP::iRangeDrag, true);

  double h = (xmax - xmin) / 100000;
  char input_string[256] = {0};
  from_QStr_to_string(input_string, ui->label->text());
  char polish[256] = {0};

  if (parser(input_string, polish)) {
    ui->label->setText("Error");
  } else {
    if ((xmin <= xmax) && (ymin <= ymax)) {
      double res = 0;
      for (double X = xmin; X <= xmax; X += h) {
        x_graph.push_back(X);
        calculation(polish, &res, X);
        y_graph.push_back(res);
      }
      ui->widget->xAxis->setRange(xmax, xmin);
      ui->widget->yAxis->setRange(ymax, ymin);
      ui->widget->addGraph();
      ui->widget->graph(0)->setData(x_graph, y_graph);
      ui->widget->replot();
      x_graph.clear();
      y_graph.clear();
    } else {
      ui->label->setText("Error");
    }
  }
}
