#include "cc.h"

int consume(int ty);
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();
Vector *tokens;
int pos = 0;

/// Consume token
int consume(int ty) {
  Token *token = (Token *)tokens->data[pos];
  if (token->ty != ty)
    return 0;
  pos++;
  return 1;
}

/// Syntax Rules
Node *equality() {
  Node *node = relational();
  for (;;) {
    if (consume(TK_EQ))
      node = new_node(TK_EQ, node, relational());
    else if (consume(TK_NE))
      node = new_node(TK_NE, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();
  for (;;) {
    if (consume('<'))
      node = new_node('<', node, add());
    else if (consume('>'))
      node = new_node('<', add(), node);
    else if (consume(TK_LE))
      node = new_node(TK_LE, node, add());
    else if (consume(TK_GE))
      node = new_node(TK_LE, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, unary());
    else if (consume('/'))
      node = new_node('/', node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume('+'))
    return term();
  else if (consume('-'))
    return new_node('-', new_node_num(0), term());
  else
    return term();
}

Node *term() {
  Token *token = (Token *)tokens->data[pos];
  if (consume('(')) {
    Node *node = equality();
    if (!consume(')'))
      error("There is no closing parenthesis: %s", token->input);
    return node;
  }

  if (token->ty == TK_NUM)
    pos++;
    return new_node_num(token->val);

  error("Not number nor parenthesis: %s",
        token->input);
}




Node *parse(char *p) {
  //tokens = new_vector();
	tokens = tokenize(p);
  return equality();
}