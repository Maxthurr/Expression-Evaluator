#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/queue.h"
#include "evalrpn.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
    if (argc > 2)
        return 4;

    int rpn = argc == 2;
    if (rpn && strcmp("-rpn", argv[1]) != 0)
        return 4;

    char *expr = NULL;
    size_t l;
    while (getline(&expr, &l, stdin) != EOF)
    {
        struct queue *queue = lexer(expr, rpn);

        if (!queue)
        {
            free(expr);
            return errno;
        }

        if (!rpn)
        {
            struct queue *tmp = parser(queue);
            queue_destroy(queue, 0);
            if (!tmp)
            {
                queue_destroy(tmp, 1);
                free(expr);
                return errno;
            }

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
    return 0;
}
