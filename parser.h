/* parser */
#ifndef __PARSER_H__
#define __PARSER_H__

#include "source.h"
#include "scanner.h"

class Parser {

    public:
        Parser(Source &in);
        int parse();

    private:
        Token *lookahead;
//        Token *nextPointer;
//        int lookahead;
        Scanner scanner;
		void match(int expected);
		void TranslationUnit();
        void typeSpecifier();
        void Parameter();
        void compoundStatement();
        void declaration();
        void statement();
        void expressionStatement();
        void selectionStatement();
        void repetitionStatement();
        void returnStatement();
        void expression();
        void andExpression();
        void relationExpression();
        void simpleExpression();
        void term();
        void factor();
        void value();
};

#endif
