#include "9cc.h"

int nlabel = 0;

static void p(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  printf("\n");
}

static void emit(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\t");
  vprintf(fmt, ap);
  printf("\n");
}

void gen_lval(Node *node) {
  if (node->ty != ND_IDENT)
    error("The left hand of assignment is not an identifier.");

  emit("mov rax, rbp");
  emit("sub rax, %d", node->offset);
  emit("push rax");
}

/// Generate assembly by Node
void gen(Node *node) {
  switch (node->ty) {
  case ND_RETURN:
    gen(node->lhs);
    emit("pop rax");
    emit("mov rsp, rbp");
    emit("pop rbp");
    emit("ret");
    return;
  case ND_IF:
    gen(node->cond);
    emit("pop rax");
    emit("cmp rax, 0");
    if (node->els) {
      // ifelse
      emit("je .Lelse%d", nlabel);
      gen(node->then);
      emit("jmp .Lend%d", nlabel);
      p(".Lelse%d:", nlabel);
      gen(node->els);
      p(".Lend%d:", nlabel);
    } else {
      // if
      emit("je .Lend%d", nlabel);
      gen(node->then);
      p(".Lend%d:", nlabel);
    }
    nlabel++;
    return;
  case ND_WHILE:
    p(".Lbegin%d:", nlabel);
    gen(node->cond);
    emit("pop rax");
    emit("cmp rax, 0");
    emit("je .Lend%d", nlabel);
    gen(node->body);
    emit("jmp .Lbegin%d", nlabel);
    p(".Lend%d:", nlabel);
    nlabel++;
    return;
  case ND_FOR:
    gen(node->init);
    p(".Lbegin%d:", nlabel);
    gen(node->cond);
    emit("pop rax");
    emit("cmp rax, 0");
    emit("je .Lend%d", nlabel);
    gen(node->body);
    gen(node->inc);
    emit("jmp .Lbegin%d", nlabel);
    p(".Lend%d:", nlabel);
    nlabel++;
    return;
  case ND_COMP_STMT:
    for (int i = 0; i < node->stmts->len; i++) {
      gen(node->stmts->data[i]);
      emit("pop rax");
    }
    return;
  case ND_CALL:
    // push args to stack
    for (int i = node->args->len - 1; i > 0; i--) {
      gen(node->args->data[i]);
      switch (i) {
      case 0:
        emit("pop rdi");
        break;
      case 1:
        emit("pop rsi");
        break;
      case 2:
        emit("pop rdx");
        break;
      case 3:
        emit("pop rcx");
        break;
      case 4:
        emit("pop r8");
        break;
      case 5:
        emit("pop r9");
        break;
      }
    }

    emit("call %s", node->name);
    // Returned value is expected by main()
    // If called function is void, it can cause a problem.
    return;
  case ND_NUM:
    emit("push %d", node->val);
    return;
  case ND_IDENT:
    gen_lval(node);
    emit("pop rax");
    emit("mov rax, [rax]");
    emit("push rax");
    return;
  case '=':
    gen_lval(node->lhs);
    gen(node->rhs);

    emit("pop rdi");
    emit("pop rax");
    emit("mov [rax], rdi");
    emit("push rdi");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  emit("pop rdi");
  emit("pop rax");

  switch (node->ty) {
  case '+':
    emit("add rax, rdi");
    break;
  case '-':
    emit("sub rax, rdi");
    break;
  case '*':
    emit("mul rdi");
    break;
  case '/':
    emit("mov rdx, 0");
    emit("div rdi");
    break;
  case TK_EQ:
    emit("cmp rax, rdi");
    emit("sete al");
    emit("movzb rax, al");
    break;
  case TK_NE:
    emit("cmp rax, rdi");
    emit("setne al");
    emit("movzb rax, al");
    break;
  case '<':
    emit("cmp rax, rdi");
    emit("setl al");
    emit("movzb rax, al");
    break;
  case TK_LE:
    emit("cmp rax, rdi");
    emit("setle al");
    emit("movzb rax, al");
    break;
  }

  emit("push rax");
}
