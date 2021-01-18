#include "ch22.h"
#include "../dbg/dbg.h"

static const char* MY_NAME = "Ismail Arafa";

void scope_demo(int count) {
  log_info("Count is: %d", count);

  if (count > 10) {
    int count = 100;
    log_info("count in this scope is %d", count);
  }

  log_info("count is at exit %d", count);

  count = 3000;
  log_info("count after assign: %d", count);
}

int main(int argc, char* argv[]) {
  log_info("My name is %s, and age is %d", MY_NAME, get_age());
  set_age(100);

  log_info("My age now is %d", get_age());

  log_info("THE_SIZE is: %d", THE_SIZE);

  print_size();

  THE_SIZE = 9;

  log_info("THE_SIZE is: %d", THE_SIZE);

  MY_NAME = "NOT";

  print_size();

  double r = update_ratio(2.0);
  double* r1 = &r;
  update_ratio(300.0);
  update_ratio(10.0);
  update_ratio(10.0);
  update_ratio(200.0);
  log_info("Ratio at first: %f", *r1);
  log_info("Ratio again: %f", *r1);
  log_info("Ratio last time: %f", *r1);
  log_info("Ratio last time: %f", *r1);
  log_info("Ratio time: %f", *r1);
  log_info("Ratio last time: %f", *r1);

  int count = 4;
  scope_demo(count);
  scope_demo(count * 20);

  log_info("count after calling scope demo %d", count);
  return 0;
}
