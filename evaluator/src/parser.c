#include "parser.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "data_structures/queue.h"
#include "data_structures/token.h"
#include "data_structures/token_stack.h"

static bool is_operator(struct token *token)
{
    switch (token->type)
    {
    case INT:
        return false;
    case LEFT_P:
        return false;
    case RIGHT_P:
        return false;
    default:
        return true;
    }
}

static bool is_left_associative(struct token *token)
{
    return token->type == MULT || token->type == DIV || token->type == MOD
        || token->is_bin;
}

static bool operator_cmp(struct token *t1, struct token *t2)
{
    int p1 = 0;
    int p2 = 0;

    if (t1->type == LEFT_P || t1->type == RIGHT_P)
        p1 = 5;
    if (t2->type == LEFT_P || t2->type == RIGHT_P)
        p2 = 5;

    if ((t1->type == PLUS && !t1->is_bin) || (t1->type == MINUS && !t1->is_bin))
        p1 = 4;
    if ((t2->type == PLUS && !t2->is_bin) || (t2->type == MINUS && !t2->is_bin))
        p2 = 4;

    if (t1->type == POWER)
        p1 = 3;
    if (t2->type == POWER)
        p2 = 3;

    if (t1->type == MULT || t1->type == DIV || t1->type == MOD)
        p1 = 2;
    if (t2->type == MULT || t2->type == DIV || t2->type == MOD)
        p2 = 2;

    if ((t1->type == PLUS && t1->is_bin) || (t1->type == MINUS && t1->is_bin))
        p1 = 1;
    if ((t2->type == PLUS && t2->is_bin) || (t2->type == MINUS && t2->is_bin))
        p2 = 1;

    return p2 > p1 || (p1 == p2 && is_left_associative(t1));
}

static struct token_stack *handle_right_par(struct queue *output,
                                            struct token_stack *operators)
{
    while (operators && operators->data->type != LEFT_P)
    {
        queue_push(output, operators->data);
        operators = token_stack_pop(operators);
    }

    if (!operators)
    {
        errno = 2;
        return NULL;
    }

    if (operators->data->type == LEFT_P)
    {
        free(operators->data);
        operators = token_stack_pop(operators);
    }

    return operators;
}

static struct token_stack *handle_operator(struct token_stack *operators,
                                           struct queue *output,
                                           struct token *token)
{
    while (operators && is_operator(operators->data)
           && operators->data->type != LEFT_P
           && operator_cmp(token, operators->data))
    {
        queue_push(output, operators->data);
        operators = token_stack_pop(operators);
    }
    operators = token_stack_push(operators, token);

    return operators;
}

static struct token_stack *parse_queue(struct queue *tokens,
                                       struct queue *output)
{
    struct token_stack *operators = NULL;
    while (tokens->size > 0)
    {
        struct token *token = queue_head(tokens);
        queue_pop(tokens, 0); // Do not free token when popping

        if (token->type == INT)
            queue_push(output, token);

        else if (is_operator(token))
            operators = handle_operator(operators, output, token);
        else if (token->type == LEFT_P)
            operators = token_stack_push(operators, token);

        else if (token->type == RIGHT_P)
        {
            struct token_stack *tmp = handle_right_par(output, operators);
            if (errno)
            {
                free(token);
                while (tmp)
                {
                    free(tmp->data);
                    tmp = token_stack_pop(tmp);
                }

                errno = 2;
                return operators;
            }
            operators = tmp;

            free(token);
        }
    }

    return operators;
}

struct queue *parser(struct queue *tokens)
{
    struct queue *output = queue_init();
    struct token_stack *operators = parse_queue(tokens, output);
    if (errno)
    {
        int olde = errno;
        while (operators)
        {
            free(operators->data);
            operators = token_stack_pop(operators);
        }
        queue_destroy(output, 1);
        queue_destroy(tokens, 1);

        errno = olde;
        return NULL;
    }

    // Empty operators stack
    while (operators)
    {
        if (operators->data->type == LEFT_P)
        {
            while (operators)
            {
                free(operators->data);
                operators = token_stack_pop(operators);
            }
            queue_destroy(output, 1);
            queue_destroy(tokens, 1);

            errno = 2;
            return NULL;
        }

        queue_push(output, operators->data);
        operators = token_stack_pop(operators);
    }

    return output;
}
