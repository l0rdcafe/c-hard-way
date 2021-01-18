#include <stdio.h>

int main(int argc, char* argv[]) {
  int ages[] = { 23, 43, 12, 89, 2 };
  char* names[] = { "Alan", "Frank", "Mary", "John", "List" };

  int count = sizeof(ages) / sizeof(int);

  for (int i = count - 1; i >= 0; i--) {
    printf("%s has %d years alive.\n", names[i], ages[i]);
  }
  printf("---\n");

  int* cur_age = ages;
  char** cur_name = names;

  for (int i = count - 1; i >= 0; i--) {
    printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
  }
  printf("---\n");

  for (int i = count - 1; i >= 0; i--) {
    printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
  }
  printf("---\n");

  for (cur_name = names + count - 1, cur_age = ages + count - 1; (cur_age - ages) >= 0; cur_name--, cur_age--) {
    printf("%s lived %d years so far.\n", *cur_name, *cur_age);
  }
  printf("---\n");

  for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++) {
    printf("Name address at %p and age address at %p.\n", cur_name, cur_age);
    printf("Size of '%s' string is %lu bytes\n", *cur_name, sizeof(*cur_name));
    printf("Size of '%d' integer is %lu bytes\n", *cur_age, sizeof(*cur_age));
  }
  printf("---\n");

  char** cur_arg = argv;
  for (int i = 0; i < argc; i++) {
    printf("Size of argument '%s' is %lu bytes\n", argv[i], sizeof(argv[i]));
    printf("Argument '%s' located at '%p'\n", *(cur_arg + i), *(cur_arg + i));
  }

  return 0;
}
