#include <cstdlib>
#include "scanner.h"

extern "C" {
#include "messages.h"
}

#define BUFF_INIT_SIZE  500
#define BUFF_INC_SIZE   100

Scanner::Scanner() {
    in = NULL;
}

Scanner::Scanner(Source &input) {
	message(DEBUG, "SCANNER: constructor called with input: '%s'", input.name());

	in = &input;
  init_buffer();
}

Scanner::~Scanner() {
    //free_buffer();
}

// KW perfect hash function
static int KWhash(string s) {
    return (1*s.length() + 2*(s[0]+s[1]) + 2*s[s.length()-1]) % 10;
}
// KW arrays
static int   KWidx[] = { 4, 0, 7, 8, 1, 5, 6, 3, 2, 0 }; // indirect index using hash value
static char* KWstr[] = { "", "else", "extern", "float", "if",
                         "int", "return", "void", "while" };
static int   KWtok[] = { Token::IDENT, Token::KW_ELSE, Token::KW_EXTERN,
                         Token::KW_FLOAT, Token::KW_IF, Token::KW_INT,
                         Token::KW_RETURN, Token::KW_VOID, Token::KW_WHILE };
// KW lookup function
static int KWsearch(char *s) {
    int h = KWhash(s); // get hashing index
    message(DEBUG, "hash: keyword hash value = %i", h);
    message(DEBUG, "hash: keyword index value = %i", KWidx[h]);
    message(DEBUG, "hash: matching to keyword \"%s\"", KWstr[KWidx[h]]);
    if (strcmp(KWstr[KWidx[h]], s) == 0)
        return KWtok[KWidx[h]];
    else
        return KWtok[0];
}

Token *Scanner::nextToken() {
	int ch;
	int tok;  // Token::Value or Error::Value

	// consume whitespace and comments
	while (isspace(in->peek()) || (in->peek(1) == '/' && (in->peek(2) == '*' || in->peek(2) == '/'))) { // found whitespace or comment
		message(DEBUG, "SCANNER: processing whitepace");
		if (in->peek() == '/') { // process comment
		   message(DEBUG, "SCANNER: processing comment");
		   reset_buffer(); // start new lexeme
		   nextCh(); // throw away '/'
		   if (nextCh() == '*') { // multi-line comment
			   while ((ch = nextCh()) > 0 && (ch != '*' || in->peek() != '/'))
				   /* empty */;
			   if (nextCh() != '/') // throw away '/' if found
				   return new Token(Error::UnterminatedComment, row, col, buffer);
		   } else { // single-line comment
         while ((ch = nextCh()) > 0 && ch != '\n')
           /* empty */;
		   }
		} else
		  nextCh(); // throw away space
	}

	reset_buffer();	// remember row, col for lexeme
	ch = nextCh();	// current non-whitespace character
	message(DEBUG, "SCANNER: found char CODE(%d)", ch);
	if (ch < 0) { // EOI
		tok = Token::EOI;
  } else if (isalpha(ch) || ch == '_') { // IDENT
    while (isalpha(in->peek()) || in->peek() == '_' || isdigit(in->peek()))
      nextCh(); // get the next alpha, number or '_'
    tok = KWsearch(buffer);
  } else if (isdigit(ch) || ch == '.') { // LIT_INT or LIT_FLOAT
    tok = Token::LIT_FLOAT; // default to LIT_FLOAT
    if (isdigit(ch)) {
      while (isdigit(in->peek()))
        nextCh(); // get next digit
      if (in->peek() == '.') {
        nextCh(); // get '.'
        while (isdigit(in->peek())) // optional fractional digits
          nextCh(); // get next digit
      } else
        tok = Token::LIT_INT;
    } else { // ch == '.'
      if (!isdigit(in->peek())) // requires at least one digit after '.'
        tok = Error::IllegalCharacter;
      else
        while (isdigit(in->peek()))
          nextCh(); // get next digit
    }
    if (tok == Token::LIT_FLOAT && tolower(in->peek()) == 'e') { // optional exponent for LIT_FLOAT only
      nextCh(); // get 'e'
      if (in->peek() == '+' || in->peek() == '-') // optional '+' or '-'
        nextCh(); // get '+' or '-'
      if (!isdigit(in->peek())) // requires at least one digit after 'e'
        tok = Error::IllegalFloat;
      else
        while (isdigit(in->peek()))
          nextCh(); // get next digit
    }
  } else if (ch == '"') { // LIT_STRING
    while ((ch = nextCh()) > 0 && ch != '\n' && ch != '"')
      /* empty */;
    if (ch == '"')
      tok = Token::LIT_STRING;
    else
      tok = Error::UnterminatedString;
  } else if (ch == ';') {
    tok = Token::SYM_SEMICOLON;
  } else if (ch == ',') {
    tok = Token::SYM_COMMA;
  } else if (ch == '{') {
    tok = Token::SYM_OBRACE;
  } else if (ch == '}') {
    tok = Token::SYM_CBRACE;
  } else if (ch == '(') {
    tok = Token::SYM_OPAREND;
  } else if (ch == ')') {
    tok = Token::SYM_CPAREND;
  } else if (ch == '[') {
    tok = Token::SYM_OSQUARE;
  } else if (ch == ']') {
    tok = Token::SYM_CSQUARE;
  } else if (ch == '+') {
    tok = Token::SYM_PLUS;
  } else if (ch == '-') {
    tok = Token::SYM_MINUS;
  } else if (ch == '*') {
    tok = Token::SYM_TIMES;
  } else if (ch == '/') {
    tok = Token::SYM_DIVIDE;
  } else if (ch == '%') {
    tok = Token::SYM_MODULO;
  } else if (ch == '=') {
    if (in->peek() == '=') {
      nextCh(); // read '='
      tok = Token::SYM_EQUAL;
    } else
      tok = Token::SYM_ASSIGN;
  } else if (ch == '<') {
    if (in->peek() == '=') {
      nextCh(); // read '='
      tok = Token::SYM_LESSEQ;
    } else
      tok = Token::SYM_LESS;
  } else if (ch == '>') {
    if (in->peek() == '=') {
      nextCh(); // read '='
      tok = Token::SYM_GRTREQ;
    } else
      tok = Token::SYM_GRTR;
  } else if (ch == '!') {
    if (in->peek() == '=') {
      nextCh(); // read '='
      tok = Token::SYM_NOTEQ;
    } else
      tok = Token::SYM_NOT;
  } else if (ch == '|') {
    if (in->peek() == '|') {
      nextCh(); // read '|'
      tok = Token::SYM_OR;
    } else
      tok = Error::IllegalCharacter;
  } else if (ch == '&') {
    if (in->peek() == '&') {
      nextCh(); // read '&'
      tok = Token::SYM_AND;
    } else
      tok = Error::IllegalCharacter;
	} else {
		tok = Error::IllegalCharacter;
	}

	message(DEBUG, "SCANNER: nextToken() called: lexeme found '%s'", buffer);

	return new Token(tok, row, col, buffer);
}

void Scanner::init_buffer() {
    buffer = new char[BUFF_INIT_SIZE];
    bsize = BUFF_INIT_SIZE;
    reset_buffer();
}

void Scanner::reset_buffer() {
    buffer[0] = '\0';
    bindex = 0;
    row = in->row();
    col = in->col();
}

void Scanner::append_buffer(char c) {
	if (bindex >= bsize - 1) { // resize and copy
	    char *temp = new char[bsize+BUFF_INC_SIZE];
	    for(int i=0; i < bsize; ++i)
	        temp[i] = buffer[i];
	    delete[] buffer;
	    buffer = temp;
	    bsize += BUFF_INC_SIZE;
	}
    buffer[bindex] = c;
    buffer[++bindex] = '\0';
}

void Scanner::free_buffer() {
    delete[] buffer;
}

int Scanner::nextCh() {
    int c = -1;     // eof
    if (!in->eof()) {
      c = in->get();
      append_buffer((char)c);
    }
	return c;
}
