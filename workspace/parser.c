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
    else if (consume(TK_LE))
      node = new_node(TK_LE, node, add());
    else if (consume('>'))
      node = new_node('>', node, add());
    else if (consume(TK_GE))
      node = new_node(TK_GE, node, add());
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

void tokenize(char *p) {
	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}

		// == or !=
    if (!strncmp(p, "==", 2)) {
      Token *token = new_token(TK_EQ, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, "!=", 2)) {
      Token *token = new_token(TK_NE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, "<=", 2)) {
      Token *token = new_token(TK_LE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, ">=", 2)) {
      Token *token = new_token(TK_GE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
      Token *token = new_token(*p, p);
      vec_push(tokens, token);
			p++;
			continue;
		}

    if ('a' <= *p && *p <= 'z') {
      Token *token = new_token_id(p);
			vec_push(tokens, token);
      p++;
      continue;
    }

		if (isdigit(*p)) {
      Token *token = new_token_num(strtol(p, &p, 10), p);
      vec_push(tokens, token);
			continue;
		}

		error("Cannot tokenize: %s", p);
		exit(1);
	}

  Token *token = new_token_eof(p);
  vec_push(tokens, token);
}


Node *parse(char *p) {
  tokens = new_vector();
	tokenize(p);
  return equality();
}
