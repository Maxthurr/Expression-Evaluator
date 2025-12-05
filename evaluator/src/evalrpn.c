#include "evalrpn.h"

#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/queue.h"
#include "data_structures/stack.h"
#include "data_structures/token.h"
#include "operation.h"

static struct stack *handle_unary(struct stack *stack, struct token *token,
                                  operation *operation)
{
    if (!stack)
        return NULL;

    // Get the top value and apply the unary operation
    // then push the result back onto the stack
    int prev = stack_peek(stack);
    struct stack *new_stack = stack_pop(stack);
    new_stack = stack_push(new_stack, operation[token->type](0, prev));

    return new_stack;
}

static struct stack *handle_binary(struct stack *stack, struct token *token,
                                   operation *operation)
{
    if (!stack || !stack->next)
    {
        warnx("Binary operator requires two operands");
        errno = 2;
        return NULL;
    }

    int prev = stack_peek(stack);
    int prev2 = stack_peek(stack->next);

    if (token->type == POWER && prev < 0)
    {
        warnx("Negative powers are not supported");
        errno = 3;
        return NULL;
    }

    if ((token->type == DIV || token->type == MOD) && !prev)
    {
        warnx("Division by zero");
        errno = 3;
        return NULL;
    }

    // Pop the two values and push the operation result back onto the stack
    struct stack *rest = stack_pop(stack);
    rest = stack_pop(rest);
    rest = stack_push(rest, operation[token->type](prev2, prev));

    return rest;
}

static struct stack *handle_operator(struct stack *stack, struct token *token,
                                     operation *operation)
{
    if (token->is_bin)
        return handle_binary(stack, token, operation);

    return handle_unary(stack, token, operation);
}

static void cleanup(struct stack *stack, operation *operation,
                    struct queue *queue)
{
    free_stack(stack);
    free(operation);
    queue_destroy(queue, 1);
}

int evalrpn(struct queue *queue)
{
    operation *operation = build_operation();
    if (!operation)
        return 0;

    struct stack *stack = NULL;
    while (queue->size)
    {
        struct token *token = queue_head(queue);
        if (token->type == INT)
            stack = stack_push(stack, token->value);
        else
        {
            struct stack *tmp = handle_operator(stack, token, operation);
            if (!tmp)
            {
                int olde = errno;
                cleanup(stack, operation, queue);
                errno = olde ? olde : 1;
                return 0;
            }

            stack = tmp;
        }

        // free the token when popping
        queue_pop(queue, 1);
    }

    // More than one element in the stack or no stack at all
    if (!stack || stack->next)
    {
        warnx("Invalid expression");
        cleanup(stack, operation, queue);
        errno = 2;
        return 0;
    }

    int result = stack_peek(stack);
    cleanup(stack, operation, queue);

    return result;
}
