#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "source.h"
#include "error.h"
#include "token.h"

class Scanner {

    public:
        Scanner();
        Scanner(Source &input);
        ~Scanner();
        Token *nextToken();

    private:
        Source *in;
        char *buffer;
        int bindex, bsize;
        void init_buffer();
        void reset_buffer();
        void append_buffer(char c);
        void free_buffer();
        int peekCh();
        int nextCh();
        void ungetCh();
        int row,col;
};

#endif
