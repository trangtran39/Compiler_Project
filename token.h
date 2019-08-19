#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <iostream>
#include <string>

#include "error.h"

using namespace std;

class Token {

    public:
        enum Value {
            EOI,            // End-of-Input
            IDENT,          // [a-zA-Z_][a-zA-Z_0-9]*

            // literals
            LIT_INT,		// [0-9]+
            LIT_FLOAT,      // ([0-9]+\.[0-9]*|\.[0-9]+)([eE][+-]?[0-9]+)?
            LIT_STRING,		// "[^\n]"

            // keywords
            KW_ELSE,
            KW_EXTERN,
            KW_FLOAT,
            KW_IF,
            KW_INT,
            KW_RETURN,
            KW_VOID,
            KW_WHILE,

            // symbols
            SYM_ASSIGN,
            SYM_COMMA,
            SYM_SEMICOLON,
            SYM_OPAREND,
            SYM_CPAREND,
            SYM_OBRACE,
            SYM_CBRACE,
            SYM_OSQUARE,
            SYM_CSQUARE,
            SYM_PLUS,
            SYM_MINUS,
            SYM_TIMES,
            SYM_DIVIDE,
            SYM_MODULO,
            SYM_OR,
            SYM_AND,
            SYM_NOT,
            SYM_EQUAL,
            SYM_NOTEQ,
            SYM_LESS,
            SYM_LESSEQ,
            SYM_GRTR,
            SYM_GRTREQ,
        };
		static const char *toStr(int t);

        Token();  // default initializer
        Token(int val, int row, int col, string lex);
        int value();
        int row();
        int col();
        string lexeme();
        bool error();


    private:
        int val;             // Token.value or Error.value
        int r,c;             // row, col of lexeme matched
        string lex;          // lexeme matched

        friend std::ostream& operator<<(std::ostream&, const Token&);
};

#endif
