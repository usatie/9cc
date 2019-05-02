#include "9cc.h"

/// Variables
static Node *code[5];
static Vector *tokens;
static int pos = 0;
static int num_ids = 0;
static Map *offset_map;

/// Header
static bool consume(int ty);
static Node *assign();
static Node *stmt();
static void program();
static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *term();
static Node *ident();

/// Consume token
bool consume(int ty) {
  Token *token = (Token *)tokens->data[pos];
  if (token->ty != ty)
    return 0;
  pos++;
  return 1;
}

void expect(int ty) {
  Token *t = tokens->data[pos];
  if (t->ty == ty) {
    pos++;
    return;
  }

  if (isprint(ty)) {
    error("%c expected", ty);
  }
  error("Not printable expected character for type %d", ty);
}

/// Syntax Rules
void program() {
  int i = 0;
  Token *token = (Token *)tokens->data[pos];
  while (token->ty != TK_EOF) {
    code[i] = stmt();
    i++;
    token = (Token *)tokens->data[pos];
  }
  code[i] = NULL;
}

Node *stmt() {
  Node *node;
  if (consume(TK_IF)) {
    node = new_node(ND_IF);
    expect('(');
    node->cond = equality();
    expect(')');

    node->then = stmt();

    if (consume(TK_ELSE)) {
      node->els = stmt();
    }
    return node;
  }

  if (consume(TK_WHILE)) {
    node = new_node(ND_WHILE);
    expect('(');
    node->cond = equality();
    expect(')');

    node->body = stmt();
    return node;
  }

  if (consume(TK_RETURN)) {
    node = new_node(ND_RETURN);
    node->lhs = assign();
  } else {
    node = assign();
  }

  if (!consume(';')) {
    Token *token = (Token *)tokens->data[pos];
    error("';' is expected but got: [%d] %s", token->ty, token->input);
  }
  return node;
}

Node *assign() {
  Node *node = equality();
  while (consume('='))
    node = new_binop('=', node, assign());
  return node;
}

Node *equality() {
  Node *node = relational();
  for (;;) {
    if (consume(TK_EQ))
      node = new_binop(ND_EQ, node, relational());
    else if (consume(TK_NE))
      node = new_binop(ND_NE, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();
  for (;;) {
    if (consume('<'))
      node = new_binop('<', node, add());
    else if (consume('>'))
      node = new_binop('<', add(), node);
    else if (consume(TK_LE))
      node = new_binop(ND_LE, node, add());
    else if (consume(TK_GE))
      node = new_binop(ND_LE, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_binop('+', node, mul());
    else if (consume('-'))
      node = new_binop('-', node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_binop('*', node, unary());
    else if (consume('/'))
      node = new_binop('/', node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume('+'))
    return term();
  else if (consume('-'))
    return new_binop('-', new_node_num(0), term());
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

  if (token->ty == TK_NUM) {
    pos++;
    return new_node_num(token->val);
  } else if (token->ty == TK_IDENT) {
    pos++;

    // If id is seen for the first time, increment num_ids and save offset for
    // the id.
    int offset = (int)map_get(offset_map, token->name);
    if (offset == 0) {
      num_ids++;
      offset = num_ids * 8;
      map_put(offset_map, token->name, offset);
    }

    // Identity Node is only created here. So I don't create init func.
    Node *node = new_node(ND_IDENT);
    node->offset = offset;
    return node;
  }

  error("Not number, identifier nor parenthesis: %s", token->input);
}

Node *parse(char *p) {
  pos = 0;
  num_ids = 0;
  offset_map = new_map();
  tokens = tokenize(p);
  program();
  return code;
}
