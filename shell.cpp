#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "shell.h"
#include "source.h"
#include "parser.h"

extern "C" {
#include "messages.h"
}

const int NAME_MAXLEN = 256;
int tabwidth   = DEFTABSIZE;
bool use_stdin = false;
bool use_ext = true;
int findex     = 0;

void processCmdLine(char *args[]);

int main(int argc, char *argv[]) {
    char filename[NAME_MAXLEN];
    char *fileext;
    Source in;
    int errors = 0;

    messagesInit();
    //messageLevel(DEBUG);

    processCmdLine(argv);
    if (findex != 0) {
      message(DEBUG, "SHELL: opening file: '%s'", argv[findex]);
    	strncpy(filename,argv[findex],NAME_MAXLEN);
    	if (use_ext && ((fileext=strrchr(argv[findex], '.')) == NULL || strcmp(fileext,EXTENSION) != 0))
    		strncat(filename,EXTENSION,NAME_MAXLEN-strlen(filename));
      if ((in = Source(filename, tabwidth)).fail())
        message(PANIC, "file not found: '%s'", filename);
      if (use_stdin)
        message(WARNING, "'--stdin' ignored: using '%s'", filename);
    } else if (use_stdin) {
        message(DEBUG, "SHELL: using <stdin>");
        in = Source(&cin, tabwidth);
    } else { // no input
        message(NORMAL, "no input specified");
        std::exit(0);
    }

    Parser parser(in);

    errors = parser.parse();

    if (errors==0)
        message(NORMAL, "compile successful");
    else
        message(NORMAL, "compile failed: %d errors detected", errors);

    return errors;
}

void processCmdLine(char *args[]) {
    int i;
    char *opt, *arg;

    for (i=1; args[i] != NULL && args[i][0] == '-'; ++i) {
        opt = strtok(args[i],":=");
        arg = strtok(NULL, "");
        message(DEBUG, "SHELL: ARGS(%d): opt = '%s', arg = '%s'", i, opt, arg);
        if (opt == NULL) {
            message(WARNING, "illegal option: '%s'", args[i]);
        } else if (strcmp(opt, "--help")==0 || strcmp(opt, "-h")==0 || strcmp(opt, "-?")==0) {
            message(NORMAL, "%s [options] filename[%s]", args[0], EXTENSION);
            message(NORMAL, "\t--help\t\tprint this help message (or -h, -?)");
            message(NORMAL, "\t--version\tprint the version number (or -v)");
            message(NORMAL, "\t--tabwidth:n\tset tabwidth to n (or -t:n)");
            message(NORMAL, "\t--stdin\t\tset input to stdin (or --)");
            message(NORMAL, "\t--debug[:file]\tturn on debug messages (or -d[:file])");
            message(NORMAL, "\t--errors:file\tredirect errors to file");
            message(NORMAL, "\t--silent\tprint only errors (or -s)");
            message(NORMAL, "\t--noext\t\tdon't add file extension '%s'", EXTENSION);
            exit(0);
        } else if (strcmp(opt, "--version")==0 || strcmp(opt, "-v")==0) {
		        message(NORMAL, COMPILER " %d.%d%d " TAG, VERSION, REVISION, PATCH);
            exit(0);
        } else if (strcmp(opt, "--tabwidth")==0 || strcmp(opt, "-t")==0) {
            if (arg != NULL) {
            	int isInt = 1;
            	for (int j=0; j<strlen(arg); ++j)
            		if (!isdigit(arg[j])) {
            			message(WARNING, "tabwidth not a number: '%s'", arg);
        	    		isInt = 0;
        		    	break;
        		    }
           	    if (isInt)
        		    tabwidth = atoi(arg);
            } else
                message(WARNING, "tabwidth missing: number required");
        } else if (strcmp(opt, "--stdin")==0 || strcmp(opt, "--")==0) {
            use_stdin = true;
        } else if (strcmp(opt, "--debug")==0 || strcmp(opt, "-d")==0) {
		        messageLevel(DEBUG);
            if (arg != NULL) {
              FILE *out;
              if ((out = fopen(arg, "w")) == NULL)
                message(WARNING, "could not open debug file: '%s': using stdout", arg);
              else
                fdbg = out;
            }
        } else if (strcmp(opt, "--errors")==0) {
            if (arg != NULL) {
              FILE *out;
              if ((out = fopen(arg, "w")) == NULL)
                message(WARNING, "could not open error file: '%s': using stdout", arg);
              else
                ferr = out;
            } else
              message(WARNING, "error file missing: using stdout", arg);
        } else if (strcmp(opt, "--silent")==0 || strcmp(opt, "-s")==0) {
		        messageLevel(SILENT);
        } else if (strcmp(opt, "--noext")==0) {
		        use_ext = false;
        } else {
            message(WARNING, "unknown option: '%s'", args[i]);
        }
    }

    if (args[i] != NULL) {
        findex = i;
        message(DEBUG, "SHELL: found filename on command-line: ARGS[%d]", findex);
        if (args[i+1] != NULL) {
            message(WARNING, "unprocessed options: '%s'", args[i+1]);
        }
    }
}
