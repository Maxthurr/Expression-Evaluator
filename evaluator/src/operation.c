#include "operation.h"

#include <stdlib.h>

#include "data_structures/token.h"

int add(int a, int b)
{
    return a + b;
}
int sub(int a, int b)
{
    return a - b;
}
int mul(int a, int b)
{
    return a * b;
}
int divide(int a, int b)
{
    return a / b;
}
int mod(int a, int b)
{
    return a % b;
}
int power(int a, int b)
{
    if (b == 0 || a == 1)
        return 1;

    if (b == 1)
        return a;

    if (b % 2 == 0)
        return power(a * a, b / 2);

    return a * power(a * a, (b - 1) / 2);
}

operation *build_operation()
{
    operation *arr = malloc(8 * sizeof(operation *));
    if (!arr)
        return NULL;

    arr[PLUS] = add;
    arr[MINUS] = sub;
    arr[MULT] = mul;
    arr[DIV] = divide;
    arr[POWER] = power;
    arr[MOD] = mod;

    return arr;
}
