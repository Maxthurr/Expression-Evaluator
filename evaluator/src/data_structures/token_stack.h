#ifndef TOKEN_STACK_H
#define TOKEN_STACK_H

#include "token.h"

struct token_stack
{
    struct token *data;
    struct token_stack *next;
};

struct token_stack *token_stack_push(struct token_stack *s,
                                     struct token *token);
struct token_stack *token_stack_pop(struct token_stack *s);

#endif /* ! TOKEN_STACK_H */
