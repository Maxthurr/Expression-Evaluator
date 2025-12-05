#ifndef LEXER_H
#define LEXER_H

#include "data_structures/queue.h"
#include "data_structures/token.h"

struct queue *lexer(char *expr, int rpn);

#endif /* ! LEXER_H */
