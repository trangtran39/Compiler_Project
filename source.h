#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "buffer.h"

using namespace std;

class Source {
	private:
		static const int DEFAULTTW=8;
    public:
		static const int BUFFERERROR=Buffer::INDEXERROR;
				Source();                  									// initializes to NULL input stream
        Source(const char *name, int tw=DEFAULTTW);	// opens file "name" for input
        Source(istream *in, int tw=DEFAULTTW);     	// uses previously opened file
        const char *name(); // name of open file
        bool fail();        // true if error found (badbit || failbit)
        bool eof();         // true if eof found
        int get();          // read next character, return -1 on EOF
        int unget();        // put back the last character read, return BUFFERERROR on error
		int unget(char c);  // put back c as the last character read, return -1 on error
        int peek();         // look into input without reading
        int peek(int i);	// peek ahead i characters, peek(1)==peek(), return BUFFERERROR error
        int tabsize();      // known tab size of text
        int row();          // current row (of next read)
        int col();          // current col (of next read)

    private:
    		static const int MAX_NAME = 100;
        char filename[MAX_NAME] = "(null)";
        istream *input = NULL;		// input source file
        Buffer *buff = NULL;		// input buffer
        int tabwidth = DEFAULTTW;	// used in calculating col
};

#endif
