#ifndef CC_CC_H
#define CC_CC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
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

// Map
typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);

// Token
enum {
  TK_NUM = 256, // Integer
  TK_IDENT,     // ID
  TK_EQ,        // ==
  TK_NE,        // !=
  TK_LE,        // <=
  TK_GE,        // >=
  TK_RETURN,    // return
  TK_IF,        // if
  TK_ELSE,      // else
  TK_EOF,       // End of file token
};

typedef struct {
  int ty;     // Token type
  int val;    // The value if ty is TK_NUM
  char *name; // The name if ty is TK_IDENT
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
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LE,        // <=
  ND_RETURN,    // return
  ND_IF,        // if
};

typedef struct Node Node;
typedef struct Node {
  int ty;    // Operator or ND_NUM
  Node *lhs; // Left hand
  Node *rhs; // Right hand
  int val;   // Use if ND_NUM

  // Variable
  int offset; // Use if ND_IDENT

  // Control statements
  Node *cond;
  Node *then;
  Node *els;
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void gen(Node *node);

// Parser
Node *parse(char *p);

// Utility
void error(char *fmt, ...);

// Test
void runtest();

#endif
