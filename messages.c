/* Arkansas State University
 * CS 4-5133 Compilers
 * Dr. Jeff Jenness
 * 2015-07-05
 *
 * messages.c
 */
#include "messages.h"

#define maxlen 256

static level plvl;
bool warnings;
int warningCount;
int errorCount;
FILE *fdbg;
FILE *ferr;

void messagesInit() {
    plvl = NORMAL;
    warnings = true;
    warningCount = 0;
    errorCount = 0;
    fdbg = stdout;
    ferr = stderr;

    message(DEBUG, "messages: initialized");
}

void messageLevel(int l) {
    if (l>=SILENT && l<=DEBUG)
        plvl = l;
}

void message(level mlvl, const char *fmt, ...) {
    static char msg[maxlen];
    va_list args;
    va_start(args,fmt);

    vsnprintf(msg, maxlen, fmt, args);
    if (mlvl == PANIC || mlvl == ERROR) {
    	++errorCount;
        fprintf(ferr, "ERROR: %s\n", msg);
		fflush(ferr);
		if (mlvl == PANIC)
            exit (-1);
    } else if (mlvl == WARNING) {
        if (WARNING && plvl > SILENT) {
        	++warningCount;
            fprintf(ferr, "WARNING: %s\n", msg);
			fflush(ferr);
		}
    } else if (mlvl <= plvl) {
        if (mlvl == DEBUG) {
            fprintf(fdbg, "DEBUG: %s\n", msg);
			fflush(fdbg);
		}
		else {
			fprintf(stdout, "%s\n", msg);
			fflush(stdout);
		}
    }
    va_end(args);
}
