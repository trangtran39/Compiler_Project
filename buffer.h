/* peek/unget buffer */
#ifndef __BUFFER_H__
#define __BUFFER_H__
#define DEBUGGING 0

#include <iostream>

#define BLOCK_EXPANDSIZE	5

using namespace std;

class Char {				// character, row, col buffered for input
	public:
	    enum Values { EOI = -1 };
		Char(int v, int r, int c) { val = v; row = r; col = c; }
		Char() { val = EOI; row = 0; col = 0; }
		int val;
		int row;
		int col;
};

class Buffer {

    public:
		enum Values {
			EOI = Char::EOI,	// indicates end-of-input
			INDEXERROR = -2,	// indicates index error for methods
		};
    Buffer(istream *in, int tw);	// uses previously opened file
    ~Buffer();
    void expand(int n=BLOCK_EXPANDSIZE); // increase buffer size by n
		Char peekChar(int i = 1);	// -back<i<=fore, peek(0) will return last read Char
		int peek(int i = 1);	// -back<i<=fore, peek(0) will return last read character
		Char getChar();				// get Char function
		int get();						// normal get function, return only character
		int unget(int c);			// replace last read character with c
		int unget();					// unget last read character without replacemnet
		int eoi();						// return 1 on end of input
		int size();						// total buffers size in Char, size = back+fore
		int behind();					// number of characters behind next get
		int ahead();					// number of characters ahead of last get
#if DEBUGGING==1
		void state();
    void dump();
#endif
    private:
        istream *input;		// input source file
        int tab;					// used in calculating col
        Char *buffer;			// internal circular queue for buffer
        int block;				// block size, buffer size is 3*block
        int bsize;				// actual buffer size
        int head;					// index for last character read from input
        int fore;					// count for get (ahead)
        int back;					// count for unget (behind)
        int row;					// row after last read
        int col;					// col after last read
        Char next();			// read next char, row, col from input
};

#endif
