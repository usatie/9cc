#ifndef CC_CC_H
#define CC_CC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// vector.c
typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);

// token.c
enum {
	TK_NUM = 256, // integer
  TK_EOF,
};

typedef struct {
	int ty;				// Token type
	int val;			// The value if ty is TK_NUM
  char *input;
} Token;

// test.c
int expect(int line, int expected, int actual);
void runtest();

#endif

