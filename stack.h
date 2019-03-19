#ifndef STACK_H
#define STACK_H

typedef struct _stack {
    Node *data;
    struct _stack *next;
}stack;

typedef struct _lifo_stack {
    stack *top;
}lifo_stack;

void push(lifo_stack *l_s,Node *info);
Node *pop(lifo_stack *l_s);
isEmpty(lifo_stack *l_s);
lifo_stack *init_stack(void);
void stack_free(lifo_stack *l_s);

#endif // STACK_H
