
void push(lifo_stack *l_s,Node *info) {
    stack *temp;
    temp = (stack *)malloc(sizeof(stack));
    if( temp == NULL) {
        fprintf(stderr,"Memory overflow!");
        exit(EXIT_FAILURE);
    }
    temp->data = info;
    temp->next = l_s->top;
    l_s->top = temp;
}

Node *pop(lifo_stack *l_s) {
    Node *info = NULL;
    stack *t;
    if( l_s->top ==  NULL) {
        return NULL;
    }
    else
    {
        t = l_s->top;
        info = l_s->top->data;
        l_s->top = l_s->top->next;
        t->next = NULL;
        free(t);
        return info;
    }
}

bool isEmpty(lifo_stack *l_s) {
    return l_s->top == NULL;
}

lifo_stack *init_stack(void) {
    lifo_stack *ret = calloc(1,sizeof(lifo_stack));
    ret->top = NULL;
    return ret;
}

void stack_free(lifo_stack *l_s) {
    while(!isEmpty(l_s)){
        pop(l_s);
    }
    free(l_s);
}
