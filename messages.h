/* Arkansas State University
 * CS 4-5133 Compilers
 * Dr. Jeff Jenness
 * 2015-07-05
 *
 * messages.h
 */
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum
  {PANIC=-3, ERROR=-2, WARNING=-1, SILENT, CONCISE, NORMAL, VERBOSE, DEBUG}
level;

extern bool warnings;
extern int warningCount;
extern int errorCount;
extern FILE *fdbg;
extern FILE *ferr;

void messagesInit();
void messageLevel(int level);
void message(level mlvl, const char *fmt, ...);

#endif
