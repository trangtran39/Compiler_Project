#include "parser.h"
#include "token.h"

extern "C" {
#include "messages.h"
}

Parser::Parser(Source &in) {
    message(DEBUG, "PARSER: constructor called with input: '%s'", in.name());
    
    scanner = Scanner(in);
}

int Parser::parse() {
    message(DEBUG, "PARSER: parse called");
    
    int errors = 0;
    
    lookahead = scanner.nextToken();
    TranslationUnit();
    match(Token::EOI);
    
    return errors;
}

void Parser::match(int expected) {
    if (lookahead->value() < 0) // there was an scanner error
        message(PANIC, "%s", Error::toStr(lookahead->value()));
    else if (lookahead->value() == expected)
        lookahead = scanner.nextToken();
    else
        message(PANIC, "%d, %d: expecting %s: found '%s'", lookahead->row(), lookahead->col(), Token::toStr(expected), lookahead->lexeme().c_str());
}

void Parser::TranslationUnit() {
    while(lookahead->value() == Token::KW_EXTERN || lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT) {
        if (lookahead->value() == Token::KW_EXTERN) {
            match(Token::KW_EXTERN);
        }
        typeSpecifier();
        match(Token::IDENT);
        if (lookahead->value() == Token::SYM_OSQUARE || lookahead->value() == Token::SYM_COMMA || lookahead->value() == Token::SYM_SEMICOLON) {
            if (lookahead->value() == Token::SYM_OSQUARE) {
                match(Token::SYM_OSQUARE);
                match(Token::LIT_INT);
                match(Token::SYM_CSQUARE);
            }
            while (lookahead->value() == Token::SYM_COMMA) {
                match(Token::SYM_COMMA);
                match(Token::IDENT);
                if (lookahead->value() == Token::SYM_OSQUARE) {
                    match(Token::SYM_OSQUARE);
                    match(Token::LIT_INT);
                    match(Token::SYM_CSQUARE);
                }
            }
            match(Token::SYM_SEMICOLON);
        } else if (lookahead->value() == Token::SYM_OPAREND) {
            match(Token::SYM_OPAREND);
            if (lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT) {
                Parameter();
                while (lookahead->value() == Token::SYM_COMMA) {
                    match(Token::SYM_COMMA);
                    Parameter();
                }
            }
            match(Token::SYM_CPAREND);
            if (lookahead->value() == Token::SYM_OBRACE) { //(compoundStatement | ";")
                compoundStatement();
            }
            else if(lookahead->value() == Token::SYM_SEMICOLON) {
                match(Token::SYM_SEMICOLON);
            } else {
                message(PANIC, "expecting '{' or ';'");
            }
        } else {
            message(PANIC,"expecting '[' or ',' or ';' or '('");
        }
    }
}

void Parser::typeSpecifier() {
    if (lookahead->value() == Token::KW_VOID) {
        match(Token::KW_VOID);
    } else if (lookahead->value() == Token::KW_INT) {
        match(Token::KW_INT);
    } else if (lookahead->value() == Token::KW_FLOAT) {
        match(Token::KW_FLOAT);
    } else {
        message(PANIC, "expecting 'void' or 'int' or 'float'");
    }
}

void Parser::Parameter() {
    if (lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT) {
        typeSpecifier();
        match(Token::IDENT);
        if (lookahead->value() == Token::SYM_OSQUARE) {
            match(Token::SYM_OSQUARE);
            match(Token::SYM_CSQUARE);
        }
    } else {
        message(PANIC, "expecting 'void' or 'int' or 'float'");
    }
}

void Parser::compoundStatement() {
    if (lookahead->value() == Token::SYM_OBRACE) {
        match(Token::SYM_OBRACE);
        while (lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT || lookahead->value() == Token::SYM_SEMICOLON || lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING || lookahead->value() == Token::SYM_OBRACE || lookahead->value() == Token::KW_IF || lookahead->value() == Token::KW_WHILE || lookahead->value() == Token::KW_RETURN) {
            if (lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT) {
                declaration();
            } else {
                statement();
            }
        }
        match(Token::SYM_CBRACE);
    } else {
        message(PANIC,"expecting '{'");
    }
}

void Parser::declaration() {
    if (lookahead->value() == Token::KW_VOID || lookahead->value() == Token::KW_INT || lookahead->value() == Token::KW_FLOAT) {
        typeSpecifier();
        match(Token::IDENT);
        if (lookahead->value() == Token::SYM_OSQUARE) {
            match(Token::SYM_OSQUARE);
            match(Token::LIT_INT);
            match (Token::SYM_CSQUARE);
        }
        while (lookahead->value() == Token::SYM_COMMA) {
            match(Token::SYM_COMMA);
            match(Token::IDENT);
            if (lookahead->value() == Token::SYM_OSQUARE) {
                match(Token::SYM_OSQUARE);
                match(Token::LIT_INT);
                match (Token::SYM_CSQUARE);
            }
        }
        match(Token::SYM_SEMICOLON);
    } else {
        message(PANIC, "expecting 'void' or 'int' or 'float'");
    }
}

void Parser::statement() {
    if (lookahead->value() == Token::SYM_SEMICOLON || lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        expressionStatement();
    } else if (lookahead->value() == Token::SYM_OBRACE){
        compoundStatement();
    } else if (lookahead->value() == Token::KW_IF) {
        selectionStatement();
    } else if (lookahead->value() == Token::KW_WHILE) {
        repetitionStatement();
    } else if (lookahead->value() == Token::KW_RETURN) {
        returnStatement();
    } else {
        message(PANIC, "expecting statement");
    }
}

void Parser::expressionStatement() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING || lookahead->value() == Token::SYM_SEMICOLON) {
        if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING){
            expression();
            if (lookahead->value() == Token::SYM_ASSIGN) {
                match(Token::SYM_ASSIGN);
                expression();
            }
        }
        match(Token::SYM_SEMICOLON);
    } else {
        message(PANIC, "expecting expression or semicolon");
    }
}

void Parser::selectionStatement() {
    if (lookahead->value() == Token::KW_IF) {
        match(Token::KW_IF);
        match(Token::SYM_OPAREND);
        expression();
        match(Token::SYM_CPAREND);
        statement();
        if (lookahead->value() == Token::KW_ELSE) {
            match(Token::KW_ELSE);
            statement();
        }
    } else {
        message(PANIC, "expecting 'if'");
    }
}

void Parser::repetitionStatement() {
    if (lookahead->value() == Token::KW_WHILE) {
        match(Token::KW_WHILE);
        match(Token::SYM_OPAREND);
        expression();
        match(Token::SYM_CPAREND);
        statement();
    } else {
        message(PANIC, "expecting 'while'");
    }
}

void Parser::returnStatement() {
    if (lookahead->value() == Token::KW_RETURN) {
        match(Token::KW_RETURN);
        if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
            expression();
        }
        match(Token::SYM_SEMICOLON);
    } else {
        message(PANIC, "expecting 'return'");
    }
}

void Parser::expression() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        andExpression();
        while ( lookahead->value() == Token::SYM_OR){
            match(Token::SYM_OR);
            andExpression();
        }
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::andExpression() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        relationExpression();
        while (lookahead->value() == Token::SYM_AND) {
            match(Token::SYM_AND);
            relationExpression();
        }
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::relationExpression() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        simpleExpression();
        if (lookahead->value() == Token::SYM_GRTREQ || lookahead->value() == Token::SYM_GRTR || lookahead->value() == Token::SYM_LESSEQ || lookahead->value() == Token::SYM_LESS || lookahead->value() == Token::SYM_EQUAL || lookahead->value() == Token::SYM_NOTEQ) {
            if (lookahead->value() == Token::SYM_GRTREQ) {
                match(Token::SYM_GRTREQ);
            } else if (lookahead->value() == Token::SYM_GRTR) {
                match(Token::SYM_GRTR);
            } else if (lookahead->value() == Token::SYM_LESSEQ) {
                match(Token::SYM_LESSEQ);
            } else if (lookahead->value() == Token::SYM_LESS) {
                match(Token::SYM_LESS);
            } else if (lookahead->value() == Token::SYM_EQUAL) {
                match(Token::SYM_EQUAL);
            } else {
                match(Token::SYM_NOTEQ);
            }
            simpleExpression();
        }
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::simpleExpression(){
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        term();
        while (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS) {
            if (lookahead->value() == Token::SYM_PLUS) {
                match(Token::SYM_PLUS);
            } else {
                match(Token::SYM_MINUS);
            }
            term();
        }
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::term() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        factor();
        while (lookahead->value() == Token::SYM_TIMES || lookahead->value() == Token::SYM_DIVIDE || lookahead->value() == Token::SYM_MODULO) {
            if (lookahead->value() == Token::SYM_TIMES) {
                match(Token::SYM_TIMES);
            } else if (lookahead->value() == Token::SYM_DIVIDE) {
                match(Token::SYM_DIVIDE);
            } else {
                match(Token::SYM_MODULO);
            }
            factor();
        }
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::factor() {
    if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
        while (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT) {
            if (lookahead->value() == Token::SYM_PLUS) {
                match(Token::SYM_PLUS);
            } else if (lookahead->value() == Token::SYM_MINUS){
                match(Token::SYM_MINUS);
            } else {
                match(Token::SYM_NOT);
            }
        }
        value();
    } else {
        message(PANIC, "expecting '+' or '-' or '!' or '(' or identifier or integer or float or string");
    }
}

void Parser::value() {
    if (lookahead->value() == Token::SYM_OPAREND) {
        match(Token::SYM_OPAREND);
        expression();
        match(Token::SYM_CPAREND);
    } else if (lookahead->value() == Token::IDENT) {
        match(Token::IDENT);
        if (lookahead->value() == Token::SYM_OSQUARE) {
            match(Token::SYM_OSQUARE);
            expression();
            match(Token::SYM_CSQUARE);
        } else if (lookahead->value() == Token::SYM_OPAREND) {
            match(Token::SYM_OPAREND);
            if (lookahead->value() == Token::SYM_PLUS || lookahead->value() == Token::SYM_MINUS || lookahead->value() == Token::SYM_NOT || lookahead->value() == Token::SYM_OPAREND || lookahead->value() == Token::IDENT || lookahead->value() == Token::LIT_INT || lookahead->value() == Token::LIT_FLOAT || lookahead->value() == Token::LIT_STRING) {
                expression();
                while (lookahead->value() == Token::SYM_COMMA) {
                    match(Token::SYM_COMMA);
                    expression();
                }
            }
            match(Token::SYM_CPAREND);
        }
    } else if (lookahead->value() == Token::LIT_INT) {
        match(Token::LIT_INT);
    } else if (lookahead->value() == Token::LIT_FLOAT) {
        match(Token::LIT_FLOAT);
    } else if (lookahead->value() == Token::LIT_STRING) {
        match(Token::LIT_STRING);
    } else {
        message(PANIC, "expect '(' or identifier or integer or float or string");
    }
}
