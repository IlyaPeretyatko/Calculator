#include <check.h>

#include "s21_smartCalc.h"

START_TEST(operation_test) {
  char input[256] = "-33+(-4+8)*2";
  char polish[256] = {0};
  parser(input, polish);
  double res = 0;
  int status = calculation(polish, &res, 0);
  ck_assert_double_eq(res, -25.000000);
  ck_assert_int_eq(status, 0);

  char input1[256] = "((-33/3)*8)%5";
  char polish1[256] = {0};
  parser(input1, polish1);
  status = calculation(polish1, &res, 0);
  ck_assert_double_eq(res, -3);
  ck_assert_int_eq(status, 0);

  char input2[256] = "-2^5+3";
  char polish2[256] = {0};
  parser(input2, polish2);
  status = calculation(polish2, &res, 0);
  ck_assert_double_eq(res, -29);
  ck_assert_int_eq(status, 0);

  char input3[256] = "-5+(3-0)";
  char polish3[256] = {0};
  parser(input3, polish3);
  status = calculation(polish3, &res, 0);
  ck_assert_double_eq(res, -2);
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(function_test) {
  char input[256] = "(sqrt(64)+2)*3";
  char polish[256] = {0};
  parser(input, polish);
  double res = 0;
  int status = calculation(polish, &res, 0);
  ck_assert_double_eq(res, 30);
  ck_assert_int_eq(status, 0);

  char input1[256] = "sin(1+0.678)";
  char polish1[256] = {0};
  parser(input1, polish1);
  status = calculation(polish1, &res, 0);
  ck_assert_ldouble_eq_tol(res, 0.994259, 1e-6);
  ck_assert_int_eq(status, 0);

  char input2[256] = "cos(1.32)";
  char polish2[256] = {0};
  parser(input2, polish2);
  status = calculation(polish2, &res, 0);
  ck_assert_ldouble_eq_tol(res, 0.248175, 1e-6);
  ck_assert_int_eq(status, 0);

  char input3[256] = "asin(0.56)";
  char polish3[256] = {0};
  parser(input3, polish3);
  status = calculation(polish3, &res, 0);
  ck_assert_ldouble_eq_tol(res, 0.594386, 1e-6);
  ck_assert_int_eq(status, 0);

  char input4[256] = "acos(0.56)";
  char polish4[256] = {0};
  parser(input4, polish4);
  status = calculation(polish4, &res, 0);
  ck_assert_ldouble_eq_tol(res, 0.976411, 1e-6);
  ck_assert_int_eq(status, 0);

  char input5[256] = "tan(1)";
  char polish5[256] = {0};
  parser(input5, polish5);
  status = calculation(polish5, &res, 0);
  ck_assert_ldouble_eq_tol(res, 1.557408, 1e-6);
  ck_assert_int_eq(status, 0);

  char input6[256] = "ln(atan(5))";
  char polish6[256] = {0};
  parser(input6, polish6);
  status = calculation(polish6, &res, 0);
  ck_assert_ldouble_eq_tol(res, 0.317290, 1e-6);
  ck_assert_int_eq(status, 0);

  char input7[256] = "log(90+10-0)";
  char polish7[256] = {0};
  parser(input7, polish7);
  status = calculation(polish7, &res, 0);
  ck_assert_ldouble_eq_tol(res, 2, 1e-6);
  ck_assert_int_eq(status, 0);
}
END_TEST

int main() {
  Suite *suite = suite_create("S21_SMARTCALC");
  SRunner *srunner = srunner_create(suite);

  TCase *operation_tcase = tcase_create("operation_test");
  suite_add_tcase(suite, operation_tcase);
  tcase_add_test(operation_tcase, operation_test);

  TCase *function_tcase = tcase_create("function_test");
  suite_add_tcase(suite, function_tcase);
  tcase_add_test(function_tcase, function_test);

  srunner_run_all(srunner, CK_VERBOSE);
  srunner_free(srunner);

  return 0;
}
