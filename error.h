#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>

using namespace std;

class Error {

public:
	enum Value {
		InternalError		= -1,
		TokenNotInitialized = -2,
		IllegalCharacter    = -3,
		UnterminatedComment = -4,
		UnterminatedString  = -5,
		IllegalFloat		= -6,
	};
	static const char *toStr(int err);
};

#endif
