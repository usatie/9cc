#ifndef CC_CC_H
#define CC_CC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vector
typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);

// Token
enum {
	TK_NUM = 256, // integer
  TK_EOF,
};

typedef struct {
	int ty;				// Token type
	int val;			// The value if ty is TK_NUM
  char *input;
} Token;

// Node
enum {
  ND_NUM = 256,
};

typedef struct Node {
  int ty;
  struct Node *lhs;
  struct Node *rhs;
  int val;
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);

// Utility
void error(char *fmt, ...);

// Test
int expect(int line, int expected, int actual);
void runtest();

#endif

