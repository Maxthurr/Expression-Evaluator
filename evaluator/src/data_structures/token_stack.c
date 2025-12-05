#include "token_stack.h"

#include <stdlib.h>

struct token_stack *token_stack_push(struct token_stack *s, struct token *token)
{
    struct token_stack *head = malloc(sizeof(struct token_stack));
    head->data = token;
    head->next = s;

    return head;
}

struct token_stack *token_stack_pop(struct token_stack *s)
{
    if (!s)
        return NULL;

    struct token_stack *old_h = s;
    s = s->next;

    free(old_h);
    return s;
}
