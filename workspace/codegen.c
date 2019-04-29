#include "cc.h"

void gen_lval(Node *node) {
  if (node->ty != ND_IDENT)
    error("The left hand of assignment is not an identifier.");

  int offset = ('z' - node->name + 1) * 8;
  printf("	mov rax, rbp\n");
  printf("	sub rax, %d\n", offset);
  printf("	push rax\n");
}

/// Generate assembly by Node
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("	push %d\n", node->val);
    return;
  }

  if (node->ty == ND_IDENT) {
    gen_lval(node);
    printf("	pop rax\n");
    printf("	mov rax [rax]\n");
    printf("	push rax\n");
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("	pop rdi\n");
    printf("	pop rax\n");
    printf("	mov [rax] rdi\n");
    printf("	push rdi\n");
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