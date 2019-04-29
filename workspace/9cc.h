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
  TK_NUM = 256, // Integer
  TK_IDENT,     // ID
  TK_EQ,        // ==
  TK_NE,        // !=
  TK_LE,        // <=
  TK_GE,        // >=
  TK_EOF,       // End of file token
};

typedef struct {
  int ty;  // Token type
  int val; // The value if ty is TK_NUM
  char *input;
} Token;

Token *new_token(int ty, char *input);
Token *new_token_num(int val, char *input);
Token *new_token_id(char *input);
Token *new_token_eof(char *input);
Vector *tokenize(char *p);

// Node
enum {
  ND_NUM = 256, // Integer
  ND_IDENT,     // ID
};

typedef struct Node {
  int ty;           // Operator or ND_NUM
  struct Node *lhs; // Left hand
  struct Node *rhs; // Right hand
  int val;          // Use if ND_NUM
  char name;        // Use if ND_IDENT
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_ident(char name);
void gen(Node *node);

// Parser
Node *parse(char *p);

// Utility
void error(char *fmt, ...);

// Test
int expect(int line, int expected, int actual);
void runtest();

#endif
