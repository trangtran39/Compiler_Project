#include "buffer.h"

extern "C" {
#include "messages.h"
}

#define BLOCK_STARTSIZE 10

Buffer::Buffer(istream *in, int tw) {
	input = in;
	tab = (tw<=0)?8:tw;
	block = BLOCK_STARTSIZE;
	bsize = 3*block;
	row = col = 1;
	buffer = new Char[bsize];
	for (fore = 0; !input->eof() && fore < bsize; ++fore)
		buffer[fore] = next();
	head = 0; back = 0;  // head indexes the next character to read
	message(DEBUG, "BUFFER: created buffer of %d Char", bsize);
}

Buffer::~Buffer() {
	delete[] buffer;
}

void Buffer::expand(int n) {
    Char *oldbuff;
    int oldsize, i, j;

	if (n <= 0) 	// can only grow, nothing to be done
		return;

	oldbuff = buffer;
	oldsize = bsize;
    block += n;
    bsize = 3*block;	// need one block fore and one block back and a current working block
	buffer = new Char[bsize];

	j=(head-back);
 	if (j<0) j+=oldsize;
	for (i=0; i < oldsize; ++i)
		buffer[i] = oldbuff[(j+i)%oldsize];
	head = back-1;
	while (!input->eof() && head+fore<bsize)
		buffer[head+(fore++)] = next();

	delete[] oldbuff;
}

// NOTE: valid indexes have the range -back<i<=fore, peek(0) will return
// last read character, that is, range of peeks back are from 0 to -back+1
// the range of peeks fore are from 1 to fore
Char Buffer::peekChar(int i) {
	if (-back < i && i <= fore)
	    return buffer[(head+i-1)<0?(head+i-1)+bsize:(head+i-1)%bsize];
	else
		return Char(INDEXERROR,-1,-1);
}

// same as peekChar(i) except returns only character
int Buffer::peek(int i) {
		return peekChar(i).val;
}

// NOTE: eof() is not set until attempting to read past EOF, so a -1 (EOI)
// makes it into the buffer to indicate end-of-input has been reached
Char Buffer::getChar() {
	Char c;
	if (!input->eof() && fore<block)	// fill & refill buffer on input
		for (int i=0; !input->eof() && i<=block; ++i,++fore,--back)
			buffer[(head+fore)%bsize] = next();
	if (fore > 0) {
		c = buffer[head];
		head = (head+1)%bsize;
		--fore; ++back;
	} else
		c = buffer[head];	// EOI marker is all that's left
	return c;
}

// same as getChar() except returns only characters
int Buffer::get() {
	return getChar().val;
}

// NOTE: the number of ungets is guaranteed to be at least the size of a
// block, it's the same as the number of peeks backward in the buffer,
int Buffer::unget(int c) {
	int success = c;

	if (back > 0) {
		head = (head==0)?bsize-1:head-1;
		buffer[head].val = c;
		++fore; --back;
	} else
		success = INDEXERROR;

	return success;
}

// same as unget(c) without replacement
int Buffer::unget() {
	return unget(buffer[(head==0)?bsize-1:head-1].val);
}

int Buffer::eoi() {
	return head>0 && buffer[head%bsize].val == EOI;
}

int Buffer::size() {
	return bsize;
}

int Buffer::behind() {
	return back;
}

int Buffer::ahead() {
	return fore;
}

// function for retrieving next character in input with row and col
Char Buffer::next() {
    Char c;
    c.val = EOI;									// EOI by default
    c.row = row; c.col = col;						// row, col of last read in input
    if (!input->eof()) {
        c.val = input->get();
		// update row and col for next read in input
        if (c.val == '\r' || c.val == '\n') {       // Mac or Unix EOL
            if (c.val == '\r' && input->peek() == '\n') // DOS style EOL
                c.val = input->get();
            c.val = '\n';							// normalize EOL
            ++row;
            col = 1;
        } else if (c.val == '\t')
            col = col/tab * tab + tab + 1;			// calculate the tab column
        else
            ++col;
    }
		//message(DEBUG, "BUFFER: found Char(%d,%d,'%c')", c.row, c.col, c.val);
    return c;
}

#if DEBUGGING==1
char printable(int c) {
	char ch = (char)c;
	if (ch <= 13)
	  ch = ' ';
	return ch;
}

void Buffer::state() {
	cout << "back=" << back << "; head=" << head << "; fore=" << fore << endl;
}

void Buffer::dump() {
    int start, i, j;
    start = head-back;
    if (start<0) start+=bsize;
    for (i=0; i<bsize; ++i)
    	cout << buffer[(start+i)%bsize].col%10;
    cout << endl;
    for (i=0; i<bsize; ++i)
    	cout << printable(buffer[(start+i)%bsize].val);
    cout << endl;
    for (i=0; i<bsize; ++i) {
    	j = (start+i)%bsize;
        if (j==0 || j==head)
        	cout << '^';
        else
        	cout << ' ';
    }
    cout << endl;
    for (i=0; i<bsize; ++i) {
    	j = (start+i)%bsize;
        if (j==head)
        	cout << 'H';
        else if (j==0)
        	cout << '0';
        else
        	cout << ' ';
    }
    cout << endl;
}
#endif
