#include "cc.h"


int consume(int ty);
Node *add();
Node *mul();
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
  Node *node = term();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, term());
    else if (consume('/'))
      node = new_node('/', node, term());
    else
      return node;
  }
}

Node *term() {
  Token *token = (Token *)tokens->data[pos];
  if (consume('(')) {
    Node *node = add();
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
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->ty) {
    case '+':
      printf("  add rax, rdi\n");
      break;
    case '-':
      printf("  sub rax, rdi\n");
      break;
    case '*':
      printf("  mul rdi\n");
      break;
    case '/':
      printf("  mov rdx, 0\n");
      printf("  div rdi\n");
      break;
  }

  printf("  push rax\n");
}

void tokenize(char *p) {
	int i = 0;
	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}


		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
      Token *token = new_token(*p, p);
      vec_push(tokens, token);
			i++;
			p++;
			continue;
		}

		if (isdigit(*p)) {
      Token *token = new_token_num(strtol(p, &p, 10), p);
      vec_push(tokens, token);
			i++;
			continue;
		}

		error("Cannot tokenize: %s", p);
		exit(1);
	}

  Token *token = new_token_eof(p);
  vec_push(tokens, token);
}


// Main
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of argments is invalid.");
    return 1;
  }

	if (!strcmp(argv[1], "-test")) {
		runtest();
		return 0;
	}

  tokens = new_vector();

	// Tokenize and parse
	tokenize(argv[1]);
  Node *node = add();

	// Print assembly's first half
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Generate code with going down abstract tree
  gen(node);

  // The result should be at the top of stack
  // Load it to RAX as a return value.
  printf("  pop rax\n");
  printf("	ret\n");
  return 0;
}
