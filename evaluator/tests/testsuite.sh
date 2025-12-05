#!/bin/sh

### Pretty printing variables

RED="$(printf '\033[31m')"     # Red text
GREEN="$(printf '\033[32m')"   # Green text
YELLOW="$(printf '\033[33m')"  # Yellow text
BLUE="$(printf '\033[34m')"    # Blue text
BOLD="$(printf '\033[1m')"     # Bold text
RESET="$(printf '\033[0m')"    # Reset to default

### Testsuite

PASSED=0
RPN=1

test()
{
    # Get evaluator output
    if [ "$RPN" -eq 1 ]; then
        RESULT=$(echo "$2" | ./evalexpr "-rpn" 2>/dev/null || true)
    else
        RESULT=$(echo "$2" | ./evalexpr 2>/dev/null || true)
    fi

    if [ "$3" != "$RESULT" ]; then
        printf '\t%s%sTest %d Failed%s: %s --> Got %s%s%s, Expected: %s%s%s\n' "$BOLD" "$RED" "$1" "$RESET" "$2" "$RED" "$RESULT" "$RESET" "$GREEN" "$EXPECTED_STR" "$RESET"
    else
        PASSED=$(($PASSED + 1))
    fi
}


## RPN

TESTNB=0
echo "Testing RPN:"
while read line; do
    EXPR="$line"
    EXPECTED=$(dc --expression="$EXPR p" 2> /dev/null)
    test "$TESTNB" "$EXPR" "$EXPECTED"
    TESTNB=$(($TESTNB + 1))
done < tests/rpn.expr

printf '%s%sRPN Tests%s: Passed: %s%d%s | Failed: %s%d%s | Total: %d\n\n' "$BOLD" "$BLUE" "$RESET" "$GREEN" "$PASSED" "$RESET" "$RED" "$(($TESTNB - $PASSED))" "$RESET" "$TESTNB"

## Infix

RPN=0
PASSED=0
TESTNB=0
echo "Testing Infix:"
while read line; do
    EXPR="$line"
    EXPECTED=$(echo "$EXPR" | bc)
    test "$TESTNB" "$EXPR" "$EXPECTED"
    TESTNB=$(($TESTNB + 1))
done < tests/infix.expr

printf '%s%sInfix Tests%s: Passed: %s%d%s | Failed: %s%d%s | Total: %d\n\n' "$BOLD" "$BLUE" "$RESET" "$GREEN" "$PASSED" "$RESET" "$RED" "$(($TESTNB - $PASSED))" "$RESET" "$TESTNB"


## Errors

testerror()
{
    # Determine expected code (default to 0 when not provided)
    if [ -z "$3" ]; then
        EXPECTED_CODE=0
    else
        EXPECTED_CODE=$3
    fi

    # Run evaluator with the expression on stdin; suppress stdout/stderr
    echo "$2" | ./evalexpr >/dev/null 2>/dev/null
    CODE=$?

    if [ "$CODE" -ne "$EXPECTED_CODE" ]; then
        printf '\t%s%sTest %d Failed%s: %s --> Got code %s%d%s, Expected: %s%d%s\n\n' "$BOLD" "$RED" "$1" "$RESET" "$2" "$RED" "$CODE" "$RESET" "$GREEN" "$EXPECTED_CODE" "$RESET"
    else
        PASSED=$(($PASSED + 1))
    fi
}

RPN=0
ERRORS=
PASSED=0
TESTNB=0
echo "Testing Errors:"
while read line; do
    EXPR=$(echo "$line" | cut -d '|' -f 1)
    EXPECTED=$(echo "$line" | cut -d '|' -f 2)
    testerror "$TESTNB" "$EXPR" "$EXPECTED"
    TESTNB=$(($TESTNB + 1))
done < tests/errors.expr

printf '%s%sErrors Tests%s: Passed: %s%d%s | Failed: %s%d%s | Total: %d\n' "$BOLD" "$BLUE" "$RESET" "$GREEN" "$PASSED" "$RESET" "$RED" "$(($TESTNB - $PASSED))" "$RESET" "$TESTNB"


exit 0
