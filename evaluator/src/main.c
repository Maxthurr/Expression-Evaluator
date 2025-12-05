#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
            int olde = errno;
            free(expr);
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
        errx(4, "Usage: %s [-rpn | -h]", argv[0]);

    if (strcmp("-h", argv[1]) == 0)
    {
        printf("Usage: %s [-rpn | -h]\n", argv[0]);
        puts("Evaluate mathematical expressions from standard input.");
        puts("\t-rpn\tInterpret input expressions as Reverse Polish Notation "
             "(RPN).");
        puts("\t-h\tDisplay this help message and exit.");
        return 0;
    }

    bool rpn = argc == 2;
    if (rpn && strcmp("-rpn", argv[1]) != 0)
        errx(4, "Usage: %s [-rpn | -h]", argv[0]);

    // If input is from terminal, print prompt
    struct stat st;
    fstat(STDIN_FILENO, &st);
    if (S_ISCHR(st.st_mode))
        puts("Enter expression(s) to evaluate (Ctrl+D to end):");

    return evaluate(rpn);
}
