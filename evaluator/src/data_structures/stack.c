#include "stack.h"

#include <stdlib.h>

struct stack *stack_push(struct stack *s, int e)
{
    struct stack *head = malloc(sizeof(struct stack));
    head->data = e;
    head->next = s;

    return head;
}

struct stack *stack_pop(struct stack *s)
{
    if (!s)
        return NULL;

    struct stack *old_h = s;
    s = s->next;

    free(old_h);
    return s;
}

int stack_peek(struct stack *s)
{
    return s->data;
}

void free_stack(struct stack *s)
{
    while (s)
    {
        struct stack *tmp = s;
        s = s->next;
        free(tmp);
    }
}
