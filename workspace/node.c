#include "9cc.h"

/// Create new Node (Node)
Node *new_node(int ty) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  return node;
}

/// Create new Binary operator Node (Node)
Node *new_binop(int ty, Node *lhs, Node *rhs) {
  Node *node = new_node(ty);
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

/// Create new Node (Num)
Node *new_node_num(int val) {
  Node *node = new_node(ND_NUM);
  node->val = val;
  return node;
}
