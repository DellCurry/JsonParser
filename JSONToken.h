#pragma once

typedef enum {
    END_DOCUMENT,
    BEGIN_OBJECT,
    END_OBJECT,
    BEGIN_ARRAY,
    END_ARRAY,
    SEP_COLON,
    SEP_COMMA,
    NUMBER,
    STRING,
    BOOLEAN,
    NUL
} Token;

typedef enum {
    INT_PART,
    FRA_PART,
    EXP_PART,
    NUM_END
} Number_PART;

