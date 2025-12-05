#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/queue.h"
#include "evalrpn.h"
#include "lexer.h"
#include "parser.h"

static int evaluate(bool rpn)
{
    char *expr = NULL;
    size_t l;
    while (getline(&expr, &l, stdin) != EOF)
    {
        if (*expr == '\n')
            continue;

        struct queue *queue = lexer(expr, rpn);

        if (!queue)
        {
            free(expr);
            return errno;
        }

        if (!queue->size) // No expressions were given
        {
            queue_destroy(queue, 1);
            continue;
        }

        if (!rpn)
        {
            struct queue *tmp = parser(queue);
            if (!tmp)
            {
                free(expr);
                return errno;
            }

            queue_destroy(queue, 0);
            queue = tmp;
        }

        int result = evalrpn(queue);
        if (errno)
        {
            free(expr);
            int olde = errno;
            return olde;
        }

        printf("%d\n", result);
    }

    free(expr);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2)
        return 4;

    bool rpn = argc == 2;
    if (rpn && strcmp("-rpn", argv[1]) != 0)
        return 4;

    return evaluate(rpn);
}
