#include <stdio.h>
#include <stdlib.h>

struct Stack {
  int size;
  unsigned limit;
  int* list;
};

struct Stack* createStack(unsigned limit) {
  struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
  stack->limit = limit;
  stack->size = 0;
  stack->list = (int*) malloc(stack->limit * sizeof(int));
  return stack;
}

int isFull(struct Stack* stack) {
  return stack->limit == stack->size;
}

int isEmpty(struct Stack* stack) {
  return stack->size == 0;
}

void push(struct Stack* stack, int elm) {
  if (isFull(stack)) {
    return;
  }

  stack->list[++stack->size] = elm;
}
