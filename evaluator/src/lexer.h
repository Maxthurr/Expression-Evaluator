#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "data_structures/queue.h"
#include "data_structures/token.h"

struct queue *lexer(char *expr, bool rpn);

#endif /* ! LEXER_H */
