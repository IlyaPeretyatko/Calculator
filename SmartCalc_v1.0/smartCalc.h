#ifndef CALC_H
#define CALC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {  // тип данных для всех вариаций
  NUMBER = 0,
  X = 1,
  PLUS = 2,
  MINUS = 3,
  MULT = 4,
  DIV = 5,
  POW = 6,
  MOD = 7,
  COS = 8,
  SIN = 9,
  TAN = 10,
  ACOS = 11,
  ASIN = 12,
  ATAN = 13,
  SQRT = 14,
  LN = 15,
  LOG = 16,
  LEFT = 17,
  RIGHT = 18,
  UNARPLUS = 19,
  UNARMINUS = 20,
} flag_t;

typedef struct stack {
  double value;
  struct stack *next;
  flag_t flag;
} stack;

int check_input(char *string);
int is_number(char symbol);
int is_unar(char *sign);
int is_function(char symbol);
int parser(char *input, char *polish);
int push_stack(stack **topNode, double value, flag_t flag);
flag_t pop_stack(stack **topNode);
double pop_stack_with_value(stack **topNode);
int is_operation(char symbol);
flag_t get_flag(stack **topNode);
char from_flag_to_char(flag_t symbol);
flag_t get_flag_of_function(char *string);
flag_t get_flag_of_operation(char *operation);
int calculation(char *polish, double *result, double x);
int calculation_operation(char operation, stack **stack);
int calculation_function(char function, stack **stack);

#endif  // CALC_H
