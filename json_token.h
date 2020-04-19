#pragma once

typedef enum {
    END_JSON, // 0
    BEGIN_OBJ, // 1
    END_OBJ, // 2
    BEGIN_ARR, // 3
    END_ARR, // 4
    COLON, // 5
    COMMA, // 6
    NUMBER, // 7
    STRING, // 8
    BOOL, // 9
    NUL // 10
} token_type;

typedef enum {
    INT_PART,
    FRA_PART,
    EXP_PART,
    NUM_END
} number_part;

