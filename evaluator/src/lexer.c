#include "lexer.h"

#include <ctype.h>
#include <errno.h>
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
                        int is_bin)
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

static int is_binary(char operator, struct token * previous)
{
    if (operator== '*' || operator== '/' || operator== '^')
        return 1;

    if (!previous)
        return 0;

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

static int read_and_add(struct queue *queue, char c, int rpn)
{
    enum token_type type = get_type(c);
    if (errno)
        return 1;

    if (type == RIGHT_P && queue->tail && queue->tail->token->type == LEFT_P)
        return 2;

    struct token *prev = queue->tail ? queue->tail->token : NULL;
    return add_to_queue(queue, type, -1, rpn ? 1 : is_binary(c, prev));
}

struct queue *lexer(char *expr, int rpn)
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
            if (!rpn && queue->tail && queue->tail->token->type == INT)
            {
                queue_destroy(queue, 1);
                errno = 2;
                return NULL;
            }
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
