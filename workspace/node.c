#include "cc.h"

/// Create new Node (Node)
Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

/// Create new Node (Num)
Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

/// Generate assembly by Node
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("	push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("	pop rdi\n");
  printf("	pop rax\n");

  switch (node->ty) {
  case '+':
    printf("	add rax, rdi\n");
    break;
  case '-':
    printf("	sub rax, rdi\n");
    break;
  case '*':
    printf("	mul rdi\n");
    break;
  case '/':
    printf("	mov rdx, 0\n");
    printf("	div rdi\n");
    break;
  case TK_EQ:
    printf("	cmp rax, rdi\n");
    printf("	sete al\n");
    printf("	movzb rax, al\n");
    break;
  case TK_NE:
    printf("	cmp rax, rdi\n");
    printf("	setne al\n");
    printf("	movzb rax, al\n");
    break;
  case '<':
    printf("	cmp rax, rdi\n");
    printf("	setl al\n");
    printf("	movzb rax, al\n");
    break;
  case TK_LE:
    printf("	cmp rax, rdi\n");
    printf("	setle al\n");
    printf("	movzb rax, al\n");
    break;
  }

  printf("	push rax\n");
}
