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
    NULL
} Token;

