#include "s21_smartCalc.h"

// int main() {
//  char input3[256] = "6**";
//   char polish3[256] = {0};
//   int rv = parser(input3, polish3);
//   printf("%d", rv);
//   rv = parser(input3, polish3);
//   printf("%d", rv);
//   return 0;
// }

int is_number(char symbol) {
  return (symbol == 'x' || symbol == '.' || (symbol >= '0' && symbol <= '9'));
}

int check_input(char *string) {
  int return_value = 0;
  int left = 0;
  int right = 0;
  int not_empty = 0;
  for (int i = 0; string[i] != '\0' && return_value == 0; ++i) {
    if (string[i] == 'x' &&
        (is_number(string[i + 1]) || is_function(string[i + 1]))) {
      return_value = 1;
    }
    if (i != 0) {
      if (string[i] == 'x' &&
          (is_number(string[i - 1]) || is_function(string[i - 1]) ||
           string[i - 1] == ')')) {
        return_value = 1;
      }
    }
    if ((is_operation(string[i]) && is_operation(string[i + 1])) ||
        (string[i] == '.' && string[i + 1] == '.')) {
      return_value = 1;
    }
    if (string[i] == '(') {
      left++;
      not_empty = 0;
    } else if (string[i] == ')') {
      if (!not_empty) {
        return_value = 1;
      }
      right++;
    } else if (is_number(string[i]) || string[i] == 's' || string[i] == 'c' ||
               string[i] == 'l' || string[i] == 'a' || string[i] == 't') {
      not_empty = 1;
    }
    if (left < right) {
      return_value = 1;
    }
    if (string[i] == '(' && (string[i + 1] == '*' || string[i + 1] == '/' ||
                             string[i + 1] == '^' || string[i + 1] == '%')) {
      return_value = 1;
    }
    if ((string[i] == '*' || string[i] == '/' || string[i] == '%' ||
         string[i] == '^') &&
        is_number(string[i - 1]) == 0) {
      if (string[i - 1] != ')') {
        return_value = 1;
      }
    }
    if ((string[i] == '*' || string[i] == '/' || string[i] == '%' ||
         string[i] == '^' || string[i] == '-' || string[i] == '+') &&
        is_number(string[i + 1]) == 0 && string[i + 1] != '(') {
      if (!(string[i + 1] == 'c' || string[i + 1] == 's' ||
            string[i + 1] == 't' || string[i + 1] == 'a' ||
            string[i + 1] == 'l')) {
        return_value = 1;
      }
    }
  }
  if (left != right) {
    return_value = 1;
  }
  return return_value;
}

int get_priority(flag_t symbol) {
  int return_value = -1;
  if (symbol == UNARMINUS || symbol == UNARPLUS) {
    return_value = 5;
  } else if (symbol == SIN || symbol == COS || symbol == TAN ||
             symbol == ASIN || symbol == ACOS || symbol == ATAN ||
             symbol == SQRT || symbol == LOG || symbol == LN) {
    return_value = 4;
  } else if (symbol == POW) {
    return_value = 3;
  } else if (symbol == MULT || symbol == DIV || symbol == MOD) {
    return_value = 2;
  } else if (symbol == PLUS || symbol == MINUS) {
    return_value = 1;
  } else if (symbol == LEFT || symbol == RIGHT) {
    return_value = 0;
  }
  return return_value;
}

int is_unar(char *sign) {
  int return_value = 0;
  if (is_number(*(sign + 1)) && *(sign - 1) == '(' &&
      (*sign == '+' || *sign == '-')) {
    return_value = 1;
  }
  if (is_number(*(sign + 1)) && *(sign - 1) == 0 &&
      (*sign == '+' || *sign == '-')) {
    return_value = 1;
  }
  return return_value;
}

int push_stack(stack **topNode, double value, flag_t flag) {
  int return_value = 0;
  stack *newNode = malloc(sizeof(struct stack));
  if (newNode != NULL) {
    newNode->value = value;
    newNode->flag = flag;
    newNode->next = *topNode;
    *topNode = newNode;
  } else {
    return_value = 1;
  }
  return return_value;
}

flag_t pop_stack(stack **topNode) {
  stack *tempNode = *topNode;
  flag_t return_value = (*topNode)->flag;
  *topNode = (*topNode)->next;
  free(tempNode);
  return return_value;
}

int parser(char *input, char *polish) {
  int return_value = check_input(input);
  stack *stack = NULL;
  if (return_value == 0) {
    int count = 0;
    for (int i = 0; input[i] != '\0' && return_value != 1; i++) {
      if (is_number(input[i])) {
        while (is_number(input[i])) {
          polish[count] = input[i];
          count++;
          i++;
        }
        polish[count] = ' ';
        count++;
        if (input[i] == '\0') {
          break;
        }
      }
      if (input[i] == 'x') {
        polish[count] = 'x';
        polish[count + 1] = ' ';
        count += 2;
      } else if (input[i] == '(') {
        if (push_stack(&stack, 0, LEFT)) {
          return_value = 1;
        }
      } else if (input[i] == ')') {
        flag_t operation = pop_stack(&stack);
        while (operation != LEFT) {
          polish[count] = from_flag_to_char(operation);
          count++;
          operation = pop_stack(&stack);
        }
      } else {
        if (is_operation(input[i]) == 0) {
          char string[12] = {0};
          int j = 0;
          for (j = i; input[j] != '('; j++) {
            if (j - i == 12) {
              return_value = 1;
              break;
            }
            string[j - i] = input[j];
          }
          i = j;
          flag_t flag_operatiom = get_flag_of_function(string);
          if (*&stack == NULL) {
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
            if (push_stack(&stack, 0, LEFT)) {
              return_value = 1;
            }
          } else if (get_priority(flag_operatiom) >
                     get_priority(get_flag(&stack))) {
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
            if (push_stack(&stack, 0, LEFT)) {
              return_value = 1;
            }
          } else {
            while (get_priority(flag_operatiom) <=
                   get_priority(get_flag(&stack))) {
              polish[count] = from_flag_to_char(pop_stack(&stack));
              count++;
            }
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
          }
        } else {
          flag_t flag_operatiom = get_flag_of_operation(input + i);
          if (stack == NULL) {
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
          } else if (get_priority(flag_operatiom) >
                     get_priority(get_flag(&stack))) {
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
          } else {
            while (get_priority(flag_operatiom) <=
                   get_priority(get_flag(&stack))) {
              polish[count] = from_flag_to_char(pop_stack(&stack));
              count++;
            }
            if (push_stack(&stack, 0, flag_operatiom)) {
              return_value = 1;
            }
          }
        }
      }
    }
    flag_t operation;
    while (stack != NULL) {
      operation = pop_stack(&stack);
      polish[count] = from_flag_to_char(operation);
      count++;
    }
  }
  return return_value;
}

int is_operation(char symbol) {
  return (symbol == '/' || symbol == '*' || symbol == '+' || symbol == '-' ||
          symbol == '^' || symbol == '%' || symbol == '~');
}

flag_t get_flag(stack **topNode) {
  flag_t return_value = 0;
  if (*topNode != NULL) {
    return_value = (*topNode)->flag;
  }
  return return_value;
}

char from_flag_to_char(flag_t symbol) {
  char return_value = 0;
  if (symbol == LEFT) {
    return_value = '(';
  } else if (symbol == RIGHT) {
    return_value = ')';
  } else if (symbol == PLUS) {
    return_value = '+';
  } else if (symbol == MINUS) {
    return_value = '-';
  } else if (symbol == MULT) {
    return_value = '*';
  } else if (symbol == DIV) {
    return_value = '/';
  } else if (symbol == MOD) {
    return_value = '%';
  } else if (symbol == POW) {
    return_value = '^';
  } else if (symbol == SIN) {
    return_value = 's';
  } else if (symbol == COS) {
    return_value = 'c';
  } else if (symbol == TAN) {
    return_value = 't';
  } else if (symbol == ASIN) {
    return_value = 'S';
  } else if (symbol == ACOS) {
    return_value = 'C';
  } else if (symbol == ATAN) {
    return_value = 'T';
  } else if (symbol == SQRT) {
    return_value = 'q';
  } else if (symbol == LOG) {
    return_value = 'L';
  } else if (symbol == LN) {
    return_value = 'l';
  } else if (symbol == UNARMINUS) {
    return_value = '~';
  } else if (symbol == UNARPLUS) {
    return_value = '#';
  }
  return return_value;
}

flag_t get_flag_of_function(char *string) {
  flag_t return_value = 0;
  if (strcmp(string, "sin") == 0) {
    return_value = SIN;
  } else if (strcmp(string, "cos") == 0) {
    return_value = COS;
  } else if (strcmp(string, "tan") == 0) {
    return_value = TAN;
  } else if (strcmp(string, "asin") == 0) {
    return_value = ASIN;
  } else if (strcmp(string, "acos") == 0) {
    return_value = ACOS;
  } else if (strcmp(string, "atan") == 0) {
    return_value = ATAN;
  } else if (strcmp(string, "sqrt") == 0) {
    return_value = SQRT;
  } else if (strcmp(string, "ln") == 0) {
    return_value = LN;
  } else if (strcmp(string, "log") == 0) {
    return_value = LOG;
  }
  return return_value;
}

flag_t get_flag_of_operation(char *operation) {
  flag_t return_value = 0;
  if (*operation == '+' && is_unar(operation) == 1) {
    return_value = UNARPLUS;
  } else if (*operation == '+' && is_unar(operation) == 0) {
    return_value = PLUS;
  } else if (*operation == '-' && is_unar(operation) == 1) {
    return_value = UNARMINUS;
  } else if (*operation == '-' && is_unar(operation) == 0) {
    return_value = MINUS;
  } else if (*operation == '*') {
    return_value = MULT;
  } else if (*operation == '/') {
    return_value = DIV;
  } else if (*operation == '^') {
    return_value = POW;
  } else if (*operation == '%') {
    return_value = MOD;
  }
  return return_value;
}

int calculation(char *polish, double *result, double x) {
  int return_value = 0;
  stack *stack = NULL;
  for (size_t i = 0; i < strlen(polish) && return_value != 1; i++) {
    if (polish[i] == 'x') {
      if (push_stack(&stack, x, 1)) {
        return_value = 1;
      }
    } else if (is_number(polish[i])) {
      int index = 0;
      char string[256] = {0};
      double number = 0;
      while (is_number(polish[i])) {
        string[index] = polish[i];
        index++;
        i++;
      }
      number = atof(string);
      if (push_stack(&stack, number, 0)) {
        return_value = 1;
      }
    } else if (is_operation(polish[i])) {
      return_value = calculation_operation(polish[i], &stack);
    } else if (is_function(polish[i])) {
      return_value = calculation_function(polish[i], &stack);
    }
  }
  *result = pop_stack_with_value(&stack);
  return return_value;
}

int calculation_function(char function, stack **stack) {
  int return_value = 0;
  double number = 0;
  if (function == 's') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, sin(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'c') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, cos(number), 0)) {
      return_value = 1;
    }
  } else if (function == 't') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, tan(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'S') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, asin(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'C') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, acos(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'T') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, atan(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'q') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, sqrt(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'L') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, log10(number), 0)) {
      return_value = 1;
    }
  } else if (function == 'l') {
    number = pop_stack_with_value(stack);
    if (push_stack(stack, log(number), 0)) {
      return_value = 1;
    }
  }
  return return_value;
}

int calculation_operation(char operation, stack **stack) {
  int return_value = 0;
  double number_1, number_2;
  if (operation == '+') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, number_1 + number_2, 0)) {
      return_value = 1;
    }
  } else if (operation == '-') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, number_1 - number_2, 0)) {
      return_value = 1;
    }
  } else if (operation == '*') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, number_1 * number_2, 0)) {
      return_value = 1;
    }
  } else if (operation == '/') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (number_2 != 0) {
      if (push_stack(stack, number_1 / number_2, 0)) {
        return_value = 1;
      }
    }
  } else if (operation == '^') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, pow(number_1, number_2), 0)) {
      return_value = 1;
    }
  } else if (operation == '%') {
    number_2 = pop_stack_with_value(stack);
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, fmod(number_1, number_2), 0)) {
      return_value = 1;
    }
  } else if (operation == '~') {
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, (0 - number_1), 0)) {
      return_value = 1;
    }
  } else if (operation == '#') {
    number_1 = pop_stack_with_value(stack);
    if (push_stack(stack, number_1, 0)) {
      return_value = 1;
    }
  }
  return return_value;
}

int is_function(char symbol) {
  return (symbol == 's' || symbol == 'c' || symbol == 't' || symbol == 'S' ||
          symbol == 'C' || symbol == 'T' || symbol == 'q' || symbol == 'L' ||
          symbol == 'l');
}

double pop_stack_with_value(stack **topNode) {
  stack *tempNode = *topNode;
  double return_value = 0;
  if (*topNode != NULL) {
    return_value = (*topNode)->value;
    *topNode = (*topNode)->next;
    free(tempNode);
  }
  return return_value;
}
