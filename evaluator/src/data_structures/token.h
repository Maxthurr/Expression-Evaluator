#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    INT,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    POWER,
    LEFT_P,
    RIGHT_P
};

struct token
{
    enum token_type type;
    int value;
    int is_bin;
};

#endif /* TOKEN_H */
