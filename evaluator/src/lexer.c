#include "lexer.h"

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/queue.h"
#include "data_structures/token.h"

static int get_number(char **expr)
{
    char *cur = *expr;

    while (*cur && isdigit(*cur))
    {
        cur++;
    }

    char tmp = *cur;
    *cur = '\0';

    int nb = atoi(*expr);
    *cur = tmp;
    *expr = cur;

    return nb;
}

static int add_to_queue(struct queue *queue, enum token_type type, int value,
                        bool is_bin)
{
    struct token *token = malloc(sizeof(struct token));
    if (!token)
        return 1;

    token->type = type;
    token->value = value;
    token->is_bin = is_bin;
    queue_push(queue, token);

    return 0;
}

static bool is_binary(char operator, struct token * previous)
{
    if (operator== '*' || operator== '/' || operator== '^')
        return true;

    // If a + or a - has no left operand, it is unary
    if (!previous)
        return false;

    return previous->type == INT || previous->type == RIGHT_P;
}

enum token_type get_type(char c)
{
    switch (c)
    {
    case '+':
        return PLUS;
    case '-':
        return MINUS;
    case '*':
        return MULT;
    case '/':
        return DIV;
    case '%':
        return MOD;
    case '^':
        return POWER;
    case '(':
        return LEFT_P;
    case ')':
        return RIGHT_P;
    default: // unexpected character
        errno = 1;
        return 1;
    }
}

static int read_and_add(struct queue *queue, char c, bool rpn)
{
    enum token_type type = get_type(c);
    if (errno)
    {
        warnx("Unexpected token '%c' in expression", c);
        return 1;
    }

    // RPN does not allow parentheses
    if (rpn && (type == LEFT_P || type == RIGHT_P))
    {
        warnx("Parentheses are not allowed in RPN expressions");
        errno = 1;
        return 1;
    }

    // Empty parentheses are not allowed
    if (type == RIGHT_P && queue->tail && queue->tail->token->type == LEFT_P)
    {
        warnx("Empty parentheses in expression");
        errno = 2;
        return 2;
    }

    struct token *prev = queue->tail ? queue->tail->token : NULL;
    return add_to_queue(queue, type, -1, rpn ? true : is_binary(c, prev));
}

struct queue *lexer(char *expr, bool rpn)
{
    if (!expr)
        return NULL;

    struct queue *queue = queue_init();
    if (!queue)
        return NULL;

    while (*expr)
    {
        if (isspace(*expr))
        {
            expr++;
            continue;
        }

        if (isdigit(*expr))
        {
            // Two numbers in a row is invalid in infix notation
            if (!rpn && queue->tail && queue->tail->token->type == INT)
            {
                queue_destroy(queue, 1);
                errno = 2;
                return NULL;
            }

            // Add the number token to the queue
            int nb = get_number(&expr);
            if (add_to_queue(queue, INT, nb, 0))
            {
                queue_destroy(queue, 1);
                errno = 1;
                return NULL;
            }
        }
        else
        {
            // Add the operator token to the queue
            int e = read_and_add(queue, *expr, rpn);
            if (e)
            {
                queue_destroy(queue, 1);
                errno = e;
                return NULL;
            }
            expr++;
        }
    }

    return queue;
}
