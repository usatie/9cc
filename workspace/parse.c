#include "9cc.h"

/// Variables
static Vector *functions;
static Node *code[5];
static Vector *tokens;
static int pos = 0;
static int num_ids = 0;
static Map *offset_map;

/// Header
static bool consume(int ty);
static Node *assign();
static Node *toplevel();
static Node *compound_stmt();
static Node *stmt();
static void program();
static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *term();
static char *ident();

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
    error("%c expected, %s", ty, t->input);
  }
  error("Not printable expected character for type %d", ty);
}

/// Syntax Rules
// program: func + program
// program: ε
void program() {
  int i = 0;
  while (!consume(TK_EOF)) {
    code[i] = toplevel();
    i++;
  }
  code[i] = NULL;
}

// param_declaration:
Node *param_declaration() {
  Node *param = new_node(ND_DECL_PARAM);
  char *name = ident();
  param->name = name;
  num_ids++;
  int offset = num_ids * 8;
  map_put(offset_map, name, offset);
  param->offset = offset;
  return param;
}

// toplevel: ident + "(" + params + ")" + compound_stmt
Node *toplevel() {
  // Function
  char *name = ident();
  expect('(');
  Vector *params = new_vector();
  while (!consume(')')) {
    vec_push(params, param_declaration());
    consume(',');
  }

  Node *node = new_node(ND_DECL_FUNC);
  node->params = params;
  node->name = name;
  expect('{');
  node->body = compound_stmt();
  return node;
}

Node *compound_stmt() {
  Node *node = new_node(ND_COMP_STMT);
  node->stmts = new_vector();
  while (!consume('}')) {
    vec_push(node->stmts, stmt());
  }
  return node;
}

// stmt: "{" + stmts + "}"
// stmt: "if" + "(" + equality + ")" + stmt
// stmt: "if" + "(" + equality + ")" + stmt + "else" + stmt
// stmt: "while" + "(" + equality + ")" + stmt
// stmt: "for" + "(" + assign + ";" + equality + ";" + assign + ";" + ")" + stmt
// stmt: "return" + equality + ";"
// stmt: assign + ";"
Node *stmt() {
  Node *node;
  if (consume('{')) {
    return compound_stmt();
  }

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

  if (consume(TK_FOR)) {
    node = new_node(ND_FOR);
    expect('(');
    node->init = assign();
    expect(';');
    node->cond = equality();
    expect(';');
    node->inc = assign();
    expect(';');
    expect(')');

    node->body = stmt();
    return node;
  }

  if (consume(TK_RETURN)) {
    node = new_node(ND_RETURN);
    node->lhs = equality();
  } else {
    node = assign();
  }

  expect(';');
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

// "(" + equality ")"
// num
// identity
// identity + "(" + args + ")"
Node *term() {
  Token *token = (Token *)tokens->data[pos];
  if (consume('(')) {
    Node *node = equality();
    expect(')');
    return node;
  }

  if (consume(TK_NUM)) {
    return new_node_num(token->val);
  }

  char *name = ident();
  if (consume('(')) {
    // function call
    Node *node = new_node(ND_CALL);
    node->name = name;
    node->args = new_vector();
    while (!consume(')')) {
      vec_push(node->args, equality());
      consume(',');
    }
    return node;
  } else {
    // identity
    // If id is seen for the first time, increment num_ids and save offset for
    // the id.
    int offset = (int)map_get(offset_map, name);
    if (offset == 0) {
      num_ids++;
      offset = num_ids * 8;
      map_put(offset_map, token->name, offset);
    }

    Node *node = new_node(ND_IDENT);
    node->offset = offset;
    return node;
  }
}

char *ident() {
  Token *t = (Token *)tokens->data[pos++];
  if (t->ty != TK_IDENT)
    error("expected Identifier: %s", t->input);
  return t->name;
}

Node *parse(char *p) {
  pos = 0;
  num_ids = 0;
  offset_map = new_map();
  tokens = tokenize(p);
  program();
  return code;
}
