#include <string.h>
#include "source.h"

extern "C" {
#include "messages.h"
}

Source::Source(const char *name, int tw) {
    message(DEBUG, "SOURCE: opening source file: '%s'", name);
    strncpy(filename,name,MAX_NAME);
    input = new ifstream(filename);
    tabwidth = tw;
    buff = new Buffer(input, tabwidth);
}

Source::Source(istream *in, int tw) {
    if (in == &cin)
      strncpy(filename,"<stdin>",MAX_NAME);
    message(DEBUG, "SOURCE: initializing input: '%s'", filename);
    input = in;
    tabwidth = tw;
    buff = new Buffer(input, tabwidth);
}

Source::Source() {
    /* empty - use default values */
    message(DEBUG, "SOURCE: constructing empty object");
}

const char *Source::name() {
    return filename;
}

bool Source::fail() {
	return !input->eof() && input->fail();
}

bool Source::eof() {
  return buff->eoi();
}

int Source::get() {
	return buff->getChar().val;
}

int Source::unget() {
	return buff->unget();
}

int Source::unget(char c) {
	return buff->unget(c);
}

int Source::peek() {
  return buff->peekChar().val;
}

int Source::peek(int i) {
	return buff->peekChar(i).val;
}

int Source::tabsize() {
  return tabwidth;
}

int Source::row() {
  return buff->peekChar().row;
}

int Source::col() {
  return buff->peekChar().col;
}
