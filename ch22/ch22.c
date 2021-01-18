#include <stdio.h>
#include "ch22.h"
#include "dbg.h"

int THE_SIZE = 1000;
static int THE_AGE = 37;

int get_age() {
  return THE_AGE;
}

void set_age(int age) {
  THE_AGE = age;
}

double update_ratio(double ratio) {
  static double val = 1.0;
  double last_ratio = val;
  val = ratio;
  return last_ratio;
}

void print_size() {
  log_info("I think size is: %d", THE_SIZE);
}
