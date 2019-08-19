#include "error.h"

const char *Error::toStr(int err) {
	string s;
	switch (err) {
	    case InternalError:         s = "unknown internal error";                    break;
		case TokenNotInitialized:   s = "internal error: token not initialized";     break;
		case IllegalCharacter:		s = "illegal character in input";				 break;
		case UnterminatedComment:	s = "comment not terminated before end-of-file"; break;
		case UnterminatedString:	s = "string not terminated before newline";		 break;
		case IllegalFloat:			s = "float literal missing exponent";			 break;
		default:					s = "unknown error reported";
	}
	return s.c_str();
}
