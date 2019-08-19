#include "token.h"

const char *Token::toStr(int t) {
	switch (t) {
		case EOI:			return "<end-of-file>";
		case IDENT:			return "identifier";
		case LIT_INT:		return "integer literal";
		case LIT_FLOAT:		return "float literal";
		case LIT_STRING:	return "string literal";
		case KW_ELSE:		return "'else'";
		case KW_EXTERN:		return "'extern'";
		case KW_FLOAT:		return "'float'";
		case KW_IF:			return "'if'";
		case KW_INT:		return "'int'";
		case KW_RETURN:		return "'return'";
		case KW_VOID:		return "'void'";
		case KW_WHILE:		return "'while'";
		case SYM_ASSIGN:	return "'='";
		case SYM_COMMA:		return "','";
		case SYM_SEMICOLON: return "';'";
		case SYM_OPAREND:	return "'('";
		case SYM_CPAREND:	return "')'";
		case SYM_OBRACE:	return "'{'";
		case SYM_CBRACE:	return "'}'";
		case SYM_OSQUARE:	return "'['";
		case SYM_CSQUARE:	return "']'";
		case SYM_PLUS:		return "'+'";
		case SYM_MINUS:		return "'-'";
		case SYM_TIMES:		return "'*'";
		case SYM_DIVIDE:	return "'/'";
		case SYM_MODULO:	return "'%'";
		case SYM_OR:		return "'||'";
		case SYM_AND:		return "'&&'";
		case SYM_NOT:		return "'!'";
		case SYM_EQUAL:		return "'=='";
		case SYM_NOTEQ:		return "'!='";
		case SYM_LESS:		return "'<'";
		case SYM_LESSEQ:	return "'<='";
		case SYM_GRTR:		return "'>'";
		case SYM_GRTREQ:	return "'>='";
		default:			return "<unknown token>";
	}
}

Token::Token() {
    val = Error::TokenNotInitialized;
}

Token::Token(int val, int row, int col, string lex) {
    this->val = val;
    this->r = row;
    this->c = col;
    this->lex = lex;
}

int Token::value() {
    return val;
}

int Token::row() {
    return r;
}

int Token::col() {
    return c;
}

string Token::lexeme() {
    return lex;
}

bool Token::error() {
    return val<0;
}

// TODO: fix this to use the Token::to_string() and Error::to_string()
std::ostream& operator<<(std::ostream& out, const Token& t) {
    out << "<" << t.r << "," << t.c << ":";
    switch (t.val) {
    	case Token::EOI: out << "EOI";
    		break;
    	case Token::IDENT: out << "IDENT(" << t.lex << ")";
    		break;
    	case Token::LIT_INT: out << "LITERAL(int," << t.lex << ")";
    		break;
    	case Token::LIT_FLOAT: out << "LITERAL(float," << t.lex << ")";
    		break;
    	case Token::LIT_STRING: out << "LITERAL(string," << t.lex << ")";
    		break;
    	case Token::KW_ELSE: out << "KEYWORD(else)";
    		break;
    	case Token::KW_EXTERN: out << "KEYWORD(extern)";
    		break;
    	case Token::KW_FLOAT: out << "KEYWORD(float)";
    		break;
    	case Token::KW_IF: out << "KEYWORD(if)";
    		break;
    	case Token::KW_INT: out << "KEYWORD(int)";
    		break;
    	case Token::KW_RETURN: out << "KEYWORD(return)";
    		break;
    	case Token::KW_VOID: out << "KEYWORD(void)";
    		break;
    	case Token::KW_WHILE: out << "KEYWORD(while)";
    		break;
    	case Token::SYM_ASSIGN: out << "SYMBOL('=')";
    		break;
    	case Token::SYM_COMMA: out << "SYMBOL(',')";
    		break;
    	case Token::SYM_SEMICOLON: out << "SYMBOL(';')";
    		break;
    	case Token::SYM_OPAREND: out << "SYMBOL('(')";
    		break;
    	case Token::SYM_CPAREND: out << "SYMBOL(')')";
    		break;
    	case Token::SYM_OBRACE: out << "SYMBOL('{')";
    		break;
    	case Token::SYM_CBRACE: out << "SYMBOL('}')";
    		break;
    	case Token::SYM_OSQUARE: out << "SYMBOL('[')";
    		break;
    	case Token::SYM_CSQUARE: out << "SYMBOL(']')";
    		break;
    	case Token::SYM_PLUS: out << "SYMBOL('+')";
    		break;
    	case Token::SYM_MINUS: out << "SYMBOL('-')";
    		break;
    	case Token::SYM_TIMES: out << "SYMBOL('*')";
    		break;
    	case Token::SYM_DIVIDE: out << "SYMBOL('/')";
    		break;
    	case Token::SYM_MODULO: out << "SYMBOL('%')";
    		break;
    	case Token::SYM_OR: out << "SYMBOL('||')";
    		break;
    	case Token::SYM_AND: out << "SYMBOL('&&')";
    		break;
    	case Token::SYM_NOT: out << "SYMBOL('!')";
    		break;
    	case Token::SYM_EQUAL: out << "SYMBOL('==')";
    		break;
    	case Token::SYM_NOTEQ: out << "SYMBOL('!=')";
    		break;
    	case Token::SYM_LESS: out << "SYMBOL('<')";
    		break;
    	case Token::SYM_LESSEQ: out << "SYMBOL('<=')";
    		break;
    	case Token::SYM_GRTR: out << "SYMBOL('>')";
    		break;
    	case Token::SYM_GRTREQ: out << "SYMBOL('>=')";
    		break;
    	case Error::IllegalCharacter: out << "ERROR:IllegalCharacter('" << t.lex << "')";
    		break;
    	case Error::UnterminatedComment: out << "ERROR:UnterminatedComment";
    		break;
    	case Error::UnterminatedString: out << "ERROR:UnterminatedString";
    		break;
    	case Error::IllegalFloat: out << "ERROR:IllegalFloat(" << t.lex << ")";
    		break;
    	case Error::InternalError: out << "ERROR:InternalError";
    		break;
    	default:
    		out << "ERROR:UnknownToken(" << t.val << ")";
    }
    out << ">";
    return out;
}
